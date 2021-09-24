// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransferableOutput.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

bool TransactionOutput::operator<(const TransactionOutput& other) const {
    Data thisData;
    Data otherData;
    encode(thisData);
    other.encode(otherData);
    return std::lexicographical_compare(thisData.begin(), thisData.end(), otherData.begin(),
                                        otherData.end());
}

void TransferableOutput::encode(Data& data) const {
    for (auto byte : assetID) {
        data.push_back(byte);
    }
    output->encode(data);
}

bool TransferableOutput::operator<(const TransferableOutput& other) const {
    Data thisData;
    Data otherData;
    encode(thisData);
    other.encode(otherData);
    return std::lexicographical_compare(thisData.begin(), thisData.end(), otherData.begin(),
                                        otherData.end());
}

TransferableOutput& TransferableOutput::operator=(const TransferableOutput& other) {
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    } else {
        // assign members
        assetID = other.assetID;
        output = other.output->duplicate();
        return *this;
    }
}

void SECP256k1TransferOutput::encode(Data& data) const {
    encode32BE(typeID, data);
    encode64BE(amount, data);
    encode64BE(locktime, data);
    encode32BE(threshold, data);
    encode32BE(static_cast<uint32_t>(addresses.size()), data);
    for (auto Address : addresses) {
        for (auto byte : Address.getKeyHash()) {
            data.push_back(byte);
        }
    }
}

void SECP256k1MintOutput::encode(Data& data) const {
    encode32BE(typeID, data);
    encode64BE(locktime, data);
    encode32BE(threshold, data);
    encode32BE(static_cast<uint32_t>(addresses.size()), data);
    for (auto Address : addresses) {
        for (auto byte : Address.getKeyHash()) {
            data.push_back(byte);
        }
    }
}

void NFTTransferOutput::encode(Data& data) const {
    encode32BE(typeID, data);
    encode32BE(groupID, data);
    encode32BE(static_cast<uint32_t>(payload.size()), data);
    for (auto byte : payload) {
        data.push_back(byte);
    }
    encode64BE(locktime, data);
    encode32BE(threshold, data);
    encode32BE(static_cast<uint32_t>(addresses.size()), data);
    for (auto Address : addresses) {
        for (auto byte : Address.getKeyHash()) {
            data.push_back(byte);
        }
    }
}

void NFTMintOutput::encode(Data& data) const {
    encode32BE(typeID, data);
    encode32BE(groupID, data);
    encode64BE(locktime, data);
    encode32BE(threshold, data);
    encode32BE(static_cast<uint32_t>(addresses.size()), data);
    for (auto Address : addresses) {
        for (auto byte : Address.getKeyHash()) {
            data.push_back(byte);
        }
    }
}
