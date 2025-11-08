#!/bin/bash
# Local test script - simulates CI build
set -e
BUILD_DIR="build-test"
BUILD_TYPE="${1:-Release}"

echo "Platform: $(uname -s)"
echo "Build type: $BUILD_TYPE"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Basic CMake configure
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" || { echo "CMake configure failed"; exit 1; }
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --parallel || { echo "Build failed"; exit 1; }

echo "Looking for artifacts..."
if [ "$(uname -s)" = "Darwin" ]; then
  find "$BUILD_DIR" -name "*.app" -type d || true
else
  find "$BUILD_DIR" -type f -executable -name "ShowMIDI" || true
fi

echo "Local test build complete"