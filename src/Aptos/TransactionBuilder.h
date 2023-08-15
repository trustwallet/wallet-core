// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "HexCoding.h"
#include "TransactionPayload.h"

#include <nlohmann/json.hpp>
#include <memory>

namespace TW::Aptos {

struct TransactionBase;

using TransactionBasePtr = std::unique_ptr<TransactionBase>;

struct TransactionBase {
    virtual ~TransactionBase() = default;

    virtual TxCompiler::Proto::PreSigningOutput preImage() noexcept = 0;

    virtual Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) = 0;

    virtual Proto::SigningOutput sign(const Proto::SigningInput& input) = 0;
};

class BlindBuilder final : public TransactionBase {
public:
    BlindBuilder() noexcept = default;

    BlindBuilder& encodedCallHex(const std::string& encodedCallHex) {
        mEncodedCall = parse_hex(encodedCallHex);
        return *this;
    }

    TxCompiler::Proto::PreSigningOutput preImage() noexcept override {
        TxCompiler::Proto::PreSigningOutput output;
        // Aptos has no preImageHash.
        output.set_data(mEncodedCall.data(), mEncodedCall.size());
        return output;
    }

    Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) override {
        Proto::SigningOutput output;
        const auto& pubKeyData = publicKey.bytes;

        BCS::Serializer serializer;
        serializer.add_bytes(begin(mEncodedCall), end(mEncodedCall));

        output.set_raw_txn(mEncodedCall.data(), mEncodedCall.size());
        output.mutable_authenticator()->set_public_key(pubKeyData.data(), pubKeyData.size());
        output.mutable_authenticator()->set_signature(signature.data(), signature.size());
        serializer << BCS::uleb128{.value = 0} << pubKeyData << signature;
        output.set_encoded(serializer.bytes.data(), serializer.bytes.size());

        // clang-format off
        nlohmann::json json = {
            {"type", "ed25519_signature"},
            {"public_key", hexEncoded(pubKeyData)},
            {"signature", hexEncoded(signature)}
        };
        // clang-format on
        output.set_json(json.dump());

        return output;
    }

    Proto::SigningOutput sign(const Proto::SigningInput& input) override {
        auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        auto signature = privateKey.sign(mEncodedCall, TWCurveED25519);
        return compile(signature, publicKey);
    }

private:
    Data mEncodedCall;
};

// Standard transaction builder.
class TransactionBuilder final : public TransactionBase {
public:
    TransactionBuilder() noexcept = default;

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

    BCS::Serializer prepareSerializer() noexcept {
        BCS::Serializer serializer;
        serializer << mSender << mSequenceNumber << mPayload << mMaxGasAmount << mGasUnitPrice << mExpirationTimestampSecs << mChainId;
        return serializer;
    }

    Data msgToSign() noexcept {
        auto serialized = prepareSerializer().bytes;
        auto preImageOutput = TW::Hash::sha3_256(gAptosSalt.data(), gAptosSalt.size());
        append(preImageOutput, serialized);
        return preImageOutput;
    }

    TxCompiler::Proto::PreSigningOutput preImage() noexcept override {
        TxCompiler::Proto::PreSigningOutput output;
        auto signingMsg = msgToSign();
        // Aptos has no preImageHash.
        output.set_data(signingMsg.data(), signingMsg.size());
        return output;
    }

    Proto::SigningOutput compile(const Data& signature, const PublicKey& publicKey) noexcept override {
        Proto::SigningOutput output;
        const auto& pubKeyData = publicKey.bytes;

        auto serializer = prepareSerializer();

        output.set_raw_txn(serializer.bytes.data(), serializer.bytes.size());
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
        return output;
    }

    Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept override {
        auto signingMsg = msgToSign();
        auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        auto signature = privateKey.sign(signingMsg, TWCurveED25519);
        auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
        return compile(signature, publicKey);
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
