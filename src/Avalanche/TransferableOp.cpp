// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransferableOp.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

bool TransferableOp::sortUTXOIDs(TransferableOp::UTXOID lhs, TransferableOp::UTXOID rhs) {
    if (lhs.first == rhs.first) {
        return lhs.second < rhs.second;
    }
    return std::lexicographical_compare(lhs.first.begin(), lhs.first.end(), rhs.first.begin(),
                                        rhs.first.end());
}

void TransferableOp::encode(Data& data) const {
    for (auto byte : assetID) {
        data.push_back(byte);
    }
    encode32BE(static_cast<uint32_t>(utxoIDs.size()), data);
    for (auto utxoID : utxoIDs) {
        for (auto byte : utxoID.first) {
            data.push_back(byte);
        }
        encode32BE(utxoID.second, data);
    }
    transferOp->encode(data);
}

bool TransferableOp::operator<(const TransferableOp& other) const {
    Data thisData;
    Data otherData;
    encode(thisData);
    other.encode(otherData);
    return std::lexicographical_compare(thisData.begin(), thisData.end(), otherData.begin(),
                                        otherData.end());
}

TransferableOp& TransferableOp::operator=(const TransferableOp& other) {
    // check for "self assignment" and do nothing in that case
    if (this == &other) {
        return *this;
    } else {
        // assign members
        utxoIDs = other.utxoIDs;
        std::sort(utxoIDs.begin(), utxoIDs.end(), sortUTXOIDs);
        assetID = other.assetID;
        transferOp = other.transferOp->duplicate();
        return *this;
    }
}

void SECP256k1MintOperation::encode(Data& data) const {
    encode32BE(typeID, data);
    encode32BE(static_cast<uint32_t>(addressIndices.size()), data);
    for (auto index : addressIndices) {
        encode32BE(index, data);
    }
    mintOutput.encode(data);
    transferOutput.encode(data);
}

void NFTMintOperation::encode(Data& data) const {
    encode32BE(typeID, data);
    encode32BE(static_cast<uint32_t>(addressIndices.size()), data);
    for (auto index : addressIndices) {
        encode32BE(index, data);
    }
    encode32BE(groupID, data);
    encode32BE(static_cast<uint32_t>(payload.size()), data);
    for (auto byte : payload) {
        data.push_back(byte);
    }
    EncodeOutputs(outputs, data);
}

void NFTTransferOperation::encode(Data& data) const {
    encode32BE(typeID, data);
    encode32BE(static_cast<uint32_t>(addressIndices.size()), data);
    for (auto index : addressIndices) {
        encode32BE(index, data);
    }
    encode32BE(transferOutput.groupID, data);
    encode32BE(static_cast<uint32_t>(transferOutput.payload.size()), data);
    for (auto byte : transferOutput.payload) {
        data.push_back(byte);
    }
    encode64BE(transferOutput.locktime, data);
    encode32BE(transferOutput.threshold, data);
    encode32BE(static_cast<uint32_t>(transferOutput.addresses.size()), data);
    for (auto Address : transferOutput.addresses) {
        for (auto byte : Address.getKeyHash()) {
            data.push_back(byte);
        }
    }
}
