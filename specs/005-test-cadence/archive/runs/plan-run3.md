# Implementation Plan: Phase 11 — Testing Cadence & Governance (Run 3)

**Branch**: `005-test-cadence` | **Date**: 2025-11-12 | **Spec**: `/specs/005-test-cadence/spec.md`
**Input**: Feature specification from `/specs/005-test-cadence/spec.md`

## Summary

Define a comprehensive, cost-conscious testing cadence and governance model that delivers fast, deterministic PR feedback (≤5 min median), deeper scheduled assurance (nightly, release), and explicit flake governance and ownership. Balance cross-platform confidence (macOS, Windows, Linux) with developer velocity by running essential, parallelized checks on PRs and moving heavy suites (system, performance, packaging, installer smoke) to nightlies and release gates. Output amended constitutional recommendations to codify taxonomy, triggers, time budgets, platform coverage, flake policy, and skip/bypass governance.

## Technical Context

**Language/Version**: C++17 (JUCE 7.0.5), CMake ≥ 3.15; GitHub Actions YAML; Bash/PowerShell for CI scripts  
**Primary Dependencies**: JUCE framework; GitHub Actions runners (macos-latest, windows-latest, ubuntu-latest); Steinberg VST3 SDK (submodule)  
**Storage**: N/A (configuration and CI artifacts only)  
**Testing**: JUCE UnitTest + CTest; smoke/system scripts via CI; static analysis, formatting, linting via CI jobs  
**Target Platform**: macOS, Windows, Linux (PR=minimal macOS + conditional others; nightly/release=all)  
**Project Type**: Desktop audio plugin/tooling; CI/CD configuration and documentation feature  
**Performance Goals**: Real-time app latency <10ms (unchanged); CI: PR required checks median ≤5m, p95 ≤10m; deterministic re-run rate ≤2%  
**Constraints**: Determinism, parallelizable checks, cost control; heavy suites excluded from PR; flake quarantine with SLA  
**Scale/Scope**: Repo-wide CI and governance; affects all contributors and branches

Unknowns: NONE — clarified in spec (blocking policy=Balanced with branch-specific overrides; flake thresholds; pre-commit/pre-push split; nightly platform scope with change-detection skipping; PR budgets/time).

## Constitution Check

Gate pre-Phase 0:  
- Multi-Platform Architecture: PASS — PR runs stay lean; nightly/release cover all platforms.  
- JUCE Standards: PASS — No deviation to framework usage; testing infra only.  
- Real-Time Performance: PASS — No runtime changes; emphasis on CI determinism supports quality.  
- User-Centric Design: PASS — Fast, reliable feedback improves contributor UX.  
- Maintainability: PASS — Ownership, SLAs, and taxonomy codified; CI documented.  
- Development Workflow: PASS — Aligns with GitFlow and existing CI triggers.

Status: GATE PASSED (re-check after Phase 1 output).

## Project Structure

### Documentation (this feature)

```text
specs/005-test-cadence/
├── plan.003.md        # This file (/speckit.plan Run 3 output)
├── research.md        # Phase 0 output (decisions, rationale, alternatives)
├── data-model.md      # Phase 1 output (entities and relationships)
├── quickstart.md      # Phase 1 output (how to run checks locally/CI)
└── contracts/         # Phase 1 output (schemas/contracts for checks/policies)
```

### Source Code (repository root)

This feature updates CI/CD configuration and documentation; no source layout changes required. Existing CMake/JUCE build targets remain unchanged.

**Structure Decision**: Keep current repository structure; introduce CI workflow updates and governance docs only.

## Complexity Tracking

No constitutional violations requiring justification.

## Constitution Check (Post-Design)

Re-evaluation after Phase 1 artifacts:
- Multi-Platform Architecture: PASS — Nightly/release cover all platforms; PR lean coverage upheld.
- Real-Time Performance: PASS — No runtime changes; CI cadence supports quality.
- Maintainability: PASS — Owners, SLAs, schemas, and quickstart improve clarity.
- Development Workflow: PASS — Aligns with GitFlow and existing CI; adds explicit gates.

Status: GATE PASSED.
