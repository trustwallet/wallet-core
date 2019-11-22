// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "WalletConsole.h"
#include "CommandExecutor.h"
#include "Keys.h"
#include "Coins.h"
#include "Util.h"
#include "Address.h"
#include "TonCoin.h"

#include "Base64.h"
#include "HexCoding.h"
#include "Data.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;
using namespace TW;


void WalletConsole::init() {
    cout << endl;
    cout << "Wallet-core Console                          (c) TrustWallet" << endl;
    cout << "Type 'help' for list of commands." << endl;
    cout << endl;

    _executor.init();
}

void WalletConsole::loop() {
    while (true) {
        cout << "> ";
        string line;
        getline(cin, line);
        //cout << "Read line: '" << line << "'" << endl;
        trimLeft(line);
        if (line.length() == 0) { continue; }
        vector<string> params;
        auto cmd = parseLine(line, params);
        //cout << "Read cmd: '" << cmd << "'" << endl;
        if (isExit(cmd)) {
            break;
        }

        _executor.execute(cmd, params);
    }
    cout << "Bye!" << endl << endl;
}

bool WalletConsole::isExit(const string& cmd) {
    if (cmd == "quit" || cmd == "exit") { return true; }
    return false;
}

vector<string> WalletConsole::tokenize(const string& line) {
    vector<string> tok;
    size_t n = line.length();
    size_t idx = 0;
    while (true) {
        auto spaceIdx = line.find(' ', idx);
        if (spaceIdx == string::npos || spaceIdx >= n) {
            // no more space
            if (idx < n) {
                tok.push_back(line.substr(idx, n - idx));
            }
            return tok;
        }
        // there is a space, take current word up to it
        if (idx < spaceIdx) {
            tok.push_back(line.substr(idx, spaceIdx - idx));
        }
        idx = spaceIdx + 1;
    }
}

string WalletConsole::parseLine(const string& line, vector<string>& params) {
    auto tokens = tokenize(line);
    assert(tokens.size() > 0);
    auto cmd = tokens[0];
    WalletConsole::toLower(cmd);
    params = tokens;
    return cmd;
}

void WalletConsole::trimLeft(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void WalletConsole::toLower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
}

} // namespace TW::WalletConsole
