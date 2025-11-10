# Remote Testing Quick-Start Guide

**Feature**: 003-ci-build-fix  
**Date**: 2025-11-10

This quick-start guide helps you execute the comprehensive remote testing plan efficiently.

---

## Prerequisites

Install required tools:

```bash
# GitHub CLI (if not installed)
brew install gh

# YAML linter
brew install yamllint

# Markdown linter (optional)
npm install -g markdownlint-cli

# Authenticate with GitHub
gh auth login
```

---

## Quick Execution Path

### Option A: Full Testing (Recommended)

Execute all phases sequentially for complete validation:

```bash
# 1. Pre-flight checks (5 minutes)
yamllint .github/workflows/*.yml
mkdir -p build_test && cd build_test && cmake .. && cd .. && rm -rf build_test

# 2. Baseline testing (2 hours)
# Execute tests 1.1 through 1.8 from REMOTE_TESTING_PLAN.md

# 3. Fix iterations (as needed)
# Address any failures using Phase 2 workflow

# 4. Performance benchmarking (1 hour)
# Execute benchmarks 3.1 through 3.5

# 5. Edge case testing (2 hours)
# Execute edge cases 4.1 through 4.5

# 6. Production monitoring (30 days)
# Set up monitoring scripts

# 7. Final validation (1 day)
# Complete Phase 6 checklist
```

**Timeline**: ~1 week + 30 days monitoring

---

### Option B: Critical Path Only (MVP)

Execute only the most critical tests for rapid validation:

```bash
# 1. Pre-flight checks (5 minutes)
yamllint .github/workflows/ci.yml

# 2. Critical tests only (1 hour)
# - Test 1.1: Trivial Code Change PR
# - Test 1.2: Documentation-Only PR
# - Test 1.6: Xcode Adaptive Selection

# 3. Fix any critical failures

# 4. Quick performance check
# - Verify build times reasonable
# - Verify doc PRs skip builds

# 5. Deploy to production
```

**Timeline**: 2-3 hours for MVP validation

---

## Test Execution Scripts

### Script 1: Create Test PR

```bash
#!/bin/bash
# create-test-pr.sh <test-number> <description> <file-to-change>

TEST_NUM=$1
DESCRIPTION=$2
FILE=$3

git checkout 003-ci-build-fix
git pull origin 003-ci-build-fix
git checkout -b "test/ci-validation-$(printf "%03d" $TEST_NUM)-${DESCRIPTION}"

echo "// CI validation test $TEST_NUM" >> "$FILE"
git add "$FILE"
git commit -m "test(ci): validation test $TEST_NUM - $DESCRIPTION"
git push origin "test/ci-validation-$(printf "%03d" $TEST_NUM)-${DESCRIPTION}"

gh pr create --base 003-ci-build-fix \
  --title "test(ci): Validation Test $TEST_NUM - $DESCRIPTION" \
  --body "Testing: $DESCRIPTION

Expected behavior: [Fill in]

Validation for: #003-ci-build-fix"
```

**Usage**:
```bash
chmod +x create-test-pr.sh
./create-test-pr.sh 1 "trivial-code-change" "Source/Main.cpp"
```

---

### Script 2: Monitor Test Results

```bash
#!/bin/bash
# monitor-test.sh <branch-name>

BRANCH=$1

echo "Monitoring CI for branch: $BRANCH"
echo "Press Ctrl+C to stop"
echo ""

gh pr checks "$BRANCH" --watch
```

**Usage**:
```bash
chmod +x monitor-test.sh
./monitor-test.sh test/ci-validation-001-trivial-code-change
```

---

### Script 3: Collect Performance Metrics

```bash
#!/bin/bash
# collect-metrics.sh <run-id>

RUN_ID=$1

echo "=== Performance Metrics for Run $RUN_ID ==="
echo ""

# Extract job timing
gh run view "$RUN_ID" --json jobs | jq -r '.jobs[] | 
  "\(.name): \(.startedAt) → \(.completedAt)"'

echo ""
echo "=== Artifacts ==="
gh run view "$RUN_ID" --json artifacts | jq -r '.artifacts[] | 
  "\(.name) - \(.size_in_bytes) bytes - expires \(.expiresAt)"'

echo ""
echo "=== Build Summary ==="
gh run view "$RUN_ID" --json conclusion,status | jq '.'
```

**Usage**:
```bash
chmod +x collect-metrics.sh
./collect-metrics.sh 19198918833
```

---

### Script 4: Automated Test Suite

