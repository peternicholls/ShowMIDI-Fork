# Testing Governance Configuration Schema

**Feature**: 005-test-cadence  
**Version**: 1.0.0  
**Format**: YAML

This schema defines the structure for testing governance configuration files that implement FR-001 through FR-016.

---

## File: `testing-governance.yaml`

Primary configuration defining check categories, trigger contexts, policies, and ownership.

### Schema

```yaml
version: "1.0.0"  # Schema version

check_categories:
  - id: unit-tests
    name: Unit Tests
    purpose: Validate individual component behavior in isolation
    tools:
      - JUCE UnitTest
      - CTest
    owner: core-team
    blocking_policy: always  # enum: always, conditional, never
    expected_duration_sec: 120
    timeout_sec: 180
    flake_threshold:
      failure_count_window: 10
      failure_threshold: 3
      failure_rate_7d: 0.20

  - id: integration-tests
    name: Integration Tests
    purpose: Validate component interactions and module boundaries
    tools:
      - JUCE UnitTest with fixtures
      - Mock adapters
    owner: core-team
    blocking_policy: conditional
    condition_expression: "trigger_context == 'pr-to-develop' AND files_changed contains 'Source/'"
    expected_duration_sec: 180
    timeout_sec: 270
    flake_threshold:
      failure_count_window: 10
      failure_threshold: 3
      failure_rate_7d: 0.20

  # ... (repeat for all 12 categories)

trigger_contexts:
  - id: pre-commit
    event_type: pre-commit
    scope: local
    enabled_checks:
      - formatting
      - linting
    time_budget_sec: 10
    time_budget_p95_sec: 15
    parallelization: sequential
    change_detection_enabled: false

  - id: pr-to-develop
    event_type: pull_request
    scope: develop
    enabled_checks:
      - unit-tests
      - integration-tests
      - static-analysis
      - formatting
      - linting
      - build-verification
      - security-sast  # only High/Critical
      - security-sca   # only High/Critical
    time_budget_sec: 300  # 5 min median
    time_budget_p95_sec: 600  # 10 min p95
    parallelization: matrix
    change_detection_enabled: false

  - id: nightly-develop
    event_type: nightly
    scope: develop
    enabled_checks:
      - unit-tests
      - integration-tests
      - system-tests
      - performance-tests
      - ui-visual-regression
      - static-analysis
      - formatting
      - linting
      - security-sast
      - security-sca
      - license-compliance
      - build-verification
    time_budget_sec: 3600  # 60 min
    time_budget_p95_sec: 4500  # 75 min
    parallelization: matrix
    change_detection_enabled: true

  # ... (repeat for release, hotfix, etc.)

policies:
  - id: flake-quarantine
    policy_type: flake
    applies_to:
      - "*"  # All trigger contexts
    rules:
      failure_count_window: 10
      failure_threshold: 3
      failure_rate_7d_threshold: 0.20
      quarantine_action: remove_from_required_gates
      owner_assignment: auto_from_codeowners
      re_enable_criteria:
        pass_rate_14d: 0.95
        owner_signoff_required: true
    enforcement: hard
    sla_hours: 48

  - id: skip-approval-required
    policy_type: skip
    applies_to:
      - pr-to-develop
      - pr-to-main
      - release-gate
    rules:
      approval_required: true
      approvers:
        - "@release-manager"
        - "@security-team"
      justification_template: "Skip: [category] — Reason: [text] — Approved by: [@github_handle]"
      audit_log_retention_days: 365
    enforcement: hard
    sla_hours: null

  - id: retry-on-infrastructure-failure
    policy_type: retry
    applies_to:
      - "*"
    rules:
      max_retries: 2
      retry_delay_sec: 30
      retry_on_exit_codes: [137, 143]  # OOM, timeout signals
      exponential_backoff: true
    enforcement: soft

  # ... (caching, reporting, escalation policies)

owners:
  - id: core-team
    name: Core Development Team
    contact: "#showmidi-dev"
    escalation_path:
      - "@tech-lead"
      - "@project-maintainer"
    sla_default_hours: 48
    owned_check_categories:
      - unit-tests
      - integration-tests
      - system-tests

  - id: platform-macos
    name: macOS Platform Maintainers
    contact: "#platform-macos"
    escalation_path:
      - "@macos-lead"
      - "@tech-lead"
    sla_default_hours: 72
    owned_check_categories:
      - build-verification  # shared ownership possible

  - id: security-team
    name: Security & Compliance
    contact: "security@example.com"
    escalation_path:
      - "@security-lead"
      - "@cto"
    sla_default_hours: 24  # Faster SLA for security
    owned_check_categories:
      - security-sast
      - security-sca
      - license-compliance
```

---

## File: `flake-tracking.json` (CI Artifact)

