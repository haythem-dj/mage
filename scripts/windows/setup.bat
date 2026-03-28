@echo off

echo "Generating build files for DEBUG configuration..."
call "%~dp0setup-debug.bat"
echo "DEBUG build files generated successfully."
echo "Generating build files for RELEASE configuration..."
call "%~dp0setup-release.bat"
echo "RELEASE build files generated successfully."

PAUSE