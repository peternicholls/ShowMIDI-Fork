# Testing Workflow Diagrams

**Purpose**: Visual reference for understanding ShowMIDI's testing infrastructure  
**Last Updated**: 2025-11-12

## Overview: Testing Triggers

```mermaid
flowchart TD
    A[Developer makes code change] --> B{What action?}
    B -->|git commit| C[Pre-commit Hook]
    B -->|git push| D[Pre-push Hook]
    B -->|Open PR| E[PR Workflow]
    
    C --> C1[clang-format auto-fix]
    C --> C2[GPL header check]
    C --> C3{Pass?}
    C3 -->|Yes| C4[Commit succeeds]
    C3 -->|No| C5[Commit blocked - fix issues]
    
    D --> D1[Run unit tests]
    D --> D2{All pass?}
    D2 -->|Yes| D3[Push succeeds]
    D2 -->|No| D4[Push blocked - fix tests]
    
    E --> E1[GitHub Actions: ci.yml]
    E1 --> E2[Config Loader]
    E2 --> E3[Code Quality Checks]
    E3 --> E4[Build & Test Matrix]
    E4 --> E5[Test Summary Report]
    E5 --> E6{All checks green?}
    E6 -->|Yes| E7[PR can merge]
    E6 -->|No| E8[Fix issues, push again]
    
    F[Scheduled 2 AM UTC] --> G[Nightly Workflow]
    G --> G1{Code changes?}
    G1 -->|No| G2[Skip - no changes]
    G1 -->|Yes| G3[Run extended suites]
    G3 --> G4[System tests all platforms]
    G3 --> G5[Performance tests]
    G3 --> G6[Security scans]
    G4 --> G7[Summary report]
    G5 --> G7
    G6 --> G7
```

## PR Validation Flow (ci.yml)

```mermaid
sequenceDiagram
    participant Dev as Developer
    participant GH as GitHub
    participant CL as config-loader
    participant CQ as code-quality
    participant Build as build jobs
    participant Test as test-suite
    participant Sum as test-summary
    
    Dev->>GH: Push to PR branch
    GH->>CL: Trigger ci.yml
    CL->>CL: Parse testing-governance.yaml
    CL->>CL: Extract timeouts, budgets
    CL-->>CQ: Output: unit_tests_timeout_sec
    
    CQ->>CQ: Install dependencies (Linux)
    CQ->>CQ: Check GPL headers
    CQ->>CQ: Configure & build (warnings as errors)
    CQ-->>Build: ✅ Code quality passed
    
    par Build all platforms
        Build->>Build: macOS build + test
        Build->>Build: Windows build
        Build->>Build: Linux build
    end
    
    Build-->>Test: ✅ Builds successful
    
    par Test all platforms (fail-fast)
        Test->>Test: macOS tests (primary)
        Test->>Test: Windows tests
        Test->>Test: Linux tests
    end
    
    Test->>Test: Generate test-run-summary.json
    Test-->>Sum: Upload artifacts
    
    Sum->>Sum: Download all test results
    Sum->>Sum: Parse CTest logs
    Sum->>Sum: Generate summary table
    Sum->>GH: Post PR comment
    Sum-->>Dev: 📊 Test results visible
```

## Nightly Validation Flow (nightly.yml)

```mermaid
flowchart TD
    A[Cron: 0 2 * * *] --> B[change-detection job]
    B --> C{Git diff develop vs last success}
    C -->|No changes| D[Skip entire workflow]
    C -->|Changes found| E[config-loader job]
    
    E --> F[Parse testing-governance.yaml]
    F --> G[Extract timeouts for system/perf tests]
    
    G --> H{Run extended suites}
    
    H --> I1[system-tests: macOS]
    H --> I2[system-tests: Windows]
    H --> I3[system-tests: Linux]
    
    H --> J1[performance-tests: macOS]
    H --> J2[performance-tests: Windows]
    H --> J3[performance-tests: Linux]
    
    H --> K1[full-platform-matrix: macOS]
    H --> K2[full-platform-matrix: Windows]
    H --> K3[full-platform-matrix: Linux]
    
    I1 --> L[nightly-summary job]
    I2 --> L
    I3 --> L
    J1 --> L
    J2 --> L
    J3 --> L
    K1 --> L
    K2 --> L
    K3 --> L
    
    L --> M{All jobs succeeded?}
    M -->|Yes| N[✅ Nightly passed - update last success]
    M -->|No| O[❌ Report failures - notify team]
```

## Platform Coverage Decision Tree

```mermaid
flowchart TD
    A[PR opened to develop] --> B{Trigger context?}
    B -->|pr-to-develop| C[Check platform_matrix config]
    
    C --> D[Primary: macos-latest ALWAYS runs]
    C --> E{Changed files contain platform-specific code?}
    
    E -->|JUCE_WINDOWS or Builds/VisualStudio| F[Run windows-latest tests]
    E -->|JUCE_LINUX or Builds/LinuxMakefile| G[Run ubuntu-latest tests]
    E -->|No platform markers| H[Skip conditional platforms]
    
    D --> I[All required checks]
    F --> I
    G --> I
    H --> I
    
    I --> J{fail-fast enabled?}
    J -->|Yes| K{First failure?}
    K -->|Yes| L[Cancel remaining jobs]
    K -->|No| M[Continue until first failure]
    
    J -->|No| N[Run all jobs to completion]
    
    B -->|nightly-develop| O[ALL platforms run unconditionally]
    O --> P[macOS, Windows, Linux in parallel]
    P --> Q[fail-fast: false - complete all]
```

