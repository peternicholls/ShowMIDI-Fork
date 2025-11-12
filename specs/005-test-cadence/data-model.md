# Data Model: Phase 11 Testing Cadence & Governance (Run 1)

**Feature**: 005-test-cadence  
**Date**: 2025-11-12  
**Purpose**: Define entities, relationships, and validation rules for testing governance framework

---

## Entity 1: CheckCategory

Represents a distinct category of automated checks/tests in the validation pipeline.

### Fields

| Field | Type | Required | Validation | Description |
|-------|------|----------|------------|-------------|
| `id` | string | ✅ | Unique identifier (kebab-case) | e.g., `unit-tests`, `static-analysis`, `packaging-smoke` |
| `name` | string | ✅ | Human-readable display name | e.g., "Unit Tests", "Static Analysis" |
| `purpose` | string | ✅ | 1-2 sentence description | Why this check exists |
| `tools` | string[] | ✅ | Non-empty array | Tools/frameworks used (e.g., `["JUCE UnitTest", "CTest"]`) |
| `owner` | string | ✅ | Team/area identifier | Who maintains this check category (maps to FR-008) |
| `blocking_policy` | enum | ✅ | `always`, `conditional`, `never` | When this check blocks merges |
| `expected_duration_sec` | number | ✅ | > 0 | Typical runtime in seconds (for budget planning) |
| `timeout_sec` | number | ✅ | >= expected_duration_sec | Hard timeout before failure |
| `flake_threshold` | object | ✅ | See FlakeThreshold schema | Thresholds for quarantine detection |

### Relationships

- **One-to-many** with `TriggerContext`: A CheckCategory can run in multiple contexts (PR, nightly, release)
- **One-to-one** with `Owner`: Each category has one accountable owner

### State Transitions

```
[Created] → [Active] → [Quarantined] → [Active] (after remediation)
                    ↓
                 [Deprecated]
```

- **Created**: New check category defined but not yet in CI
- **Active**: Running in at least one trigger context
- **Quarantined**: Auto-removed from required gates due to flakiness
- **Deprecated**: Archived (no longer runs); preserved for historical data

### Validation Rules

- `blocking_policy == 'conditional'` MUST have accompanying `condition_expression` field
- `expected_duration_sec * 1.5 <= timeout_sec` (timeout allows 50% buffer)
- `flake_threshold.failure_rate_7d` MUST be between 0.0 and 1.0

---

## Entity 2: TriggerContext

Represents a specific event/environment where checks execute (e.g., PR, nightly, release).

### Fields

| Field | Type | Required | Validation | Description |
|-------|------|----------|------------|-------------|
| `id` | string | ✅ | Unique identifier | e.g., `pr-to-develop`, `nightly-develop`, `release-gate` |
| `event_type` | enum | ✅ | `local`, `pre-commit`, `pre-push`, `pull_request`, `on-merge`, `nightly`, `release`, `hotfix` | GitHub Actions event or local hook |
| `scope` | string | ✅ | Branch pattern or "local" | e.g., `develop`, `release/**`, `hotfix/**` |
| `enabled_checks` | string[] | ✅ | Array of CheckCategory IDs | Which checks run in this context |
| `time_budget_sec` | number | ✅ | > 0 | Total allowed time (median target) |
| `time_budget_p95_sec` | number | ✅ | >= time_budget_sec | 95th percentile target |
| `parallelization` | enum | ✅ | `sequential`, `parallel`, `matrix` | Execution strategy |
| `change_detection_enabled` | boolean | ✅ | - | Skip if no code changes since last run? |

### Relationships

- **Many-to-many** with `CheckCategory`: A trigger runs multiple checks; a check runs in multiple triggers
- **One-to-many** with `Policy`: Each trigger context has multiple policies (retry, caching, etc.)

### State Transitions

```
[Defined] → [Enabled] → [Disabled]
               ↓
          [Modified] (updates to enabled_checks or budgets)
```

### Validation Rules

- `event_type == 'nightly'` SHOULD have `change_detection_enabled == true` (optimization)
- `time_budget_p95_sec <= 600` for `event_type == 'pull_request'` (10 min hard cap)
- `enabled_checks` MUST reference existing CheckCategory IDs

