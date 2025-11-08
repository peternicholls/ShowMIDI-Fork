#!/usr/bin/env bash

# create-hotfix.sh - Create a new GitFlow hotfix branch
# Usage: ./create-hotfix.sh <version>
# Example: ./create-hotfix.sh 1.0.2

set -euo pipefail

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Check arguments
if [ $# -ne 1 ]; then
    echo "Usage: $0 <version>"
    echo "Example: $0 1.0.2"
    exit 1
fi

VERSION="$1"
BRANCH_NAME="hotfix/$VERSION"

# Validate semantic version format (basic check)
if ! [[ "$VERSION" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "❌ Error: Invalid version format. Use Semantic Versioning 2.0.0 (e.g., 1.0.2)"
    echo "See https://semver.org/spec/v2.0.0.html for specification"
    exit 1
fi

# Validate we're in a git repository
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo "❌ Error: Not a git repository"
    exit 1
fi

# Check if branch already exists
if git show-ref --verify --quiet "refs/heads/$BRANCH_NAME"; then
    echo "❌ Error: Branch '$BRANCH_NAME' already exists"
    exit 1
fi

# Ensure we're starting from main (production)
echo "📥 Fetching latest changes from origin..."
git fetch origin

# Switch to main and pull latest
echo "🔄 Switching to main branch..."
git checkout main
git pull origin main

# Create and switch to new hotfix branch
echo "🔥 Creating hotfix branch: $BRANCH_NAME"
git checkout -b "$BRANCH_NAME"

# Push hotfix branch to remote (triggers CI)
echo "📤 Pushing hotfix branch to origin..."
git push origin "$BRANCH_NAME"

echo ""
echo "✅ Hotfix branch created successfully!"
echo ""
echo "⚠️  CRITICAL: Hotfixes are for PRODUCTION EMERGENCIES ONLY"
echo ""
echo "Next steps:"
echo "  1. Fix the critical bug"
echo "     git commit -m 'fix(<scope>): <critical bug description>'"
echo "     git push"
echo ""
echo "  2. Update version numbers in both files:"
echo "     - CMakeLists.txt: project(ShowMIDI VERSION $VERSION)"
echo "     - showmidi.jucer: <VERSION value=\"$VERSION\"/>"
echo ""
echo "  3. Commit version bump:"
echo "     git commit -am 'chore(hotfix): bump version to $VERSION'"
echo "     git push"
echo ""
echo "  4. Create PR to 'main' on GitHub"
echo ""
echo "  5. After merge, tag the hotfix:"
echo "     git checkout main && git pull"
echo "     git tag -a v$VERSION -m 'Hotfix $VERSION - <description>'"
echo "     git push origin v$VERSION"
echo ""
echo "  6. Merge back to develop:"
echo "     git checkout develop && git merge $BRANCH_NAME && git push"
echo ""
echo "  7. Delete hotfix branch:"
echo "     git branch -d $BRANCH_NAME"
echo "     git push origin --delete $BRANCH_NAME"
echo ""
echo "📖 See .specify/memory/gitflow-workflow.md for complete workflow"
