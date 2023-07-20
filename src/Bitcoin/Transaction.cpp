// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "SegwitAddress.h"
#include "SignatureVersion.h"
#include "SigHashType.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

#include "../BinaryCoding.h"

#include <cassert>

namespace TW::Bitcoin {

Data Transaction::getPreImage(const Script& scriptCode, size_t index,
                              enum TWBitcoinSigHashType hashType, uint64_t amount) const {
    assert(index < inputs.size());

    Data data;

    // Version
    encode32LE(_version, data);

    // Input prevouts (none/all, depending on flags)
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) == 0) {
        auto hashPrevouts = getPrevoutHash();
        std::copy(std::begin(hashPrevouts), std::end(hashPrevouts), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // Input nSequence (none/all, depending on flags)
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) == 0 && !hashTypeIsSingle(hashType) &&
        !hashTypeIsNone(hashType)) {
        auto hashSequence = getSequenceHash();
        std::copy(std::begin(hashSequence), std::end(hashSequence), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // The input being signed (replacing the scriptSig with scriptCode + amount)
    // The prevout may already be contained in hashPrevout, and the nSequence
    // may already be contained in hashSequence.
    reinterpret_cast<const OutPoint&>(inputs[index].previousOutput).encode(data);
    scriptCode.encode(data);

    encode64LE(amount, data);
    encode32LE(inputs[index].sequence, data);

    // Outputs (none/one/all, depending on flags)
    if (!hashTypeIsSingle(hashType) && !hashTypeIsNone(hashType)) {
        auto hashOutputs = getOutputsHash();
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else if (hashTypeIsSingle(hashType) && index < outputs.size()) {
        Data outputData;
        outputs[index].encode(outputData);
        auto hashOutputs = Hash::hash(hasher, outputData);
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else {
        fill_n(back_inserter(data), 32, 0);
    }

    // Locktime
    encode32LE(lockTime, data);

    // Sighash type
    encode32LE(hashType, data);

    return data;
}

Data Transaction::getPrevoutHash() const {
    Data data;
    for (auto& input : inputs) {
        auto& outpoint = reinterpret_cast<const OutPoint&>(input.previousOutput);
        outpoint.encode(data);
    }
    auto hash = Hash::hash(hasher, data);
    return hash;
}

Data Transaction::getSequenceHash() const {
    Data data;
    for (auto& input : inputs) {
        encode32LE(input.sequence, data);
    }
    auto hash = Hash::hash(hasher, data);
    return hash;
}

Data Transaction::getOutputsHash() const {
    Data data;
    for (auto& output : outputs) {
        output.encode(data);
    }
    auto hash = Hash::hash(hasher, data);
    return hash;
}

void Transaction::encode(Data& data, enum SegwitFormatMode segwitFormat) const {
    bool useWitnessFormat = true;
    switch (segwitFormat) {
    case NonSegwit:
        useWitnessFormat = false;
        break;
    case IfHasWitness:
        useWitnessFormat = hasWitness();
        break;
    case Segwit:
        useWitnessFormat = true;
        break;
    }

    encode32LE(_version, data);

    if (useWitnessFormat) {
        // Use extended format in case witnesses are to be serialized.
        data.push_back(0); // marker
        data.push_back(1); // flag
    }

    // txins
    encodeVarInt(inputs.size(), data);
    for (auto& input : inputs) {
        input.encode(data);
    }

    // txouts
    encodeVarInt(outputs.size(), data);
    for (auto& output : outputs) {
        output.encode(data);
    }

    if (useWitnessFormat) {
        encodeWitness(data);
    }

    encode32LE(lockTime, data); // nLockTime
}

void Transaction::encodeWitness(Data& data) const {
    for (auto& input : inputs) {
        input.encodeWitness(data);
    }
}

bool Transaction::hasWitness() const {
    return std::any_of(inputs.begin(), inputs.end(), [](auto& input) { return !input.scriptWitness.empty(); });
}

Data Transaction::getSignatureHash(const Script& scriptCode, size_t index,
                                   enum TWBitcoinSigHashType hashType, uint64_t amount,
                                   enum SignatureVersion version) const {
    if (version == BASE) {
        return getSignatureHashBase(scriptCode, index, hashType);
    }
    // version == WITNESS_V0
    return getSignatureHashWitnessV0(scriptCode, index, hashType, amount);
}

/// Generates the signature hash for Witness version 0 scripts.
Data Transaction::getSignatureHashWitnessV0(const Script& scriptCode, size_t index,
                                            enum TWBitcoinSigHashType hashType,
                                            uint64_t amount) const {
    auto preimage = getPreImage(scriptCode, index, hashType, amount);
    auto hash = Hash::hash(hasher, preimage);
    return hash;
}

/// Generates the signature hash for for scripts other than witness scripts.
Data Transaction::getSignatureHashBase(const Script& scriptCode, size_t index,
                                       enum TWBitcoinSigHashType hashType) const {
    assert(index < inputs.size());

    Data data;

    encode32LE(_version, data);

    auto serializedInputCount =
        (hashType & TWBitcoinSigHashTypeAnyoneCanPay) != 0 ? 1 : inputs.size();
    encodeVarInt(serializedInputCount, data);
    for (auto subindex = 0ul; subindex < serializedInputCount; subindex += 1) {
        serializeInput(subindex, scriptCode, index, hashType, data);
    }

    auto hashNone = hashTypeIsNone(hashType);
    auto hashSingle = hashTypeIsSingle(hashType);
    auto serializedOutputCount = hashNone ? 0 : (hashSingle ? index + 1 : outputs.size());
    encodeVarInt(serializedOutputCount, data);
    for (auto subindex = 0ul; subindex < serializedOutputCount; subindex += 1) {
        if (hashSingle && subindex != index) {
            auto output = TransactionOutput(-1, {});
            output.encode(data);
        } else {
            outputs[subindex].encode(data);
        }
    }

    // Locktime
    encode32LE(lockTime, data);

    // Sighash type
    encode32LE(hashType, data);

    auto hash = Hash::hash(hasher, data);
    return hash;
}

void Transaction::serializeInput(size_t subindex, const Script& scriptCode, size_t index,
                                 enum TWBitcoinSigHashType hashType, Data& data) const {
    // In case of SIGHASH_ANYONECANPAY, only the input being signed is
    // serialized
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) != 0) {
        subindex = index;
    }

    reinterpret_cast<const OutPoint&>(inputs[subindex].previousOutput).encode(data);

    // Serialize the script
    if (subindex != index) {
        encodeVarInt(0, data);
    } else {
        scriptCode.encode(data);
    }

    // Serialize the nSequence
    auto hashNone = hashTypeIsNone(hashType);
    auto hashSingle = hashTypeIsSingle(hashType);
    if (subindex != index && (hashSingle || hashNone)) {
        encode32LE(0, data);
    } else {
        encode32LE(inputs[subindex].sequence, data);
    }
}

std::optional<uint64_t> Transaction::calculateFee(const Data& encoded, uint64_t satVb) {
    Rust::CUInt64ResultWrapper res = Rust::tw_bitcoin_calculate_transaction_fee(encoded.data(), encoded.size(), satVb);
    if (res.isErr()) {
        return std::nullopt;
    }
    return res.unwrap().value;
}

Proto::Transaction Transaction::proto() const {
    auto protoTx = Proto::Transaction();
    protoTx.set_version(_version);
    protoTx.set_locktime(lockTime);

    for (const auto& input : inputs) {
        auto* protoInput = protoTx.add_inputs();
        protoInput->mutable_previousoutput()->set_hash(input.previousOutput.hash.data(),
                                                       input.previousOutput.hash.size());
        protoInput->mutable_previousoutput()->set_index(input.previousOutput.index);
        protoInput->set_sequence(input.sequence);
        protoInput->set_script(input.script.bytes.data(), input.script.bytes.size());
    }

    for (const auto& output : outputs) {
        auto* protoOutput = protoTx.add_outputs();
        protoOutput->set_value(output.value);
        protoOutput->set_script(output.script.bytes.data(), output.script.bytes.size());
    }

    return protoTx;
}

} // namespace TW::Bitcoin
