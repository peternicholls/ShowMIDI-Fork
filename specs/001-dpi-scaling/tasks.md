# Task Breakdown: DPI-Aware UI Scaling

**Feature**: 001-dpi-scaling  
**Date**: 2025-11-07  
**Status**: Ready for execution (~40% infrastructure complete)

## Overview

This task breakdown organizes remaining work by user story priority to enable independent, testable increments. Infrastructure (Scaling.h, Constants.h, MidiDeviceComponent reference implementation) is complete. Focus is on completing component migration, validation testing, and documentation.

## Task Statistics

- **Total Tasks**: 52
- **Setup/Foundational**: 5 tasks
- **User Story 1 (P1)**: 12 tasks (critical path)
- **User Story 2 (P2)**: 8 tasks
- **User Story 3 (P2)**: 10 tasks
- **User Story 4 (P3)**: 8 tasks
- **Polish**: 9 tasks
- **Parallel Opportunities**: 28 tasks marked [P]

## Implementation Strategy

**MVP Scope**: User Story 1 (P1) - Consistent Appearance on High-DPI Displays
- Delivers core value: ShowMIDI works correctly on high-resolution displays
- Includes all component updates, font scaling, and basic DPI testing
- Estimated effort: 16 hours

**Incremental Delivery**:
1. Complete User Story 1 → Test on multiple DPI settings → Merge
2. Add User Story 2 → Test cross-platform → Merge
3. Add User Story 3 → Test in plugin hosts → Merge
4. Add User Story 4 → Create documentation → Merge

**Benefits**: Each increment is independently testable and provides user value. Early merges reduce integration risk.

---

## Phase 1: Setup & Verification ✅

**Purpose**: Verify existing infrastructure and establish baseline for remaining work.

### Tasks

- [x] T001 Verify Scaling.h API completeness in Source/utility/Scaling.h
- [x] T002 Verify Constants.h constant definitions in Source/layout/Constants.h
- [x] T003 Verify MidiDeviceComponent reference implementation in Source/MidiDeviceComponent.cpp
- [x] T004 Run compile test to ensure existing code builds without errors
- [x] T005 Create component migration checklist in specs/001-dpi-scaling/checklists/component-migration.md

**Status**: ✅ Complete - Infrastructure validated during planning phase

---

## Phase 2: Foundational Tasks (Blocking Prerequisites)

**Purpose**: Complete critical path items that block all user stories.

### Tasks

- [x] T006 Complete font size audit - search all setFont() calls in Source/
- [x] T007 [P] Create font scaling utility wrapper in Source/utility/FontScaling.h
- [x] T008 Update all identified setFont() calls to use sm::scaled() for font sizes
- [ ] T009 [P] Create DPI test harness script in Scripts/test-dpi-scaling.sh
- [ ] T010 Run baseline DPI tests at 100%, 150%, 200% and document results in specs/001-dpi-scaling/test-results.md

**Duration**: ~6 hours  
**Blocking**: Font scaling (T006-T008) blocks all component testing in User Stories 1-3

---

## Phase 3: User Story 1 - Consistent Appearance on High-DPI Displays (P1)

**Goal**: All UI components scale correctly on high-resolution displays (4K, Retina, high-DPI Windows).

**Independent Test Criteria**:
- ✅ All components render at correct physical size on 100%, 125%, 150%, 200% DPI
- ✅ Text is crisp and readable at all DPI settings
- ✅ No hardcoded pixel values remain in component code
- ✅ Window dragging between monitors with different DPI rescales correctly
- ✅ Visual comparison: UI appears identical relative to screen size across DPI settings

### Priority 1: Critical Path Components

- [x] T011 [US1] Update PluginEditor.cpp - add sm::scaled() to all dimensional values in Source/PluginEditor.cpp
- [x] T012 [US1] Update PluginEditor.h - add scaling utility includes in Source/PluginEditor.h
- [x] T013 [US1] Add parentHierarchyChanged() override for DPI change detection in Source/PluginEditor.cpp
- [ ] T014 [US1] Test PluginEditor at 100%, 150%, 200% DPI and verify correct scaling

