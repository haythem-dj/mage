@echo off

echo "Building DEBUG configuration..."
call "%~dp0build-debug.bat"
echo "DEBUG configuration built successfully."
echo "Building RELEASE configuration..."
call "%~dp0build-release.bat"
echo "RELEASE configuration built successfully."

PAUSE