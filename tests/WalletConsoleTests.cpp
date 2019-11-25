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
    cmd.executeLine("addr #");
}

TEST(WalletConsole, setmnemo) {
    auto pos1 = outputss.str().length();
    cmd.executeLine("setmnemo edge defense waste choose enrich upon flee junk siren film clown finish luggage leader kid quick brick print evidence swap drill paddle truly occur");
    string res1 = outputss.str().substr(pos1);
    //cerr << res1 << endl;
    EXPECT_TRUE(res1.find("Mnemonic set (24 words)") >= 0);
    auto pos2 = outputss.str().length();
    cmd.executeLine("dumpseed");
    string res2 = outputss.str().substr(pos2);
    EXPECT_TRUE(res2.length() > 135);
    EXPECT_TRUE(res2.find("7e74b1a8195ae1e8d06f29c9a306f678e5a8cf908075bc52eb3b716f9e50ce8860065c2c18b8a960bb363855d3a340074cba5db505d4f78dd1d94c4e19f20b7a") > 0);
    auto pos3 = outputss.str().length();
    cmd.executeLine("dumpmnemo");
    string res3 = outputss.str().substr(pos3);
    EXPECT_TRUE(res3.length() > 140);
    EXPECT_TRUE(res3.find("edge defense waste choose enrich upon flee junk siren film clown finish luggage leader kid quick brick print evidence swap drill paddle truly occur") > 0);
}

TEST(WalletConsole, newmnemo) {
    auto pos1 = outputss.str().length();
    cmd.executeLine("newmnemo");
    string res1 = outputss.str().substr(pos1);
    EXPECT_TRUE(res1.find("Mnemonic set (24 words)") >= 0);
    auto pos2 = outputss.str().length();
    cmd.executeLine("dumpseed");
    string res2 = outputss.str().substr(pos2);
    EXPECT_TRUE(res2.length() > 135);
    EXPECT_TRUE(res2.find("Result") >= 0);
    auto pos3 = outputss.str().length();
    cmd.executeLine("dumpmnemo");
    string res3 = outputss.str().substr(pos3);
    EXPECT_TRUE(res3.length() > 140);
}

TEST(WalletConsole, dumpdp) {
    {
        cmd.executeLine("coin btc");
        auto pos1 = outputss.str().length();
        cmd.executeLine("dumpdp");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("m/84'/0'/0'/0/0") > 0);
    }
    {
        cmd.executeLine("coin eth");
        auto pos1 = outputss.str().length();
        cmd.executeLine("dumpdp");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("m/44'/60'/0'/0/0") > 0);
    }
    {
        cmd.executeLine("coin algo");
        auto pos1 = outputss.str().length();
        cmd.executeLine("dumpdp");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("m/44'/283'/0'/0'/0'") > 0);
    }
}

TEST(WalletConsole, derive) {
    // Step-by-step derivation, mnemo -> pri -> pub -> addr
    cmd.executeLine("coin btc");
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("pridp m/84'/0'/0'/0/1");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("5133262c125d7019da000e6639be23c1726083980862cfb7199f849109875d5b") > 0);
        EXPECT_TRUE(res1.find("m/84'/0'/0'/0/1") > 0);
    }
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("pubpri #");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("02a3e079928c5a4c9ec4a5cea9a499e6bfbb7772b03ea8c1ea7921d0aced43b209") > 0);
    }
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrpub #");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("bc1qejkm69ert6jqrp2u4n0m6g9ds4ravas2dw3af0") > 0);
    }
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("addr #");
        string res1 = outputss.str().substr(pos1);
        //cerr << res1 << endl;
        EXPECT_TRUE(res1.find("bc1qejkm69ert6jqrp2u4n0m6g9ds4ravas2dw3af0") > 0);
        EXPECT_TRUE(res1.find("not") == string::npos);
        EXPECT_TRUE(res1.find("NOT") == string::npos);
    }
}

TEST(WalletConsole, addrdef) {
    {
        cmd.executeLine("coin btc");
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrdef");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("bc1q5mv7jf4uzyf0524sxzrpucdf6tnrd0maq9k8zv") > 0);
    }
    {
        cmd.executeLine("coin eth");
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrdef");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("0x35A3cAF748eD8d08B26f944A9Cad1f320DBbDFbd") > 0);
    }
    {
        cmd.executeLine("coin nano");
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrdef");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("nano_3yyipbgtnd7183k61nkh5mxnt9wpsfhto95mksdqj6s7p45mwj9osai7asad") > 0);
    }
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

TEST(WalletConsole, buffer) {
    cmd.executeLine("newkey");
    cmd.executeLine("newkey");
    auto pos0 = outputss.str().length();
    cmd.executeLine("#");
    string res = outputss.str().substr(pos0);
    auto pos1 = outputss.str().length();
    cmd.executeLine("#1");
    string res2 = outputss.str().substr(pos1);
    EXPECT_TRUE(res.length() > 70);
    EXPECT_TRUE(res2.length() > 70);
    EXPECT_TRUE(res != res2);
    auto pos2 = outputss.str().length();
    cmd.executeLine("buffer");
    string res3 = outputss.str().substr(pos2);
    int lines3 = countLines(res3);
    EXPECT_TRUE(lines3 >= 3);
}

