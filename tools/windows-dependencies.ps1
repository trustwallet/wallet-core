
# Downloads and builds dependencies for Windows
# Run this PowerShell script from the repository root folder
# > powershell .\tools\windows-dependencies.ps1

# Ported from the `install-dependencies` bash script

# TODO: GoogleTest
# TODO: Check
# Downloads the single-header Nlohmann JSON library
# Downloads and builds Protobuf in static debug and release mode, with dynamic C runtime
# Builds the Wallet Core protobuf plugins in release mode
# (next script) Builds Wallet Core in static release mode, to build the console wallet
# (next script) Builds Wallet Core in dynamic release and debug mode, to build a DLL for applications

$root = $pwd
$prefix = Join-Path $pwd "build\local"

$cmakeGenerator = "Visual Studio 17 2022"
$cmakePlatform = "x64"
$cmakeToolset = "v143"

# Nlohmann JSON
$jsonVersion = "3.10.4"
$jsonDir = Join-Path $prefix "include\nlohmann"
$jsonUrl = "https://github.com/nlohmann/json/releases/download/v$jsonVersion/json.hpp"
$jsonFile = Join-Path $jsonDir "json.hpp"
if (Test-Path -Path $jsonFile -PathType Leaf) {
    Remove-Item –Path $jsonFile
}
if (-not(Test-Path -Path $jsonDir -PathType Container)) {
    mkdir $jsonDir | Out-Null
}
Invoke-WebRequest -Uri $jsonUrl -OutFile $jsonFile

# Protobuf
$protobufVersion = "3.19.1"
$protobufDir = Join-Path $prefix "src\protobuf"
$protobufZip = "protobuf-cpp-$protobufVersion.zip"
$protobufUrl = "https://github.com/protocolbuffers/protobuf/releases/download/v$protobufVersion/$protobufZip"

# Download and extract
if (-not(Test-Path -Path $protobufDir -PathType Container)) {
    mkdir $protobufDir | Out-Null
}
cd $protobufDir
if (-not(Test-Path -Path $protobufZip -PathType Leaf)) {
    Invoke-WebRequest -Uri $protobufUrl -OutFile $protobufZip
}
if (Test-Path -Path protobuf-$protobufVersion -PathType Container) {
     Remove-Item –Path protobuf-$protobufVersion -Recurse
}
Expand-Archive -LiteralPath $protobufZip -DestinationPath $protobufDir

# Build debug and release libraries
cd protobuf-$protobufVersion
mkdir build_msvc
cd build_msvc
$protobufCMake = Get-Content ..\cmake\CMakeLists.txt # Bugfix
$protobufCMake = $protobufCMake.Replace("set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)",
    "if (MSVC AND protobuf_MSVC_STATIC_RUNTIME)`r`nset(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)`r`nendif()") # Bugfix
$protobufCMake | Out-File -encoding UTF8 ..\cmake\CMakeLists.txt # Bugfix
cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_INSTALL_PREFIX=$prefix" "-DCMAKE_BUILD_TYPE=Release" "-Dprotobuf_WITH_ZLIB=OFF" "-Dprotobuf_MSVC_STATIC_RUNTIME=OFF" "-Dprotobuf_BUILD_TESTS=OFF" "-Dprotobuf_BUILD_SHARED_LIBS=OFF" ../cmake
cmake --build . --target INSTALL --config Debug
cmake --build . --target INSTALL --config Release

# Protobuf plugins
$pluginSrc = Join-Path $root "protobuf-plugin"
cd $pluginSrc
if (Test-Path -Path build -PathType Container) {
     Remove-Item –Path build -Recurse
}
mkdir build
cd build
cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_INSTALL_PREFIX=$prefix" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build . --target INSTALL --config Release

cd $root
