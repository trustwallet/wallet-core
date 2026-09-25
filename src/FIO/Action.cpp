// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Action.h"
#include "Data.h"

namespace TW::FIO {

using namespace std;

uint8_t encodeVarInt(uint64_t num, Data& data) {
    uint8_t count = 0;
    while (true) {
        ++count;
        if (num >> 7) {
            // >= 0x80, write it out, eat up 7 bits, and continue
            TW::append(data, 0x80 | (num & 0x7F));
            num = num >> 7;
        } else {
            // < 0x80, write it out and stop
            TW::append(data, static_cast<uint8_t>(num));
            break;
        }
    }
    return count;
}

void encodeString(const string& str, vector<uint8_t>& data) {
    size_t size = str.size();
    encodeVarInt(size, data);
    data.insert(data.end(), str.data(), str.data() + size);
}

void Action::serialize(Data& out) const {
    EOS::Name(account).serialize(out);
    EOS::Name(name).serialize(out);
    auth.serialize(out);
    encodeVarInt(actionDataSer.size(), out);
    append(out, actionDataSer);
    append(out, 0); // 00
}

void PubAddressActionData::serialize(Data& out) const {
    encodeString(fioAddress, out);
    addresses.serialize(out);
    encode64LE(fee, out);
    EOS::Name(actor).serialize(out);
    encodeString(tpid, out);
}

void RemoveAllPubAddressActionData::serialize(Data& out) const {
    encodeString(fioAddress, out);
    encode64LE(fee, out);
    EOS::Name(actor).serialize(out);
    encodeString(tpid, out);
}

void RegisterFioAddressData::serialize(Data& out) const {
    encodeString(fioAddress, out);
    encodeString(ownerPublicKey, out);
    encode64LE(fee, out);
    EOS::Name(actor).serialize(out);
    encodeString(tpid, out);
}

void TransferData::serialize(Data& out) const {
    encodeString(payeePublicKey, out);
    encode64LE(amount, out);
    encode64LE(fee, out);
    EOS::Name(actor).serialize(out);
    encodeString(tpid, out);
}

void RenewFioAddressData::serialize(Data& out) const {
    encodeString(fioAddress, out);
    encode64LE(fee, out);
    EOS::Name(actor).serialize(out);
    encodeString(tpid, out);
}

void NewFundsRequestData::serialize(Data& out) const {
    encodeString(payerFioName, out);
    encodeString(payeeFioName, out);
    encodeString(encryptedContent, out);
    encode64LE(fee, out);
    encodeString(actor, out);
    encodeString(tpid, out);
}

void AddBundledTransactionsActionData::serialize(Data& out) const {
    encodeString(fioAddress, out);
    encode64LE(bundledSets, out);
    encode64LE(fee, out);
    encodeString(tpid, out);
    EOS::Name(actor).serialize(out);
}

} // namespace TW::FIO
