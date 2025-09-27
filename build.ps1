# Cross-Platform Physics Engine Build Script for PowerShell
# Compatible with PowerShell 5.1+ and PowerShell Core

param(
    [switch]$Help
)

if ($Help) {
    Write-Host "Physics Engine Cross-Platform Build Script for PowerShell" -ForegroundColor Cyan
    Write-Host "Usage: .\build.ps1" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "This script will:" -ForegroundColor Green
    Write-Host "  - Check all prerequisites automatically" -ForegroundColor White
    Write-Host "  - Provide installation instructions if anything is missing" -ForegroundColor White
    Write-Host "  - Configure CMake with optimal settings for your platform" -ForegroundColor White
    Write-Host "  - Build the project with parallel compilation" -ForegroundColor White
    Write-Host "  - Locate the executable and provide run instructions" -ForegroundColor White
    exit 0
}

# Colors for output
$Colors = @{
    Info = "Cyan"
    Success = "Green"
    Warning = "Yellow"
    Error = "Red"
}

function Write-Status {
    param([string]$Message, [string]$Type = "Info")
    Write-Host "[$Type] $Message" -ForegroundColor $Colors[$Type]
}

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "Physics Engine Cross-Platform Build Script" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host ""

# Detect build environment
$BuildEnv = "unknown"
if (Get-Command cl -ErrorAction SilentlyContinue) {
    $BuildEnv = "msvc"
    Write-Status "Detected MSVC environment"
} elseif (Get-Command g++ -ErrorAction SilentlyContinue) {
    $BuildEnv = "mingw"
    Write-Status "Detected MinGW environment"
} else {
    Write-Status "No C++ compiler found!" "Error"
    Write-Host ""
    Write-Host "Please install one of the following:" -ForegroundColor Yellow
    Write-Host "  1. Visual Studio 2022 with C++ development tools" -ForegroundColor White
    Write-Host "  2. MSYS2 with MinGW toolchain" -ForegroundColor White
    Write-Host ""
    Read-Host "Press Enter to exit"
    exit 1
}

# Check prerequisites
Write-Status "Checking prerequisites..."

$MissingTools = @()

# Check CMake
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    $MissingTools += "CMake"
} else {
    $CmakeVersion = (cmake --version | Select-String "cmake version").Line.Split(' ')[2]
    Write-Status "CMake version: $CmakeVersion"
}

# Check Git
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    $MissingTools += "Git"
} else {
    Write-Status "Git found"
}

# Report missing tools
if ($MissingTools.Count -gt 0) {
    Write-Status "Missing required tools:" "Error"
    foreach ($tool in $MissingTools) {
        Write-Host "  - $tool" -ForegroundColor Red
    }
    Write-Host ""
    Write-Host "Installation instructions:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Windows:" -ForegroundColor Cyan
    Write-Host "  Option 1 - Visual Studio (Recommended):" -ForegroundColor White
    Write-Host "    1. Install Visual Studio 2022 with C++ development tools" -ForegroundColor Gray
    Write-Host "    2. Install CMake: https://cmake.org/download/" -ForegroundColor Gray
    Write-Host "    3. Install Git: https://git-scm.com/download/win" -ForegroundColor Gray
    Write-Host ""
    Write-Host "  Option 2 - MSYS2/MinGW:" -ForegroundColor White
    Write-Host "    1. Install MSYS2: https://www.msys2.org/" -ForegroundColor Gray
    Write-Host "    2. Install toolchain: pacman -S mingw-w64-x86_64-toolchain" -ForegroundColor Gray
    Write-Host "    3. Install CMake: pacman -S mingw-w64-x86_64-cmake" -ForegroundColor Gray
    Write-Host "    4. Install Git: pacman -S git" -ForegroundColor Gray
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Status "All prerequisites found!" "Success"
Write-Host ""

# Build the project
Write-Status "Building Physics Engine..."

# Clean and create build directory
if (Test-Path "build") {
    Write-Status "Cleaning build directory..."
    Remove-Item -Recurse -Force "build\*"
} else {
    Write-Status "Creating build directory..."
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# Configure with CMake
Write-Status "Configuring with CMake..."
Set-Location "build"

try {
    if ($BuildEnv -eq "msvc") {
        cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    } else {
        cmake .. -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    }
    
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed!"
    }
    
    # Build the project
    Write-Status "Building..."
    if ($BuildEnv -eq "msvc") {
        cmake --build . --config Release -j
    } else {
        cmake --build . -j
    }
    
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed!"
    }
} catch {
    Write-Status $_.Exception.Message "Error"
    Set-Location ".."
    Read-Host "Press Enter to exit"
    exit 1
}

Set-Location ".."

# Find executable
Write-Status "Locating executable..."
$Executable = $null

if (Test-Path "build\Release\PhysicsEngine.exe") {
    $Executable = "build\Release\PhysicsEngine.exe"
} elseif (Test-Path "build\PhysicsEngine.exe") {
    $Executable = "build\PhysicsEngine.exe"
}

if ($Executable) {
    Write-Status "Build complete! Executable: $Executable" "Success"
    Write-Host ""
    Write-Status "To run the physics engine:"
    Write-Host "  $Executable" -ForegroundColor White
    Write-Host ""
    Write-Status "Available scenes:"
    Write-Host "  1. Basic Demo (Cube + Sphere with gravity)" -ForegroundColor White
    Write-Host "  2. Beautiful Terrain (Procedural landscape)" -ForegroundColor White
    Write-Host "  3. Mesh Intensive Demo (Performance stress test)" -ForegroundColor White
    Write-Host "  4. Ball Collision Scene (Multiple balls on bounded plane)" -ForegroundColor White
    Write-Host "  5. Advanced Demo (Coming soon)" -ForegroundColor White
    Write-Host "  6. Particle System (Coming soon)" -ForegroundColor White
} else {
    Write-Status "Executable not found! Build may have failed." "Error"
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Status "Build process completed successfully!" "Success"
Read-Host "Press Enter to exit"
