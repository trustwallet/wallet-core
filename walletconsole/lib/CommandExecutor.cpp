// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CommandExecutor.h"
#include "WalletConsole.h"
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


CommandExecutor::CommandExecutor(ostream& out)
    : 
    _out(out),
    _coins(out), 
    _buffer(out),
    _keys(out, _coins),
    _address(out, _coins, _keys)
{
}

void CommandExecutor::init() {
    _coins.init();
    setCoin("btc", true);
}

void CommandExecutor::help() const {
    _out << "Commands:" << endl;
    _out << "  exit                    Exit" << endl;
    _out << "  quit                    Exit" << endl;
    _out << "  help                    This help" << endl;
    _out << "Inputs, buffer:" << endl;
    _out << "  #                       Take last result" << endl;
    _out << "  #<n>                    Take nth previous result" << endl;
    _out << "  buffer                  Take buffer values" << endl;
    _out << "Coins:" << endl;
    _out << "  coins                   List known coins" << endl;
    _out << "  coin <coin>             Set active coin, selected by its ID or symbol or name" << endl;
    _out << "Keys:" << endl;
    _out << "  newkey                  Create new pseudo-random 32-byte key (secret!)" << endl;
    _out << "  pubpri <prikey>         Derive public key from a secret private key (type is coin-dependent)" << endl;
    _out << "  pripub <pubkey>         Derive private key from public key :)" << endl;
    _out << "  setmnemo <word1> ...    Set current mnemonic, several words (secret!)" << endl;
    _out << "  newmnemo <strength>     Create and store a new mnemonic, of strength (128 -- 256) (secret!)" << endl;
    _out << "  dumpseed                Dump the seed of the current mnemonic (secret!)" << endl;
    _out << "  dumpmnemo               Dump the current mnemonic (secret!)" << endl;
    _out << "  dumpdp                  Dump the default derivation path of the current coin (ex.: m/84'/0'/0'/0/0)" << endl;
    _out << "  pridp [<derivpath>]     Derive a new private key for the coin, from the current menmonic and given derivation path." << endl;
    _out << "                          If derivation path is missing, the default one is used (see dumpdp)." << endl;
    _out << "Addresses:" << endl;
    _out << "  addrpub <pubkey>        Create <coin> address from public key" << endl;
    _out << "  addrpri <prikey>        Create <coin> address from private key" << endl;
    _out << "  addr <addr>             Check string <coin> address" << endl;
    _out << "  addrdef                 Derive default address, for current coin, fom current mnemonic; see dumpdp" << endl;
    _out << "  addrdp <derivpath>      Derive a new address with the given derivation path (using current coin and mnemonic)" << endl;
    _out << "Coin-specific methods:" << endl;
    _out << "  tonmsg <prikey>         Build TON account initialization message." << endl;
    _out << "Transformations:" << endl;
    _out << "  hex <inp>               Encode given string to hex" << endl;
    _out << "  base64enc <inp>         Encode given hex data to Base64" << endl;
    _out << "  base64dec <inp>         Decode given Base64 string to hex data" << endl;
    _out << "File methods:" << endl;
    _out << "  filew <filename> <data> Write data to a (new) binary file." << endl;
    _out << "  filer <filename>        Read data from a binary file." << endl;
    _out << endl;
    _out << "Examples:" << endl;
    _out << "> coin bitcoin <Enter> newkey <Enter> addrpri # <Enter> filew btcaddr.txt #" << endl;
    _out << "> newkey <Enter> pubpri # <Enter> addrpub #" << endl;
    _out << "> coin algo <Enter> addr LCSUSBOLNVT6BND6DWWGM4DLVUYJN3PGBT4T7LTCMDMKS7TR7FZAOHOVPE" << endl;
    _out << "> coin btc <Enter> dumpdp <Enter> pridb <Enter> pridp m/84'/0'/0'/0/1 <Enter> pubpri # <Enter> addrpub # <Enter> addr #" << endl;
    _out << "> coin nano <Enter> dumpdp <Enter> setmnemo word1 word2 ... word12 <Enter> addrdef <Enter> addrdp m/44'/165'/0' <Enter> addrdp m/44'/165'/1'" << endl;
    _out << "> hex Hello <Enter> base64enc # <Enter> base64dec # <Enter> buffer" << endl;

    _out << endl;
}

