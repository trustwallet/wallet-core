// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <filesystem>
#include <gtest/gtest.h>
#include <string>

std::string TESTS_ROOT;

int main(int argc, char** argv) {
    // current path
    auto path = std::filesystem::current_path();
    // executable path
    path.append(argv[0]);
    // normalize
    path = std::filesystem::canonical(path);
    // root path
    path = path.parent_path().parent_path().parent_path();
    TESTS_ROOT = path.append("tests").string();
    std::cout<<"TESTS_ROOT: "<<path<<std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
