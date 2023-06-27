// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Account.h"
#include "../Base58.h"
#include "../Base58Address.h"

using namespace TW;
using namespace TW::IOST;

bool isAccountValid(const std::string& account) {
    if (account.size() < 5 || account.size() > 11) {
        return false;
    }
    for (char ch : account) {
        if ((ch < 'a' || ch > 'z') && (ch < '0' || ch > '9') && ch != '_') {
            return false;
        }
    }
    return true;
}

bool isBase58AddressValid(const std::string& address) {
    auto decoded = Base58::decode(address);
    return decoded.size() == Base58Address<32>::size;
}

bool Account::isValid(const std::string& s) {
    return isAccountValid(s) ? true : isBase58AddressValid(s);
}

std::string Account::encodePubKey(const PublicKey& publicKey) {
    return Base58::encode(publicKey.bytes);
}

Account::Account(const Proto::AccountInfo& account) {
    name = account.name();
    if (account.active_key() != "") {
        auto activeKeyBytesPtr = account.active_key().begin();
        activeKey = Data(activeKeyBytesPtr, activeKeyBytesPtr + PrivateKey::_size);
    }
    if (account.owner_key() != "") {
        auto ownerKeyBytesPtr = account.owner_key().begin();
        ownerKey = Data(ownerKeyBytesPtr, ownerKeyBytesPtr + PrivateKey::_size);
    }
}

Data Account::sign(const Data& digest, TWCurve curve) const {
    return PrivateKey(activeKey).sign(digest, curve);
}

Data Account::publicActiveKey() const {
    return PrivateKey(activeKey).getPublicKey(TWPublicKeyTypeED25519).bytes;
}

Data Account::publicOwnerKey() const {
    return PrivateKey(ownerKey).getPublicKey(TWPublicKeyTypeED25519).bytes;
}

std::string Account::address(const std::string& publickey) {
    auto publicKeyData = TW::data(publickey);
    return Base58::encode(publicKeyData);
}