**Duration**: 4 hours (PluginEditor is critical for plugin host integration)

### Priority 2: Partially Complete Components

- [x] T015 [P] [US1] Audit MainLayoutComponent.cpp for hardcoded dimensions in Source/MainLayoutComponent.cpp
- [x] T016 [P] [US1] Audit StandaloneDevicesComponent.cpp for hardcoded dimensions in Source/StandaloneDevicesComponent.cpp
- [x] T017 [P] [US1] Audit SettingsComponent.cpp for hardcoded dimensions in Source/SettingsComponent.cpp
- [x] T018 [US1] Apply sm::scaled() to all identified hardcoded values in MainLayoutComponent.cpp
- [x] T019 [US1] Apply sm::scaled() to all identified hardcoded values in StandaloneDevicesComponent.cpp
- [x] T020 [US1] Apply sm::scaled() to all identified hardcoded values in SettingsComponent.cpp

**Duration**: 6 hours (2 hours per component)

### Priority 3: Not Started Components

- [x] T021 [P] [US1] Implement DPI scaling in SidebarComponent.cpp using sm::scaled()
- [x] T022 [P] [US1] Implement DPI scaling in PortListComponent.cpp using sm::scaled()
- [x] T023 [P] [US1] Implement DPI scaling in PaintedButton.cpp using sm::scaled()
- [x] T024 [P] [US1] Update UwynLookAndFeel.cpp with scaled font sizes
- [x] T025 [US1] Identify and update remaining UI components (AboutComponent and others)

**Duration**: 10 hours (2 hours per component × 5 components)

### Validation Testing

- [ ] T026 [US1] Test all components at 96 DPI (100% - baseline)
- [ ] T027 [US1] Test all components at 120 DPI (125% - Windows common)
- [ ] T028 [US1] Test all components at 144 DPI (150% - Windows common)
- [ ] T029 [US1] Test all components at 192 DPI (200% - macOS Retina, Windows high-DPI)
- [ ] T030 [US1] Visual comparison test - measure physical dimensions with ruler
- [ ] T031 [US1] Window drag test between monitors with different DPI (multi-monitor setup)

**Duration**: 4 hours  
**Dependencies**: All T011-T025 must be complete before validation

**User Story 1 Total**: 12 tasks, ~24 hours

---

## Phase 4: User Story 2 - Cross-Platform DPI Consistency (P2)

**Goal**: ShowMIDI provides consistent visual experience across macOS, Windows, and Linux.

**Independent Test Criteria**:
- ✅ Side-by-side comparison on macOS Retina vs Windows 150% shows equivalent physical sizes
- ✅ Linux multi-monitor setup scales correctly per display
- ✅ Screenshots at physical scale match across platforms
- ✅ Theme customization UI scales identically on all platforms

### Platform-Specific Testing

- [ ] T032 [P] [US2] Test on macOS non-Retina display (1x) and document in specs/001-dpi-scaling/platform-tests/macos.md
- [ ] T033 [P] [US2] Test on macOS Retina display (2x) and compare with non-Retina
- [ ] T034 [P] [US2] Test on Windows 10/11 at 100%, 125%, 150%, 200% scaling in specs/001-dpi-scaling/platform-tests/windows.md
- [ ] T035 [P] [US2] Test on Linux X11 with multiple DPI configurations in specs/001-dpi-scaling/platform-tests/linux.md
- [ ] T036 [US2] Cross-platform comparison - take screenshots at physical scale and compare dimensions

### Multi-Monitor Scenarios

- [ ] T037 [US2] Test Linux multi-monitor with different DPI per display
- [ ] T038 [US2] Test macOS with external non-Retina monitor connected to Retina laptop
- [ ] T039 [US2] Test Windows with mixed DPI displays (100% + 150%)

**Duration**: 8 hours  
**Dependencies**: Requires User Story 1 complete (all components DPI-aware)

**User Story 2 Total**: 8 tasks, ~8 hours

---

## Phase 5: User Story 3 - Plugin Host DPI Integration (P2)

