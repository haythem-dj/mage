#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Generating build files for DEBUG configuration..."
"$SCRIPT_DIR/setup-debug.sh"
echo "DEBUG build files generated successfully."

echo "Generating build files for RELEASE configuration..."
"$SCRIPT_DIR/setup-release.sh"
echo "RELEASE build files generated successfully."
