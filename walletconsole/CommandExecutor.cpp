// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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


CommandExecutor::CommandExecutor()
    : 
    _coins(), 
    _keys(_coins),
    _address(_coins)
{
}

void CommandExecutor::init() {
    _coins.init();
    setCoin("btc", true);
}

void CommandExecutor::help() {
    cout << "Commands:" << endl;
    cout << "  exit                    Exit" << endl;
    cout << "  quit                    Exit" << endl;
    cout << "  help                    This help" << endl;
    cout << "Inputs, buffer:" << endl;
    cout << "  #<n>                    Print nth previous result" << endl;
    cout << "  #                       Print last result" << endl;
    cout << "  buffer                  Print buffer values" << endl;
    cout << "Coins:" << endl;
    cout << "  coins                   List known coins" << endl;
    cout << "  coin <coin>             Set active coin, selected by its ID or symbol or name" << endl;
    cout << "Keys:" << endl;
    cout << "  newkey                  Create new pseudo-random 32-byte key" << endl;
    cout << "  pubpri <prikey>         Derive public key from private key (type is coin-dependent)" << endl;
    cout << "  pripub <pubkey>         Derive private key from public key :)" << endl;
    cout << "Addresses:" << endl;
    cout << "  addrpub <pubkey>        Create <coin> address from public key." << endl;
    cout << "  addrpri <prikey>        Create <coin> address from private key." << endl;
    cout << "  addr <addr>             Check string <coin> address." << endl;
    cout << "Coin-specific methods:" << endl;
    cout << "  tonmsg <prikey>         Build TON account initialization message." << endl;
    cout << "Transformations:" << endl;
    cout << "  hex <inp>               Encode given string to hex" << endl;
    cout << "  base64enc <inp>         Encode given hex data to Base64" << endl;
    cout << "  base64dec <inp>         Decode given Base64 string to hex data" << endl;
    cout << "File methods:" << endl;
    cout << "  filew <filename> <data> Write data to a (new) binary file." << endl;
    cout << "  filer <filename>        Read data from a binary file." << endl;
    cout << endl;
    cout << "Examples:" << endl;
    cout << "> coin bitcoin <Enter> newkey <Enter> addrpri # <Enter> filew btcaddr.txt #" << endl;
    cout << "> newkey <Enter> pubpri # <Enter> addrpub #" << endl;
    cout << "> coin algo <Enter> addr LCSUSBOLNVT6BND6DWWGM4DLVUYJN3PGBT4T7LTCMDMKS7TR7FZAOHOVPE" << endl;
    cout << "> hex Hello <Enter> base64enc # <Enter> base64dec # <Enter> buffer" << endl;
    cout << endl;
}

bool CommandExecutor::executeOne(const string& cmd, const vector<string>& params_in, string& res) {
    if (cmd == "exit" || cmd == "quit") {
        // handled in loop
        cout << "Bye!" << endl;
        return false;
    }

    // prepare inputs
    vector<string> params;
    if (!prepareInputs(params_in, params)) {
        cout << "Error processing input(s)" << endl;
        return false;
    }
    
    if (cmd == "help") { help(); return false; }
    if (cmd[0] == '#') {
        // echo input, with substitution
        cout << "Previous result is:  " << params[0] << endl;
        return false;
    }
    if (cmd == "buffer") { _buffer.buffer(); return false; }

    if (cmd == "coins") { _coins.coins(); return false; }
    if (cmd == "coin") { if (!checkMinParams(params, 1)) { return false; } setCoin(params[1], true); return false; }

    if (cmd == "newkey") { return _keys.newkey(res); }
    if (cmd == "pubpri") { if (!checkMinParams(params, 1)) { return false; } return _keys.pubpri(_activeCoin, params[1], res); }
    if (cmd == "pripub") { if (!checkMinParams(params, 1)) { return false; } return _keys.pripub(params[1], res); }

    if (cmd == "addrpub") { if (!checkMinParams(params, 1)) { return false; } return _address.addrpub(_activeCoin, params[1], res); }
    if (cmd == "addrpri") { if (!checkMinParams(params, 1)) { return false; } return _address.addrpri(_activeCoin, params[1], res); }
    if (cmd == "addr") { if (!checkMinParams(params, 1)) { return false; } return _address.addr(_activeCoin, params[1], res); }

    if (cmd == "tonmsg") { if (!checkMinParams(params, 1)) { return false; } setCoin("ton", false); return TonCoin::tonmsg(params[1], res); }

    if (cmd == "hex") { if (!checkMinParams(params, 1)) { return false; } return Util::hex(params[1], res); }
    if (cmd == "base64enc") { if (!checkMinParams(params, 1)) { return false; } return Util::base64enc(params[1], res); }
    if (cmd == "base64dec") { if (!checkMinParams(params, 1)) { return false; } return Util::base64dec(params[1], res); }
    
    if (cmd == "filew") { if (!checkMinParams(params, 2)) { return false; } return Util::filew(params[1], params[2], res); }
    if (cmd == "filer") { if (!checkMinParams(params, 1)) { return false; } return Util::filer(params[1], res); }

    // fallback
    help();
    return false;
}

void CommandExecutor::execute(const string& cmd, const vector<string>& params) {
    try {
        string resultStr;
        bool res = executeOne(cmd, params, resultStr);
        if (res && resultStr.length() > 0) {
            // there is a new result
            cout << "Result:  " << resultStr << endl;
            _buffer.addResult(resultStr);
        }
    } catch (exception& ex) {
        cout << "Error while executing command, " << ex.what() << endl;
    }
}

bool CommandExecutor::prepareInputs(const vector<string>& p_in, vector<string>& p_out) {
    p_out = vector<string>{};
    for (auto p: p_in) {
        string p2;
        if (!_buffer.prepareInput(p, p2)) {
            return false;
        }
        p_out.push_back(p2);
    }
    assert(p_out.size() == p_in.size());
    return true;
}

bool CommandExecutor::checkMinParams(const vector<string>& params, int n) {
    if (params.size() - 1 >= n) {
        return true;
    }
    cout << "At least " << n << " parameters are needed! See 'help'" << endl;
    //help();
    return false;
}

bool CommandExecutor::setCoin(const string& coin, bool force) {
    Coin c;
    if (!_coins.findCoin(coin, c)) {
        return false;
    }
    if (_activeCoin == c.id && !force) {
        // already on that coin
        return true;
    }
    _activeCoin = c.id;
    cout << "Set active coin to: " << c.id << "    Use 'coin' to change.  (name: '" << c.name << "'  symbol: " << c.symbol << "  numericalid: " << c.c << ")" << endl;
    return true;
}

} // namespace TW::WalletConsole