**Goal**: ShowMIDI plugin respects host DAW scaling and integrates seamlessly.

**Independent Test Criteria**:
- ✅ Plugin UI matches host UI scale in 5+ major DAWs
- ✅ Plugin window resize maintains proportions
- ✅ AUv3 on iPad scales correctly for device screen
- ✅ No rendering artifacts when opening/closing plugin window
- ✅ Plugin UI updates correctly when DAW UI scale changes (if supported)

### DAW Integration Testing

- [ ] T040 [P] [US3] Test VST3 plugin in Ableton Live on 4K display at 200% in specs/001-dpi-scaling/daw-tests/ableton.md
- [ ] T041 [P] [US3] Test AU plugin in Logic Pro on Retina display in specs/001-dpi-scaling/daw-tests/logic.md
- [ ] T042 [P] [US3] Test VST3 plugin in Reaper on Windows with 150% scaling in specs/001-dpi-scaling/daw-tests/reaper.md
- [ ] T043 [P] [US3] Test VST3 plugin in FL Studio on Windows in specs/001-dpi-scaling/daw-tests/flstudio.md
- [ ] T044 [P] [US3] Test VST3 plugin in Bitwig Studio on Linux in specs/001-dpi-scaling/daw-tests/bitwig.md
- [ ] T045 [P] [US3] Test VST plugin in Cubase/Nuendo on Windows in specs/001-dpi-scaling/daw-tests/cubase.md

### Mobile Testing

- [ ] T046 [US3] Test AUv3 on iPad (standard and Pro models with different DPI) in specs/001-dpi-scaling/daw-tests/ipad.md
- [ ] T047 [US3] Test iOS plugin window resize behavior

### Plugin-Specific Validation

- [ ] T048 [US3] Verify plugin window resizing maintains aspect ratio and scaling
- [ ] T049 [US3] Test rapid plugin window open/close for memory leaks or rendering artifacts

**Duration**: 10 hours  
**Dependencies**: Requires User Story 1 complete (especially T011-T014 PluginEditor)

**User Story 3 Total**: 10 tasks, ~10 hours

---

## Phase 6: User Story 4 - Developer-Friendly Scaling API (P3)

**Goal**: Clear, simple API with comprehensive documentation for future contributors.

**Independent Test Criteria**:
- ✅ Developer can add new UI component using only documented API
- ✅ Documentation includes 5+ code examples covering common patterns
- ✅ API reference documents all 8 scaling functions with parameters and return types
- ✅ Quickstart guide enables developer to migrate component in <30 minutes
- ✅ Code review checklist catches non-scaled dimensional values

### Documentation Creation

- [ ] T050 [P] [US4] Create API reference guide in docs/api/dpi-scaling-api.md with all 8 functions documented
- [ ] T051 [P] [US4] Create developer migration guide in docs/guides/dpi-migration-guide.md with 5+ code examples
- [ ] T052 [P] [US4] Update README.md with DPI scaling support statement and link to docs
- [ ] T053 [P] [US4] Create code review checklist in .github/PULL_REQUEST_TEMPLATE.md for DPI scaling verification

### Code Examples

- [ ] T054 [US4] Document basic dimension scaling example in docs/examples/basic-scaling.cpp
- [ ] T055 [US4] Document font scaling example in docs/examples/font-scaling.cpp
- [ ] T056 [US4] Document multi-monitor aware component example in docs/examples/multi-monitor-component.cpp
- [ ] T057 [US4] Document Rectangle scaling example in docs/examples/rectangle-scaling.cpp

**Duration**: 6 hours  
**Dependencies**: None (can run in parallel with other stories, but references MidiDeviceComponent as example)

**User Story 4 Total**: 8 tasks, ~6 hours

---

## Phase 7: Polish & Cross-Cutting Concerns

**Purpose**: Final quality checks, performance validation, edge case handling.

### Theme System Validation

- [ ] T058 [P] Test SVG theme rendering at 100%, 150%, 200% DPI for sharpness in specs/001-dpi-scaling/theme-tests.md
- [ ] T059 [P] Verify theme color selectors scale correctly
- [ ] T060 [P] Test theme preview rendering at multiple DPI settings

