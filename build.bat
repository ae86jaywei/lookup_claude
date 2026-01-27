@echo off
echo Building ZWLookupPlugin...
echo Using Visual Studio 2026 (v145 platform toolset)

"C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" ZWLookupPlugin.sln /p:Configuration=Debug /p:Platform=x64

if %ERRORLEVEL% EQU 0 (
    echo Build succeeded!
) else (
    echo Build failed with error level %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)