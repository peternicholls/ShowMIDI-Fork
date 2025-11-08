# ShowMIDI GitFlow Workflow

**Version**: 1.0.0 | **Last Updated**: 2025-11-08

Based on **Atlassian SourceTree's GitFlow Workflow**, adapted for ShowMIDI's C++/JUCE architecture and fork structure.

## Fork Architecture Context

ShowMIDI development operates in a fork environment:
- **Upstream**: `gbevin/ShowMIDI` (original repository)
- **Origin**: `peternicholls/ShowMIDI-Fork` (development fork)

This workflow applies to the **origin fork** (`peternicholls/ShowMIDI-Fork`). Contributions to upstream follow separate procedures.

---

## Branch Structure

### `main` (Production) Branch

- **Purpose**: Stable releases for distribution
- **Protection**: Only `hotfix/*` and `release/*` branches may merge to `main`
- **Tagging**: All commits tagged with semantic version (e.g., `v1.0.1`) matching `CMakeLists.txt` and `.jucer` version
- **CI/CD**: Full multi-platform build suite runs on every push
- **Stability**: Always deployable; represents official release history

**Rules:**
- ✅ Merge from `release/*` branches (via PR)
- ✅ Merge from `hotfix/*` branches (via PR)
- ❌ Never merge features directly
- ❌ Never commit directly to `main`

---

### `develop` (Integration) Branch

- **Purpose**: Integration branch for features; preparation ground for next release
- **Protection**: Only `feature/*` branches may merge to `develop` (via PR)
- **History**: Contains complete development history
- **CI/CD**: No CI on direct pushes; validation happens at PR time
- **Quality**: Should always be in a buildable state across all platforms

**Rules:**
- ✅ Merge from `feature/*` branches (via PR with full CI validation)
- ✅ Merge back from `release/*` branches (when release includes fixes)
- ✅ Merge back from `hotfix/*` branches (to keep in sync with production)
- ❌ Never merge directly to `main`
- ⚠️ Keep up-to-date by pulling regularly

---

### `feature/*` (Feature) Branches

- **Naming**: `feature/<description>` or `feature/<number>-<description>` (e.g., `feature/001-dpi-scaling`)
- **Fork from**: `develop`
- **Merge into**: `develop` (via Pull Request)
- **Lifespan**: Duration of feature development
- **CI/CD**: Full validation when PR is opened to `develop`

**Purpose:**
- One branch per discrete feature
- Isolates development work from other features
- Never interacts directly with `main`

**Workflow:**

```bash
# Start a new feature
git checkout develop
git pull origin develop
git checkout -b feature/002-midi-export

# Work on feature, commit locally using conventional commits
git add .
git commit -m "feat(export): add MIDI file export functionality"

# Push to remote at any time for backup
git push origin feature/002-midi-export

# When ready, update from develop and test locally
git checkout develop
git pull origin develop
git checkout feature/002-midi-export
git merge develop

# Build and test on macOS (minimum validation)
cd Builds/MacOSX
xcodebuild -project ShowMIDI.xcodeproj -scheme ShowMIDI clean build

# Or use CMake
cd ../../
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# If tests pass, create PR to develop
# CI will run full multi-platform validation suite automatically
```

**If build fails:**
- Fix issues in the same branch, or
- Create `feature/<description>-v2` to restart cleanly

---

### `release/*` (Release Candidate) Branches

- **Naming**: `release/<version>` (e.g., `release/1.1.0`)
- **Fork from**: `develop`
- **Merge into**: `main` (via PR) **and** back to `develop`
- **Lifespan**: Duration of release stabilization
- **CI/CD**: Full validation on every push

**Purpose:**
- Temporary staging branch to prepare for production release
- Bug fixes, version bumps, and polish **only** – no new features
- Allows `develop` to continue receiving new features for next release

**Workflow:**