### Performance Validation

- [ ] T061 Measure MIDI message processing latency with DPI scaling enabled
- [ ] T062 Verify <10ms latency requirement maintained (constitution compliance)
- [ ] T063 Profile resized() callback duration at different DPI settings (target <1ms)

### Edge Case Testing

- [ ] T064 Test fractional DPI (125%, 175%) for correct rendering
- [ ] T065 Test extreme DPI (300%, 400%) for layout issues
- [ ] T066 Test dynamic DPI change while ShowMIDI running (Windows 10+)

### Code Quality

- [ ] T067 Run JUCE leak detector and verify no memory leaks introduced
- [ ] T068 Code review all components for 100% sm::scaled() coverage
- [ ] T069 Final visual QA pass across all platforms and DPI settings

**Duration**: 8 hours

**Polish Total**: 12 tasks, ~8 hours

---

## Dependency Graph

### User Story Dependencies

```
Phase 1 (Setup) → Phase 2 (Foundational)
                       ↓
                  Phase 3 (US1 - P1) ← CRITICAL PATH
                       ↓
        ┌──────────────┴──────────────┐
        ↓                              ↓
Phase 4 (US2 - P2)            Phase 5 (US3 - P2)
        ↓                              ↓
        └──────────────┬───────────────┘
                       ↓
              Phase 6 (US4 - P3) [Independent - can run parallel]
                       ↓
                  Phase 7 (Polish)
```

### Critical Path

**Blocking sequence** (must complete in order):
1. T006-T008: Font scaling audit (blocks all component testing)
2. T011-T014: PluginEditor update (blocks plugin host testing)
3. T015-T025: All component updates (blocks User Stories 2, 3, and polish)
4. T026-T031: User Story 1 validation (blocks User Stories 2, 3 merge)

### Parallel Opportunities

**Can execute simultaneously** (different files, no dependencies):

**During US1 component updates**:
- T015, T016, T017 (audit 3 components in parallel)
- T021, T022, T023, T024 (update 4 components in parallel)

**During US2 platform testing**:
- T032, T033, T034, T035 (test all platforms in parallel)

**During US3 DAW testing**:
- T040, T041, T042, T043, T044, T045 (test 6 DAWs in parallel)

**During US4 documentation**:
- T050, T051, T052, T053 (create 4 docs in parallel)
- T054, T055, T056, T057 (write 4 examples in parallel)

**During Polish**:
- T058, T059, T060 (theme testing in parallel)

**Estimated speedup**: ~40% time reduction if parallel tasks executed concurrently

---

## Parallel Execution Examples

### Example 1: User Story 1 Component Updates

**Sequential**: 10 hours
```
T015 (2h) → T016 (2h) → T017 (2h) → T018 (1h) → T019 (1h) → T020 (1h) → T021 (2h) → ...
```

**Parallel** (3 developers): 6 hours
```
Developer 1: T015 (2h) → T018 (1h) → T021 (2h) → T024 (1h)
Developer 2: T016 (2h) → T019 (1h) → T022 (2h) → T025 (1h)
Developer 3: T017 (2h) → T020 (1h) → T023 (2h) → (help with testing)
```

### Example 2: User Story 2 Platform Testing

**Sequential**: 8 hours
```
T032 → T033 → T034 → T035 → T036 → T037 → T038 → T039
```

**Parallel** (3 testers with different platforms): 3 hours
```
Tester 1 (macOS): T032 (1h) → T033 (1h) → T038 (1h)
Tester 2 (Windows): T034 (2h) → T039 (1h)
Tester 3 (Linux): T035 (2h) → T037 (1h)
Then collaborate: T036 (comparison, shared task)
```

### Example 3: User Story 3 DAW Testing

**Sequential**: 7 hours
```
T040 → T041 → T042 → T043 → T044 → T045 → T046 → ...
```