bool CommandExecutor::executeOne(const string& cmd, const vector<string>& params_in, string& res) {
    if (cmd == "exit" || cmd == "quit") {
        // handled in loop
        _out << "Bye!" << endl;
        return false;
    }

    // prepare inputs
    vector<string> params;
    if (!prepareInputs(params_in, params)) {
        _out << "Error processing input(s)" << endl;
        return false;
    }
    
    if (cmd == "help") { help(); return false; }
    if (cmd[0] == '#') {
        // echo input, with substitution
        _out << "Previous result is:  " << params[0] << endl;
        return false;
    }
    if (cmd == "buffer") { _buffer.buffer(); return false; }

    if (cmd == "coins") { _coins.coins(); return false; }
    if (cmd == "coin") { if (!checkMinParams(params, 1)) { return false; } setCoin(params[1], true); return false; }

    if (cmd == "newkey") { return _keys.newkey(res); }
    if (cmd == "pubpri") { if (!checkMinParams(params, 1)) { return false; } return _keys.pubpri(_activeCoin, params[1], res); }
    if (cmd == "pripub") { if (!checkMinParams(params, 1)) { return false; } return _keys.pripub(params[1], res); }
    if (cmd == "setmnemo") { if (!checkMinParams(params, 1)) { return false; } _keys.setmnemo(params); return false; }
    if (cmd == "newmnemo") { if (!checkMinParams(params, 1)) { return false; } return _keys.newmnemo(params[1], res); }
    if (cmd == "dumpseed") { return _keys.dumpseed(res); }
    if (cmd == "dumpmnemo") { return _keys.dumpmnemo(res); }
    if (cmd == "dumpdp") { return _keys.dumpdp(_activeCoin, res); }
    if (cmd == "pridp") { string dp; if (params.size() >= 2) dp = params[1]; return _keys.pridp(_activeCoin, dp, res); }

    if (cmd == "addrpub") { if (!checkMinParams(params, 1)) { return false; } return _address.addrpub(_activeCoin, params[1], res); }
    if (cmd == "addrpri") { if (!checkMinParams(params, 1)) { return false; } return _address.addrpri(_activeCoin, params[1], res); }
    if (cmd == "addr") { if (!checkMinParams(params, 1)) { return false; } return _address.addr(_activeCoin, params[1], res); }
    if (cmd == "addrdef") { return _address.addrdef(_activeCoin, res); }
    if (cmd == "addrdp") { if (!checkMinParams(params, 1)) { return false; } return _address.addrdp(_activeCoin, params[1], res); }

    if (cmd == "tonmsg") { if (!checkMinParams(params, 1)) { return false; } setCoin("ton", false); return TonCoin::tonmsg(params[1], res); }

    if (cmd == "hex") { if (!checkMinParams(params, 1)) { return false; } return Util::hex(params[1], res); }
    if (cmd == "base64enc") { if (!checkMinParams(params, 1)) { return false; } return Util::base64enc(params[1], res); }
    if (cmd == "base64dec") { if (!checkMinParams(params, 1)) { return false; } return Util::base64dec(params[1], res); }
    
    if (cmd == "filew") { if (!checkMinParams(params, 2)) { return false; } return Util::filew(params[1], params[2], res); }
    if (cmd == "filer") { if (!checkMinParams(params, 1)) { return false; } return Util::filer(params[1], res); }

    // fallback
    _out << "Unknown command:  " << cmd << endl << "Type 'help' for list of commands." << endl;
    return false;
}

void CommandExecutor::executeLine(const string& line) {
    vector<string> params;
    auto cmd = parseLine(line, params);
    //cerr << "Read cmd: '" << cmd << "'" << endl;
    execute(cmd, params);
}

void CommandExecutor::execute(const string& cmd, const vector<string>& params) {
    try {
        string resultStr;
        bool res = executeOne(cmd, params, resultStr);
        if (res && resultStr.length() > 0) {
            // there is a new result
            _out << "Result:  " << resultStr << endl;
            _buffer.addResult(resultStr);
        }
    } catch (exception& ex) {
        _out << "Error while executing command, " << ex.what() << endl;
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

string CommandExecutor::parseLine(const string& line, vector<string>& params) {
    auto tokens = Util::tokenize(line);
    assert(tokens.size() > 0);
    auto cmd = tokens[0];
    Util::toLower(cmd);
    params = tokens;
    return cmd;
}

bool CommandExecutor::checkMinParams(const vector<string>& params, int n) const {
    if (params.size() - 1 >= n) {
        return true;
    }
    _out << "At least " << n << " parameters are needed! See 'help'" << endl;
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
    _out << "Set active coin to: " << c.id << "    Use 'coin' to change.  (name: '" << c.name << "'  symbol: " << c.symbol << "  numericalid: " << c.c << ")" << endl;
    return true;
}

} // namespace TW::WalletConsole
