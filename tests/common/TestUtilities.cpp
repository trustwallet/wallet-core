// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <string>
#include <fstream>

using namespace std;

/// Return a writable temp dir which can be used to create files during testing
string getTestTempDir(void) {
    // In general, tests should not use hardcoded "/tmp", but TEST_TMPDIR env var.
    const char* fromEnvironment = getenv("TEST_TMPDIR");
    if (fromEnvironment == NULL || fromEnvironment[0] == '\0') { return "/tmp"; }
    return string(fromEnvironment);
}

nlohmann::json loadJson(std::string path) {
    std::ifstream stream(path);
    nlohmann::json json;
    stream >> json;
    return json;
}
