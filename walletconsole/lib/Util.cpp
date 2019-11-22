// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Util.h"

#include "Base64.h"
#include "HexCoding.h"
#include "Data.h"

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>

namespace TW::WalletConsole {

using namespace std;
using namespace TW;

bool Util::hex(const string& p, string& res) {
    res = TW::hex(data(p));
    //cout << "Hex result:  '" << res << "'" << endl;
    return true;
}

bool Util::base64enc(const string& p, string& res) {
    try {
        Data data = parse_hex(p);
        try {
            res = Base64::encode(data);
            //cout << "Base64 encode result:  '" << res << "'" << endl;
            return true;
        } catch (exception& ex) {
            cout << "Error while Base64 encode" << endl;
            return false;
        }
    } catch (exception& ex) {
        cout << "Error decoding input as hex" << endl;
        return false;
    }
}

bool Util::base64dec(const string& p, string& res) {
    try {
        auto dec = Base64::decode(p);
        res = TW::hex(dec);
        //cout << "Base64 decode result:  '" << res << "'" << endl;
        return true;
    } catch (exception& ex) {
        cout << "Error while Base64 decode" << endl;
        return false;
    }
}

bool Util::filew(const string& filename, const string& data, string& res) {
    if (filesystem::exists(filename)) {
        cout << "Warning: File '" << filename << "' already exists, not overwriting to be safe." << endl;
        return false;
    }
    try {
        ofstream outfile(filename,  std::ios::out | std::ios::binary);
        Data bindata = parse_hex(data);
        outfile.write((const char*)bindata.data(), bindata.size());
        outfile.close();
        cout << "Written to file '" << filename << "', " << bindata.size() << " bytes." << endl;
    } catch (exception& ex) {
        cout << "Error writing to file '" << filename << "': " << ex.what() << endl;
    }
    return false;
}

bool Util::filer(const string& filename, string& res) {
    if (!filesystem::exists(filename)) {
        cout << "Error: File not found '" << filename << "'" << endl;
        return false;
    }
    try {
        ifstream infile(filename,  std::ios::in | std::ios::binary);
        // get length of file:
        infile.seekg (0, infile.end);
        int length = infile.tellg();
        infile.seekg (0, infile.beg);
        char * buffer = new char [length];
        infile.read(buffer, length);
        if (!infile) {
            cout << "Could not read file '" << filename << "'" << endl;
            return false;
        }
        int red = infile.gcount();
        infile.close();
        res = string(TW::hex(data((const byte*)buffer, red)));
        delete[] buffer;
        cout << "Read " << red << " bytes from file '" << filename << "'." << endl;
        return true;
    } catch (exception& ex) {
        cout << "Error reading from file '" << filename << "': " << ex.what() << endl;
        return false;
    }
}

vector<string> Util::tokenize(const string& line) {
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

void Util::trimLeft(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void Util::toLower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
}

} // namespace TW::WalletConsole
