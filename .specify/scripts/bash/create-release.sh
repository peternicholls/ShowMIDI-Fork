#!/usr/bin/env bash

# create-release.sh - Create a new GitFlow release branch
# Usage: ./create-release.sh <version>
# Example: ./create-release.sh 1.1.0

set -euo pipefail

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Check arguments
if [ $# -ne 1 ]; then
    echo "Usage: $0 <version>"
    echo "Example: $0 1.1.0"
    exit 1
fi

VERSION="$1"
BRANCH_NAME="release/$VERSION"

# Validate semantic version format (basic check)
if ! [[ "$VERSION" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    echo "❌ Error: Invalid version format. Use Semantic Versioning 2.0.0 (e.g., 1.1.0)"
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

# Ensure we're starting from develop
echo "📥 Fetching latest changes from origin..."
git fetch origin

# Switch to develop and pull latest
echo "🔄 Switching to develop branch..."
git checkout develop
git pull origin develop

# Create and switch to new release branch
echo "🚀 Creating release branch: $BRANCH_NAME"
git checkout -b "$BRANCH_NAME"

# Push release branch to remote (triggers CI)
echo "📤 Pushing release branch to origin..."
git push origin "$BRANCH_NAME"

echo ""
echo "✅ Release branch created successfully!"
echo ""
echo "⚠️  IMPORTANT: Update version numbers in both locations:"
echo "  1. CMakeLists.txt (line ~14):"
echo "     project(ShowMIDI VERSION $VERSION)"
echo ""
echo "  2. showmidi.jucer (search for VERSION tag):"
echo "     <VERSION value=\"$VERSION\"/>"
echo ""
echo "Next steps:"
echo "  1. Update version numbers in both files"
echo "  2. Commit version bump:"
echo "     git commit -am 'chore(release): bump version to $VERSION'"
echo "     git push"
echo ""
echo "  3. Make release-specific fixes only (NO new features)"
echo ""
echo "  4. When ready, create PR to 'main' on GitHub"
echo ""
echo "  5. After merge, tag the release:"
echo "     git checkout main && git pull"
echo "     git tag -a v$VERSION -m 'Release $VERSION'"
echo "     git push origin v$VERSION"
echo ""
echo "  6. Merge back to develop:"
echo "     git checkout develop && git merge $BRANCH_NAME && git push"
echo ""
echo "  7. Delete release branch:"
echo "     git branch -d $BRANCH_NAME"
echo "     git push origin --delete $BRANCH_NAME"
echo ""
echo "📖 See .specify/memory/gitflow-workflow.md for complete workflow"