---

## Entity 3: Policy

Encapsulates governance rules (flake handling, skip approval, retry, caching).

### Fields

| Field | Type | Required | Validation | Description |
|-------|------|----------|------------|-------------|
| `id` | string | ✅ | Unique identifier | e.g., `flake-quarantine`, `skip-approval-required` |
| `policy_type` | enum | ✅ | `flake`, `skip`, `retry`, `caching`, `reporting`, `escalation` | Category of policy |
| `applies_to` | string[] | ✅ | Array of TriggerContext or CheckCategory IDs | Scope of applicability |
| `rules` | object | ✅ | JSON schema varies by policy_type | Specific rules (see examples below) |
| `enforcement` | enum | ✅ | `hard`, `soft`, `advisory` | Hard = blocking; soft = warning; advisory = info only |
| `sla_hours` | number | ❌ | > 0 | Service-level agreement (e.g., 48 hours for flake remediation) |

### Policy Type Schemas

#### Flake Policy

```json
{
  "policy_type": "flake",
  "rules": {
    "failure_count_window": 10,
    "failure_threshold": 3,
    "failure_rate_7d_threshold": 0.20,
    "quarantine_action": "remove_from_required_gates",
    "owner_assignment": "auto_from_codeowners",
    "re_enable_criteria": {
      "pass_rate_14d": 0.95,
      "owner_signoff_required": true
    }
  },
  "sla_hours": 48
}
```

#### Skip Policy

```json
{
  "policy_type": "skip",
  "rules": {
    "approval_required": true,
    "approvers": ["@release-manager", "@security-team"],
    "justification_template": "Skip: [category] — Reason: [text] — Approved by: [@github_handle]",
    "audit_log_retention_days": 365
  },
  "enforcement": "hard"
}
```

### Relationships

- **Many-to-many** with `TriggerContext` and `CheckCategory`: Policies apply to multiple contexts/checks
- **One-to-one** with `Owner` (for escalation policies)

### Validation Rules

- `policy_type == 'flake'` MUST have `sla_hours` defined
- `enforcement == 'hard'` REQUIRES `rules.approval_required == true` for skip policies

---

## Entity 4: Owner

Represents a team/area accountable for a check category or policy.

### Fields

| Field | Type | Required | Validation | Description |
|-------|------|----------|------------|-------------|
| `id` | string | ✅ | Unique identifier | e.g., `midi-team`, `platform-macos`, `security` |
| `name` | string | ✅ | Display name | e.g., "MIDI Team", "macOS Platform Maintainers" |
| `contact` | string | ✅ | Email or Slack channel | e.g., `#midi-dev`, `security@example.com` |
| `escalation_path` | string[] | ✅ | Ordered list of escalation contacts | e.g., `["@tech-lead", "@director"]` |
| `sla_default_hours` | number | ✅ | > 0 | Default SLA for owned items (overridden by specific policies) |
| `owned_check_categories` | string[] | ✅ | Array of CheckCategory IDs | Which checks this owner maintains |

### Relationships

- **One-to-many** with `CheckCategory`: An owner can own multiple check categories
- **One-to-many** with `Policy`: An owner can be responsible for multiple policies

### Validation Rules

- `owned_check_categories` MUST reference existing CheckCategory IDs
- `escalation_path` MUST have at least 1 contact

---

## Entity 5: TestRun

Represents a single execution of the test pipeline (CI job or local run).

### Fields

| Field | Type | Required | Validation | Description |
|-------|------|----------|------------|-------------|
| `id` | string | ✅ | Unique identifier | e.g., GitHub Actions run ID or local timestamp |
| `trigger_context_id` | string | ✅ | References TriggerContext | Which trigger initiated this run |
| `timestamp` | datetime | ✅ | ISO 8601 | When the run started |
| `duration_sec` | number | ✅ | > 0 | Total elapsed time |
| `platform` | enum | ✅ | `macos`, `windows`, `linux`, `ios`, `local` | Execution environment |
| `check_results` | CheckResult[] | ✅ | Non-empty array | Results per check category |
| `overall_status` | enum | ✅ | `pass`, `fail`, `flaky`, `skipped` | Aggregate result |
| `artifacts_url` | string | ❌ | Valid URL | Link to logs/artifacts (CI-specific) |

