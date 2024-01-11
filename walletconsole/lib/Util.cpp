// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    return true;
}

bool Util::base64Encode(const string& p, string& res) {
    _out << p << endl;
    Data data = parse_hex(p);
    if (data.size() == 0) {
        _out << "Error decoding input as hex" << endl;
        return false;
    }
    _out << TW::hex(data) << endl;
    res = Base64::encode(data);
    return true;
}

bool Util::base64Decode(const string& p, string& res) {
    auto dec = Base64::decode(p);
    if (dec.empty()) {
        _out << "Error while Base64 decode" << endl;
        return false;
    }
    res = TW::hex(dec);
    return true;
}

bool Util::fileW(const string& fileName, const string& data, [[maybe_unused]] string& res) {
    if (fileExists(fileName)) {
        _out << "Warning: File '" << fileName << "' already exists, not overwriting to be safe." << endl;
        return false;
    }
    try {
        ofstream outfile(fileName,  std::ios::out | std::ios::binary);
        Data bindata = parse_hex(data);
        outfile.write((const char*)bindata.data(), bindata.size());
        outfile.close();
        _out << "Written to file '" << fileName << "', " << bindata.size() << " bytes." << endl;
    } catch (exception& ex) {
        _out << "Error writing to file '" << fileName << "': " << ex.what() << endl;
    }
    return false;
}

bool Util::fileR(const string& fileName, string& res) {
    if (!fileExists(fileName)) {
        _out << "Error: File not found '" << fileName << "'" << endl;
        return false;
    }
    char* buffer = nullptr;
    try {
        ifstream infile(fileName,  std::ios::in | std::ios::binary);
        // get length of file:
        infile.seekg (0, infile.end);
        auto length = infile.tellg();
        infile.seekg (0, infile.beg);
        buffer = new char[length];
        if (!infile.read(buffer, length)) {
            _out << "Could not read file '" << fileName << "'" << endl;
            delete[] buffer;
            return false;
        }
        auto red = infile.gcount();
        infile.close();
        res = string(TW::hex(data((const byte*)buffer, red)));
        delete[] buffer;
        _out << "Read " << red << " bytes from file '" << fileName << "'." << endl;
        return true;
    } catch (exception& ex) {
        _out << "Error reading from file '" << fileName << "': " << ex.what() << endl;
        if (buffer != nullptr) {
            delete[] buffer;
        }
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

bool Util::fileExists(const std::string& fileName) {
    //return filesystem::exists(filesystem::path(fileName)); // not used due to compiler issues (on iOS)
    ifstream f(fileName.c_str());
    return f.good();
}

} // namespace TW::WalletConsole
