#!/bin/bash
# Workflow Alignment Validation Script
# Usage: .github/scripts/validate-workflow-alignment.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORKFLOWS_DIR="$(cd "$SCRIPT_DIR/../workflows" && pwd)"

echo "🔍 Validating GitHub Actions Workflow Alignment"
echo "================================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

ERRORS=0
WARNINGS=0

# Function to extract timeout value from workflow
get_timeout() {
    local file=$1
    local job=$2
    grep -A 50 "^  ${job}:" "$file" | grep "timeout-minutes:" | head -1 | awk '{print $2}' || echo "none"
}

# Function to check if needs code-quality exists
has_code_quality_dependency() {
    local file=$1
    local job=$2
    grep -A 5 "^  ${job}:" "$file" | grep -q "needs: code-quality"
}

echo "📋 Checking Code Quality Job Alignment..."
echo "----------------------------------------"

# Check if code-quality job exists in both ci.yml and test-build.yml
if ! grep -q "^  code-quality:" "$WORKFLOWS_DIR/ci.yml"; then
    echo -e "${RED}✗ FAIL: code-quality job missing in ci.yml${NC}"
    ((ERRORS++))
else
    echo -e "${GREEN}✓ PASS: code-quality job exists in ci.yml${NC}"
fi

if ! grep -q "^  code-quality:" "$WORKFLOWS_DIR/test-build.yml"; then
    echo -e "${RED}✗ FAIL: code-quality job missing in test-build.yml${NC}"
    ((ERRORS++))
else
    echo -e "${GREEN}✓ PASS: code-quality job exists in test-build.yml${NC}"
fi

echo ""
echo "🔧 Checking Fail-Fast Dependencies..."
echo "--------------------------------------"

# Check fail-fast dependencies in ci.yml
for job in build-and-test-macos build-windows build-linux; do
    if has_code_quality_dependency "$WORKFLOWS_DIR/ci.yml" "$job"; then
        echo -e "${GREEN}✓ PASS: ci.yml::$job has code-quality dependency${NC}"
    else
        echo -e "${RED}✗ FAIL: ci.yml::$job missing code-quality dependency${NC}"
        ((ERRORS++))
    fi
done

# Check fail-fast dependencies in test-build.yml
for job in test-macos test-windows test-linux; do
    if has_code_quality_dependency "$WORKFLOWS_DIR/test-build.yml" "$job"; then
        echo -e "${GREEN}✓ PASS: test-build.yml::$job has code-quality dependency${NC}"
    else
        echo -e "${RED}✗ FAIL: test-build.yml::$job missing code-quality dependency${NC}"
        ((ERRORS++))
    fi
done

echo ""
echo "⏱️  Checking Timeout Consistency..."
echo "-----------------------------------"

# Get timeouts from ci.yml
CI_MACOS_TIMEOUT=$(get_timeout "$WORKFLOWS_DIR/ci.yml" "build-and-test-macos")
CI_WINDOWS_TIMEOUT=$(get_timeout "$WORKFLOWS_DIR/ci.yml" "build-windows")
CI_LINUX_TIMEOUT=$(get_timeout "$WORKFLOWS_DIR/ci.yml" "build-linux")

# Get timeouts from test-build.yml
TEST_MACOS_TIMEOUT=$(get_timeout "$WORKFLOWS_DIR/test-build.yml" "test-macos")
TEST_WINDOWS_TIMEOUT=$(get_timeout "$WORKFLOWS_DIR/test-build.yml" "test-windows")
TEST_LINUX_TIMEOUT=$(get_timeout "$WORKFLOWS_DIR/test-build.yml" "test-linux")

echo "macOS timeouts: ci.yml=$CI_MACOS_TIMEOUT, test-build.yml=$TEST_MACOS_TIMEOUT"
if [ "$CI_MACOS_TIMEOUT" != "none" ] && [ "$TEST_MACOS_TIMEOUT" != "none" ]; then
    if [ "$CI_MACOS_TIMEOUT" = "$TEST_MACOS_TIMEOUT" ]; then
        echo -e "${GREEN}✓ PASS: macOS timeout aligned ($CI_MACOS_TIMEOUT min)${NC}"
    else
        echo -e "${YELLOW}⚠ WARNING: macOS timeout mismatch (ci: ${CI_MACOS_TIMEOUT}, test: ${TEST_MACOS_TIMEOUT})${NC}"
        ((WARNINGS++))
    fi
fi

