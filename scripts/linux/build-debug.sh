#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"


PROJECT_DIR="$SCRIPT_DIR/../.."
BUILD_DIR="$PROJECT_DIR/bin/debug"

cmake --build "$BUILD_DIR"
