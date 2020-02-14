// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "EOS/Name.h" // Name is reused
#include "../Data.h"
#include "../BinaryCoding.h"

#include <string>
#include <vector>

namespace TW::FIO {

/// Represents an authorization record (actor/permission pair)
class Authorization {
public:
    std::string actor;
    std::string permission;
    void serialize(Data& out) const {
        EOS::Name(actor).serialize(out);
        EOS::Name(permission).serialize(out);
    }
};

/// Represents an array of authorizations
class AuthorizationArray {
public:
    std::vector<Authorization> authArray;
    void serialize(Data& out) const {
        encodeVarInt(authArray.size(), out);
        for (const auto& item : authArray) {
            item.serialize(out);
        }
    }
};

/// Represents a tokentype:address pair, such as {"BTC", "bc1qvy4074rggkdr2pzw5vpnn62eg0smzlxwp70d7v"}
class PublicAddress {
public:
    std::string tokenCode;
    std::string address;
    void serialize(Data& out) const {
        encodeString(tokenCode, out);
        encodeString(address, out);
    }
};

/// An array of public addresses
class PublicAddresses {
public:
    std::vector<PublicAddress> addressList;
    PublicAddresses(const std::vector<std::pair<std::string, std::string>>& addresses) {
        for (const auto& item : addresses) {
            addressList.push_back(PublicAddress{item.first, item.second});
        }
    }
    void serialize(Data& out) const {
        encodeVarInt(addressList.size(), out);
        for (const auto& item : addressList) {
            item.serialize(out);
        }
    }
};

/// Represents an Action.  Some common fields, and an action-specific data field, stored serialized.
class Action {
public:
    std::string account;
    std::string name;
    AuthorizationArray auth;
    bool includeExtra01BeforeData = false;
    Data actionDataSer;

    void serialize(Data& out) const;
};

/// AddPubAddress action data part.
class AddPubAddressData {
public:
    std::string fioAddress;
    PublicAddresses addresses;
    uint64_t fee;
    std::string tpid;
    std::string actor;

    AddPubAddressData(const std::string& fioAddress, std::vector<std::pair<std::string, std::string>> addresses,
        uint64_t fee, const std::string& tpid, const std::string& actor) :
        fioAddress(fioAddress), addresses(addresses),
        fee(fee), tpid(tpid), actor(actor) {}
    void serialize(Data& out) const;
};

/// RegisterFioAddress action data part.
class RegisterFioAddressData {
public:
    std::string fioAddress;
    std::string ownerPublicKey;
    uint64_t fee;
    std::string tpid;
    std::string actor;

    RegisterFioAddressData(const std::string& fioAddress, const std::string& ownerPublicKey,
        uint64_t fee, const std::string& tpid, const std::string& actor) :
        fioAddress(fioAddress), ownerPublicKey(ownerPublicKey),
        fee(fee), tpid(tpid), actor(actor) {}
    void serialize(Data& out) const;
};

/// TransferTokens action data part.
class TransferData {
public:
    std::string payeePublicKey;
    uint64_t amount;
    uint64_t fee;
    std::string tpid;
    std::string actor;

    TransferData(const std::string& payeePublicKey, uint64_t amount, uint64_t fee, const std::string& tpid, const std::string& actor) :
        payeePublicKey(payeePublicKey), amount(amount), fee(fee), tpid(tpid), actor(actor) {}
    void serialize(Data& out) const;
};

/// RenewFioAddress action data part.
class RenewFioAddressData {
public:
    std::string fioAddress;
    uint64_t fee;
    std::string tpid;
    std::string actor;

    RenewFioAddressData(const std::string& fioAddress, uint64_t fee, const std::string& tpid, const std::string& actor) :
        fioAddress(fioAddress), fee(fee), tpid(tpid), actor(actor) {}
    void serialize(Data& out) const;
};

/// NewFundsRequest action data part.
class NewFundsRequestData {
public:
    std::string payerFioName;
    std::string payeeFioName;
    std::string encryptedContent;
    uint64_t fee;
    std::string tpid;
    std::string actor;

    NewFundsRequestData(const std::string& payerFioName, const std::string& payeeFioName, const std::string& encryptedContent, uint64_t fee, const std::string& tpid, const std::string& actor) :
        payerFioName(payerFioName), payeeFioName(payeeFioName), encryptedContent(encryptedContent), fee(fee), tpid(tpid), actor(actor) {}
    void serialize(Data& out) const;
};

} // namespace TW::FIO
