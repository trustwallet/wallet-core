// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../walletconsole/lib/CommandExecutor.h"
#include "../walletconsole/lib/WalletConsole.h"
#include "../walletconsole/lib/Util.h"

#include <sstream>
#include <cstdio>
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

TEST(WalletConsole, loopExit) {
    EXPECT_TRUE(TW::WalletConsole::WalletConsole::isExit("exit"));
    EXPECT_TRUE(TW::WalletConsole::WalletConsole::isExit("quit"));
    EXPECT_FALSE(TW::WalletConsole::WalletConsole::isExit("newkey"));
    EXPECT_FALSE(TW::WalletConsole::WalletConsole::isExit("help"));

    stringstream inss;
    stringstream outss;
    inss << "coin eth" << endl << "newKey" << endl << "exit" << endl;
    TW::WalletConsole::WalletConsole console(inss, outss);
    console.loop();
    string res = outss.str();
    EXPECT_TRUE(res.find("Result") != string::npos);
    EXPECT_TRUE(res.find("Bye") != string::npos);
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
    EXPECT_TRUE(res.find("bitcoin") != string::npos);
    EXPECT_TRUE(res.find("btc") != string::npos);
    EXPECT_TRUE(res.find("ethereum") != string::npos);
    EXPECT_TRUE(res.find("binance") != string::npos);
    EXPECT_TRUE(res.find("bnb") != string::npos);
}

TEST(WalletConsole, newkey1) {
    auto pos0 = outputss.str().length();
    cmd.executeLine("newKey");
    string res = outputss.str().substr(pos0);
    // tokenize output
    auto words = Util::tokenize(res);
    ASSERT_TRUE(words.size() >= 2);
    // check 2nd word
    EXPECT_TRUE(words[1].length() >= 32);
}

TEST(WalletConsole, pubPri1) {
    cmd.executeLine("coin btc");
    auto pos1 = outputss.str().length();
    cmd.executeLine("pubPri 7d40d6a74e98543f545852989d54712834f9c86eddee89303a2083219749e38c");
    string res1 = outputss.str().substr(pos1);
    EXPECT_TRUE(res1.find("035431222ba2639ea442969c280a6965374bdd092ff9daf119460806abb093c93c") != string::npos);
}

TEST(WalletConsole, addrbtc1) {
    cmd.executeLine("coin btc");
    auto pos0 = outputss.str().length();
    cmd.executeLine("addrPub 0200266ab7dc3efec040cc8b9714ff49cc8339d2f30d9bab8a4b11043e1bdfee37");
    string res = outputss.str().substr(pos0);
    EXPECT_TRUE(res.find("bc1qgk5kpecyjrs60jfxu25f2m60j50gpfsajcuses") != string::npos);
    cmd.executeLine("addr #");
}

TEST(WalletConsole, setMnemonic) {
    auto pos1 = outputss.str().length();
    cmd.executeLine("setMnemonic edge defense waste choose enrich upon flee junk siren film clown finish luggage leader kid quick brick print evidence swap drill paddle truly occur");
    string res1 = outputss.str().substr(pos1);
    //cerr << res1 << endl;
    EXPECT_TRUE(res1.find("Mnemonic set (24 words)") >= 0);
    auto pos2 = outputss.str().length();
    cmd.executeLine("dumpSeed");
    string res2 = outputss.str().substr(pos2);
    EXPECT_TRUE(res2.length() > 135);
    EXPECT_TRUE(res2.find("7e74b1a8195ae1e8d06f29c9a306f678e5a8cf908075bc52eb3b716f9e50ce8860065c2c18b8a960bb363855d3a340074cba5db505d4f78dd1d94c4e19f20b7a") != string::npos);
    auto pos3 = outputss.str().length();
    cmd.executeLine("dumpMnemonic");
    string res3 = outputss.str().substr(pos3);
    EXPECT_TRUE(res3.length() > 140);
    EXPECT_TRUE(res3.find("edge defense waste choose enrich upon flee junk siren film clown finish luggage leader kid quick brick print evidence swap drill paddle truly occur") != string::npos);
}

TEST(WalletConsole, newMnemonic) {
    auto pos1 = outputss.str().length();
    cmd.executeLine("newMnemonic");
    string res1 = outputss.str().substr(pos1);
    EXPECT_TRUE(res1.find("Mnemonic set (24 words)") >= 0);
    auto pos2 = outputss.str().length();
    cmd.executeLine("dumpSeed");
    string res2 = outputss.str().substr(pos2);
    EXPECT_TRUE(res2.length() > 135);
    EXPECT_TRUE(res2.find("Result") >= 0);
    auto pos3 = outputss.str().length();
    cmd.executeLine("dumpMnemonic");
    string res3 = outputss.str().substr(pos3);
    EXPECT_TRUE(res3.length() > 140);
}

