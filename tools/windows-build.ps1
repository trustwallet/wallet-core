# SPDX-License-Identifier: Apache-2.0
[CmdletBinding()]
param(
    [ValidateSet('All', 'Prepare', 'Build', 'Package', 'Test')][string]$Stage = 'All',
    [string]$Msys2Root = '',
    [ValidateRange(1, 128)][int]$Jobs = 8
)

$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'
$PSNativeCommandUseErrorActionPreference = $false
$Root = Split-Path $PSScriptRoot -Parent
$Build = Join-Path $Root 'build\windows'
$Prefix = Join-Path $Root 'build\local'
$Logs = Join-Path $Build 'logs'
$StaticBuild = Join-Path $Build 'static-build'
$SharedBuild = Join-Path $Build 'shared-build'
$Package = Join-Path $Build 'WalletCore-Windows-x64'
$Archive = Join-Path $Build 'WalletCore-Windows-x64.zip'
$Nightly = 'nightly-2025-12-11'
$Target = 'x86_64-pc-windows-msvc'
$EnvironmentBefore = [Environment]::GetEnvironmentVariables('Process')
$LocationBefore = Get-Location
New-Item -ItemType Directory -Force $Logs | Out-Null

function Invoke-Tool([string]$Name, [string]$Program, [string[]]$Arguments) {
    [Console]::WriteLine("Stage: $Name")
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue' # PowerShell 5 wraps native stderr in ErrorRecord.
    try {
        & $Program @Arguments 2>&1 | ForEach-Object { $_.ToString() } |
            Tee-Object -FilePath (Join-Path $Logs "$Name.log") | Out-Null
        $code = $LASTEXITCODE
    } finally { $ErrorActionPreference = $previous }
    if ($code -ne 0) {
        Select-String -Path (Join-Path $Logs "$Name.log") -Pattern ': (fatal )?error:|error LNK\d+' -Context 0,2 |
            Select-Object -First 5 | ForEach-Object { [Console]::WriteLine($_.ToString()) }
        Get-Content (Join-Path $Logs "$Name.log") -Tail 25 | ForEach-Object { [Console]::WriteLine($_) }
        throw "Failed stage: $Name (exit $code); see $Logs\$Name.log"
    }
}

function Get-Archive([string]$Url, [string]$Destination) {
    if (!(Test-Path -LiteralPath $Destination)) {
        New-Item -ItemType Directory -Force (Split-Path $Destination -Parent) | Out-Null
        Invoke-Tool 'download' "$env:SystemRoot\System32\curl.exe" @('--fail', '--location', '--retry', '3', '--output', "$Destination.download", $Url)
        Move-Item -LiteralPath "$Destination.download" -Destination $Destination -Force
    }
}

$versions = @{}
Get-Content (Join-Path $Root 'tools\dependencies-version') | ForEach-Object {
    if ($_ -match '^export (\w+)=([\d.]+)$') { $versions[$Matches[1]] = $Matches[2] }
}
foreach ($dependency in @('GTEST_VERSION', 'JSON_VERSION', 'PROTOBUF_VERSION')) {
    if (!$versions[$dependency]) { throw "Missing dependency version: $dependency" }
}
$protoVersion = $versions.PROTOBUF_VERSION
$gtestVersion = $versions.GTEST_VERSION
$jsonVersion = $versions.JSON_VERSION

