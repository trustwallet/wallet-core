// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