echo "Windows timeouts: ci.yml=$CI_WINDOWS_TIMEOUT, test-build.yml=$TEST_WINDOWS_TIMEOUT"
if [ "$CI_WINDOWS_TIMEOUT" != "none" ] && [ "$TEST_WINDOWS_TIMEOUT" != "none" ]; then
    if [ "$CI_WINDOWS_TIMEOUT" = "$TEST_WINDOWS_TIMEOUT" ]; then
        echo -e "${GREEN}✓ PASS: Windows timeout aligned ($CI_WINDOWS_TIMEOUT min)${NC}"
    else
        echo -e "${YELLOW}⚠ WARNING: Windows timeout mismatch (ci: ${CI_WINDOWS_TIMEOUT}, test: ${TEST_WINDOWS_TIMEOUT})${NC}"
        ((WARNINGS++))
    fi
fi

echo "Linux timeouts: ci.yml=$CI_LINUX_TIMEOUT, test-build.yml=$TEST_LINUX_TIMEOUT"
if [ "$CI_LINUX_TIMEOUT" != "none" ] && [ "$TEST_LINUX_TIMEOUT" != "none" ]; then
    if [ "$CI_LINUX_TIMEOUT" = "$TEST_LINUX_TIMEOUT" ]; then
        echo -e "${GREEN}✓ PASS: Linux timeout aligned ($CI_LINUX_TIMEOUT min)${NC}"
    else
        echo -e "${YELLOW}⚠ WARNING: Linux timeout mismatch (ci: ${CI_LINUX_TIMEOUT}, test: ${TEST_LINUX_TIMEOUT})${NC}"
        ((WARNINGS++))
    fi
fi

echo ""
echo "🏃 Checking Runner Consistency..."
echo "---------------------------------"

# Check macOS runner
CI_MACOS_RUNNER=$(grep -A 5 "^  build-and-test-macos:" "$WORKFLOWS_DIR/ci.yml" | grep "runs-on:" | awk '{print $2}')
TEST_MACOS_RUNNER=$(grep -A 5 "^  test-macos:" "$WORKFLOWS_DIR/test-build.yml" | grep "runs-on:" | awk '{print $2}')

echo "macOS runners: ci.yml=$CI_MACOS_RUNNER, test-build.yml=$TEST_MACOS_RUNNER"
if [ "$CI_MACOS_RUNNER" = "$TEST_MACOS_RUNNER" ]; then
    echo -e "${GREEN}✓ PASS: macOS runner aligned ($CI_MACOS_RUNNER)${NC}"
else
    echo -e "${RED}✗ FAIL: macOS runner mismatch (ci: ${CI_MACOS_RUNNER}, test: ${TEST_MACOS_RUNNER})${NC}"
    ((ERRORS++))
fi

echo ""
echo "📦 Checking Artifact Retention..."
echo "---------------------------------"

# Check retention days
CI_RETENTION=$(grep -A 10 "Upload macOS artifacts" "$WORKFLOWS_DIR/ci.yml" | grep "retention-days:" | awk '{print $2}')
TEST_RETENTION=$(grep -A 10 "Upload artifacts" "$WORKFLOWS_DIR/test-build.yml" | grep "retention-days:" | head -1 | awk '{print $2}')

echo "Artifact retention: ci.yml=$CI_RETENTION days, test-build.yml=$TEST_RETENTION days"
if [ "$CI_RETENTION" = "90" ]; then
    echo -e "${GREEN}✓ PASS: ci.yml retention is 90 days${NC}"
else
    echo -e "${YELLOW}⚠ WARNING: ci.yml retention should be 90 days (currently ${CI_RETENTION})${NC}"
    ((WARNINGS++))
fi

echo ""
echo "================================================"
echo "📊 Validation Summary"
echo "================================================"
echo -e "${RED}Errors: $ERRORS${NC}"
echo -e "${YELLOW}Warnings: $WARNINGS${NC}"
echo ""

if [ $ERRORS -gt 0 ]; then
    echo -e "${RED}❌ VALIDATION FAILED${NC}"
    echo "Please fix the errors above before committing workflow changes."
    echo "See .github/WORKFLOW_ALIGNMENT_STRATEGY.md for guidance."
    exit 1
elif [ $WARNINGS -gt 0 ]; then
    echo -e "${YELLOW}⚠️  VALIDATION PASSED WITH WARNINGS${NC}"
    echo "Consider addressing warnings for better consistency."
    exit 0
else
    echo -e "${GREEN}✅ VALIDATION PASSED${NC}"
    echo "All workflows are properly aligned."
    exit 0
fi