TEST(WalletConsole, dumpdp) {
    {
        cmd.executeLine("coin btc");
        auto pos1 = outputss.str().length();
        cmd.executeLine("dumpDP");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("m/84'/0'/0'/0/0") != string::npos);
    }
    {
        cmd.executeLine("coin eth");
        auto pos1 = outputss.str().length();
        cmd.executeLine("dumpDP");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("m/44'/60'/0'/0/0") != string::npos);
    }
    {
        cmd.executeLine("coin algo");
        auto pos1 = outputss.str().length();
        cmd.executeLine("dumpDP");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("m/44'/283'/0'/0'/0'") != string::npos);
    }
}

TEST(WalletConsole, derive) {
    // Step-by-step derivation, mnemo -> pri -> pub -> addr
    cmd.executeLine("coin btc");
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("priDP m/84'/0'/0'/0/1");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("5133262c125d7019da000e6639be23c1726083980862cfb7199f849109875d5b") != string::npos);
        EXPECT_TRUE(res1.find("m/84'/0'/0'/0/1") != string::npos);
    }
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("pubPri #");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("02a3e079928c5a4c9ec4a5cea9a499e6bfbb7772b03ea8c1ea7921d0aced43b209") != string::npos);
    }
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrPub #");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("bc1qejkm69ert6jqrp2u4n0m6g9ds4ravas2dw3af0") != string::npos);
    }
    {
        auto pos1 = outputss.str().length();
        cmd.executeLine("addr #");
        string res1 = outputss.str().substr(pos1);
        //cerr << res1 << endl;
        EXPECT_TRUE(res1.find("bc1qejkm69ert6jqrp2u4n0m6g9ds4ravas2dw3af0") != string::npos);
        EXPECT_TRUE(res1.find("not") == string::npos);
        EXPECT_TRUE(res1.find("NOT") == string::npos);
    }
}

TEST(WalletConsole, addrDefault) {
    {
        cmd.executeLine("coin btc");
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrDefault");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("bc1q5mv7jf4uzyf0524sxzrpucdf6tnrd0maq9k8zv") != string::npos);
    }
    {
        cmd.executeLine("coin eth");
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrDefault");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("0x35A3cAF748eD8d08B26f944A9Cad1f320DBbDFbd") != string::npos);
    }
    {
        cmd.executeLine("coin nano");
        auto pos1 = outputss.str().length();
        cmd.executeLine("addrDefault");
        string res1 = outputss.str().substr(pos1);
        EXPECT_TRUE(res1.find("nano_3yyipbgtnd7183k61nkh5mxnt9wpsfhto95mksdqj6s7p45mwj9osai7asad") != string::npos);
    }
}

TEST(WalletConsole, hex1) {
    auto pos0 = outputss.str().length();
    cmd.executeLine("hex Hello");
    string res = outputss.str().substr(pos0);
    EXPECT_TRUE(res.find("48656c6c6f") != string::npos);
}

TEST(WalletConsole, hexBase64EncodeDecode) {
    cmd.executeLine("hex Hello");
    cmd.executeLine("base64Encode #");
    cmd.executeLine("buffer");
    auto pos0 = outputss.str().length();
    cmd.executeLine("base64Decode #");
    string res = outputss.str().substr(pos0);
    EXPECT_TRUE(res.find("48656c6c6f") != string::npos);
}

TEST(WalletConsole, buffer) {
    cmd.executeLine("newKey");
    cmd.executeLine("newKey");
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

TEST(WalletConsole, fileWriteRead) {
    cmd.executeLine("newKey");
    auto pos1 = outputss.str().length();
    const string fileName = "./wc_out." + to_string(time(NULL)) + ".bin";
    cmd.executeLine("fileW " + fileName + " #");
    string res1 = outputss.str().substr(pos1);
    EXPECT_TRUE(res1.find("Written to ") != string::npos);

    auto pos2 = outputss.str().length();
    cmd.executeLine("fileR " + fileName);
    string res2 = outputss.str().substr(pos2);
    EXPECT_TRUE(res2.find("ile") != string::npos);

    // clean up created file
    try
    {
        std::remove(fileName.c_str());
    }
    catch(...)
    {
    }
    
    auto pos3 = outputss.str().length();
    cmd.executeLine("fileR __NO_SUCH_FILE__");
    string res3 = outputss.str().substr(pos3);
    EXPECT_TRUE(res3.find("not") != string::npos);
}

// used to fail due to incorrect public key type, see https://github.com/trustwallet/wallet-core/issues/767
TEST(WalletConsole, harmonyAddressDerivation) {
    cmd.executeLine("coin harmony");
    cmd.executeLine("newKey");
    cmd.executeLine("pubPri #");
    auto pos0 = outputss.str().length();
    cmd.executeLine("addrPub #");
    string res1 = outputss.str().substr(pos0);
    // should contain harmony address, no error
    EXPECT_TRUE(res1.find(" one") != string::npos);
    EXPECT_TRUE(res1.find("Result") != string::npos);
    EXPECT_TRUE(res1.find("rror") == string::npos);
}
