# ═══════════════════════════════════════════════════════════════════════════
#  Matrix GUI — One-click build script for PowerShell
#  Usage:  .\build.ps1
# ═══════════════════════════════════════════════════════════════════════════

$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

# ── Add MSYS2 UCRT64 to PATH (needed for g++ and Qt DLLs) ──
$MSYS2 = "C:\msys64\ucrt64"
if (Test-Path $MSYS2) {
    $env:PATH = "$MSYS2\bin;$MSYS2\share\qt6\bin;$env:PATH"
    Write-Host "[OK] MSYS2 UCRT64 added to PATH" -ForegroundColor Green
} else {
    Write-Host "[ERROR] MSYS2 UCRT64 not found at $MSYS2" -ForegroundColor Red
    Write-Host "Install MSYS2 then run: pacman -S mingw-w64-ucrt-x86_64-qt6-base"
    exit 1
}

# ── Clean & configure ──
if (Test-Path build) {
    Write-Host "[...] Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force build
}
New-Item -ItemType Directory -Force build | Out-Null

Write-Host "[...] Configuring with CMake..." -ForegroundColor Yellow
Push-Location build
cmake .. -G "MinGW Makefiles"
if ($LASTEXITCODE -ne 0) { Pop-Location; exit 1 }
Write-Host "[OK] Configuration done" -ForegroundColor Green

# ── Build ──
Write-Host "[...] Building..." -ForegroundColor Yellow
cmake --build .
if ($LASTEXITCODE -ne 0) { Pop-Location; exit 1 }
Write-Host "[OK] Build successful!" -ForegroundColor Green

Pop-Location

# ── Run ──
Write-Host "[...] Launching matrix_gui.exe..." -ForegroundColor Yellow
Start-Process -FilePath ".\build\matrix_gui.exe"
