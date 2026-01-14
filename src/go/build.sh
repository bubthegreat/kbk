#!/bin/bash
# Build script for KBK Go library
# This compiles the Go code into a C-compatible static library

set -e

echo "Building KBK Go library..."

# Check Go version
GO_VERSION=$(go version | awk '{print $3}' | sed 's/go//')
echo "Go version: $GO_VERSION"

# Build the Go code as a C archive
echo "Compiling Go code to C archive..."
go build -buildmode=c-archive -o ../libkbkgo.a

echo "Go library built successfully:"
echo "  - Static library: src/libkbkgo.a"
echo "  - Header file: src/libkbkgo.h"
echo ""
echo "Build complete!"

