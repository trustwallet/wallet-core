// Copyright © 2017-2020 Trust Wallet.
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
    _address(out, _coins, _keys),
    _util(out)
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
    _out << "  newKey                  Create new pseudo-random 32-byte key (secret!)" << endl;
    _out << "  pubPri <priKey>         Derive public key from a secret private key (type is coin-dependent)" << endl;
    _out << "  priPub <pubKey>         Derive private key from public key :)" << endl;
    _out << "  setMnemonic <word1> ... Set current mnemonic, several words (secret!)" << endl;
    _out << "  newMnemonic <strength>  Create and store a new mnemonic, of strength (128 -- 256) (secret!)" << endl;
    _out << "  dumpSeed                Dump the seed of the current mnemonic (secret!)" << endl;
    _out << "  dumpMnemonic            Dump the current mnemonic (secret!)" << endl;
    _out << "  dumpDP                  Dump the default derivation path of the current coin (ex.: m/84'/0'/0'/0/0)" << endl;
    _out << "  dumpXpub                Dump the XPUB of the current mnemonic" << endl;
    _out << "  priDP [<derivPath>]     Derive a new private key for the coin, from the current mnemonic and given derivation path." << endl;
    _out << "                          If derivation path is missing, the default one is used (see dumpDP)." << endl;
    _out << "Addresses:" << endl;
    _out << "  addrPub <pubKey>        Create <coin> address from public key" << endl;
    _out << "  addrPri <priKey>        Create <coin> address from private key" << endl;
    _out << "  addr <addr>             Check string <coin> address" << endl;
    _out << "  addrDefault             Derive default address, for current coin, fom current mnemonic; see dumpDP" << endl;
    _out << "  addrDP <derivPath>      Derive a new address with the given derivation path (using current coin and mnemonic)" << endl;
    _out << "  addrXpub <xpub> <index> Derive a new address from the given XPUB and address index (using current coin)" << endl;
    _out << "Coin-specific methods:" << endl;
    _out << "Transformations:" << endl;
    _out << "  hex <inp>               Encode given string to hex" << endl;
    _out << "  base64Encode <inp>      Encode given hex data to Base64" << endl;
    _out << "  base64Decode <inp>      Decode given Base64 string to hex data" << endl;
    _out << "File methods:" << endl;
    _out << "  fileW <fileName> <data> Write data to a (new) binary file." << endl;
    _out << "  fileR <fileName>        Read data from a binary file." << endl;
    _out << endl;
    _out << "Examples:" << endl;
    _out << "> coin bitcoin <Enter> newKey <Enter> addrPri # <Enter> fileW btcaddr.txt #" << endl;
    _out << "> newKey <Enter> pubPri # <Enter> addrPub #" << endl;
    _out << "> coin algo <Enter> addr LCSUSBOLNVT6BND6DWWGM4DLVUYJN3PGBT4T7LTCMDMKS7TR7FZAOHOVPE" << endl;
    _out << "> coin btc <Enter> dumpDP <Enter> priDP <Enter> priDP m/84'/0'/0'/0/1 <Enter> pubPri # <Enter> addrPub # <Enter> addr #" << endl;
    _out << "> coin nano <Enter> dumpDP <Enter> setMnemonic word1 word2 ... word12 <Enter> addrDefault <Enter> addrDP m/44'/165'/0' <Enter> addrDP m/44'/165'/1'" << endl;
    _out << "> hex Hello <Enter> base64Encode # <Enter> base64Decode # <Enter> buffer" << endl;

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

    if (cmd == "newkey") { return _keys.newKey(_activeCoin, res); }
    if (cmd == "pubpri") { if (!checkMinParams(params, 1)) { return false; } return _keys.pubPri(_activeCoin, params[1], res); }
    if (cmd == "pripub") { if (!checkMinParams(params, 1)) { return false; } return _keys.priPub(params[1], res); }
    if (cmd == "setmnemonic" || cmd == "setmenmonic") { if (!checkMinParams(params, 1)) { return false; } _keys.setMnemonic(params); return false; }
    if (cmd == "newmnemonic" || cmd == "newmenmonic") { if (!checkMinParams(params, 1)) { return false; } return _keys.newMnemonic(params[1], res); }
    if (cmd == "dumpseed") { return _keys.dumpSeed(res); }
    if (cmd == "dumpmnemonic" || cmd == "dumpmenmonic") { return _keys.dumpMnemonic(res); }
    if (cmd == "dumpdp") { return _keys.dumpDP(_activeCoin, res); }
    if (cmd == "dumpxpub") { return _keys.dumpXpub(_activeCoin, res); }
    if (cmd == "pridp") { string dp; if (params.size() >= 2) dp = params[1]; return _keys.priDP(_activeCoin, dp, res); }

    if (cmd == "addrpub") { if (!checkMinParams(params, 1)) { return false; } return _address.addrPub(_activeCoin, params[1], res); }
    if (cmd == "addrpri") { if (!checkMinParams(params, 1)) { return false; } return _address.addrPri(_activeCoin, params[1], res); }
    if (cmd == "addr") { if (!checkMinParams(params, 1)) { return false; } return _address.addr(_activeCoin, params[1], res); }
    if (cmd == "addrdefault") { return _address.addrDefault(_activeCoin, res); }
    if (cmd == "addrdp") { if (!checkMinParams(params, 1)) { return false; } return _address.deriveFromPath(_activeCoin, params[1], res); }
    if (cmd == "addrxpub") { if (!checkMinParams(params, 2)) { return false; } return _address.deriveFromXpubIndex(_activeCoin, params[1], params[2], res); }

    if (cmd == "hex") { if (!checkMinParams(params, 1)) { return false; } return Util::hex(params[1], res); }
    if (cmd == "base64encode") { if (!checkMinParams(params, 1)) { return false; } return _util.base64Encode(params[1], res); }
    if (cmd == "base64decode") { if (!checkMinParams(params, 1)) { return false; } return _util.base64Decode(params[1], res); }
    
    if (cmd == "filew") { if (!checkMinParams(params, 2)) { return false; } return _util.fileW(params[1], params[2], res); }
    if (cmd == "filer") { if (!checkMinParams(params, 1)) { return false; } return _util.fileR(params[1], res); }

    // fallback
    _out << "Unknown command:  " << cmd << endl << "Type 'help' for list of commands." << endl;
    return false;
}

void CommandExecutor::executeLine(const string& line) {
    vector<string> params;
    auto cmd = parseLine(line, params);
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

bool CommandExecutor::checkMinParams(const vector<string>& params, std::size_t n) const {
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
    if (_activeCoin != c.id || force) {
        // need to change
        _activeCoin = c.id;
        _out << "Set active coin to: " << c.id << "    Use 'coin' to change.  (name: '" << c.name << "'  symbol: " << c.symbol << "  numericalid: " << c.c << ")" << endl;
    }
    return true;
}

} // namespace TW::WalletConsole
