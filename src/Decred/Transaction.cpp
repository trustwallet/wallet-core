// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../Bitcoin/SigHashType.h"
#include "../BinaryCoding.h"

#include <cassert>

namespace TW::Decred {

namespace {
// Indicates the serialization does not include any witness data.
static const uint32_t sigHashSerializePrefix = 1;

// Indicates the serialization only contains witness data.
static const uint32_t sigHashSerializeWitness = 3;

std::size_t sigHashWitnessSize(const std::vector<TransactionInput>& inputs,
                               const Bitcoin::Script& signScript);
} // namespace

Data Transaction::computeSignatureHash(const Bitcoin::Script& prevOutScript, size_t index,
                                       enum TWBitcoinSigHashType hashType) const {
    assert(index < inputs.size());

    if (Bitcoin::hashTypeIsSingle(hashType) && index >= outputs.size()) {
        throw std::invalid_argument("attempt to sign single input at index "
                                    "larger than the number of outputs");
    }

    auto inputsToSign = inputs;
    auto signIndex = index;
    if ((hashType & TWBitcoinSigHashTypeAnyoneCanPay) != 0) {
        inputsToSign.clear();
        inputsToSign.push_back(inputs[index]);
        signIndex = 0;
    }

    auto outputsToSign = outputs;
    switch (hashType & Bitcoin::SigHashMask) {
    case TWBitcoinSigHashTypeNone:
        outputsToSign = {};
        break;
    case TWBitcoinSigHashTypeSingle:
        outputsToSign.clear();
        std::copy(outputs.begin(), outputs.begin() + index + 1, std::back_inserter(outputsToSign));
        break;
    default:
        // Keep all outputs
        break;
    }

    auto preimage = Data();
    preimage.reserve(Hash::sha256Size * 2 + 4);
    encode32LE(hashType, preimage);

    const auto prefixHash =
        computePrefixHash(inputsToSign, outputsToSign, signIndex, index, hashType);
    std::copy(prefixHash.begin(), prefixHash.end(), std::back_inserter(preimage));

    const auto witnessHash = computeWitnessHash(inputsToSign, prevOutScript, signIndex);
    std::copy(witnessHash.begin(), witnessHash.end(), std::back_inserter(preimage));

    return Hash::blake256(preimage);
}

Data Transaction::computePrefixHash(const std::vector<TransactionInput>& inputsToSign,
                                    const std::vector<TransactionOutput>& outputsToSign,
                                    std::size_t signIndex, std::size_t index,
                                    enum TWBitcoinSigHashType hashType) const {
    auto preimage = Data{};

    // Commit to the version and hash serialization type.
    encode32LE(static_cast<uint32_t>(version) |
                   (static_cast<uint32_t>(sigHashSerializePrefix) << 16),
               preimage);

    // Commit to the relevant transaction inputs.
    encodeVarInt(inputsToSign.size(), preimage);
    for (auto i = 0ul; i < inputsToSign.size(); i += 1) {
        auto& input = inputsToSign[i];
        input.previousOutput.encode(preimage);

        auto sequence = input.sequence;
        if ((Bitcoin::hashTypeIsNone(hashType) || Bitcoin::hashTypeIsSingle(hashType)) &&
            i != signIndex) {
            sequence = 0;
        }
        encode32LE(sequence, preimage);
    }

    // Commit to the relevant transaction outputs.
    encodeVarInt(outputsToSign.size(), preimage);
    for (auto i = 0ul; i < outputsToSign.size(); i += 1) {
        auto& output = outputsToSign[i];
        auto value = output.value;
        auto pkScript = output.script;
        if (Bitcoin::hashTypeIsSingle(hashType) && i != index) {
            value = -1;
            pkScript = {};
        }
        encode64LE(value, preimage);
        encode16LE(output.version, preimage);
        pkScript.encode(preimage);
    }

    encode32LE(lockTime, preimage);
    encode32LE(expiry, preimage);

    return Hash::blake256(preimage);
}

Data Transaction::computeWitnessHash(const std::vector<TransactionInput>& inputsToSign,
                                     const Bitcoin::Script& signScript,
                                     std::size_t signIndex) const {
    const auto size = sigHashWitnessSize(inputsToSign, signScript);
    auto witnessBuf = Data();
    witnessBuf.reserve(size);

    // Commit to the version and hash serialization type.
    encode32LE(static_cast<uint32_t>(version) |
                   (static_cast<uint32_t>(sigHashSerializeWitness) << 16),
               witnessBuf);

    // Commit to the relevant transaction inputs.
    encodeVarInt(inputsToSign.size(), witnessBuf);
    for (auto i = 0ul; i < inputsToSign.size(); i += 1) {
        if (i == signIndex) {
            signScript.encode(witnessBuf);
        } else {
            Bitcoin::Script().encode(witnessBuf);
        }
    }

    return Hash::blake256(witnessBuf);
}

Data Transaction::hash() const {
    Data preimage;
    encode32LE(static_cast<uint32_t>(version) |
                   (static_cast<uint32_t>(SerializeType::noWitness) << 16),
               preimage);
    encodePrefix(preimage);
    return Hash::blake256(preimage);
}

void Transaction::encode(Data& data) const {
    encode32LE(static_cast<uint32_t>(version) | (static_cast<uint32_t>(serializeType) << 16), data);

    switch (serializeType) {
    case SerializeType::noWitness:
        encodePrefix(data);
        break;
    case SerializeType::onlyWitness:
        encodeWitness(data);
        break;
    case SerializeType::full:
        encodePrefix(data);
        encodeWitness(data);
        break;
    }
}

void Transaction::encodePrefix(Data& data) const {
    encodeVarInt(inputs.size(), data);
    for (auto& input : inputs) {
        input.encode(data);
    }

    encodeVarInt(outputs.size(), data);
    for (auto& output : outputs) {
        output.encode(data);
    }

    encode32LE(lockTime, data);
    encode32LE(expiry, data);
}

void Transaction::encodeWitness(Data& data) const {
    encodeVarInt(inputs.size(), data);
    for (auto& input : inputs) {
        input.encodeWitness(data);
    }
}

Proto::Transaction Transaction::proto() const {
    auto protoTx = Proto::Transaction();
    protoTx.set_version(version);
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

namespace {
/// Returns the number of bytes the passed parameters would take when encoded
/// with the format used by the witness hash portion of the overall signature
/// hash.
std::size_t sigHashWitnessSize(const std::vector<TransactionInput>& inputs,
                               const Bitcoin::Script& signScript) {
    // 1) 4 bytes version/serialization type
    // 2) number of inputs varint
    // 3) per input:
    //    a) prevout pkscript varint (1 byte if not input being signed)
    //    b) N bytes prevout pkscript (0 bytes if not input being signed)
    //
    // NOTE: The prevout pkscript is replaced by nil for all inputs except
    // the input being signed.  Thus, all other inputs (aka numTxIns-1) commit
    // to a nil script which gets encoded as a single 0x00 byte.  This is
    // because encoding 0 as a varint results in 0x00 and there is no script
    // to write.  So, rather than looping through all inputs and manually
    // calculating the size per input, use (numTxIns - 1) as an
    // optimization.
    const auto numTxIns = inputs.size();
    return 4 + varIntSize(numTxIns) + (numTxIns - 1) + varIntSize(signScript.bytes.size()) +
           signScript.bytes.size();
}
} // namespace

} // namespace TW::Decred
