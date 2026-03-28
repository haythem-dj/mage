#!/usr/bin/env bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PROJECT_DIR="$SCRIPT_DIR/../.."
BUILD_DIR="$PROJECT_DIR/bin/debug"

cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="$HOME/dev/opt"
