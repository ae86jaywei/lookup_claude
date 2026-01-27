Write-Host "Building ZWLookupPlugin..." -ForegroundColor Cyan
Write-Host "Using Visual Studio 2026 (v145 platform toolset)" -ForegroundColor Cyan

$msbuild = "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
$solution = "ZWLookupPlugin.sln"

$arguments = @(
    $solution
    "/p:Configuration=Debug"
    "/p:Platform=x64"
    "/verbosity:minimal"
)

$result = & $msbuild $arguments
if ($LASTEXITCODE -eq 0) {
    Write-Host "Build succeeded!" -ForegroundColor Green
} else {
    Write-Host "Build failed with exit code: $LASTEXITCODE" -ForegroundColor Red
    exit $LASTEXITCODE
}