```bash
# Start release
git checkout develop
git pull origin develop
git checkout -b release/1.1.0
git push origin release/1.1.0  # ← CI runs immediately

# Bump version in CMakeLists.txt and showmidi.jucer
# Example: CMakeLists.txt line ~14
# project(ShowMIDI VERSION 1.1.0)

git commit -m "chore(release): bump version to 1.1.0"
git push  # ← CI validates every push

# Make release-specific fixes only
git commit -m "fix(ui): correct theme selector alignment on Windows"
git push

# When ready for production:
# 1. Create PR to main (CI validates again)
# 2. After merge, tag the release
git checkout main
git pull origin main
git tag -a v1.1.0 -m "Release 1.1.0"
git push origin v1.1.0

# 3. Merge changes back to develop
git checkout develop
git merge release/1.1.0
git push origin develop

# 4. Delete release branch
git branch -d release/1.1.0
git push origin --delete release/1.1.0
```

---

### `hotfix/*` (Production Fix) Branches

- **Naming**: `hotfix/<version>` (e.g., `hotfix/1.0.2`)
- **Fork from**: `main` (only branch that forks from production)
- **Merge into**: `main` (via PR) **and** `develop`
- **Lifespan**: Duration of critical fix
- **CI/CD**: Full validation on every push

**Purpose:**
- Urgent fixes to production release
- Bypasses normal release cycle for critical issues
- Maintains clean `develop` history

**Workflow:**

```bash
# Start hotfix
git checkout main
git pull origin main
git checkout -b hotfix/1.0.2
git push origin hotfix/1.0.2  # ← CI runs immediately

# Fix critical bug
git commit -m "fix(midi): resolve crash when disconnecting device during playback"
git push  # ← CI validates

# Bump patch version in CMakeLists.txt and .jucer
git commit -m "chore(hotfix): bump version to 1.0.2"
git push

# When ready:
# 1. Create PR to main (CI validates again)
# 2. After merge, tag the hotfix
git checkout main
git pull origin main
git tag -a v1.0.2 -m "Hotfix 1.0.2 - Device disconnect crash"
git push origin v1.0.2

# 3. Merge to develop to keep in sync
git checkout develop
git merge hotfix/1.0.2
git push origin develop

# 4. Delete hotfix branch
git branch -d hotfix/1.0.2
git push origin --delete hotfix/1.0.2
```

---

## Visual Branch Model

```
main (production)
  │
  ├─ v1.0.0 ────┬─ hotfix/1.0.1 ──→ v1.0.1 ──┐
  │             │                             ↓
  │             └─ release/1.1.0 ──→ v1.1.0 ──┤
  │                                            │
  ↓                                            ↓
develop ←──────────────────────────────────────┘
  │
  ├─ feature/001-dpi-scaling ──→ PR ──→ merge
  ├─ feature/002-midi-export ──→ PR ──→ merge
  └─ feature/003-vst3-support ──→ PR ──→ merge
```

---

## Commit Conventions

