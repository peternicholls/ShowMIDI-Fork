# Phase 0 — Research Consolidation (Run 3)

This document consolidates clarifications and decisions for Phase 11. All prior unknowns are resolved.

## Decisions

### 1) Taxonomy and Purposes
- Decision: Adopt explicit categories — unit, integration, system, performance, UI/visual regression, static analysis, formatting, linting, SCA/SAST, license checks, build verification, packaging, installer smoke.
- Rationale: Clear taxonomy enables blocking/advisory policy, ownership, and time budgets.
- Alternatives considered: Fewer categories (merge static analysis with lint/format) — rejected due to coarse-grained governance.

### 2) Triggers & Cadence
- Decision: Pre-commit (format+lint with auto-fix), pre-push (unit tests), PR (unit, integration core flows, static analysis, formatting/lint, build verification), on-merge to develop (re-validate required suite), nightly (system, performance, UI/visual regression; all platforms; change-detection skip), release branches (add packaging and installer smoke as blocking), hotfix (safety-critical subset, fast path).
- Rationale: Keeps PR cycles fast; depth is scheduled or gated at higher risk points.
- Alternatives considered: Run everything on PR — rejected due to runtime/cost; run nothing nightly — rejected due to missed regressions.

### 3) Time Budgets
- Decision: PR required checks median ≤ 5m, p95 ≤ 10m; heavy suites outside PR. Fail-fast on clear failures; allow-complete for borderline near budgets with alerting.
- Rationale: Developer velocity and predictability.
- Alternatives considered: Stricter ≤3m budget — rejected as unrealistic across platforms.

### 4) Determinism & Environment
- Decision: Require deterministic inputs (seeded randomness), isolated runners, pinned toolchain versions; no network flakiness (retry with backoff, bounded attempts). Re-run policy ≤ 2%.
- Rationale: Signal quality and trust.
- Alternatives considered: Best-effort determinism — rejected due to noise.

### 5) Flake Policy
- Decision: Flag flake at 3/10 failures OR ≥20% over 7 days; auto-quarantine from required gates; assign owner with SLA; re-enable at ≥95% pass over 14 days with owner sign-off.
- Rationale: Maintain trust without blocking progress.
- Alternatives considered: Manual-only quarantine — rejected as slow; auto-disable on first flake — too aggressive.

### 6) Blocking vs Advisory
- Decision: PR blocks on unit, integration (core), static analysis, formatting/linting, build verification. Advisory: UI/visual regression, SAST/SCA/license except High/Critical (blocking). Release/hotfix add packaging/installer smoke as blocking.
- Rationale: Balance risk and speed.
- Alternatives considered: All security advisory — rejected; all blocking — too slow.

### 7) Platform Coverage
- Decision: PR runs on macOS by default with conditional platform checks if platform code touched; nightly and release run on macOS, Windows, Linux; nightly can skip if no changes since last success.
- Rationale: Cross-platform parity without PR slowdown.
- Alternatives considered: Full tri-platform PR — rejected due to runtime/cost.

### 8) Ownership & Reporting
- Decision: Map checks to owners; track SLAs; aggregate human-readable summary with links per run.
- Rationale: Accountability and triage speed.
- Alternatives considered: Unowned checks — rejected.

### 9) Skip/Bypass Governance
- Decision: Require documented justification and approval; audit and report.
- Rationale: Prevent abuse; enable exceptions where warranted.
- Alternatives considered: Allow free-form skips — rejected.

### 10) Capacity & Cost Controls
- Decision: Parallelize jobs; shard tests; cache dependencies; schedule heavy suites; leverage public repo Actions minutes; prefer determinism and parallelism over raw single-job speed.
- Rationale: Efficiency and cost awareness.
- Alternatives considered: Single monolithic CI job — rejected.

### 11) Release/Hotfix Gates
- Decision: Release branches require full platform validation + packaging + installer smoke; hotfix runs safety-critical subsets with fast turnaround.
- Rationale: Production readiness and stability.
- Alternatives considered: Treat hotfix as normal PR — rejected (urgency differs).

### 12) Constitutional Recommendations (to be codified)
- Decision: Amend constitution to include testing cadence, definitions, time budgets, platform standards for testing, flake governance, and skip/bypass policies.
- Rationale: Make standards durable and enforceable.
- Alternatives considered: Keep in ad-hoc docs — rejected.

## Notes
- Implementation will update CI workflow(s) accordingly and add documentation for local hooks (pre-commit/pre-push) and PR expectations.