## Check Category Time Budgets

```mermaid
gantt
    title PR Validation Timeline (5 min median, 10 min p95)
    dateFormat mm:ss
    axisFormat %M:%S
    
    section Code Quality
    GPL header check          :00:00, 30s
    CMake configure           :00:30, 60s
    Build (warnings=errors)   :01:30, 120s
    
    section Test Suite (Parallel)
    Build tests               :03:30, 60s
    Unit tests (timeout 180s) :04:30, 120s
    Integration tests (270s)  :04:30, 180s
    System tests (300s)       :04:30, 180s
    
    section Reporting
    Generate summaries        :07:30, 30s
    Upload artifacts          :08:00, 30s
    Post PR comment           :08:30, 30s
    
    section Target Budgets
    Median target             :milestone, 05:00, 0s
    P95 target                :milestone, 10:00, 0s
```

## Caching Strategy

```mermaid
flowchart LR
    A[CI Job Start] --> B{Cache Hit?}
    B -->|CMake cache exists| C[Restore CMakeCache.txt]
    B -->|No cache| D[Clean configure]
    
    C --> E[Incremental build ~2 min]
    D --> F[Full build ~5 min]
    
    E --> G[Run tests]
    F --> G
    
    G --> H[Save updated cache]
    H --> I[Upload artifacts]
    
    subgraph Cache Keys
        K1[cmake-build-HASH]
        K2[juce-modules-HASH]
        K3[ccache-SHA]
    end
    
    B -.->|Lookup| K1
    B -.->|Lookup| K2
    B -.->|Lookup| K3
```

## Test Artifact Flow

```mermaid
flowchart TD
    A[CTest execution] --> B[Generate LastTest.log]
    A --> C[Generate XML reports]
    
    B --> D[generate-run-summary.sh]
    D --> E[test-run-summary.json]
    
    E --> F[Upload artifact: test-results-PLATFORM-RUN]
    C --> F
    B --> F
    
    F --> G[test-summary job downloads all]
    G --> H[Parse all platforms]
    H --> I[Generate summary table]
    I --> J[Create PR comment]
    
    J --> K{PR event?}
    K -->|Yes| L[Post comment via github-script]
    K -->|No| M[Save as GITHUB_STEP_SUMMARY]
```

## Configuration-Driven Workflow

```mermaid
flowchart TD
    A[testing-governance.yaml] --> B[yq parser]
    B --> C{Extract values}
    
    C --> D[check_categories[].timeout_sec]
    C --> E[trigger_contexts[].time_budget_sec]
    C --> F[policies[].rules]
    
    D --> G[Set ctest --timeout]
    E --> H[Set job timeout-minutes]
    F --> I[Set retry/cache behavior]
    
    G --> J[CI job execution]
    H --> J
    I --> J
    
    J --> K{Job result}
    K -->|Success| L[Proceed to next job]
    K -->|Failure| M{Retry policy?}
    M -->|max_retries > 0| N[Retry with backoff]
    M -->|No retry| O[Fail job]
    
    N --> J
```

## Flake Detection (Planned - Phase 6)

```mermaid
flowchart TD
    A[Test execution complete] --> B[Check test result]
    B -->|Pass| C[Record pass in history]
    B -->|Fail| D[Record fail in history]
    
    C --> E{Track last 10 runs}
    D --> E
    
    E --> F{failures >= 3 in window?}
    F -->|No| G[Continue as normal]
    F -->|Yes| H[Flag as potentially flaky]
    
    H --> I{7-day failure rate >= 20%?}
    I -->|No| J[Monitor for trend]
    I -->|Yes| K[QUARANTINE]
    
    K --> L[Remove from required gates]
    K --> M[Assign to owner from CODEOWNERS]
    K --> N[Create tracking issue]
    K --> O[Set 48h SLA deadline]
    
    N --> P[Owner investigates]
    P --> Q{Root cause fixed?}
    Q -->|Yes| R[Monitor 14 days]
    Q -->|No| S[Escalate per SLA]
    
    R --> T{pass rate >= 95% over 14d?}
    T -->|Yes| U[Owner approves re-enable]
    T -->|No| V[Continue investigation]
    
    U --> W[Return to required gates]
```

## Legend

### Flow Shapes
- **Rectangle**: Process/Action
- **Diamond**: Decision point
- **Parallelogram**: Input/Output
- **Circle**: Start/End
- **Milestone**: Target/Checkpoint

### Colors (Mermaid themes)
- Green: Success path
- Red: Failure path
- Blue: Information/Data
- Yellow: Warning/Conditional

---

**Usage Tips**:
1. **For PRs**: Follow "PR Validation Flow" to understand why a check failed
2. **For Nightly**: Use "Nightly Validation Flow" to see what runs when
3. **For Timeouts**: Check "Check Category Time Budgets" to see if realistic
4. **For Caching**: Understand "Caching Strategy" to debug cache misses
5. **For Flakes**: Reference "Flake Detection" (when implemented) for quarantine logic
