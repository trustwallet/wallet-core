// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "../proto/IOST.pb.h"
#include "../Base58.h"

#include <stdint.h>
#include <string>

namespace TW {
namespace IOST {

class Account {
  public:
    template <typename T>
    static bool isValid(const T& name) {
        auto s = std::string(name);
        if (s.size() < 5 || s.size() > 11) { return false; }
        for (char ch : s) {
            if (!((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_')) {
                return false;
            }
        }
        return true;
    }
    static std::string encodePubKey(const PublicKey& publicKey) {
        return Base58::bitcoin.encode(publicKey.bytes);
    } 
    Account(const std::string& name): name(name) {}
    Account(const PublicKey& publicKey) {}
    Account(const Proto::AccountInfo& account): name(account.name()), activeKey(account.active_key()), ownerKey(account.owner_key()) {}
    std::string string() const { return name; }
    std::string name;
    std::string activeKey;
    std::string ownerKey;
};

inline bool operator==(const Account& lhs, const Account& rhs) {
    return lhs.name == rhs.name;
}

} // namespace IOST
} // namespace TW

/// Wrapper for C interface.
struct TWIOSTAccount {
    TW::IOST::Account impl;
};