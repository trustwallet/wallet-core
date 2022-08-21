// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>
#include <string>

std::string TESTS_ROOT;
std::string find_executable();

void init_tests_root() {
    const auto path = find_executable();
    std::vector<std::string> components;
    boost::split(components, path, boost::is_any_of("/"));
    components.erase(components.end() - 3, components.end());
    components.emplace_back("tests");
    TESTS_ROOT = boost::join(components, "/");
}

int main(int argc, char** argv) {
    init_tests_root();
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
