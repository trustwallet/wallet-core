// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "HexCoding.h"
#include "TransactionPayload.h"
#include <nlohmann/json.hpp>

namespace TW::Aptos {

class TransactionBuilder {
public:
    TransactionBuilder() noexcept = default;

    static TransactionBuilder builder() noexcept { return {}; }

    TransactionBuilder& sender(Address sender) noexcept {
        mSender = sender;
        return *this;
    }

    TransactionBuilder& sequenceNumber(std::uint64_t sequenceNumber) noexcept {
        mSequenceNumber = sequenceNumber;
        return *this;
    }

    TransactionBuilder& payload(TransactionPayload payload) noexcept {
        mPayload = std::move(payload);
        return *this;
    }

    TransactionBuilder& maxGasAmount(std::uint64_t maxGasAmount) noexcept {
        mMaxGasAmount = maxGasAmount;
        return *this;
    }

    TransactionBuilder& gasUnitPrice(std::uint64_t gasUnitPrice) noexcept {
        mGasUnitPrice = gasUnitPrice;
        return *this;
    }

    TransactionBuilder& expirationTimestampSecs(std::uint64_t expirationTimestampSecs) noexcept {
        mExpirationTimestampSecs = expirationTimestampSecs;
        return *this;
    }

    TransactionBuilder& chainId(std::uint8_t chainId) noexcept {
        mChainId = chainId;
        return *this;
    }

    TransactionBuilder& sign(const Proto::SigningInput& input, Proto::SigningOutput& output) noexcept {
        BCS::Serializer serializer;
        serializer << mSender << mSequenceNumber << mPayload << mMaxGasAmount << mGasUnitPrice << mExpirationTimestampSecs << mChainId;
        auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        output.set_raw_txn(serializer.bytes.data(), serializer.bytes.size());
        auto msgToSign = TW::Hash::sha3_256(gAptosSalt.data(), gAptosSalt.size());
        append(msgToSign, serializer.bytes);
        auto signature = privateKey.sign(msgToSign, TWCurveED25519);
        auto pubKeyData = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;
        output.mutable_authenticator()->set_public_key(pubKeyData.data(), pubKeyData.size());
        output.mutable_authenticator()->set_signature(signature.data(), signature.size());
        serializer << BCS::uleb128{.value = 0} << pubKeyData << signature;
        output.set_encoded(serializer.bytes.data(), serializer.bytes.size());

        // https://fullnode.devnet.aptoslabs.com/v1/spec#/operations/submit_transaction
        // clang-format off
        nlohmann::json json = {
            {"sender", mSender.string()},
            {"sequence_number", std::to_string(mSequenceNumber)},
            {"max_gas_amount", std::to_string(mMaxGasAmount)},
            {"gas_unit_price", std::to_string(mGasUnitPrice)},
            {"expiration_timestamp_secs", std::to_string(mExpirationTimestampSecs)},
            {"payload", payloadToJson(mPayload)},
            {"signature", {
                {"type", "ed25519_signature"},
                {"public_key", hexEncoded(pubKeyData)},
                {"signature", hexEncoded(signature)}}
            }
        };
        // clang-format on
        output.set_json(json.dump());
        return *this;
    }

private:
    Address mSender{};
    std::uint64_t mSequenceNumber{};
    TransactionPayload mPayload{};
    std::uint64_t mMaxGasAmount{};
    std::uint64_t mGasUnitPrice{};
    std::uint64_t mExpirationTimestampSecs{};
    std::uint8_t mChainId{};
};

} // namespace TW::Aptos
