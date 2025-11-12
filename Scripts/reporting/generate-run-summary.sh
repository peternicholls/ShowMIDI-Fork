#!/bin/bash
# ==============================================================================
#
#  ShowMIDI
#  Copyright (C) 2023 Uwyn LLC.  https://www.uwyn.com
#  Copyright (C) 2025 Peter Nicholls.  https://www.peternicholls.me.uk
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ==============================================================================

# Generate test run summary JSON for CI reporting (T033)
# This script aggregates test results from CTest and creates a standardized JSON summary
# conforming to test-run-summary.schema.json

set -e

# Default values
TRIGGER_CONTEXT=${TRIGGER_CONTEXT:-"pr-to-develop"}
PLATFORM=${PLATFORM:-$(uname -s | tr '[:upper:]' '[:lower:]')}
RUN_ID=${GITHUB_RUN_ID:-"local-$(date +%s)"}
OUTPUT_FILE=${OUTPUT_FILE:-"test-run-summary.json"}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --trigger-context)
      TRIGGER_CONTEXT="$2"
      shift 2
      ;;
    --platform)
      PLATFORM="$2"
      shift 2
      ;;
    --output)
      OUTPUT_FILE="$2"
      shift 2
      ;;
    *)
      echo "Unknown option: $1"
      echo "Usage: $0 [--trigger-context CONTEXT] [--platform PLATFORM] [--output FILE]"
      exit 1
      ;;
  esac
done

# Check if CTest results exist
if [ ! -f "build/Testing/Temporary/LastTest.log" ]; then
  echo "❌ CTest results not found. Run tests first."
  exit 1
fi

# Parse CTest results
TIMESTAMP=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
TESTS_TOTAL=$(grep -c "^[0-9]" build/Testing/Temporary/LastTest.log || echo "0")
TESTS_PASSED=$(grep -c "Passed" build/Testing/Temporary/LastTest.log || echo "0")
TESTS_FAILED=$(grep -c "Failed" build/Testing/Temporary/LastTest.log || echo "0")

# Calculate overall status
if [ "$TESTS_FAILED" -eq 0 ]; then
  OVERALL_STATUS="pass"
else
  OVERALL_STATUS="fail"
fi

# Generate JSON summary
cat > "$OUTPUT_FILE" <<EOF
{
  "run_id": "$RUN_ID",
  "trigger_context": "$TRIGGER_CONTEXT",
  "timestamp": "$TIMESTAMP",
  "platform": "$PLATFORM",
  "overall_status": "$OVERALL_STATUS",
  "summary": {
    "total_tests": $TESTS_TOTAL,
    "passed": $TESTS_PASSED,
    "failed": $TESTS_FAILED,
    "skipped": 0
  },
  "check_results": [
    {
      "check_id": "unit-tests",
      "status": "$OVERALL_STATUS",
      "duration_sec": 0,
      "details": {
        "tests_run": $TESTS_TOTAL,
        "tests_passed": $TESTS_PASSED,
        "tests_failed": $TESTS_FAILED
      }
    }
  ]
}
EOF

echo "✅ Test run summary generated: $OUTPUT_FILE"
cat "$OUTPUT_FILE"