function Prepare {
    Invoke-Tool 'rust-toolchain' $Rustup @('toolchain', 'install', $Nightly, '--profile', 'minimal')
    Invoke-Tool 'rust-target' $Rustup @('target', 'add', $Target, '--toolchain', $Nightly)
    Push-Location (Join-Path $Root 'rust')
    try {
        Invoke-Tool 'rust-core' $Cargo @('build', '--locked', '--release', '-p', 'wallet-core-rs', '--target', $Target)
        Invoke-Tool 'rust-native-libraries' $Cargo @('rustc', '--locked', '--release', '-p', 'wallet-core-rs', '--target', $Target, '--', '--print', 'native-static-libs')
    } finally { Pop-Location }
    $nativeLine = Get-Content (Join-Path $Logs 'rust-native-libraries.log') | Where-Object { $_ -match 'native-static-libs:\s*(.*)$' } | Select-Object -Last 1
    if (!$nativeLine -or $nativeLine -notmatch 'native-static-libs:\s*(.*)$') { throw 'Rust did not report native-static-libs' }
    $nativeLibraries = @($Matches[1] -split '\s+' | ForEach-Object {
        $library = $_ -replace '^/defaultlib:', '' -replace '\.lib$', ''
        if ($library -notmatch '^[a-zA-Z0-9_.-]+$' -or $library -match '^(libcmt|libcmtd|msvcrtd)$') { throw "Unexpected Rust native library for /MD: $_" }
        $library
    })
    Set-Content -Encoding ASCII (Join-Path $Build 'native-libraries.cmake') ("set(TW_WINDOWS_NATIVE_LIBRARIES " + ($nativeLibraries -join ' ') + ')')

    $protoDirectory = Join-Path $Prefix "src\protobuf\protobuf-$protoVersion"
    $protoArchive = Join-Path $Prefix "src\protobuf\protobuf-java-$protoVersion.tar.gz"
    $gtestDirectory = Join-Path $Prefix "src\gtest\googletest-$gtestVersion"
    $gtestArchive = Join-Path $Prefix "src\gtest\googletest-$gtestVersion.tar.gz"
    $jsonArchive = Join-Path $Prefix 'json\include.zip'
    Get-Archive "https://github.com/protocolbuffers/protobuf/releases/download/v$protoVersion/protobuf-java-$protoVersion.tar.gz" $protoArchive
    Get-Archive "https://github.com/google/googletest/releases/download/v$gtestVersion/googletest-$gtestVersion.tar.gz" $gtestArchive
    Get-Archive "https://github.com/nlohmann/json/releases/download/v$jsonVersion/include.zip" $jsonArchive
    if (!(Test-Path $protoDirectory)) {
        Invoke-Tool 'extract-protobuf' "$env:SystemRoot\System32\tar.exe" @('-xf', $protoArchive, '-C', (Split-Path $protoDirectory -Parent))
    }
    if (!(Test-Path $gtestDirectory)) {
        Invoke-Tool 'extract-gtest' "$env:SystemRoot\System32\tar.exe" @('-xf', $gtestArchive, '-C', (Split-Path $gtestDirectory -Parent))
    }
    Expand-Archive -LiteralPath $jsonArchive -DestinationPath $Prefix -Force
    $boostArchive = Join-Path $Build 'boost_1_83_0.tar.gz'
    Get-Archive 'https://archives.boost.io/release/1.83.0/source/boost_1_83_0.tar.gz' $boostArchive
    if (!(Test-Path (Join-Path $Build 'boost_1_83_0\boost\version.hpp')) -or !(Test-Path (Join-Path $Build 'boost_1_83_0\LICENSE_1_0.txt'))) {
        Invoke-Tool 'extract-boost' "$env:SystemRoot\System32\tar.exe" @('-xf', $boostArchive, '-C', $Build, 'boost_1_83_0/boost', 'boost_1_83_0/LICENSE_1_0.txt')
    }

    $protoBuild = Join-Path $Build 'protobuf-host'
    Invoke-Tool 'configure-protobuf-host' $CMake (@('-S', "$protoDirectory\cmake", '-B', $protoBuild, "-DCMAKE_INSTALL_PREFIX=$Prefix", '-Dprotobuf_BUILD_TESTS=OFF', '-Dprotobuf_BUILD_SHARED_LIBS=OFF', '-Dprotobuf_MSVC_STATIC_RUNTIME=OFF', '-Dprotobuf_WITH_ZLIB=OFF') + $CompilerArguments)
    Invoke-Tool 'build-protobuf-host' $CMake @('--build', $protoBuild, '--parallel', "$Jobs")
    Invoke-Tool 'install-protobuf-host' $CMake @('--install', $protoBuild)
    $pluginBuild = Join-Path $Build 'protobuf-plugin'
    Invoke-Tool 'configure-protobuf-plugin' $CMake (@('-S', (Join-Path $Root 'protobuf-plugin'), '-B', $pluginBuild, "-DCMAKE_INSTALL_PREFIX=$Prefix", "-DProtobuf_DIR=$Prefix\lib\cmake\protobuf") + $CompilerArguments)
    Invoke-Tool 'build-protobuf-plugin' $CMake @('--build', $pluginBuild, '--parallel', "$Jobs")
    Invoke-Tool 'install-protobuf-plugin' $CMake @('--install', $pluginBuild)
    if (!(Test-Path (Join-Path $Build 'tools\bin\cbindgen.exe'))) {
        Invoke-Tool 'install-cbindgen' $Cargo @('install', 'cbindgen', '--locked', '--version', '0.29.2', '--root', (Join-Path $Build 'tools'))
    }
    Invoke-Tool 'generate-files' $Bash @('-c', 'export PREFIX=$PWD/build/local; tools/generate-files native')
    Invoke-Tool 'codegen-tests' $Ruby @('-Icodegen/lib', '-Icodegen/test', '-e', 'Dir.glob(%q{codegen/test/test_*.rb}).each { |path| require_relative path }')
    Invoke-Tool 'generate-exports' $Ruby @((Join-Path $PSScriptRoot 'windows-exports.rb'), (Join-Path $Root 'include\TrustWalletCore'), (Join-Path $Build 'TrustWalletCore.def'))
}

