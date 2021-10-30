
# Build Wallet Core

# Run after installing dependencies and generating code
# > powershell .\tools\windows-build.ps1

# Builds Wallet Core in static release mode, to build the console wallet
# Builds Wallet Core in dynamic release and debug mode, to build a DLL for applications

$root = $pwd
$prefix = Join-Path $pwd "build\local"

$cmakeGenerator = "Visual Studio 17 2022"
$cmakePlatform = "x64"
$cmakeToolset = "v143"

cd build

cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_INSTALL_PREFIX=$prefix" "-DCMAKE_BUILD_TYPE=Release" "-DTW_STATIC_LIBRARY=ON" ..
cmake --build . --target INSTALL --config Release

cd $root
