// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../BinaryCoding.h"
#include "../PrivateKey.h"

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

template <class T> static void writeRawBuffer(Data& data, const T& buf) {
    data.insert(std::end(data), std::begin(buf), std::end(buf));
}

static void writeString(Data& data, const std::string& str) {
    writeU32(data, static_cast<uint32_t>(str.length()));
    writeRawBuffer(data, str);
}

static void writePublicKey(Data& data, const Proto::PublicKey& publicKey) {
    writeU8(data, static_cast<uint8_t>(publicKey.key_type()));
    const auto& keyData = publicKey.data();
    writeRawBuffer(data, keyData);
}

static void writeTransfer(Data& data, const Proto::Transfer& transfer) {
    writeU128(data, transfer.deposit());
}

static void writeFunctionCall(Data& data, const Proto::FunctionCall& functionCall) {
    writeString(data, functionCall.method_name());

    writeU32(data, static_cast<uint32_t>(functionCall.args().size()));
    writeRawBuffer(data, functionCall.args());

    writeU64(data, functionCall.gas());
    writeU128(data, functionCall.deposit());
}

static void writeStake(Data& data, const Proto::Stake& stake) {
    writeU128(data, stake.stake());
    writePublicKey(data, stake.public_key());
}

static void writeFunctionCallPermission(Data& data, const Proto::FunctionCallPermission& functionCallPermission) {
    if (functionCallPermission.allowance().empty()) {
        writeU8(data, 0);
    } else {
        writeU8(data, 1);
        writeU128(data, functionCallPermission.allowance());
    }
    writeString(data, functionCallPermission.receiver_id());
    writeU32(data, static_cast<uint32_t>(functionCallPermission.method_names().size()));
    for (auto&& methodName : functionCallPermission.method_names()) {
        writeString(data, methodName);
    }
}

static void writeAccessKey(Data& data, const Proto::AccessKey& accessKey) {
    writeU64(data, accessKey.nonce());
    switch (accessKey.permission_case()) {
        case Proto::AccessKey::kFunctionCall:
            writeU8(data, 0);
            writeFunctionCallPermission(data, accessKey.function_call());
            break;
        case Proto::AccessKey::kFullAccess:
            writeU8(data, 1);
            break;
        case AccessKey::PERMISSION_NOT_SET:
            break;
        }
}

static void writeAddKey(Data& data, const Proto::AddKey& addKey) {
    writePublicKey(data, addKey.public_key());
    writeAccessKey(data, addKey.access_key());
}

static void writeDeleteKey(Data& data, const Proto::DeleteKey& deleteKey) {
    writePublicKey(data, deleteKey.public_key());
}

static void writeDeleteAccount(Data& data, const Proto::DeleteAccount& deleteAccount) {
    writeString(data, deleteAccount.beneficiary_id());
}

static void writeAction(Data& data, const Proto::Action& action) {
    writeU8(data, action.payload_case() - Proto::Action::kCreateAccount);
    switch (action.payload_case()) {
        case Proto::Action::kTransfer:
            writeTransfer(data, action.transfer());
            return;
        case Proto::Action::kFunctionCall:
            writeFunctionCall(data, action.function_call());
            return;
        case Proto::Action::kStake:
            writeStake(data, action.stake());
            return;
        case Proto::Action::kAddKey:
            writeAddKey(data, action.add_key());
            return;
        case Proto::Action::kDeleteKey:
            writeDeleteKey(data, action.delete_key());
            return;
        case Proto::Action::kDeleteAccount:
            writeDeleteAccount(data, action.delete_account());
            return;
        default:
            return;
    }
}

Data TW::NEAR::transactionData(const Proto::SigningInput& input) {
    Data data;
    writeString(data, input.signer_id());
    auto key = PrivateKey(input.private_key());
    auto public_key = key.getPublicKey(TWPublicKeyTypeED25519);
    auto public_key_proto = Proto::PublicKey();
    public_key_proto.set_data(public_key.bytes.data(), public_key.bytes.size());
    writePublicKey(data, public_key_proto);
    writeU64(data, input.nonce());
    writeString(data, input.receiver_id());
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
