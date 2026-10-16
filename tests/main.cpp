// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <cstdlib>
#include <filesystem>
#include <gtest/gtest.h>
#include <string>

std::string TESTS_ROOT;

int main(int argc, char** argv) {
#ifdef _WIN32
    const auto* sourceRoot = _wgetenv(L"TW_TESTS_ROOT");
#else
    const auto* sourceRoot = std::getenv("TW_TESTS_ROOT");
#endif
    std::filesystem::path path;
    if (sourceRoot != nullptr && sourceRoot[0] != '\0') {
#ifdef _WIN32
        path = std::filesystem::canonical(sourceRoot);
#else
        path = std::filesystem::canonical(std::filesystem::u8path(sourceRoot));
#endif
    } else {
        path = std::filesystem::canonical(std::filesystem::current_path() / argv[0]);
        path = path.parent_path().parent_path().parent_path() / "tests";
    }
#ifdef _WIN32
    const auto utf8Path = path.u8string();
    TESTS_ROOT.assign(utf8Path.begin(), utf8Path.end());
#else
    TESTS_ROOT = path.string();
#endif
    std::cout << "TESTS_ROOT: " << TESTS_ROOT << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
