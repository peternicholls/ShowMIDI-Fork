#!/usr/bin/env bash
#
# ShowMIDI Git Hooks Installation Script
# Copyright (C) 2025 Peter Nicholls.  https://www.peternicholls.me.uk
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#

set -euo pipefail

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

success() {
    echo -e "${GREEN}✓ $*${NC}"
}

info() {
    echo "$*"
}

# Determine repository root
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
HOOKS_SOURCE="$REPO_ROOT/scripts/hooks"
HOOKS_TARGET="$REPO_ROOT/.git/hooks"

info "=== ShowMIDI Git Hooks Installation ==="
info "Repository: $REPO_ROOT"
info ""

# Check if .git directory exists
if [ ! -d "$REPO_ROOT/.git" ]; then
    echo "ERROR: .git directory not found. This script must be run from the repository root."
    exit 1
fi

# Create hooks directory if it doesn't exist
mkdir -p "$HOOKS_TARGET"

# Install pre-commit hook
if [ -f "$HOOKS_SOURCE/pre-commit" ]; then
    cp "$HOOKS_SOURCE/pre-commit" "$HOOKS_TARGET/pre-commit"
    chmod +x "$HOOKS_TARGET/pre-commit"
    success "Installed pre-commit hook"
else
    echo "WARNING: pre-commit hook source not found at $HOOKS_SOURCE/pre-commit"
fi

# Install pre-push hook
if [ -f "$HOOKS_SOURCE/pre-push" ]; then
    cp "$HOOKS_SOURCE/pre-push" "$HOOKS_TARGET/pre-push"
    chmod +x "$HOOKS_TARGET/pre-push"
    success "Installed pre-push hook"
else
    echo "WARNING: pre-push hook source not found at $HOOKS_SOURCE/pre-push"
fi

info ""
success "=== Git hooks installed successfully ==="
info ""
info "The following hooks are now active:"
info "  • pre-commit: Auto-format code with clang-format, check GPL headers"
info "  • pre-push: Run unit tests before pushing to remote"
info ""
info "To bypass hooks in emergencies, use: git commit --no-verify"
info ""
