// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Bitcoin/TransactionBuilder.h"
#include "../Bitcoin/TransactionSigner.h"
#include "../BinaryCoding.h"
#include "../Data.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../proto/Bitcoin.pb.h"
#include "../proto/Common.pb.h"

#include <TrustWalletCore/TWBitcoinTransactionSigner.h>

#include <algorithm>

using namespace TW::Bitcoin;

struct TWBitcoinTransactionSigner *_Nonnull TWBitcoinTransactionSignerCreate(TW_Bitcoin_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    return new TWBitcoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input)) };
}

struct TWBitcoinTransactionSigner *_Nonnull TWBitcoinTransactionSignerCreateWithPlan(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));
    return new TWBitcoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input), std::move(plan)) };
}

void TWBitcoinTransactionSignerDelete(struct TWBitcoinTransactionSigner *_Nonnull signer) {
    delete signer;
}

TW_Bitcoin_Proto_TransactionPlan TWBitcoinTransactionSignerPlan(struct TWBitcoinTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWBitcoinTransactionSignerSign(struct TWBitcoinTransactionSigner *_Nonnull signer) {
    auto result = signer->impl.sign();
    auto protoResult = TW::Proto::Result();
    if (!result) {
        protoResult.set_success(false);
        protoResult.set_error(result.error());
        auto serialized = protoResult.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    }

    const auto& tx = result.payload();
    auto protoOutput = Proto::SigningOutput();
    *protoOutput.mutable_transaction() = tx.proto();

    TW::Data encoded;
    auto hasWitness = std::any_of(tx.inputs.begin(), tx.inputs.end(), [](auto& input) { return !input.scriptWitness.empty(); });
    tx.encode(hasWitness, encoded);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    TW::Data txHashData = encoded;
    if (hasWitness) {
        txHashData.clear();
        tx.encode(false, txHashData);
    }
    auto txHash = TW::Hash::sha256(TW::Hash::sha256(txHashData));
    std::reverse(txHash.begin(), txHash.end());
    protoOutput.set_transaction_id(TW::hex(txHash));

    protoResult.set_success(true);
    protoResult.add_objects()->PackFrom(protoOutput);

    auto serialized = protoResult.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWBitcoinTransactionSignerMessage(TW_Bitcoin_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signatureVersion = BASE;
    if ((input.hash_type() & TWBitcoinSigHashTypeFork) != 0) {
        signatureVersion = WITNESS_V0;
    }

    auto signer = new TWBitcoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input)) };
    for (auto i = 0; i < signer->impl.plan.utxos.size(); i++) {
        auto& utxo = signer->impl.plan.utxos[i];
        auto script = Script(utxo.script().begin(), utxo.script().end());
        auto sighash = signer->impl.transaction.getPreImage(script, i,
                                                            static_cast<TWBitcoinSigHashType>(input.hash_type()), utxo.amount(), signatureVersion);

        signer->impl.plan.utxos[i].set_script(reinterpret_cast<const uint8_t *>(sighash.data()), sighash.size());
    }

    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWBitcoinTransactionSignerMessageSegWit(TW_Bitcoin_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signatureVersion = WITNESS_V0;

    auto signer = new TWBitcoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input)) };
    for (auto i = 0; i < signer->impl.plan.utxos.size(); i++) {
        auto& utxo = signer->impl.plan.utxos[i];
        auto script = Script(utxo.script().begin(), utxo.script().end());
        auto sighash = signer->impl.transaction.getPreImage(script, i,
                                                            static_cast<TWBitcoinSigHashType>(input.hash_type()), utxo.amount(), signatureVersion);

        signer->impl.plan.utxos[i].set_script(reinterpret_cast<const uint8_t *>(sighash.data()), sighash.size());
    }

    auto result = signer->impl.plan.proto();
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWBitcoinTransactionSignerTransaction(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));

    auto signer = new TWBitcoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input), plan) };

    for (auto i = 0; i < signer->impl.transaction.inputs.size(); i++) {
        for (auto j = 0; j < plan.utxos().size(); j++) {
            auto planOutput = OutPoint(plan.utxos()[j].out_point());
            if (signer->impl.transaction.inputs[i].previousOutput == planOutput ){
                signer->impl.transaction.inputs[i].script = Script(plan.utxos()[j].script().begin(), plan.utxos()[j].script().end());
            }
        }
    }

    const auto& tx = signer->impl.transaction;
    auto protoOutput = Proto::SigningOutput();
    *protoOutput.mutable_transaction() = tx.proto();

    TW::Data encoded;
    auto hasWitness = std::any_of(tx.inputs.begin(), tx.inputs.end(), [](auto& input) { return !input.scriptWitness.empty(); });
    tx.encode(hasWitness, encoded);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    TW::Data txHashData = encoded;
    if (hasWitness) {
        txHashData.clear();
        tx.encode(false, txHashData);
    }

    auto txHash = TW::Hash::sha256(TW::Hash::sha256(txHashData));
    std::reverse(txHash.begin(), txHash.end());
    protoOutput.set_transaction_id(TW::hex(txHash));

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

