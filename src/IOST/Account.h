// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"
#include "../PrivateKey.h"
#include "../proto/IOST.pb.h"

#include <string>

namespace TW::IOST {

class Account {
public:
    static bool isValid(const std::string& name);
    static std::string encodePubKey(const PublicKey& publicKey);
    static std::string address(const std::string& string);
    Account(std::string name): name(std::move(name)) {}
    Account([[maybe_unused]] const PublicKey& publicKey) {}
    Account(const Proto::AccountInfo& account);
    std::string string() const { return name; }
    Data sign(const Data& digest, TWCurve curve) const;
    Data publicActiveKey() const;
    Data publicOwnerKey() const;
    std::string name;
    Data activeKey;
    Data ownerKey;
};

inline bool operator==(const Account& lhs, const Account& rhs) {
    return lhs.name == rhs.name;
}

} // namespace TW::IOST

/// Wrapper for C interface.
struct TWIOSTAccount {
    TW::IOST::Account impl;
};
