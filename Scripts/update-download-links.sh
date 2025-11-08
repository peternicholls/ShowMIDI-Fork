#!/bin/bash
# Manually update README download links for a given tag
set -e
TAG="$1"
if [ -z "$TAG" ]; then
  echo "Usage: $0 <tag>"
  exit 1
fi

README=README.md

# Replace common patterns. The -i.bak flag creates a backup of the original file.
sed -E -i.bak "s|/releases/download/v[0-9.]+/ShowMIDI-macOS-.*\\.dmg|/releases/download/$TAG/ShowMIDI-macOS-$TAG.dmg|g" "$README"
sed -E -i.bak "s|/releases/download/v[0-9.]+/ShowMIDI-Windows-.*-Setup\\.exe|/releases/download/$TAG/ShowMIDI-Windows-$TAG-Setup.exe|g" "$README"
sed -E -i.bak "s|/releases/download/v[0-9.]+/ShowMIDI-Windows-.*-Portable\\.zip|/releases/download/$TAG/ShowMIDI-Windows-$TAG-Portable.zip|g" "$README"
sed -E -i.bak "s|/releases/download/v[0-9.]+/ShowMIDI-Linux-.*\\.AppImage|/releases/download/$TAG/ShowMIDI-Linux-$TAG.AppImage|g" "$README"
sed -E -i.bak "s|/releases/download/v[0-9.]+/ShowMIDI-Linux-.*\\.deb|/releases/download/$TAG/ShowMIDI-Linux-$TAG.deb|g" "$README"

echo "Updated README links to tag $TAG"
echo "Verify and commit: git add README.md && git commit -m 'Update download links for $TAG' && git push"