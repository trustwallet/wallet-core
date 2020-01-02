// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>
#include <iostream>

namespace TW::WalletConsole {

using namespace std;

class Util {
public:
    static bool hex(const string& p, string& res);
    static bool base64Encode(const string& p, string& res);
    static bool base64Decode(const string& p, string& res);
    /// Write to file
    static bool fileW(const string& fileName, const string& data, string& res, ostream& out);
    /// Read from file
    static bool fileR(const string& fileName, string& res, ostream& out);

    // Parse string into words
    static vector<string> tokenize(const string& line);
    // trim from start (in place)
    static void trimLeft(std::string& s);
    static void toLower(std::string& s);
    static bool fileExists(const std::string& fileName);
};

} // namespace TW::WalletConsole