### Nested Type: CheckResult

```json
{
  "check_category_id": "unit-tests",
  "status": "pass",  // enum: pass, fail, skipped, timeout
  "duration_sec": 120,
  "details": {
    "tests_run": 456,
    "tests_passed": 456,
    "tests_failed": 0,
    "tests_skipped": 2
  },
  "failure_message": null  // populated if status == fail
}
```

### Relationships

- **Many-to-one** with `TriggerContext`: Each run belongs to one context
- **One-to-many** with `CheckResult`: Each run has results for multiple checks

### State Transitions

```
[Queued] → [Running] → [Completed]
              ↓
          [Cancelled]
```

### Validation Rules

- `check_results` MUST include all checks from `trigger_context.enabled_checks`
- `overall_status == 'pass'` IFF all `check_results[].status == 'pass'` (for blocking checks)

---

## Entity 6: QuarantineRecord

Tracks flaky checks that have been quarantined from required gates.

### Fields

| Field | Type | Required | Validation | Description |
|-------|------|----------|------------|-------------|
| `id` | string | ✅ | Unique identifier | Auto-generated UUID |
| `check_category_id` | string | ✅ | References CheckCategory | Which check is quarantined |
| `quarantine_date` | datetime | ✅ | ISO 8601 | When quarantine started |
| `assigned_owner_id` | string | ✅ | References Owner | Who is responsible for remediation |
| `trigger_run_ids` | string[] | ✅ | Array of TestRun IDs | Runs that triggered quarantine |
| `status` | enum | ✅ | `active`, `remediated`, `re_enabled` | Lifecycle state |
| `remediation_notes` | string | ❌ | - | Owner's investigation summary |
| `re_enable_date` | datetime | ❌ | ISO 8601 | When check returned to required gates |
| `sla_deadline` | datetime | ✅ | ISO 8601 | When remediation SLA expires |

### Relationships

- **Many-to-one** with `CheckCategory`: Multiple quarantine records can exist for same check (historical)
- **Many-to-one** with `Owner`: One owner per quarantine record
- **Many-to-many** with `TestRun`: Multiple runs can trigger quarantine

### State Transitions

```
[Active] → [Remediated] → [Re-enabled]
    ↓
[Abandoned] (if check deprecated before remediation)
```

### Validation Rules

- `re_enable_date` MUST be after `quarantine_date`
- `status == 're_enabled'` REQUIRES `re_enable_date` to be set
- `sla_deadline` MUST be `quarantine_date + policy.sla_hours`

---

## Relationships Diagram

```
CheckCategory ─1:N─ TriggerContext.enabled_checks
      │
      └─1:1─ Owner.owned_check_categories
      
TriggerContext ─1:N─ TestRun
      │
      └─M:N─ Policy.applies_to

TestRun ─1:N─ CheckResult ─M:1─ CheckCategory

QuarantineRecord ─M:1─ CheckCategory
                └─M:1─ Owner
```

---

## Validation Summary

1. **Check Category**: Blocking policy aligns with trigger contexts; timeout > expected duration
2. **Trigger Context**: Time budgets respect constitution limits (≤10 min p95 for PR)
3. **Policy**: Flake policies have SLAs; skip policies require approval
4. **Owner**: Owns at least one category; escalation path defined
5. **Test Run**: Results match enabled checks; overall status computed correctly
6. **Quarantine Record**: SLA deadline set; owner assigned; lifecycle states valid

---

## Notes for Implementation

- **Storage**: CI artifacts for TestRun history (JSON blobs); configuration files (YAML) for CheckCategory, TriggerContext, Policy, Owner
- **Persistence**: Git-versioned YAML for definitions; GitHub Actions artifacts for run history
- **Queries**: Filter TestRuns by platform, trigger, date range; aggregate pass rates for flake detection