function Build-Libraries {
    $common = @('-S', $Root, '-DBoost_NO_BOOST_CMAKE=ON', '-DBoost_NO_SYSTEM_PATHS=ON', "-DBoost_INCLUDE_DIR=$Build\boost_1_83_0", "-DTW_WINDOWS_NATIVE_LIBRARIES_FILE=$Build\native-libraries.cmake", "-DTW_WINDOWS_EXPORTS_FILE=$Build\TrustWalletCore.def") + $CompilerArguments
    Invoke-Tool 'configure-static' $CMake ($common + @('-B', $StaticBuild, '-DTW_WINDOWS_BUILD_SHARED=OFF', '-DTW_UNIT_TESTS=ON', '-DTW_BUILD_EXAMPLES=ON'))
    Invoke-Tool 'build-static' $CMake @('--build', $StaticBuild, '--target', 'TrustWalletCore', 'tests', 'WindowsRandomTests', 'walletconsole', '--parallel', "$Jobs")
    Invoke-Tool 'configure-shared' $CMake ($common + @('-B', $SharedBuild, '-DTW_WINDOWS_BUILD_SHARED=ON', '-DTW_UNIT_TESTS=OFF', '-DTW_BUILD_EXAMPLES=OFF'))
    Invoke-Tool 'build-shared' $CMake @('--build', $SharedBuild, '--target', 'TrustWalletCore', '--parallel', "$Jobs")
}

