# Build Release with Debug Info Script for ToggleHud

$ProjectRoot = $PSScriptRoot
$BuildRelFolder = Join-Path $ProjectRoot "buildRel"
$DistFolder = Join-Path $ProjectRoot "dist"
$DistPluginsFolder = Join-Path $ProjectRoot "dist" "KCSE" "Plugins"
$PackageFolder = Join-Path $ProjectRoot "package"

# Delete buildRel and dist folders if they exist
Write-Host "Cleaning up old build and dist folders..."
if (Test-Path $BuildRelFolder) {
    Remove-Item -Path $BuildRelFolder -Recurse -Force
    Write-Host "Deleted buildRel folder"
}

if (Test-Path $DistFolder) {
    Remove-Item -Path $DistFolder -Recurse -Force
    Write-Host "Deleted dist folder"
}

# Create buildRel folder if it doesn't exist
Write-Host "Creating buildRel folder if needed..."
if (-not (Test-Path $BuildRelFolder)) {
    New-Item -ItemType Directory -Path $BuildRelFolder | Out-Null
}

# Create dist/KCSE/Plugins folder if it doesn't exist
Write-Host "Creating dist/KCSE/Plugins folder if needed..."
if (-not (Test-Path $DistPluginsFolder)) {
    New-Item -ItemType Directory -Path $DistPluginsFolder -Force | Out-Null
}

# Configure CMake for Release build with debug info
Write-Host "Configuring CMake for Release with Debug Info..."
cmake -B $BuildRelFolder -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed!"
    exit 1
}

# Build the project in Release mode
Write-Host "Building ToggleHud in Release mode..."
cmake --build $BuildRelFolder
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed!"
    exit 1
}

# Find and copy ToggleHud.dll and ToggleHud.pdb
Write-Host "Copying DLL and PDB files..."
$DllPath = Get-ChildItem -Path $BuildRelFolder -Recurse -Filter "ToggleHud.dll" | Select-Object -First 1
$PdbPath = Get-ChildItem -Path $BuildRelFolder -Recurse -Filter "ToggleHud.pdb" | Select-Object -First 1

if ($DllPath) {
    Copy-Item -Path $DllPath.FullName -Destination $DistPluginsFolder -Force
    Write-Host "Copied DLL: $($DllPath.FullName)"
}
else {
    Write-Warning "ToggleHud.dll not found!"
}

if ($PdbPath) {
    Copy-Item -Path $PdbPath.FullName -Destination $DistPluginsFolder -Force
    Write-Host "Copied PDB: $($PdbPath.FullName)"
}
else {
    Write-Warning "ToggleHud.pdb not found!"
}

# Copy package folder contents
Write-Host "Copying package folder contents..."
if (Test-Path $PackageFolder) {
    Get-ChildItem -Path $PackageFolder | ForEach-Object {
        if ($_.PSIsContainer) {
            Copy-Item -Path $_.FullName -Destination $DistPluginsFolder -Recurse -Force
            Write-Host "Copied folder: $($_.Name)"
        }
        else {
            Copy-Item -Path $_.FullName -Destination $DistPluginsFolder -Force
            Write-Host "Copied file: $($_.Name)"
        }
    }
}
else {
    Write-Warning "Package folder not found at: $PackageFolder"
}

Write-Host "Build and deployment completed successfully!"
