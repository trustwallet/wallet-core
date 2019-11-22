// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../walletconsole/lib/CommandExecutor.h"
#include "../walletconsole/lib/Util.h"

#include <sstream>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::WalletConsole;

// Test some command execution

static stringstream outputss;
static CommandExecutor cmd(outputss);
static int staticInit() { cmd.init(); return 0; }
static int dummyStatic = staticInit();

int countLines(const string& text) {
    int lines = 0;
    for(int i = 0; i < text.length(); ++i)
    {
        if (text[i] == '\n') ++lines;
    }
    return lines;
}

TEST(WalletConsole, help) {
    auto pos0 = outputss.str().length();
    cmd.executeLine("help");
    string res = outputss.str().substr(pos0);
    // check that there is long output
    EXPECT_TRUE(res.length() > 500);
    int lines = countLines(res);
    EXPECT_TRUE(lines > 15);
}

TEST(WalletConsole, coins) {
    auto pos0 = outputss.str().length();
    cmd.executeLine("coins");
    string res = outputss.str().substr(pos0);
    // check that there is long output
    int lines = countLines(res);
    EXPECT_TRUE(lines > 40);
    Util::toLower(res);
    EXPECT_TRUE(res.find("bitcoin") > 0);
    EXPECT_TRUE(res.find("btc") > 0);
    EXPECT_TRUE(res.find("ethereum") > 0);
    EXPECT_TRUE(res.find("binance") > 0);
    EXPECT_TRUE(res.find("bnb") > 0);
}

TEST(WalletConsole, newkey1) {
    auto pos0 = outputss.str().length();
    cmd.executeLine("newkey");
    string res = outputss.str().substr(pos0);
    // tokenize output
    auto words = Util::tokenize(res);
    ASSERT_TRUE(words.size() >= 2);
    // check 2nd word
    EXPECT_TRUE(words[1].length() >= 32);
}

TEST(WalletConsole, pubpri1) {
    cmd.executeLine("coin btc");
    auto pos0 = outputss.str().length();
    cmd.executeLine("pubpri 7d40d6a74e98543f545852989d54712834f9c86eddee89303a2083219749e38c");
    string res = outputss.str().substr(pos0);
    EXPECT_TRUE(res.find("0200266ab7dc3efec040cc8b9714ff49cc8339d2f30d9bab8a4b11043e1bdfee37") > 0);
}

TEST(WalletConsole, addrbtc1) {
    cmd.executeLine("coin btc");
    auto pos0 = outputss.str().length();
    cmd.executeLine("addrpub 0200266ab7dc3efec040cc8b9714ff49cc8339d2f30d9bab8a4b11043e1bdfee37");
    string res = outputss.str().substr(pos0);
    EXPECT_TRUE(res.find("bc1qgk5kpecyjrs60jfxu25f2m60j50gpfsajcuses") > 0);
}

TEST(WalletConsole, hex1) {
    auto pos0 = outputss.str().length();
    cmd.executeLine("hex Hello");
    string res = outputss.str().substr(pos0);
    EXPECT_TRUE(res.find("48656c6c6f") > 0);
}

TEST(WalletConsole, hexBase64EncDec) {
    cmd.executeLine("hex Hello");
    cmd.executeLine("base64enc #");
    cmd.executeLine("buffer");
    auto pos0 = outputss.str().length();
    cmd.executeLine("base64dec #");
    string res = outputss.str().substr(pos0);
    EXPECT_TRUE(res.find("48656c6c6f") > 0);
}