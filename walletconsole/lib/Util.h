// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace TW::WalletConsole {

using namespace std;

class Util {
public:
    static bool hex(const string& p, string& res);
    static bool base64Enc(const string& p, string& res);
    static bool base64Dec(const string& p, string& res);
    /// Write to file
    static bool fileW(const string& fileName, const string& data, string& res);
    /// Read from file
    static bool fileR(const string& fileName, string& res);

    // Parse string into words
    static vector<string> tokenize(const string& line);
    // trim from start (in place)
    static void trimLeft(std::string& s);
    static void toLower(std::string& s);
    static bool fileExists(const std::string& fileName);
};

} // namespace TW::WalletConsole
