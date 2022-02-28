// Copyright © 2017-2020 Trust Wallet.
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
