# Windows x64

Wallet Core's native Windows build uses **ClangCL with the MSVC ABI**, Rust's
`x86_64-pc-windows-msvc` target, and the dynamic Microsoft CRT (`/MD`). Both
static libraries and a DLL expose the existing public C API. Release x64 is the
supported configuration; ARM64, MinGW/Cygwin, `/MT`, and building Wallet Core
itself with `cl.exe` are outside this configuration.

The Rust target requires Windows 10 or later (Windows Server 2016 or later),
as documented in [Rust platform support](https://doc.rust-lang.org/rustc/platform-support.html).

## Build and test

Install Visual Studio 2022's Desktop development with C++ workload, Windows SDK,
C++ CMake tools, and C++ Clang tools for Windows. Install rustup for Windows
MSVC, and MSYS2 with `pacman -S --needed ruby perl`. MSYS2 supplies build-time
utilities only; the distributed library does not require its runtime.

From 64-bit PowerShell in the checkout:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/windows-build.ps1
# For a nonstandard MSYS2 installation:
powershell -NoProfile -ExecutionPolicy Bypass -File tools/windows-build.ps1 -Msys2Root E:\msys64 -Jobs 8
```

The script installs the repository's fixed Rust nightly without changing the
global default, builds the real Rust core with its default features and locked
dependencies, prepares Protobuf/GTest/JSON at the versions in
`tools/dependencies-version`, and uses Boost 1.83.0 headers and cbindgen 0.29.2.
It then generates files, builds both libraries, packages them, and runs tests.
Git and dependency downloads need network access; failed downloads are not a
reason to remove coin features or update the lockfile.

For retries, `-Stage Prepare`, `Build`, `Package`, or `Test` runs just that stage
using preceding stages' outputs. Static and shared builds have separate build
directories. `TW_WINDOWS_BUILD_SHARED` defaults to `OFF`; the DLL build turns it
on. Rust linkage metadata and the public `.def` export list are generated during
`Prepare` and passed to CMake. `TW_RUST_NATIVE_TARGET` selects only the native
Rust build; it does not select Android or WASM targets.

Outputs:

```text
build/windows/WalletCore-Windows-x64.zip
build/windows/logs/                   # Build logs, GTest XML, ABI/dependency checks
```

The archive contains `include/TrustWalletCore`, `shared/bin/TrustWalletCore.dll`,
the import library in `shared/lib`, the four static libraries in `static/lib`,
`lib/cmake/WalletCore/WalletCoreConfig.cmake`, licenses, and SHA-256 metadata.
The static archive remains separate from its Rust and C++ dependencies.

## Consume the DLL

Install a compatible **Microsoft Visual C++ Redistributable x64** on deployment
machines. Both the DLL and the static package use `/MD`; neither includes the
VC runtime. Use Release consumers with `/MD`.

```cmake
cmake_minimum_required(VERSION 3.18)
project(MyWallet C CXX)
set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
find_package(WalletCore CONFIG REQUIRED)
add_executable(mywallet main.c)
target_link_libraries(mywallet PRIVATE WalletCore::Shared)
add_custom_command(TARGET mywallet POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:WalletCore::Shared> $<TARGET_FILE_DIR:mywallet>)
```

Configure with `-DCMAKE_BUILD_TYPE=Release` and
`-DWalletCore_DIR=C:/path/to/package/lib/cmake/WalletCore`. Both MSVC C/C++
consumers and dynamic loaders can call the public C functions. Only public
`TW*` functions are exported; private C++ and Rust symbols are not supported ABI.
Continue freeing handles and buffers with the corresponding Wallet Core
deletion functions.

## Consume the static libraries

Use the same configuration, replacing the linked target:

```cmake
target_link_libraries(mywallet PRIVATE WalletCore::Static)
```

This target adds TrezorCrypto, Protobuf, the Rust static library, and the native
Windows libraries reported by the real Rust core. Do not mix this package with
MinGW object files, `/MT`, or Debug CRT libraries.

## Verification

`Test` runs the existing GTest suite with `TEST_TMPDIR` and `TW_TESTS_ROOT`,
using fixture and temporary directories with spaces and non-ASCII characters,
secure RNG boundary/failure checks, five MSVC C/C++ and dynamic-loading consumers
of a relocated, extracted package without build tools on `PATH`, and
PE/export/dependency checks. Windows
keystore paths are UTF-8; temporary-file replacement supports existing targets,
keeps the destination when replacement fails, and does not copy across volumes.
Use a different temporary path on the same volume as the destination.

The legacy Check-based Trezor test target remains on its existing platforms.
Windows uses GTest and the dedicated RNG check instead. Passing Windows checks
alone does not establish Linux, macOS/iOS, Android, or WASM regression results;
those checks remain required before merging into a common mainline.
