Write-Host "Building ZWLookupPlugin..." -ForegroundColor Cyan
Write-Host "Output will be saved to build_output.txt" -ForegroundColor Cyan

$msbuild = "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
$solution = "ZWLookupPlugin.sln"

$arguments = @(
    $solution
    "/p:Configuration=Debug"
    "/p:Platform=x64"
    "/verbosity:normal"
)

# Run MSBuild and capture all output
& $msbuild $arguments *>&1 | Out-File -FilePath "build_output.txt" -Encoding UTF8

# Check exit code
if ($LASTEXITCODE -eq 0) {
    Write-Host "Build succeeded!" -ForegroundColor Green
} else {
    Write-Host "Build failed with exit code: $LASTEXITCODE" -ForegroundColor Red
    Write-Host "Last 50 lines of output:" -ForegroundColor Yellow
    Get-Content "build_output.txt" -Tail 50 | Write-Host
    exit $LASTEXITCODE
}