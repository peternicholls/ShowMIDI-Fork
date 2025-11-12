# Phase 1 — Data Model (Run 3)

This feature defines governance and CI configuration artifacts rather than runtime data. The data model captures conceptual entities for documentation, reporting, and potential tooling.

## Entities

### CheckCategory
- id: string (slug, e.g., "unit", "integration")
- name: string
- purpose: string
- expectedDuration: string (e.g., "< 2m")
- blockingDefault: boolean
- severityPolicy: enum { none, advisory, block-high, block-all }
- ownerId: string (FK → Owner.id)

### Trigger
- id: string (slug, e.g., "pre-commit", "pr")
- event: enum { pre-commit, pre-push, pr, merge-develop, nightly, release, hotfix }
- scope: string (what runs under this trigger)
- timeBudget: object { median: duration, p95: duration }
- platforms: array<enum { macos, windows, linux }>

### Policy
- id: string
- type: enum { flake, skip-bypass, determinism, retry }
- rules: object (policy-specific)
- ownerId: string (FK → Owner.id)

### Owner
- id: string (area/team)
- displayName: string
- contact: string (GitHub team/handle)
- sla: string (e.g., "2 business days")

### Check
- id: string (CI job name)
- categoryId: string (FK → CheckCategory.id)
- triggerIds: array<string> (FK → Trigger.id)
- blocking: boolean (overrides per branch/trigger)
- platforms: array<enum { macos, windows, linux }>
- ownerId: string (FK → Owner.id)

### Run
- id: string (CI run id)
- checks: array<Check>
- branch: string
- commit: string (SHA)
- startedAt: datetime
- finishedAt: datetime
- result: enum { success, failure, flaky, quarantined }

### Gate
- id: string (e.g., "pr-required", "release-required")
- description: string
- requiredChecks: array<Check.id>

## Relationships
- Owner 1..* → CheckCategory, Policy, Check
- CheckCategory 1..* → Check
- Trigger *..* → Check (many-to-many)
- Gate *..* → Check (many-to-many)

## Validation Rules
- PR required checks must fit time budgets (median ≤ 5m, p95 ≤ 10m).
- Flaky checks (per policy thresholds) must be quarantined and removed from required gates.
- High/Critical security/license findings block (else advisory).
- Nightly runs must include all platforms unless skipped by change detection.

## State Transitions (Flake Governance)
- stable → flaky (meets threshold) → quarantined (auto) → fixed (owner sign-off) → stable (re-enabled in gates)
