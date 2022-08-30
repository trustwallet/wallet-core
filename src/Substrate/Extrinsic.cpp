//
// Created by Fitz on 2022/1/27.
//

#include "Extrinsic.h"
#include <map>

using namespace TW;
namespace TW::Substrate {

static constexpr uint8_t signedBit = 0x80;
static constexpr uint8_t sigTypeEd25519 = 0x00;
static constexpr uint8_t extrinsicFormat = 4;

// max uint8
static constexpr TW::byte maxByte = 255;

static Data encodeCallIndex(int32_t moduleIndex, int32_t methodIndex) {
    if (moduleIndex > maxByte) {
        throw std::invalid_argument("module index too large");
    }
    if (methodIndex > maxByte) {
        throw std::invalid_argument("method index too large");
    }

    return Data{TW::byte(moduleIndex), TW::byte(methodIndex)};
}

Data Extrinsic::encodeCall(const Proto::SigningInput& input) {
    Data data;
    if (input.network() > maxByte) {
        throw std::invalid_argument("method index too large");
    }
    if (input.has_balance_call()) {
        data = encodeBalanceCall(input.balance_call(), input.network(), input.spec_version(), input.multi_address());
    }

    return data;
}

// Payload to sign.
Data Extrinsic::encodePayload() const {
    Data data;
    // call
    append(data, call);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // specVersion
    encode32LE(specVersion, data);
    // transactionVersion
    encode32LE(version, data);
    // genesis hash
    append(data, genesisHash);
    // block hash
    append(data, blockHash);
    return data;
}

//length prefix (2 bytes) + version header (1 bytes) + signer public key (AccountId [1byte] + pub key [32 bytes]) + signature types (1 byte, ed25519 is 0) + signature...

// Encode final data with signer public key and signature.
Data Extrinsic::encodeSignature(const PublicKey& signer, const Data& signature) const {
    Data data;
    // version header
    append(data, Data{extrinsicFormat | signedBit});
    // signer public key
    append(data, encodeAccountId(signer.bytes, encodeRawAccount(multiAddress)));
    // signature type
    append(data, sigTypeEd25519);
    // signature
    append(data, signature);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // call
    append(data, call);
    // append length
    encodeLengthPrefix(data);
    return data;
}

bool Extrinsic::encodeRawAccount(bool enableMultiAddress) {
    return !enableMultiAddress;
}

Data Extrinsic::encodeTransfer(const Proto::Balance::Transfer& transfer, int32_t network, bool enableMultiAddress) {
    Data data;
    auto address = FullSS58Address(transfer.to_address(), network);
    auto value = load(transfer.value());
    // call index
    append(data, encodeCallIndex(transfer.module_index(), transfer.method_index()));
    // destination
    append(data, encodeAccountId(address.keyBytes(), encodeRawAccount(enableMultiAddress)));
    // value
    append(data, encodeCompact(value));
    // memo
    if (transfer.memo().length() > 0) {
        append(data, 0x01);
        auto memo = transfer.memo();
        if (memo.length() < 32) {
            // padding memo with space
            memo.append(32 - memo.length(), ' ');
        }
        append(data, TW::data(memo));
    }
    return data;
}

Data Extrinsic::encodeBalanceCall(const Proto::Balance& balance, int32_t network, [[maybe_unused]] uint32_t specVersion, bool enableMultiAddress) {
    Data data;
    if (balance.has_transfer()) {
        auto transfer = balance.transfer();
        append(data, encodeTransfer(transfer, network, enableMultiAddress));
    } else if (balance.has_batch_transfer()) {
        //init call array
        auto calls = std::vector<Data>();
        auto batchTransfer = balance.batch_transfer().transfers();
        for (auto transfer : batchTransfer) {
            // put into calls array
            calls.push_back(encodeTransfer(transfer, network, enableMultiAddress));
        }
        data = encodeBatchCall(calls, balance.batch_transfer().module_index(), balance.batch_transfer().method_index());
    }

    return data;
}

Data Extrinsic::encodeBatchCall(const std::vector<Data>& calls, int32_t moduleIndex, int32_t methodIndex) {
    Data data;
    append(data, encodeCallIndex(moduleIndex, methodIndex));
    append(data, encodeVector(calls));
    return data;
}

Data Extrinsic::encodeEraNonceTip() const {
    Data data;
    // era
    append(data, era);
    // nonce
    append(data, encodeCompact(nonce));
    // tip
    append(data, encodeCompact(tip));
    return data;
}

} // namespace TW::Substrate