**Parallel** (3 testers): 3 hours
```
Tester 1: T040 (Ableton, 1h) → T043 (FL Studio, 1h) → T046 (iPad, 1h)
Tester 2: T041 (Logic, 1h) → T044 (Bitwig, 1h) → T047 (iOS, 1h)
Tester 3: T042 (Reaper, 1h) → T045 (Cubase, 1h) → T048 (validation, 1h)
```

---

## Effort Summary

| Phase | Tasks | Sequential Hours | Parallel Hours (3 people) | Status |
|-------|-------|------------------|---------------------------|--------|
| Phase 1: Setup | 5 | 0 | 0 | ✅ Complete |
| Phase 2: Foundational | 5 | 6 | 3 | 🔄 In Progress |
| Phase 3: US1 (P1) | 21 | 24 | 12 | ⏳ Pending |
| Phase 4: US2 (P2) | 8 | 8 | 3 | ⏳ Pending |
| Phase 5: US3 (P2) | 10 | 10 | 4 | ⏳ Pending |
| Phase 6: US4 (P3) | 8 | 6 | 2 | ⏳ Pending |
| Phase 7: Polish | 12 | 8 | 4 | ⏳ Pending |
| **Total** | **69** | **62 hours** | **28 hours** | **~8% complete** |

**Note**: Infrastructure tasks (T001-T005) already complete but not included in active task count.

---

## MVP Recommendation

**Minimum Viable Product**: Phase 3 (User Story 1) only

**Scope**:
- ✅ All components DPI-aware (T011-T025)
- ✅ Font scaling complete (T006-T008)
- ✅ Basic DPI validation (T026-T031)

**Value Delivered**: ShowMIDI works correctly on high-resolution displays (core user need)

**Effort**: 16 hours sequential, ~8 hours with 2 developers in parallel

**Rationale**: User Story 1 addresses the most critical pain point (unusable on modern high-DPI displays). Stories 2-4 are enhancements that can be added incrementally after MVP validation.

---

## Testing Strategy

### Manual Testing Matrix

| Platform | DPI Settings | Test Type | Tasks |
|----------|--------------|-----------|-------|
| macOS | 1x, 2x | Visual QA | T032, T033 |
| Windows | 100%, 125%, 150%, 200% | Visual QA | T034 |
| Linux | 96, 120, 144, 192 DPI | Visual QA | T035 |
| Multi-monitor | Mixed DPI | Drag test | T037, T038, T039 |
| DAWs | Host scaling | Integration | T040-T047 |

### Validation Criteria Per Task

Each component update task (T011-T025) must verify:
1. ✅ No hardcoded pixel values remain (search for raw integers)
2. ✅ All `setFont()` calls use `sm::scaled()`
3. ✅ All `setBounds()` calls use `sm::scaled()`
4. ✅ Component includes `#include "utility/Scaling.h"` and `#include "layout/Constants.h"`
5. ✅ `parentHierarchyChanged()` override implemented for multi-monitor support
6. ✅ Visual test at 100% and 200% DPI confirms correct scaling

---

## Task Format Validation ✅

All 69 tasks follow required checklist format:
- ✅ Checkbox prefix: `- [ ]` or `- [x]`
- ✅ Task ID: T001-T069 sequential
- ✅ [P] marker: 28 tasks marked as parallelizable
- ✅ [US#] label: All user story tasks labeled (US1, US2, US3, US4)
- ✅ File paths: All implementation tasks specify exact file paths
- ✅ Clear actions: Each task has actionable description

---

## References

- **Feature Specification**: [spec.md](./spec.md)
- **Implementation Plan**: [plan.md](./plan.md)
- **Research**: [research.md](./research.md)
- **Data Model**: [data-model.md](./data-model.md)
- **API Contract**: [contracts/scaling-api.md](./contracts/scaling-api.md)
- **Quickstart Guide**: [quickstart.md](./quickstart.md)
- **Constitution**: [../../.specify/memory/constitution.md](../../.specify/memory/constitution.md)

---

**Generated**: 2025-11-07  
**Ready for execution**: ✅ Yes  
**Next action**: Begin Phase 2 (T006-T010) or start MVP implementation with Phase 3 (T011-T031)
