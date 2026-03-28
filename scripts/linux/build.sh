#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Building DEBUG configuration..."
"$SCRIPT_DIR/build-debug.sh"
echo "DEBUG configuration built successfully."
echo "Building RELEASE configuration..."
"$SCRIPT_DIR/build-release.sh"
echo "RELEASE configuration built successfully."