Runtime data tracking test run history for flake detection.

### Schema

```json
{
  "schema_version": "1.0.0",
  "last_updated": "2025-11-12T14:30:00Z",
  "checks": [
    {
      "check_category_id": "unit-tests",
      "history": [
        {
          "run_id": "gh-actions-12345",
          "result": "pass",
          "timestamp": "2025-11-12T10:00:00Z",
          "platform": "macos",
          "duration_sec": 118
        },
        {
          "run_id": "gh-actions-12346",
          "result": "fail",
          "timestamp": "2025-11-12T11:00:00Z",
          "platform": "macos",
          "duration_sec": 45,
          "failure_message": "Assertion failed: expected 42, got 41"
        }
      ],
      "failure_rate_7d": 0.15,
      "failure_count_10runs": 2,
      "status": "active",  // enum: active, quarantined
      "last_quarantine_check": "2025-11-12T14:00:00Z"
    }
  ],
  "quarantined_checks": [
    {
      "check_category_id": "ui-visual-regression",
      "quarantine_date": "2025-11-10T08:00:00Z",
      "assigned_owner_id": "core-team",
      "trigger_run_ids": ["gh-actions-12340", "gh-actions-12342", "gh-actions-12343"],
      "status": "active",
      "sla_deadline": "2025-11-12T08:00:00Z",
      "remediation_notes": "Investigating timing-dependent screenshot comparison"
    }
  ]
}
```

---

## File: `test-run-summary.json` (CI Artifact per run)

Report for a single test pipeline execution.

### Schema

```json
{
  "run_id": "gh-actions-12345",
  "trigger_context_id": "pr-to-develop",
  "timestamp": "2025-11-12T10:00:00Z",
  "duration_sec": 285,
  "platform": "macos",
  "overall_status": "pass",
  "check_results": [
    {
      "check_category_id": "unit-tests",
      "status": "pass",
      "duration_sec": 120,
      "details": {
        "tests_run": 456,
        "tests_passed": 456,
        "tests_failed": 0,
        "tests_skipped": 2
      },
      "failure_message": null
    },
    {
      "check_category_id": "integration-tests",
      "status": "pass",
      "duration_sec": 165,
      "details": {
        "tests_run": 89,
        "tests_passed": 89,
        "tests_failed": 0,
        "tests_skipped": 0
      },
      "failure_message": null
    }
  ],
  "artifacts_url": "https://github.com/user/repo/actions/runs/12345",
  "metrics": {
    "total_tests_run": 545,
    "total_tests_passed": 545,
    "cache_hit_rate": 0.92,
    "time_budget_utilization": 0.95  // 285 / 300 target
  }
}
```

---

## Validation Rules

1. **Schema Version**: All files MUST include `version` or `schema_version` field
2. **ID Uniqueness**: All `id` fields MUST be unique within their scope (check categories, triggers, policies, owners)
3. **Reference Integrity**: All `_id` references MUST point to existing entities
4. **Time Budgets**: `time_budget_p95_sec >= time_budget_sec` for all trigger contexts
5. **Flake Thresholds**: `failure_rate_7d` MUST be between 0.0 and 1.0
6. **Enum Values**: All enum fields MUST use defined values (see data-model.md)

---

## Usage

### Configuration Management

```bash
# Validate configuration
scripts/validate-testing-config.sh specs/005-test-cadence/contracts/testing-governance.yaml

# Deploy to CI
cp specs/005-test-cadence/contracts/testing-governance.yaml .github/testing-governance.yaml
git add .github/testing-governance.yaml
git commit -m "config(testing): deploy Phase 11 governance config"
```

### Runtime (GitHub Actions)

```yaml
# .github/workflows/ci.yml
jobs:
  load-config:
    runs-on: ubuntu-latest
    outputs:
      enabled_checks: ${{ steps.parse.outputs.enabled_checks }}
    steps:
      - uses: actions/checkout@v4
      - name: Parse testing config
        id: parse
        run: |
          yq '.trigger_contexts[] | select(.id == "pr-to-develop") | .enabled_checks' \
            .github/testing-governance.yaml > enabled_checks.json
          echo "enabled_checks=$(cat enabled_checks.json)" >> $GITHUB_OUTPUT

  unit-tests:
    needs: load-config
    if: contains(needs.load-config.outputs.enabled_checks, 'unit-tests')
    # ... run unit tests
```

---

## Migration from Current CI

1. **Audit existing `.github/workflows/ci.yml`**: Map current jobs to CheckCategory entities
2. **Define owners**: Use CODEOWNERS file as initial mapping
3. **Baseline flake tracking**: Analyze last 30 days of CI history to seed `flake-tracking.json`
4. **Incremental rollout**: Add new checks to nightly first, then PR after validation