Follow [Conventional Commits](https://www.conventionalcommits.org/) for all commit messages:

### Format
```
<type>(<scope>): <subject>

[optional body]

[optional footer]
```

### Types
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code formatting (whitespace, indentation, etc.)
- `refactor`: Code restructuring without behavior change
- `perf`: Performance improvements
- `test`: Adding or updating tests
- `build`: Build system changes (CMake, Projucer)
- `ci`: CI/CD changes (GitHub Actions)
- `chore`: Maintenance tasks (deps, version bumps, etc.)

### Scopes (ShowMIDI-specific)
- `midi`: MIDI handling and device management
- `ui`: User interface components
- `theme`: Theme system and SVG handling
- `export`: MIDI export functionality
- `plugin`: Plugin-specific code (VST, AU, etc.)
- `settings`: Settings and preferences
- `juce`: JUCE framework integration
- `build`: Build configuration
- `release`: Release management
- `hotfix`: Hotfix-specific changes

### Examples
```bash
feat(midi): add MPE manager visualization
fix(ui): correct DPI scaling on 4K displays
docs(readme): update Linux build instructions
test(midi): add test for high-throughput MIDI streams
ci(workflow): add Windows build job
chore(release): bump version to 1.1.0
build(cmake): add CLAP plugin support
```

---

## Pull Request Process

### Creating a PR

1. **Ensure branch is up-to-date:**
   ```bash
   git checkout develop
   git pull origin develop
   git checkout feature/my-feature
   git merge develop
   ```

2. **Build and test locally (minimum: macOS):**
   ```bash
   # Xcode build
   cd Builds/MacOSX
   xcodebuild -project ShowMIDI.xcodeproj -scheme ShowMIDI clean build
   
   # OR CMake build
   cd ../..
   mkdir -p build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   ```

3. **Verify no JUCE leak detector warnings:**
   - Run the standalone app and perform basic operations
   - Check console for leak warnings
   - Test plugin version in a DAW if applicable

4. **Push and create PR:**
   ```bash
   git push origin feature/my-feature
   # Use GitHub web interface to create PR to develop
   ```

5. **CI validation runs automatically:**
   - Code quality checks
   - macOS build and basic validation
   - Windows build (cross-platform compatibility)
   - Linux build
   - iOS build (if applicable for AUv3)

### PR Requirements

- ✅ All CI jobs pass (green checks)
- ✅ No merge conflicts with target branch
- ✅ Conventional commit messages
- ✅ No JUCE leak detector warnings
- ✅ GPL-3.0 headers on new source files
- ✅ Constitution compliance (for architecture-significant changes)

### Merging Strategy

- **Feature → develop**: Squash and merge (clean history)
- **Release → main**: Merge commit (preserve release history)
- **Hotfix → main**: Merge commit (preserve fix history)

---

## Continuous Integration and Delivery

GitHub Actions pipeline (`.github/workflows/ci.yml`) validates code at strategic GitFlow integration points:

### CI Trigger Matrix

| Event | Branches | Jobs | Purpose |
|-------|----------|------|---------|
| Pull Request | `develop`, `main` | Full suite | Pre-merge validation gate |
| Push | `main` | Full suite | Production deployment validation |
| Push | `release/**` | Full suite | Release candidate verification |
| Push | `hotfix/**` | Full suite | Critical fix verification |
| Push | `develop` | None | Validated at PR time |
| Push | `feature/**` | None | Validated when PR opened |
| Manual | Any | Full suite | On-demand validation |

### Pipeline Jobs

1. **code-quality**: 
   - Compiler warnings (treat as errors for release builds)
   - JUCE leak detector validation
   - GPL-3.0 license header checks

2. **build-macos**: 
   - Full Xcode build (Standalone + all plugins)
   - Basic smoke test (launch app, verify UI)
   - macOS 11.7+ compatibility check

3. **build-windows**: 
   - Visual Studio 2022 build
   - VST/VST3 plugin validation
   - Windows 10+ compatibility check

4. **build-linux**: 
   - CMake build with system dependencies
   - VST/VST3/LV2 plugin validation
   - Verify no hardcoded paths

5. **build-ios** (optional):
   - AUv3 plugin build for iOS 12+
   - Simulator smoke test

### Cost Optimization

- ✅ **No CI waste**: Feature branches and develop don't trigger CI on push
- ✅ **Parallel builds**: Platform builds run concurrently
- ✅ **Concurrency control**: Old runs cancelled when new commits pushed
- ✅ **Quality gates**: Full validation at merge points (PRs, releases, hotfixes)
- ✅ **Caching**: Build artifacts and dependencies cached between runs

---

## Syncing with Upstream

Since this is a fork of `gbevin/ShowMIDI`, periodically sync with upstream:

```bash
# Add upstream remote (one-time setup, already done)
git remote add upstream git@github.com:gbevin/ShowMIDI.git

# Fetch upstream changes
git fetch upstream

# Sync main branch
git checkout main
git merge upstream/main
git push origin main

# Sync develop branch (be cautious of conflicts)
git checkout develop
git merge upstream/main  # or upstream/develop if it exists
git push origin develop
```

**When to sync:**
- Before starting a major feature
- Monthly (if active upstream development)
- When upstream releases a new version

**Conflict resolution:**
- Prefer upstream changes for core MIDI/JUCE code
- Prefer fork changes for .specify/, .github/, constitution
- Manually resolve feature conflicts

---

## Best Practices

### Daily Development

1. **Start your day**: Pull latest `develop`
   ```bash
   git checkout develop
   git pull origin develop
   ```

2. **Work in feature branches**: Never commit directly to `develop` or `main`

3. **Push frequently**: Backup work to remote, but PR only when ready

4. **Watch develop**: Keep your feature branch updated
   ```bash
   git checkout feature/my-feature
   git merge develop  # Resolve conflicts early
   ```

### Release Preparation

1. **Feature freeze**: Stop merging new features to `develop`
2. **Create release branch**: `release/X.Y.0` from `develop`
3. **Bump versions**: Update `CMakeLists.txt` (line ~14) and `showmidi.jucer` (Version field)
4. **Fix bugs only**: No new features during release stabilization
5. **Validate continuously**: CI runs on every push to release branch
6. **Update changelog**: Document changes in `CHANGELOG.md` (create if doesn't exist)
7. **Tag and deploy**: Merge to `main`, tag with version, merge back to `develop`

### Hotfix Protocol

1. **Assess severity**: Is this truly production-critical?
2. **Create hotfix branch**: From `main`, not `develop`
3. **Fix and validate**: CI runs on every push
4. **Bump patch version**: Update `CMakeLists.txt` and `.jucer`
5. **Merge to main**: Tag with patch version (e.g., `v1.0.2`)
6. **Sync develop**: Merge hotfix back to `develop`

### Version Management

ShowMIDI version is defined in **two locations** (must be kept in sync):

1. **CMakeLists.txt** (line ~14):
   ```cmake
   project(ShowMIDI VERSION 1.0.1)
   ```

2. **showmidi.jucer** (Projucer settings):
   ```xml
   <VERSION value="1.0.1"/>
   ```

**Semantic Versioning 2.0.0** ([semver.org](https://semver.org/spec/v2.0.0.html)):
- `MAJOR.MINOR.PATCH` (e.g., `1.0.1`)
- **MAJOR**: Breaking changes to file formats, plugin API, or UI paradigm
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes only

### Code Review Guidelines

- Review for correctness, not style (consistency with JUCE patterns is sufficient)
- Check that GPL-3.0 headers are present on new files
- Verify no JUCE leak detector warnings in testing
- Ensure multi-platform compatibility (no platform-specific code without guards)
- Validate against constitution (especially for architecture changes)
- Confirm conventional commit format

---

## Troubleshooting

### "CI failed on my PR"

1. Check which job failed in GitHub Actions tab
2. Read error logs (click on failed job)
3. Common issues:
   - **Compiler errors**: Check C++17 compliance, JUCE API usage
   - **Linker errors**: Missing library dependencies (check CMakeLists.txt)
   - **Leak warnings**: Use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR`
   - **Platform-specific**: Check ifdef guards for Windows/Linux/macOS

### "Merge conflict with develop"

```bash
git checkout feature/my-feature
git fetch origin
git merge origin/develop
# Resolve conflicts in editor
git add .
git commit -m "chore: resolve merge conflicts with develop"
git push
```

### "Need to update feature branch version"

```bash
git checkout develop
git pull origin develop
git checkout -b feature/my-feature-v2
# Continue work in new branch
```

### "Build fails on Linux but works on macOS"

```bash
# Test Linux build locally with Docker
docker run -it -v $(pwd):/workspace ubuntu:22.04
apt update && apt install -y build-essential pkg-config \
  libasound2-dev libfreetype6-dev libx11-dev libxrandr-dev \
  libxinerama-dev libxcursor-dev cmake git
cd /workspace
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### "JUCE leak detector warnings"

- Check all components use `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClassName)`
- Verify proper use of `std::unique_ptr` or `OwnedArray` for ownership
- Ensure cleanup in destructors
- Run in Debug mode for detailed leak locations

---

## References

- [Atlassian GitFlow Tutorial](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)
- [Vincent Driessen's Original Model](http://nvie.com/posts/a-successful-git-branching-model/)
- [Conventional Commits](https://www.conventionalcommits.org/)
- [JUCE Framework Documentation](https://juce.com/learn/documentation)
- [ShowMIDI Constitution](./.specify/memory/constitution.md)

---

**Version**: 1.0.0 | **Last Updated**: 2025-11-08 | **Maintained by**: peternicholls
