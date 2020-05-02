// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SegwitAddress.h"
#include "Transaction.h"
#include "SigHashType.h"
#include "../BinaryCoding.h"
#include "../Hash.h"

#include "SignatureVersion.h"

#include <cassert>

using namespace TW::Bitcoin;

std::vector<uint8_t> Transaction::getPreImage(const Script& scriptCode, size_t index,
                                              enum TWBitcoinSigHashType hashType, uint64_t amount) const {
    assert(index < inputs.size());

    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE(version, data);

    // Input prevouts (none/all, depending on flags)
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) == 0) {
        auto hashPrevouts = getPrevoutHash();
        std::copy(std::begin(hashPrevouts), std::end(hashPrevouts), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // Input nSequence (none/all, depending on flags)
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) == 0 &&
        !hashTypeIsSingle(hashType) && !hashTypeIsNone(hashType)) {
        auto hashSequence = getSequenceHash();
        std::copy(std::begin(hashSequence), std::end(hashSequence), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // The input being signed (replacing the scriptSig with scriptCode + amount)
    // The prevout may already be contained in hashPrevout, and the nSequence
    // may already be contain in hashSequence.
    reinterpret_cast<const TW::Bitcoin::OutPoint&>(inputs[index].previousOutput).encode(data);
    scriptCode.encode(data);

    encode64LE(amount, data);
    encode32LE(inputs[index].sequence, data);

    // Outputs (none/one/all, depending on flags)
    if (!hashTypeIsSingle(hashType) && !hashTypeIsNone(hashType)) {
        auto hashOutputs = getOutputsHash();
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else if (hashTypeIsSingle(hashType) && index < outputs.size()) {
        auto outputData = std::vector<uint8_t>{};
        outputs[index].encode(outputData);
        auto hashOutputs = TW::Hash::hash(hasher, outputData);
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

std::vector<uint8_t> Transaction::getPrevoutHash() const {
    auto data = std::vector<uint8_t>{};
    for (auto& input : inputs) {
        auto& outpoint = reinterpret_cast<const TW::Bitcoin::OutPoint&>(input.previousOutput);
        outpoint.encode(data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

std::vector<uint8_t> Transaction::getSequenceHash() const {
    auto data = std::vector<uint8_t>{};
    for (auto& input : inputs) {
        encode32LE(input.sequence, data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

std::vector<uint8_t> Transaction::getOutputsHash() const {
    auto data = std::vector<uint8_t>{};
    for (auto& output : outputs) {
        output.encode(data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

void Transaction::encode(bool witness, std::vector<uint8_t>& data) const {
    encode32LE(version, data);

    if (witness) {
        // Use extended format in case witnesses are to be serialized.
        data.push_back(0);
        data.push_back(1);
    }

    encodeVarInt(inputs.size(), data);
    for (auto& input : inputs) {
        input.encode(data);
    }

    encodeVarInt(outputs.size(), data);
    for (auto& output : outputs) {
        output.encode(data);
    }

    if (witness) {
        for (auto& input : inputs) {
            input.encodeWitness(data);
        }
    }

    encode32LE(lockTime, data);
}

std::vector<uint8_t> Transaction::getSignatureHash(const Script& scriptCode, size_t index,
                                                   enum TWBitcoinSigHashType hashType, uint64_t amount,
                                                   enum SignatureVersion version) const {
    switch (version) {
    case BASE:
        return getSignatureHashBase(scriptCode, index, hashType);
    case WITNESS_V0:
        return getSignatureHashWitnessV0(scriptCode, index, hashType, amount);
    }
}

/// Generates the signature hash for Witness version 0 scripts.
std::vector<uint8_t> Transaction::getSignatureHashWitnessV0(const Script& scriptCode, size_t index,
                                                            enum TWBitcoinSigHashType hashType,
                                                            uint64_t amount) const {
    auto preimage = getPreImage(scriptCode, index, hashType, amount);
    auto hash = TW::Hash::hash(hasher, preimage);
    return hash;
}

/// Generates the signature hash for for scripts other than witness scripts.
std::vector<uint8_t> Transaction::getSignatureHashBase(const Script& scriptCode, size_t index,
                                                       enum TWBitcoinSigHashType hashType) const {
    assert(index < inputs.size());

    auto data = std::vector<uint8_t>{};

    encode32LE(version, data);

    auto serializedInputCount =
        (hashType & TWBitcoinSigHashTypeAnyoneCanPay) != 0 ? 1 : inputs.size();
    encodeVarInt(serializedInputCount, data);
    for (auto subindex = 0; subindex < serializedInputCount; subindex += 1) {
        serializeInput(subindex, scriptCode, index, hashType, data);
    }

    auto hashNone = hashTypeIsNone(hashType);
    auto hashSingle = hashTypeIsSingle(hashType);
    auto serializedOutputCount = hashNone ? 0 : (hashSingle ? index + 1 : outputs.size());
    encodeVarInt(serializedOutputCount, data);
    for (auto subindex = 0; subindex < serializedOutputCount; subindex += 1) {
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

    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

void Transaction::serializeInput(size_t subindex, const Script& scriptCode, size_t index,
                                 enum TWBitcoinSigHashType hashType, std::vector<uint8_t>& data) const {
    // In case of SIGHASH_ANYONECANPAY, only the input being signed is
    // serialized
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) != 0) {
        subindex = index;
    }

    reinterpret_cast<const TW::Bitcoin::OutPoint&>(inputs[subindex].previousOutput).encode(data);

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

Proto::Transaction Transaction::proto() const {
    auto protoTx = Proto::Transaction();
    protoTx.set_version(version);
    protoTx.set_locktime(lockTime);

    for (const auto& input : inputs) {
        auto protoInput = protoTx.add_inputs();
        protoInput->mutable_previousoutput()->set_hash(input.previousOutput.hash.data(),
                                                       input.previousOutput.hash.size());
        protoInput->mutable_previousoutput()->set_index(input.previousOutput.index);
        protoInput->set_sequence(input.sequence);
        protoInput->set_script(input.script.bytes.data(), input.script.bytes.size());
    }

    for (const auto& output : outputs) {
        auto protoOutput = protoTx.add_outputs();
        protoOutput->set_value(output.value);
        protoOutput->set_script(output.script.bytes.data(), output.script.bytes.size());
    }

    return protoTx;
}
