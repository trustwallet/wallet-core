// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    istream& _inst;
    ostream& _outst;
    CommandExecutor _executor;

public:
    WalletConsole(istream& ins, ostream& outs) : _inst(ins), _outst(outs), _executor(outs) { init(); }
    void init();
    void loop();
    static bool isExit(const string& cmd);
};

} // namespace TW::WalletConsole