function Package-Libraries {
    $staticStage = Join-Path $Build 'stage-static'
    $sharedStage = Join-Path $Build 'stage-shared'
    Invoke-Tool 'install-static' $CMake @('--install', $StaticBuild, '--prefix', $staticStage)
    Invoke-Tool 'install-shared' $CMake @('--install', $SharedBuild, '--prefix', $sharedStage)
    # Replace only this script's package output, never a checkout or external artifact.
    if (Test-Path $Package) { Remove-Item -LiteralPath $Package -Recurse -Force }
    foreach ($directory in @('include', 'static\lib', 'shared\lib', 'shared\bin', 'lib\cmake\WalletCore', 'licenses')) {
        New-Item -ItemType Directory -Force (Join-Path $Package $directory) | Out-Null
    }
    Copy-Item -LiteralPath "$staticStage\include\TrustWalletCore" -Destination "$Package\include" -Recurse
    foreach ($library in @('TrustWalletCore.lib', 'TrezorCrypto.lib', 'protobuf.lib')) {
        Copy-Item -LiteralPath "$staticStage\lib\$library" -Destination "$Package\static\lib"
    }
    Copy-Item -LiteralPath "$Root\rust\target\$Target\release\wallet_core_rs.lib" -Destination "$Package\static\lib"
    Copy-Item -LiteralPath "$sharedStage\lib\TrustWalletCore.lib" -Destination "$Package\shared\lib"
    Copy-Item -LiteralPath "$sharedStage\bin\TrustWalletCore.dll" -Destination "$Package\shared\bin"
    Copy-Item -LiteralPath "$StaticBuild\WalletCoreConfig.cmake" -Destination "$Package\lib\cmake\WalletCore"
    Copy-Item -LiteralPath "$Root\LICENSE" -Destination "$Package\licenses\WalletCore.txt"
    Copy-Item -LiteralPath "$Prefix\src\protobuf\protobuf-$protoVersion\LICENSE" -Destination "$Package\licenses\Protobuf.txt"
    Copy-Item -LiteralPath "$Root\trezor-crypto\crypto\LICENSE" -Destination "$Package\licenses\TrezorCrypto.txt"
    Copy-Item -LiteralPath "$Build\boost_1_83_0\LICENSE_1_0.txt" -Destination "$Package\licenses\Boost.txt"
    Copy-Item -LiteralPath "$Root\docs\windows.md" -Destination "$Package\README.md"
    @{
        target = $Target; rustToolchain = $Nightly; runtime = 'MD'; configuration = 'Release'
        clang = (& $Clang --version | Select-Object -First 1)
        libraries = @(Get-ChildItem "$Package\static\lib", "$Package\shared\lib", "$Package\shared\bin" -File | ForEach-Object {
            @{ path = $_.FullName.Substring($Package.Length + 1); sha256 = (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash }
        })
    } | ConvertTo-Json -Depth 5 | Set-Content -Encoding UTF8 "$Package\manifest.json"
    Add-Type -AssemblyName System.IO.Compression, System.IO.Compression.FileSystem
    if (Test-Path $Archive) { Remove-Item -LiteralPath $Archive -Force }
    $zip = [IO.Compression.ZipFile]::Open($Archive, [IO.Compression.ZipArchiveMode]::Create)
    try {
        Get-ChildItem -LiteralPath $Package -Recurse -File | ForEach-Object {
            # PowerShell 5's Compress-Archive emits nonstandard backslash ZIP paths.
            $entry = $_.FullName.Substring($Package.Length + 1).Replace('\', '/')
            [IO.Compression.ZipFileExtensions]::CreateEntryFromFile($zip, $_.FullName, $entry) | Out-Null
        }
    } finally { $zip.Dispose() }
    [Console]::WriteLine("Package: $Archive")
}

function Test-Libraries {
    # CTest must work without the wrapper's directory overrides.
    $env:TEST_TMPDIR = $null
    $env:TW_TESTS_ROOT = $null
    $filterBefore = $env:GTEST_FILTER
    $env:GTEST_FILTER = 'StoredKey.BackwardCompatibility:TWStoredKey.storeWithTemporaryFileSuccess'
    try {
        Invoke-Tool 'ctest-default-paths' (Join-Path (Split-Path $CMake) 'ctest.exe') @('--test-dir', "$StaticBuild\tests", '-R', '^example_test$', '--no-tests=error', '-VV')
    } finally { $env:GTEST_FILTER = $filterBefore }
    & (Join-Path $Root 'tests\windows\exports.ps1')
    # Exercise paths with spaces and non-ASCII characters even in an ASCII checkout.
    $unicode = [string][char]0x4E2D + [char]0x6587
    $env:TEST_TMPDIR = Join-Path $Build "test-temp $unicode"
    $env:TW_TESTS_ROOT = Join-Path $Build "test sources $unicode"
    New-Item -ItemType Directory -Force $env:TEST_TMPDIR | Out-Null
    New-Item -ItemType Directory -Force $env:TW_TESTS_ROOT | Out-Null
    foreach ($directory in @('common', 'chains')) {
        $link = Join-Path $env:TW_TESTS_ROOT $directory
        if (!(Test-Path -LiteralPath $link)) {
            New-Item -ItemType Junction -Path $link -Value (Join-Path $Root "tests\$directory") | Out-Null
        }
    }
    Invoke-Tool 'gtest' (Join-Path $StaticBuild 'tests\tests.exe') @("--gtest_output=xml:$Logs\gtest.xml")
    [xml]$report = Get-Content (Join-Path $Logs 'gtest.xml') -Raw
    [ordered]@{
        tests = [int]$report.testsuites.tests
        passed = $report.SelectNodes('//testcase[@result="completed" and not(failure)]').Count
        failures = [int]$report.testsuites.failures
        skipped = $report.SelectNodes('//testcase/skipped').Count
        disabled = [int]$report.testsuites.disabled
        errors = [int]$report.testsuites.errors
        time = $report.testsuites.time
    } | ConvertTo-Json | Set-Content -Encoding UTF8 (Join-Path $Logs 'test-summary.json')
    Invoke-Tool 'random-boundaries' (Join-Path $StaticBuild 'tests\WindowsRandomTests.exe') @()
    & (Join-Path $StaticBuild 'tests\WindowsRandomTests.exe') 'failure'
    if ($LASTEXITCODE -ne 3) { throw "Expected abort exit 3 for secure RNG failure, got $LASTEXITCODE" }
    Set-Content -Encoding ASCII (Join-Path $Logs 'random-failure.log') 'Secure RNG failure terminated with expected abort exit 3'
    $relocated = Join-Path $Build 'relocated package'
    if (Test-Path $relocated) { Remove-Item -LiteralPath $relocated -Recurse -Force }
    Expand-Archive -LiteralPath $Archive -DestinationPath $relocated
    $consumerBuild = Join-Path $Build 'consumer-build'
    Invoke-Tool 'configure-consumers' $CMake @('-S', "$Root\tests\windows\consumer", '-B', $consumerBuild, '-G', 'Ninja', '-DCMAKE_BUILD_TYPE=Release', '-DCMAKE_C_COMPILER=cl', '-DCMAKE_CXX_COMPILER=cl', "-DWalletCore_DIR=$relocated\lib\cmake\WalletCore", "-DWALLET_CORE_PACKAGE=$relocated")
    Invoke-Tool 'build-consumers' $CMake @('--build', $consumerBuild, '--parallel', "$Jobs")
    $dll = "$relocated\shared\bin\TrustWalletCore.dll"
    Copy-Item -LiteralPath $dll -Destination $consumerBuild -Force
    $toolPath = $env:PATH
    $env:PATH = "$env:SystemRoot\System32;$env:SystemRoot"
    try {
        foreach ($consumer in @('consumer_Shared_c', 'consumer_Shared_cpp', 'consumer_Static_c', 'consumer_Static_cpp')) {
            Invoke-Tool $consumer "$consumerBuild\$consumer.exe" @()
        }
        Invoke-Tool 'consumer-dynamic' "$consumerBuild\consumer_dynamic.exe" @($dll)
    } finally { $env:PATH = $toolPath }
    $image = [IO.File]::ReadAllBytes($dll)
    $pe = [BitConverter]::ToInt32($image, 0x3c)
    if ([BitConverter]::ToUInt32($image, $pe) -ne 0x4550 -or [BitConverter]::ToUInt16($image, $pe + 4) -ne 0x8664) { throw 'Expected Windows x64 PE library' }
    Invoke-Tool 'dll-exports' 'dumpbin.exe' @('/exports', $dll)
    $actualExports = @(Get-Content (Join-Path $Logs 'dll-exports.log') | ForEach-Object {
        if ($_ -match '^\s+\d+\s+[0-9A-F]+\s+[0-9A-F]+\s+(\S+)') { $Matches[1] }
    })
    $expectedExports = @(Get-Content (Join-Path $Build 'TrustWalletCore.def') | Select-Object -Skip 2 | ForEach-Object { $_.Trim() } | Where-Object { $_ })
    if (Compare-Object $expectedExports $actualExports) { throw 'DLL exports differ from the public C API definition' }
    & (Join-Path $PSScriptRoot 'windows-check-exports.ps1') -Exports $actualExports
    Invoke-Tool 'dll-dependencies' 'dumpbin.exe' @('/dependents', $dll)
    $dependencies = @(Get-Content (Join-Path $Logs 'dll-dependencies.log') | Where-Object { $_ -match '^\s+[\w.-]+\.dll\s*$' } | ForEach-Object { $_.Trim() })
    foreach ($dependency in $dependencies) {
        if ($dependency -notmatch '^(api-ms-win-|ext-ms-win-|VCRUNTIME140(?:_1)?\.dll$|MSVCP140(?:_\w+)?\.dll$|ucrtbase\.dll$|KERNEL32\.dll$|NTDLL\.dll$|USERENV\.dll$|WS2_32\.dll$|DBGHELP\.dll$|BCRYPT(?:PRIMITIVES)?\.dll$|ADVAPI32\.dll$)') {
            throw "Unexpected runtime dependency: $dependency"
        }
    }
    [Console]::WriteLine("Verified x64 DLL, $($actualExports.Count) public exports, /MD dependencies, and five relocated consumers")
}

try {
    if (![Environment]::Is64BitOperatingSystem -or ![Environment]::Is64BitProcess) { throw 'Run 64-bit PowerShell on Windows x64' }
    Set-Location $Root
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (!(Test-Path $vswhere)) { throw 'Install Visual Studio 2022 C++ tools, Windows SDK, and C++ Clang tools for Windows' }
    $vs = & $vswhere -latest -version '[17.0,18.0)' -products '*' -property installationPath
    if (!$vs) { throw 'Visual Studio 2022 was not found' }
    $command = '"' + $vs + '\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul && set'
    & cmd.exe /s /c $command | ForEach-Object { if ($_ -match '^([^=]+)=(.*)$') { [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process') } }
    if ($LASTEXITCODE -ne 0) { throw 'Cannot initialize Visual Studio x64 tools' }
    $Link = Join-Path $env:VCToolsInstallDir 'bin\Hostx64\x64\link.exe'
    $Clang = Join-Path $vs 'VC\Tools\Llvm\x64\bin\clang-cl.exe'
    $CMake = Join-Path $vs 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
    $Ninja = Join-Path $vs 'Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe'
    foreach ($tool in @($Clang, $Link, $CMake, $Ninja)) { if (!(Test-Path $tool)) { throw "Missing VS tool: $tool; install the C++ Clang and CMake components" } }
    if (!$Msys2Root) {
        $Msys2Root = @('C:\msys64', 'E:\msys64', 'F:\msys64') | Where-Object { Test-Path "$_\usr\bin\bash.exe" } | Select-Object -First 1
    }
    if (!$Msys2Root) { throw 'MSYS2 was not found; pass -Msys2Root with its installation directory' }
    $Bash = Join-Path $Msys2Root 'usr\bin\bash.exe'
    $Ruby = Join-Path $Msys2Root 'usr\bin\ruby.exe'
    foreach ($tool in @($Bash, $Ruby, (Join-Path $Msys2Root 'usr\bin\perl.exe'))) {
        if (!(Test-Path $tool)) { throw 'MSYS2 Bash, Ruby and Perl are required: pacman -S --needed ruby perl' }
    }
    $Rustup = Join-Path $env:USERPROFILE '.cargo\bin\rustup.exe'
    $Cargo = Join-Path $env:USERPROFILE '.cargo\bin\cargo.exe'
    if (!(Test-Path $Rustup) -or !(Test-Path $Cargo)) { throw 'Install rustup for Windows x64 MSVC' }
    # Keep MSVC's link.exe before MSYS2's unrelated coreutils link.exe.
    $env:PATH = "$Build\tools\bin;$(Split-Path $Cargo);$(Split-Path $Clang);$(Split-Path $CMake);$(Split-Path $Ninja);$env:PATH;$Msys2Root\usr\bin"
    $env:CARGO_BUILD_JOBS = "$Jobs"
    $env:PREFIX = $Prefix
    $env:RUSTUP_TOOLCHAIN = $Nightly
    $env:RUSTFLAGS = '-C target-feature=-crt-static'
    $env:TW_RUST_NATIVE_TARGET = $Target
    $env:CARGO_NET_GIT_FETCH_WITH_CLI = 'true'
    $CompilerArguments = @('-G', 'Ninja', '-DCMAKE_BUILD_TYPE=Release', "-DCMAKE_C_COMPILER=$Clang", "-DCMAKE_CXX_COMPILER=$Clang", "-DCMAKE_LINKER=$Link", '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL')
    if ($Stage -in @('All', 'Prepare')) { Prepare }
    if ($Stage -in @('All', 'Build')) { Build-Libraries }
    if ($Stage -in @('All', 'Package')) { Package-Libraries }
    if ($Stage -in @('All', 'Test')) { Test-Libraries }
} finally {
    Set-Location $LocationBefore
    foreach ($name in @([Environment]::GetEnvironmentVariables('Process').Keys)) {
        if (!$EnvironmentBefore.Contains($name)) { [Environment]::SetEnvironmentVariable($name, $null, 'Process') }
    }
    foreach ($name in $EnvironmentBefore.Keys) { [Environment]::SetEnvironmentVariable($name, $EnvironmentBefore[$name], 'Process') }
}
