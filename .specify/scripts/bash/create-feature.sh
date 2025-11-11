#!/usr/bin/env bash

# create-feature.sh - Create a new GitFlow feature branch
# Usage: ./create-feature.sh <number> <description>
# Example: ./create-feature.sh 002 "midi-export"

set -euo pipefail

# Source common utilities
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/common.sh"

# Check arguments
if [ $# -ne 2 ]; then
    echo "Usage: $0 <number> <description>"
    echo "Example: $0 002 'midi-export'"
    exit 1
fi

FEATURE_NUM="$1"
FEATURE_DESC="$2"
BRANCH_NAME="feature/${FEATURE_NUM}-${FEATURE_DESC}"

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

# Create and switch to new feature branch
echo "🌿 Creating feature branch: $BRANCH_NAME"
git checkout -b "$BRANCH_NAME"

echo ""
echo "✅ Feature branch created successfully!"
echo ""
echo "Next steps:"
echo "  1. Make your changes and commit using conventional commits:"
echo "     git commit -m 'feat($FEATURE_DESC): <description>'"
echo ""
echo "  2. Push your branch to remote:"
echo "     git push origin $BRANCH_NAME"
echo ""
echo "  3. When ready, create a Pull Request to 'develop' on GitHub"
echo ""
echo "📖 See .specify/memory/gitflow-workflow.md for complete workflow"
