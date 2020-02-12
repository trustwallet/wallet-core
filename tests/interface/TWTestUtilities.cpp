// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <string>
#include <filesystem>

using namespace std;

/// Return a writeable temp dir which can be used to create files during testing
string getTestTempDir(void) {
    // In general, tests should not use hardcoded "/tmp", but TEST_TMPDIR env var.
    const char* fromEnvironment = getenv("TEST_TMPDIR");
    if (fromEnvironment == NULL || fromEnvironment[0] == '\0') { return "/tmp"; }
    return string(fromEnvironment);
}
