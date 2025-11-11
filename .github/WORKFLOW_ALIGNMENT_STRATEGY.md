# GitHub Actions Workflow Alignment Strategy

**Last Updated**: 2025-11-11  
**Purpose**: Ensure consistency across workflows while allowing necessary divergence

## Workflow Inventory

| Workflow | Purpose | Triggers | Platform Builds | Code Quality |
|----------|---------|----------|-----------------|--------------|
| `ci.yml` | Main validation for PRs/pushes | PR (develop, main), push (main, release/*, hotfix/*) | macOS, Windows, Linux, iOS | ✅ Required |
| `test-build.yml` | Manual testing & PR validation | workflow_dispatch, PR (main, develop) on code changes | macOS, Windows, Linux | ✅ Required |
| `release.yml` | Production releases | Tags (v*.*.*), workflow_dispatch | macOS, Windows, Linux | ⚠️ Optional (pre-vetted) |
| `release-with-readme-update.yml` | Release + docs | Tags | macOS, Windows, Linux | ⚠️ Optional (pre-vetted) |

---

## Shared Components (MUST Align)

These elements should be **identical** across `ci.yml` and `test-build.yml`:

### 1. Code Quality Job

**Location**: Both `ci.yml` and `test-build.yml`

**Must Match**:
```yaml
jobs:
  code-quality:
    name: Code Quality Checks
    runs-on: ubuntu-latest
    
    steps:
      - name: Checkout code
        uses: actions/checkout@v4
        with:
          submodules: recursive
      
      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y build-essential pkg-config \
            libasound2-dev libfreetype6-dev libx11-dev \
            libxrandr-dev libxinerama-dev libxcursor-dev
      
      - name: Check GPL-3.0 headers
        run: |
          echo "Checking for GPL-3.0 license headers in source files..."
          missing_headers=0
          for file in $(find Source -name "*.cpp" -o -name "*.h"); do
            if ! grep -q "GNU General Public License" "$file"; then
              echo "❌ Missing GPL-3.0 header: $file"
              missing_headers=$((missing_headers + 1))
            fi
          done
          if [ $missing_headers -gt 0 ]; then
            echo "⚠️  Found $missing_headers files without GPL-3.0 headers"
            echo "This is a warning, not blocking the build"
          else
            echo "✅ All source files have GPL-3.0 headers"
          fi
      
      - name: Configure CMake
        run: |
          mkdir -p build
          cd build
          cmake .. -DCMAKE_BUILD_TYPE=Release
        env:
          SHOWMIDI_WERROR: "1"  # Enable warnings-as-errors for ShowMIDI code
      
      - name: Build (warnings as errors)
        run: |
          cd build
          cmake --build . --config Release
```

**Why**: Code quality standards must be consistent regardless of how builds are triggered.

---

### 2. Fail-Fast Dependencies

**All build jobs MUST include**:
```yaml
needs: code-quality  # Only run builds if code quality checks pass (fail-fast)
```

**Applies to**:
- ✅ `ci.yml`: build-and-test-macos, build-windows, build-linux, build-ios
- ✅ `test-build.yml`: test-macos, test-windows, test-linux
- ⚠️ `release.yml`: Optional (releases are pre-vetted, but could add for consistency)

**Why**: Prevents wasted runner minutes and failure emails when code quality fails.

---

### 3. Platform Build Steps

**macOS Build Steps (CMake-based)**:

Must align on:
- Runner: `macos-14` (JUCE 7.0.5 incompatibility with macOS 15)
- Build tool: CMake with Ninja generator
- Architecture: Universal binary `arm64;x86_64`
- Deployment target: `11.0`

```yaml
runs-on: macos-14  # Pin to macOS 14 - JUCE 7.0.5 doesn't support macOS 15 API deprecations

- name: Install dependencies
  run: brew install cmake ninja

- name: Configure CMake
  run: |
    cmake -B build -G Ninja \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0

- name: Build with CMake
  run: cmake --build build --config Release -j$(sysctl -n hw.ncpu)
```

**Windows Build Steps (MSBuild)**:

Must align on:
- Runner: `windows-latest`
- Build tool: MSBuild (Visual Studio 2022)
- Build order: SharedCode → VST3ManifestHelper → Standalone + VST3
- VST2 exclusion: Proprietary SDK not available

```yaml
runs-on: windows-latest

- name: Build with Visual Studio 2022
  run: |
    cd Builds\VisualStudio2022
    # Build shared code and helpers first
    msbuild ShowMIDI_SharedCode.vcxproj /p:Configuration=Release /p:Platform=x64
    msbuild ShowMIDI_VST3ManifestHelper.vcxproj /p:Configuration=Release /p:Platform=x64
    # Build plugin targets (skip VST2 - requires proprietary SDK)
    msbuild ShowMIDI_StandalonePlugin.vcxproj /p:Configuration=Release /p:Platform=x64
    msbuild ShowMIDI_VST3.vcxproj /p:Configuration=Release /p:Platform=x64
```

**Linux Build Steps (CMake)**:

Must align on:
- Runner: `ubuntu-latest`
- System dependencies: libasound2-dev, libx11-dev, libfreetype6-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev
- Build tool: CMake

```yaml
runs-on: ubuntu-latest

- name: Install dependencies
  run: |
    sudo apt-get update
    sudo apt-get install -y build-essential pkg-config cmake \
      libasound2-dev libfreetype6-dev libx11-dev \
      libxrandr-dev libxinerama-dev libxcursor-dev

- name: Configure CMake
  run: cmake -B build -DCMAKE_BUILD_TYPE=Release

- name: Build
  run: cmake --build build --config Release -j$(nproc)
```

**Why**: Platform build steps define core functionality and must produce identical artifacts.

---

### 4. Timeout Configuration

**Must align across all workflows**:

```yaml
build-and-test-macos:
  timeout-minutes: 30

build-windows:
  timeout-minutes: 25

build-linux:
  timeout-minutes: 20
```

**Why**: Timeout values are based on empirical measurements and protect against hung builds.

---

### 5. Artifact Uploads

**Must align on**:
- Retention period: `90 days`
- Artifact paths for each platform

```yaml
- name: Upload macOS artifacts
  uses: actions/upload-artifact@v4
  with:
    retention-days: 90
    if-no-files-found: ignore
    path: |
      build/**/*.app
      build/**/*.vst3
      build/**/*.component

- name: Upload Windows artifacts
  uses: actions/upload-artifact@v4
  with:
    retention-days: 90
    if-no-files-found: ignore
    path: |
      Builds/VisualStudio2022/x64/Release/**/*.exe
      Builds/VisualStudio2022/x64/Release/**/*.vst3

- name: Upload Linux artifacts
  uses: actions/upload-artifact@v4
  with:
    retention-days: 90
    if-no-files-found: ignore
    path: |
      build/ShowMIDI_artefacts/Release/**/*
```

**Why**: Artifact retention policy is a project-wide decision, not workflow-specific.

---

## Allowed Divergence

These elements are **intentionally different** across workflows:

### 1. Triggers

**`ci.yml`**:
- PRs to develop/main
- Pushes to main/release/*/hotfix/*
- paths-ignore for documentation

**`test-build.yml`**:
- workflow_dispatch (manual testing)
- PRs to main/develop (code changes only)
- paths filter for code files only

**`release.yml`**:
- Tags (v*.*.*)
- workflow_dispatch with version input

**Why**: Each workflow serves a different purpose in the development lifecycle.

---

### 2. Job Names

**`ci.yml`**: `build-and-test-macos`, `build-windows`, `build-linux`  
**`test-build.yml`**: `test-macos`, `test-windows`, `test-linux`  
**`release.yml`**: `build-macos`, `build-windows`, `build-linux`

**Why**: Job names communicate context (CI validation vs testing vs release).

---

### 3. Artifact Names

**`ci.yml`**: `ShowMIDI-macOS`, `ShowMIDI-Windows`, `ShowMIDI-Linux`  
**`test-build.yml`**: `test-build-macos-${{ github.run_number }}`, etc.  
**`release.yml`**: `ShowMIDI-macOS-${{ github.ref_name }}`, etc.

**Why**: Release artifacts need version tagging, test builds need run numbers to avoid conflicts.

---

### 4. Concurrency Control

**`ci.yml`**: Uses concurrency groups with cancel-in-progress  
**`test-build.yml`**: No concurrency control (manual testing)  
**`release.yml`**: No concurrency control (tagged releases are unique)

**Why**: Only CI needs rapid iteration cancellation.

---

## Maintenance Procedures

### When Making Changes

**1. Identify Change Scope**:
- [ ] Does this affect code quality checks? → Update BOTH `ci.yml` AND `test-build.yml`
- [ ] Does this affect build steps? → Update ALL workflows with that platform
- [ ] Does this affect only triggers? → Update single workflow only

**2. Update Checklist**:

When changing **code quality**:
- [ ] Update `ci.yml` code-quality job
- [ ] Update `test-build.yml` code-quality job
- [ ] Verify steps are identical (diff the jobs)

When changing **macOS build**:
- [ ] Update `ci.yml` build-and-test-macos
- [ ] Update `test-build.yml` test-macos
- [ ] Update `release.yml` build-macos (if present)
- [ ] Verify CMAKE_OSX_* flags match

When changing **Windows build**:
- [ ] Update `ci.yml` build-windows
- [ ] Update `test-build.yml` test-windows
- [ ] Update `release.yml` build-windows (if present)
- [ ] Verify MSBuild project order matches

When changing **Linux build**:
- [ ] Update `ci.yml` build-linux
- [ ] Update `test-build.yml` test-linux
- [ ] Update `release.yml` build-linux (if present)
- [ ] Verify system dependencies match

When changing **timeout values**:
- [ ] Update ALL workflows with matching platform jobs
- [ ] Document reason in commit message

When changing **artifact retention**:
- [ ] Update ALL workflows with artifact uploads
- [ ] Document retention policy change

**3. Validation**:
```bash
# Compare code-quality jobs
diff <(yq '.jobs.code-quality' .github/workflows/ci.yml) \
     <(yq '.jobs.code-quality' .github/workflows/test-build.yml)

# Compare macOS build steps
diff <(yq '.jobs.build-and-test-macos.steps[] | select(.name == "Configure CMake")' .github/workflows/ci.yml) \
     <(yq '.jobs.test-macos.steps[] | select(.name == "Configure")' .github/workflows/test-build.yml)
```

---

## Automated Alignment Checks

**GitHub Actions Validation** (future enhancement):

Create `.github/workflows/workflow-validation.yml`:
```yaml
name: Workflow Alignment Validation

on:
  pull_request:
    paths:
      - '.github/workflows/**'

jobs:
  validate-alignment:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      
      - name: Install yq
        run: |
          sudo wget -qO /usr/local/bin/yq https://github.com/mikefarah/yq/releases/latest/download/yq_linux_amd64
          sudo chmod +x /usr/local/bin/yq
      
      - name: Compare code-quality jobs
        run: |
          # Extract and compare code-quality jobs from ci.yml and test-build.yml
          # Fail if they differ
          
      - name: Validate timeout consistency
        run: |
          # Check that timeout-minutes match across workflows for same platforms
```

---

## Quick Reference

**Before Committing Workflow Changes**:

```bash
# 1. Check what changed
git diff .github/workflows/

# 2. If code-quality or build steps changed, verify both workflows
diff <(cat .github/workflows/ci.yml) <(cat .github/workflows/test-build.yml)

# 3. Test locally if possible
act -j code-quality  # Using nektos/act

# 4. Create PR and verify CI passes with new workflows
```

**Template for Workflow Change Commits**:
```
<type>(ci): <brief description>

Changes:
- ci.yml: <what changed>
- test-build.yml: <what changed>
- release.yml: <what changed> (if applicable)

Alignment status:
- [x] Code quality jobs match
- [x] Build steps verified
- [x] Timeouts consistent
- [x] Artifact retention aligned

Divergence (if any):
- <intentional differences>

Testing: <how validated>
```

---

## Rollout Plan for Existing Workflows

**Immediate** (already complete):
- ✅ Fail-fast pattern in `ci.yml`
- ✅ Fail-fast pattern in `test-build.yml`
- ✅ Code quality job in both workflows

**Next Steps**:
1. Audit `release.yml` for alignment opportunities
2. Add workflow validation automation
3. Document workflow-specific deviations in this file
4. Create pre-commit hook to warn about workflow changes

---

## Ownership & Review

**Workflow Changes Require**:
- Code review from at least 1 maintainer
- Validation that alignment rules are followed
- Test run on actual PR (not just syntax validation)

**This Document**:
- Updated whenever workflow alignment rules change
- Reviewed quarterly for accuracy
- Referenced in CONTRIBUTING.md

---

**Version**: 1.0  
**Effective Date**: 2025-11-11
