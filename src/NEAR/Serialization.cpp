// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../NEAR/Address.h"
#include "../Base64.h"
#include "../BinaryCoding.h"
#include <TrustWalletCore/TWHRP.h>

using namespace TW;
using namespace TW::NEAR;
using namespace TW::NEAR::Proto;


static void writeU8(Data& data, uint8_t number) {
    data.push_back(number);
}

static void writeU32(Data& data, uint32_t number) {
    encode32LE(number, data);
}

static void writeU64(Data& data, uint64_t number) {
    encode64LE(number, data);
}

static void writeU128(Data& data, const std::string& numberData) {
    data.insert(std::end(data), std::begin(numberData), std::end(numberData));
}

template <class T> static void writeRawBuffer(Data &data, const T& buf) {
    data.insert(std::end(data), std::begin(buf), std::end(buf));
}

static void writeString(Data& data, const std::string& str) {
    writeU32(data, str.length());
    writeRawBuffer(data, str);
}

static void writePublicKey(Data& data, const Proto::PublicKey& publicKey) {
    writeU8(data, publicKey.key_type());
    const auto& keyData = publicKey.data();
    writeRawBuffer(data, keyData);
}

static void writeTransfer(Data& data, const Proto::Transfer& transfer) {
    writeU128(data, transfer.deposit());
}

static void writeAction(Data& data, const Proto::Action& action) {
    writeU8(data, action.payload_case() - Proto::Action::kCreateAccount);
    switch (action.payload_case()) {
        case Proto::Action::kTransfer:
            writeTransfer(data, action.transfer());
            return;
        default:
            // TODO: Report error properly?
            return;
    }
}

Data TW::NEAR::transactionData(const Proto::SigningInput& input) {
    Data data;
    writeString(data, input.signer_id());
    writePublicKey(data, input.public_key());
    writeU64(data, input.nonce());
    writeString(data, input.receiver_id());
    // TODO: assert fixed size buffer sizes
    const auto& block_hash = input.block_hash();
    writeRawBuffer(data, block_hash);
    writeU32(data, input.actions_size());
    for (const auto& action : input.actions()) {
        writeAction(data, action);
    }
    return data;
}

Data TW::NEAR::signedTransactionData(const Data& transactionData, const Data& signatureData) {
    Data data;
    writeRawBuffer(data, transactionData);
    writeU8(data, 0);
    writeRawBuffer(data, signatureData);
    return data;
}