template <typename It>
std::uint64_t readVarInt(It begin, It end, std::uint64_t& offset) {
    std::uint64_t size = 0;
    TW::byte discriminant = *begin;
    switch (discriminant) {
    case 0xff:
        offset = 9;
        size = TW::decode64LE(&*(begin + 1));
        break;
    case 0xfe:
        offset = 5;
        size = TW::decode32LE(&*(begin + 1));
        break;
    case 0xfd:
        offset = 3;
        size = TW::decode16LE(&*(begin + 1));
        break;
    default:
        offset = 1;
        size = std::uint64_t(discriminant);
        break;
    }
    return size;
}

std::vector<TW::Data> decodeWitnessScript(const Script& s) {
    std::vector<TW::Data> witness;
    if (s.empty()) {
        return witness;
    }

    std::uint64_t offset = 1;
    std::uint64_t signatureSize = readVarInt(s.bytes.begin(), s.bytes.end(), offset);
    witness.push_back(TW::Data(s.bytes.begin() + offset, s.bytes.begin() + offset + signatureSize));

    std::uint64_t nextPos = offset + signatureSize;
    offset = 1;
    std::uint64_t pubkeySize = readVarInt(s.bytes.begin() + nextPos, s.bytes.end(), offset);
    witness.push_back(TW::Data(s.bytes.begin() + nextPos + offset, s.bytes.begin() + nextPos + offset + pubkeySize));
    
    return witness;
}

TWData *_Nonnull TWBitcoinTransactionSignerTransactionSegWit(TW_Bitcoin_Proto_SigningInput data, TW_Bitcoin_Proto_TransactionPlan planData) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    Proto::TransactionPlan plan;
    plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)));

    auto signer = new TWBitcoinTransactionSigner{ TransactionSigner<Transaction>(std::move(input), plan) };

    for (auto i = 0; i < signer->impl.transaction.inputs.size(); i++) {
        for (auto j = 0; j < plan.utxos().size(); j++) {
            auto planOutput = OutPoint(plan.utxos()[j].out_point());
            if (signer->impl.transaction.inputs[i].previousOutput == planOutput ){
                signer->impl.transaction.inputs[i].scriptWitness = decodeWitnessScript(Script(plan.utxos()[j].script().begin(), plan.utxos()[j].script().end()));
            }
        }
    }

    const auto& tx = signer->impl.transaction;
    auto protoOutput = Proto::SigningOutput();
    *protoOutput.mutable_transaction() = tx.proto();

    TW::Data encoded;
    auto hasWitness = std::any_of(tx.inputs.begin(), tx.inputs.end(), [](auto& input) { return !input.scriptWitness.empty(); });
    tx.encode(hasWitness, encoded);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    TW::Data txHashData = encoded;
    if (hasWitness) {
        txHashData.clear();
        tx.encode(false, txHashData);
    }

    auto txHash = TW::Hash::sha256(TW::Hash::sha256(txHashData));
    std::reverse(txHash.begin(), txHash.end());
    protoOutput.set_transaction_id(TW::hex(txHash));

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
