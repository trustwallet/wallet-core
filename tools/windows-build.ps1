
# Build Wallet Core

# Run after installing dependencies and generating code
# > powershell .\tools\windows-build.ps1

# Builds Wallet Core in static release mode, to build the console wallet
# Builds Wallet Core in dynamic release and debug mode, to build a DLL for applications

$ErrorActionPreference = "Stop"

$root = $pwd
$prefix = Join-Path $pwd "build\local"
$install = Join-Path $pwd "build\install"

$cmakeGenerator = "Visual Studio 17 2022"
$cmakePlatform = "x64"
$cmakeToolset = "v143"

cd build

if (-not(Test-Path -Path "static" -PathType Container)) {
    mkdir "static" | Out-Null
}
cd static
cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_PREFIX_PATH=$prefix" "-DCMAKE_INSTALL_PREFIX=$install" "-DCMAKE_BUILD_TYPE=Release" "-DTW_STATIC_LIBRARY=ON" ../..
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
cmake --build . --target INSTALL --config Release
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
cd ..

if (-not(Test-Path -Path "shared" -PathType Container)) {
    mkdir "shared" | Out-Null
}
cd shared
cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_PREFIX_PATH=$prefix" "-DCMAKE_INSTALL_PREFIX=$install" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_DEBUG_POSTFIX=d" "-DTW_STATIC_LIBRARY=OFF" ../..
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
cmake --build . --target INSTALL --config Debug
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
cmake --build . --target INSTALL --config Release
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
cd ..

cd $root
