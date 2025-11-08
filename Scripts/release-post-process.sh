#!/bin/bash
# Orchestrates post-release updates: README links, sizes, changelog, stats
set -e
TAG="${1:-$GITHUB_REF_NAME}"
if [ -z "$TAG" ]; then
  echo "Usage: $0 <tag>"
  exit 1
fi

echo "Post-processing release $TAG"

# 1. Update README download links
./scripts/update-download-links.sh "$TAG" || true

# 2. Update file sizes - uses update-file-sizes workflow or local script
python3 - <<'PY'
print("File sizes will be updated by CI workflow (update-file-sizes.yml).")
PY

# 3. Generate changelog (local helper)
if command -v auto-changelog >/dev/null 2>&1; then
  auto-changelog --output CHANGELOG.md || true
fi

echo "Post-processing complete. Review changes, commit and push as needed."