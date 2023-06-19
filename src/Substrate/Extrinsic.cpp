// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../BinaryCoding.h"
#include "Extrinsic.h"
#include <map>

namespace TW::Substrate {

static constexpr uint8_t signedBit = 0x80;
static constexpr uint8_t sigTypeEd25519 = 0x00;
static constexpr uint8_t extrinsicFormat = 4;

// max uint8
static constexpr byte maxByte = 255;

static Data encodeCallIndex(int32_t moduleIndex, int32_t methodIndex) {
    if (moduleIndex > maxByte) {
        throw std::invalid_argument("module index too large");
    }
    if (methodIndex > maxByte) {
        throw std::invalid_argument("method index too large");
    }

    return Data{byte(moduleIndex), byte(methodIndex)};
}

Data Extrinsic::encodeCall() {
    Data data;
    if (_network > maxByte) {
        throw std::invalid_argument("method index too large");
    }
    if (_input.has_balance_call()) {
        data = encodeBalanceCall(_input.balance_call());
    } else if (_input.has_polymesh_call()) {
        if (_input.polymesh_call().has_authorization_call()) {
            data = encodeAuthorizationCall(_input.polymesh_call().authorization_call());
        } else if (_input.polymesh_call().has_identity_call()) {
            data = encodeIdentityCall(_input.polymesh_call().identity_call());
        } else {
            throw std::invalid_argument("Invalid polymesh call message");
        }
    } else {
        throw std::invalid_argument("Invalid call message");
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
    // fee asset id
    if (feeAssetId.size() > 0) {
        append(data, feeAssetId);
    }
    // specVersion
    encode32LE(_specVersion, data);
    // transactionVersion
    encode32LE(version, data);
    // genesis hash
    append(data, genesisHash);
    // block hash
    append(data, blockHash);
    return data;
}

// length prefix (2 bytes) + version header (1 bytes) + signer public key (AccountId [1byte] + pub key [32 bytes]) + signature types (1 byte, ed25519 is 0) + signature...

// Encode final data with signer public key and signature.
Data Extrinsic::encodeSignature(const PublicKey& signer, const Data& signature) const {
    Data data;
    // version header
    append(data, Data{extrinsicFormat | signedBit});
    // signer public key
    append(data, Polkadot::encodeAccountId(signer.bytes, encodeRawAccount(multiAddress)));
    // signature type
    append(data, sigTypeEd25519);
    // signature
    append(data, signature);
    // era / nonce / tip
    append(data, encodeEraNonceTip());
    // fee asset id
    if (feeAssetId.size() > 0) {
        append(data, feeAssetId);
    }
    // call
    append(data, call);
    // append length
    Polkadot::encodeLengthPrefix(data);
    return data;
}

bool Extrinsic::encodeRawAccount(bool enableMultiAddress) const {
    return !enableMultiAddress;
}

Data Extrinsic::encodeTransfer(const Proto::Balance::Transfer& transfer, int32_t network, bool enableMultiAddress) const {
    Data data;
    auto address = FullSS58Address(transfer.to_address(), network);
    auto value = load(transfer.value());
    // call index
    append(data, encodeCallIndex(transfer.module_index(), transfer.method_index()));
    // destination
    append(data, Polkadot::encodeAccountId(address.keyBytes(), encodeRawAccount(enableMultiAddress)));
    // value
    append(data, Polkadot::encodeCompact(value));
    // memo
    if (transfer.memo().length() > 0) {
        append(data, 0x01);
        auto memo = transfer.memo();
        if (memo.length() < 32) {
            // padding memo with null
            memo.append(32 - memo.length(), '\0');
        }
        append(data, TW::data(memo));
    }
    return data;
}

Data Extrinsic::encodeAssetTransfer(const Proto::Balance::AssetTransfer& transfer, int32_t network, bool enableMultiAddress) {
    Data data;
    auto address = FullSS58Address(transfer.to_address(), network);
    auto value = load(transfer.value());

    // call index
    append(data, encodeCallIndex(transfer.module_index(), transfer.method_index()));
    // asset id
    if (transfer.asset_id() > 0) {
        // For native token transfer, should ignore asset id
        append(data, Polkadot::encodeCompact(transfer.asset_id()));
    }
    // destination
    append(data, Polkadot::encodeAccountId(address.keyBytes(), encodeRawAccount(enableMultiAddress)));
    // value
    append(data, Polkadot::encodeCompact(value));
    return data;
}

Data Extrinsic::encodeBalanceCall(const Proto::Balance& balance) {
    Data data;
    if (balance.has_transfer()) {
        auto transfer = balance.transfer();
        append(data, encodeTransfer(transfer, _network, multiAddress));
    } else if (balance.has_batch_transfer()) {
        // init call array
        auto calls = std::vector<Data>();
        auto batchTransfer = balance.batch_transfer().transfers();
        for (auto transfer : batchTransfer) {
            // put into calls array
            calls.push_back(encodeTransfer(transfer, _network, multiAddress));
        }
        data = encodeBatchCall(calls, balance.batch_transfer().module_index(), balance.batch_transfer().method_index());
    } else if (balance.has_asset_transfer()) {
        // keep fee asset id encoding which will be used in encodePayload & encodeSignature.
        // see: https://github.com/paritytech/substrate/blob/d1221692968b8bc62d6eab9d10cb6b5bf38c5dc2/frame/transaction-payment/asset-tx-payment/src/lib.rs#L152
        auto rawFeeAssetId = balance.asset_transfer().fee_asset_id();
        if (rawFeeAssetId > 0) {
            this->feeAssetId.push_back(0x01);
            Data feeEncoding;
            encode32LE(rawFeeAssetId, feeEncoding);
            append(this->feeAssetId, feeEncoding);
        } else {
            // use native token
            this->feeAssetId.push_back(0x00);
        }

        append(data, encodeAssetTransfer(balance.asset_transfer(), _network, multiAddress));
    }  else if (balance.has_batch_asset_transfer()) {
        // keep fee asset id encoding which will be used in encodePayload & encodeSignature.
        // see: https://github.com/paritytech/substrate/blob/d1221692968b8bc62d6eab9d10cb6b5bf38c5dc2/frame/transaction-payment/asset-tx-payment/src/lib.rs#L152
        auto rawFeeAssetId = balance.batch_asset_transfer().fee_asset_id();
        if (rawFeeAssetId > 0) {
            this->feeAssetId.push_back(0x01);
            Data feeEncoding;
            encode32LE(rawFeeAssetId, feeEncoding);
            append(this->feeAssetId, feeEncoding);
        } else {
            // use native token
            this->feeAssetId.push_back(0x00);
        }
        
        // init call array
        auto calls = std::vector<Data>();
        auto batchTransfer = balance.batch_asset_transfer().transfers();
        for (auto transfer : batchTransfer) {
            // put into calls array
            calls.push_back(encodeAssetTransfer(transfer, _network, multiAddress));
        }
        data = encodeBatchCall(calls, balance.batch_asset_transfer().module_index(), balance.batch_asset_transfer().method_index());
    }

    return data;
}

Data Extrinsic::encodeBatchCall(const std::vector<Data>& calls, int32_t moduleIndex, int32_t methodIndex) const {
    Data data;
    append(data, encodeCallIndex(moduleIndex, methodIndex));
    append(data, Polkadot::encodeVector(calls));
    return data;
}

Data Extrinsic::encodeEraNonceTip() const {
    Data data;
    // era
    append(data, era);
    // nonce
    append(data, Polkadot::encodeCompact(nonce));
    // tip
    append(data, Polkadot::encodeCompact(tip));
    return data;
}

Data Extrinsic::encodeAuthorizationCall(const Proto::Authorization& authorization) const {
    Data data;
    if (authorization.has_join_identity()) {
        auto identity = authorization.join_identity();

        // call index
        append(data, encodeCallIndex(identity.module_index(), identity.method_index()));

        // target
        append(data, 0x01);

        auto address = FullSS58Address(identity.target(), _network);
        append(data, Polkadot::encodeAccountId(address.keyBytes(), true));

        // join identity
        append(data, 0x05);

        if (identity.has_data()) {
            auto authData = identity.data();

            // asset
            if (authData.has_asset()) {
                append(data, 0x01);
                append(data, TW::data(authData.asset().data()));
            } else {
                append(data, 0x00);
            }

            // extrinsic
            if (authData.has_extrinsic()) {
                append(data, 0x01);
                append(data, TW::data(authData.extrinsic().data()));
            } else {
                append(data, 0x00);
            }

            // portfolio
            if (authData.has_portfolio()) {
                append(data, 0x01);
                append(data, TW::data(authData.portfolio().data()));
            } else {
                append(data, 0x00);
            }
        } else {
            // authorize all permissions
            append(data, {0x01, 0x00}); // asset
            append(data, {0x01, 0x00}); // extrinsic
            append(data, {0x01, 0x00}); // portfolio
        }
        append(data, Polkadot::encodeCompact(identity.expiry()));
    } else {
        throw std::invalid_argument("Invalid authorization message");
    }

    return data;
}

Data Extrinsic::encodeIdentityCall(const Proto::Identity& identity) const {
    Data data;
    if (identity.has_join_identity_as_key()) {
        auto key = identity.join_identity_as_key();

        // call index
        append(data, encodeCallIndex(key.module_index(), key.method_index()));

        // data
        encode64LE(key.auth_id(), data);
    } else {
        throw std::invalid_argument("Invalid identity message");
    }

    return data;
}

} // namespace TW::Substrate
