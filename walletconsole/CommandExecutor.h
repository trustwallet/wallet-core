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

#include "Data.h"

#include <string>
#include <vector>
#include <cassert>
#include <iostream>

namespace TW::WalletConsole {

using namespace std;

class CommandExecutor {
protected:
    Coins _coins;
    Buffer _buffer;
    Keys _keys;
    Address _address;
    string _activeCoin;

public:
    CommandExecutor();
    void init();
    /// Put result in res.  Return true if meaningful result is returned. 
    bool executeOne(const string& cmd, const vector<string>& params, string& res);
    void execute(const string& cmd, const vector<string>& params);
    bool prepareInputs(const vector<string>& p_in, vector<string>& p_out);
    bool setCoin(const string& coin, bool force);
    static void help();
    static bool checkMinParams(const vector<string>& params, int n);
};

} // namespace TW::WalletConsole
