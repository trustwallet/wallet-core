// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Buffer.h"
#include "WalletConsole.h"
#include "Util.h"

#include "Data.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;

void Buffer::addResult(const string& val) {
    if (val.length() == 0) { return; }
    _last = SavedValue(val);
    _prev.emplace_back(SavedValue(val));
}

bool Buffer::prepareInput(const string& in, string& in_out) {
    string in2 = in;
    Util::trimLeft(in2);    
    if (in2.length() < 1) { in_out = in2; return true; }
    if (in2[0] != '#') {
        // no special handling, return as it is
        in_out = in2;
        return true;
    }
    if (in2 == "#") {
        // # represents last input
        in_out = _last.get();
        return true;
    }
    // assume #n
    try {
        int n = std::stoi(in2.substr(1));
        // of the form #n
        int idx = n - 1;
        if (idx < 0 || idx >= static_cast<int>(_prev.size())) {
            _out << "Requested " << in2 << ", but out of range of buffers (n=" << _prev.size() << ")." << endl;
            return false;
        }
        in_out = _prev[idx].get();
        return true;
    } catch (exception& ex) {
        _out << "Invalid input: " << in2 << endl;
        return false;
    }
}

void Buffer::buffer() const {
    _out << "Last value:  " << _last.get() << endl;
    _out << _prev.size() << " previous values:" << endl;
    for (auto i = 0ul; i < _prev.size(); ++i) {
        _out << "  #" << i + 1 << "  " << _prev[i].get() << endl;
    }
}

} // namespace TW::WalletConsole
