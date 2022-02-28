// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "HexCoding.h"
#include "Data.h"

#include <iostream>
#include <vector>
#include <cassert>

namespace TW::WalletConsole {

using namespace std;

class SavedValue {
protected:
    string _val;
public:
    SavedValue() {}
    SavedValue(const string& v) : _val(v) {}
    string get() const { return _val; }
};

// Keep last result and buffer variables
class Buffer {
protected:
    ostream& _out;
    SavedValue _last;
    vector<SavedValue> _prev;

public:
    Buffer(ostream& out) : _out(out) {}

    void addResult(const string& val);
    /// Return the given input string, if # or #n, substitute it
    bool prepareInput(const string& in, string& in_out);
    /// Print out values
    void buffer() const;
};

} // namespace TW::WalletConsole
