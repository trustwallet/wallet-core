
# Build Wallet Core

# Run after installing dependencies and generating code
# > powershell .\tools\windows-build.ps1

# Builds Wallet Core in static release mode, to build the console wallet
# Builds Wallet Core in dynamic release and debug mode, to build a DLL for applications

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
cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_INSTALL_PREFIX=$install" "-DCMAKE_BUILD_TYPE=Release" "-DTW_STATIC_LIBRARY=ON" ..
cmake --build . --target INSTALL --config Release
cd ..

if (-not(Test-Path -Path "shared" -PathType Container)) {
    mkdir "shared" | Out-Null
}
cd shared
cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_INSTALL_PREFIX=$install" "-DCMAKE_BUILD_TYPE=Release" "-DTW_STATIC_LIBRARY=OFF" ..
cmake --build . --target INSTALL --config Debug
cmake --build . --target INSTALL --config Release
cd ..

cd $root
