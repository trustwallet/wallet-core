// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

/// Return a writable temp dir which can be used to create files during testing
string getTestTempDir(void) {
    // In general, tests should not use hardcoded "/tmp", but TEST_TMPDIR env var.
#ifdef _WIN32
    const auto* fromEnvironment = _wgetenv(L"TEST_TMPDIR");
    const auto path = (fromEnvironment == nullptr || fromEnvironment[0] == L'\0'
        ? std::filesystem::temp_directory_path()
        : std::filesystem::path(fromEnvironment)).u8string();
    return string(path.begin(), path.end());
#else
    const char* fromEnvironment = getenv("TEST_TMPDIR");
    if (fromEnvironment == NULL || fromEnvironment[0] == '\0') { return "/tmp"; }
    return string(fromEnvironment);
#endif
}

nlohmann::json loadJson(std::string path) {
    std::ifstream stream(std::filesystem::u8path(path));
    nlohmann::json json;
    stream >> json;
    return json;
}
