// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Buffer.h"
#include "Keys.h"
#include "Coins.h"
#include "Address.h"
#include "CommandExecutor.h"

#include "Data.h"

#include <string>
#include <vector>
#include <cassert>
#include <iostream>

namespace TW::WalletConsole {

using namespace std;

class WalletConsole {
protected:
    CommandExecutor _executor;

public:
    WalletConsole() { init(); }
    void init();
    void loop();
    static bool isExit(const string& cmd);
    static vector<string> tokenize(const string& line);
    static string parseLine(const string& line, vector<string>& params);
    // trim from start (in place)
    static void trimLeft(std::string& s);
    static void toLower(std::string& s);
};

} // namespace TW::WalletConsole
