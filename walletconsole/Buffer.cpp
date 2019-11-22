// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Buffer.h"

#include "WalletConsole.h"
#include "Data.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;

void Buffer::addResult(const string& val) {
    if (val.length() == 0) { return; }
    _last = SavedValue(val);
    _prev.push_back(SavedValue(val));
}

bool Buffer::prepareInput(const string& in, string& in_out) {
    string in2 = in;
    WalletConsole::trimLeft(in2);    
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
    int n = 0;
    try {
        n = std::stoi(in2.substr(1));
        // of the form #n
        int idx = n - 1;
        if (idx < 0 || idx >= _prev.size()) {
            cout << "Requested " << in2 << ", but out of range of buffers (n=" << _prev.size() << ")." << endl;
            return false;
        }
        in_out = _prev[idx].get();
        return true;
    } catch (exception& ex) {
        cout << "Invalid input: " << in2 << endl;
        return false;
    }
}

void Buffer::buffer() const {
    cout << "Last value:  " << _last.get() << endl;
    cout << _prev.size() << " previous values:" << endl;
    for (int i = 0; i < _prev.size(); ++i) {
        cout << "  #" << i + 1 << "  " << _prev[i].get() << endl;
    }
}

} // namespace TW::WalletConsole
