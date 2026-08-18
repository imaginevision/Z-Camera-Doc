@echo off
echo ========================================
echo ZCAMFreeD Plugin Compilation Script
echo ========================================
echo.

set UE_PATH=E:\Program Files\Epic Games\UE_5.5
set PLUGIN_PATH=E:\ue_workspace\FreeD\Plugins\ZCAMFreeD

echo [1/4] Cleaning previous build files...
if exist "%PLUGIN_PATH%\Binaries" (
    rmdir /s /q "%PLUGIN_PATH%\Binaries"
    echo   - Removed Binaries folder
) else (
    echo   - Binaries folder not found, skipping
)

if exist "%PLUGIN_PATH%\Intermediate" (
    rmdir /s /q "%PLUGIN_PATH%\Intermediate"
    echo   - Removed Intermediate folder
) else (
    echo   - Intermediate folder not found, skipping
)

if exist "%PLUGIN_PATH%\Package" (
    rmdir /s /q "%PLUGIN_PATH%\Package"
    echo   - Removed Package folder
) else (
    echo   - Package folder not found, skipping
)

echo.
echo [2/4] Checking UE5.5 installation...
if not exist "%UE_PATH%\Engine\Build\BatchFiles\RunUAT.bat" (
    echo   ERROR: UE5.5 not found at expected location!
    echo   Please update the UE5.5 path in this script.
    pause
    exit /b 1
) else (
    echo   - UE5.5 found at %UE_PATH%
)

echo.
echo [3/4] Checking plugin files...
if not exist "%PLUGIN_PATH%\ZCAMFreeD.uplugin" (
    echo   ERROR: ZCAMFreeD.uplugin not found!
    echo   Please check the plugin path in this script.
    pause
    exit /b 1
) else (
    echo   - ZCAMFreeD.uplugin found
)

echo.
echo [4/4] Compiling plugin...
echo   - Using UE5.5 UAT
echo   - Target: Win64
echo   - Plugin: ZCAMFreeD
echo.

"%UE_PATH%\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%PLUGIN_PATH%\ZCAMFreeD.uplugin" -Package="%PLUGIN_PATH%\Package" -TargetPlatforms=Win64

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo COMPILATION SUCCESSFUL!
    echo ========================================
    echo.
    echo Plugin compiled successfully.
    echo You can now:
    echo   1. Restart UE5.5 editor
    echo   2. Enable the plugin in Edit > Plugins
    echo   3. Test the LiveLink functionality
    echo.
) else (
    echo.
    echo ========================================
    echo COMPILATION FAILED!
    echo ========================================
    echo.
    echo Error code: %ERRORLEVEL%
    echo Please check the error messages above.
    echo.
)

echo Press any key to exit...
pause > nul 