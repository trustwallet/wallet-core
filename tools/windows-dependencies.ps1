
# Downloads and builds dependencies for Windows
# Run this PowerShell script from the repository root folder
# > powershell .\tools\windows-dependencies.ps1

# Ported from the `install-dependencies` bash script

# Downloads the single-header JSON library
# Downloads and builds Protobuf in static debug and release mode, with dynamic C runtime
# Builds the Wallet Core protobuf plugins
# (next script) Builds Wallet Core in static release mode, to build the console wallet
# (next script) Builds Wallet Core in dynamic release and debug mode, to build a DLL for applications

$root = $pwd
$prefix = Join-Path $pwd "build\local"

$cmakeGenerator = "Visual Studio 17 2022"
$cmakePlatform = "x64"
$cmakeToolset = "v143"

# Protobuf
$protobufVersion = "3.19.1"
$protobufDir = Join-Path $prefix "src\protobuf"

$protobufZip = "protobuf-cpp-$protobufVersion.zip"
$protobufUrl = "https://github.com/protocolbuffers/protobuf/releases/download/v$protobufVersion/$protobufZip"

# Download and extract
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
if (Test-Path -Path build_temp -PathType Container) {
     Remove-Item –Path build_temp -Recurse
}
mkdir build_temp
cd build_temp
cmake -G $cmakeGenerator -A $cmakePlatform -T $cmakeToolset "-DCMAKE_INSTALL_PREFIX=$prefix" "-DCMAKE_BUILD_TYPE=Release" "-Dprotobuf_WITH_ZLIB=OFF" "-Dprotobuf_MSVC_STATIC_RUNTIME=OFF" "-Dprotobuf_BUILD_TESTS=OFF" "-Dprotobuf_BUILD_SHARED_LIBS=OFF" ../cmake
cmake --build . --target INSTALL --config Debug
cmake --build . --target INSTALL --config Release

cd $root