```bash
#!/bin/bash
# run-test-suite.sh

echo "=== CI/CD Remote Testing Suite ==="
echo "Starting automated test execution..."
echo ""

# Array of tests
declare -a tests=(
  "1:trivial-code-change:Source/Main.cpp"
  "2:doc-only-change:README.md"
)

for test in "${tests[@]}"; do
  IFS=':' read -r num desc file <<< "$test"
  
  echo "=== Running Test $num: $desc ==="
  ./create-test-pr.sh "$num" "$desc" "$file"
  
  # Wait for CI to start
  sleep 30
  
  # Monitor (will need manual Ctrl+C)
  ./monitor-test.sh "test/ci-validation-$(printf "%03d" $num)-${desc}"
  
  echo ""
  read -p "Test $num complete. Continue to next? (y/n) " -n 1 -r
  echo
  if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Test suite paused. Re-run to continue."
    exit 0
  fi
done

echo "=== All tests complete! ==="
```

**Usage**:
```bash
chmod +x run-test-suite.sh
./run-test-suite.sh
```

---

## Common Tasks

### Check CI Status

```bash
# List recent runs
gh run list --workflow ci.yml --limit 10

# Check specific PR
gh pr checks <pr-number>

# View run details
gh run view <run-id>

# Download run logs
gh run view <run-id> --log > run-logs.txt
```

### Analyze Failures

```bash
# Get failed runs
gh run list --workflow ci.yml --status failure --limit 10

# View failure logs
gh run view <run-id> --log-failed

# Extract error messages
gh run view <run-id> --log | grep -i "error\|fail\|fatal"
```

### Performance Analysis

```bash
# Calculate average build time
gh run list --workflow ci.yml --limit 10 --json startedAt,updatedAt | \
  jq -r '.[] | (.updatedAt | fromdateiso8601) - (.startedAt | fromdateiso8601)' | \
  awk '{sum+=$1; count++} END {print "Average:", sum/count, "seconds"}'
```

### Clean Up Test Branches

```bash
# List test branches
git branch -r | grep "test/ci-validation"

# Delete local test branches
git branch | grep "test/ci-validation" | xargs git branch -D

# Delete remote test branches
git branch -r | grep "test/ci-validation" | \
  sed 's/origin\///' | xargs -I {} git push origin --delete {}
```

---

## Troubleshooting

### Issue: YAML syntax error

```bash
# Validate YAML
yamllint .github/workflows/ci.yml

# Common issues:
# - Incorrect indentation (use 2 spaces)
# - Missing quotes around special characters
# - Invalid GitHub Actions syntax
```

### Issue: CMake configuration fails

```bash
# Test locally first
mkdir -p build_test
cd build_test
cmake .. -DCMAKE_BUILD_TYPE=Release
# Review error messages
cd ..
rm -rf build_test
```

### Issue: Workflow not triggering

```bash
# Check workflow triggers
cat .github/workflows/ci.yml | grep -A10 "^on:"

# Verify branch name matches trigger
git branch --show-current

# Force trigger
gh workflow run ci.yml --ref $(git branch --show-current)
```

### Issue: Artifacts not uploading

```bash
# Check artifact paths exist
ls -R Builds/MacOSX/build/Release/
ls -R Builds/VisualStudio2022/x64/Release/
ls build/

# Verify upload action syntax
cat .github/workflows/ci.yml | grep -A10 "upload-artifact"
```

---

## Success Criteria Checklist

Quick reference for validation:

- [ ] **SC-001**: 100% CI success rate (monitor 30 days)
- [ ] **SC-002**: CMake config <2min (benchmark)
- [ ] **SC-003**: macOS builds <15min (benchmark)
- [ ] **SC-004**: Windows builds <20min (benchmark)
- [ ] **SC-005**: Doc-only PRs <30s (test 1.2)
- [ ] **SC-006**: Concurrency cancel <10s (test 1.3)
- [ ] **SC-007**: CLAP builds succeed (test 1.4)
- [ ] **SC-008**: Zero "invalid developer directory" (test 1.6)
- [ ] **SC-009**: Build summary <5s (verify in test-build.yml)
- [ ] **SC-010**: 90% actionable errors (review error messages)
- [ ] **SC-011**: <5% infrastructure failures (monitor 30 days)

---

## Next Steps

After completing remote testing:

1. ✅ Document all findings in iteration logs
2. ✅ Update VALIDATION_SUMMARY.md with results
3. ✅ Create PR from `003-ci-build-fix` to `develop`
4. ✅ Request code review from team
5. ✅ Merge to develop after approval
6. ✅ Monitor production for 30 days
7. ✅ Document lessons learned

---

**Quick-Start Guide Created**: 2025-11-10  
**For**: Feature 003-ci-build-fix Remote Testing  
**Full Plan**: See REMOTE_TESTING_PLAN.md
