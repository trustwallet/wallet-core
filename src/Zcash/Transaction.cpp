// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "../Bitcoin/BinaryCoding.h"
#include "../Hash.h"

#include <cassert>

using namespace TW;
using namespace TW::Zcash;

const auto sigHashPersonalization = Data({'Z','c','a','s','h','S','i','g','H','a','s','h'});
const auto prevoutsHashPersonalization = Data({'Z','c','a','s','h','P','r','e','v','o','u','t','H','a','s','h'});
const auto sequenceHashPersonalization = Data({'Z','c','a','s','h','S','e','q','u','e','n','c','H','a','s','h'});
const auto outputsHashPersonalization = Data({'Z','c','a','s','h','O','u','t','p','u','t','s','H','a','s','h'});
const auto joinsplitsHashPersonalization = Data({'Z','c','a','s','h','J','S','p','l','i','t','s','H','a','s','h'});
const auto shieldedSpendHashPersonalization = Data({'Z','c','a','s','h','S','S','p','e','n','d','s','H','a','s','h'});
const auto shieldedOutputsHashPersonalization = Data({'Z','c','a','s','h','S','O','u','t','p','u','t','H','a','s','h'});

/// See also https://github.com/zcash/zcash/blob/36243f41f1d8df98bdc834825ba539263f1da121/src/consensus/upgrades.cpp#L28
const byte saplingBranchID[] = {0xbb, 0x09, 0xb8, 0x76};

Data Transaction::getPreImage(const Bitcoin::Script& scriptCode, int index, uint32_t hashType, uint64_t amount) const {
    assert(index < inputs.size());

    auto data = Data{};

    // header
    Bitcoin::encode32(version, data);

    // nVersionGroupId
    Bitcoin::encode32(versionGroupId, data);

    // Input prevouts (none/all, depending on flags)
    if ((hashType & TWSignatureHashTypeAnyoneCanPay) == 0) {
        auto hashPrevouts = getPrevoutHash();
        std::copy(std::begin(hashPrevouts), std::end(hashPrevouts), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // Input nSequence (none/all, depending on flags)
    if ((hashType & TWSignatureHashTypeAnyoneCanPay) == 0 && !TWSignatureHashTypeIsSingle(hashType) && !TWSignatureHashTypeIsNone(hashType)) {
        auto hashSequence = getSequenceHash();
        std::copy(std::begin(hashSequence), std::end(hashSequence), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // Outputs (none/one/all, depending on flags)
    if (!TWSignatureHashTypeIsSingle(hashType) && !TWSignatureHashTypeIsNone(hashType)) {
        auto hashOutputs = getOutputsHash();
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else if (TWSignatureHashTypeIsSingle(hashType) && index < outputs.size()) {
        auto outputData = Data{};
        outputs[index].encode(outputData);
        auto hashOutputs = TW::Hash::blake2b(outputData, outputData.size(), outputsHashPersonalization);
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else {
        fill_n(back_inserter(data), 32, 0);
    }

    // JoinSplits
    auto hashJoinSplits = getJoinSplitsHash();
    data.insert(std::end(data), std::begin(hashJoinSplits), std::end(hashJoinSplits));

    // ShieldedSpends
    auto hashShieldedSpends = getShieldedSpendsHash();
    data.insert(std::end(data), std::begin(hashShieldedSpends), std::end(hashShieldedSpends));

    // ShieldedOutputs
    auto hashShieldedOutputs = getShieldedOutputsHash();
    data.insert(std::end(data), std::begin(hashShieldedOutputs), std::end(hashShieldedOutputs));

    // Locktime
    Bitcoin::encode32(lockTime, data);

    // ExpiryHeight
    Bitcoin::encode32(expiryHeight, data);

    // ValueBalance
    Bitcoin::encode64(valueBalance, data);

    // Sighash type
    Bitcoin::encode32(hashType, data);

    // The input being signed (replacing the scriptSig with scriptCode + amount)
    // The prevout may already be contained in hashPrevout, and the nSequence
    // may already be contain in hashSequence.
    reinterpret_cast<const TW::Bitcoin::OutPoint&>(inputs[index].previousOutput).encode(data);
    scriptCode.encode(data);

    Bitcoin::encode64(amount, data);
    Bitcoin::encode32(inputs[index].sequence, data);

    return data;
}

Data Transaction::getPrevoutHash() const {
    auto data = Data{};
    for (auto& input : inputs) {
        auto& outpoint = input.previousOutput;
        outpoint.encode(data);
    }
    auto hash = TW::Hash::blake2b(data, 32, prevoutsHashPersonalization);
    return hash;
}

Data Transaction::getSequenceHash() const {
    auto data = Data{};
    for (auto& input : inputs) {
        Bitcoin::encode32(input.sequence, data);
    }
    auto hash = TW::Hash::blake2b(data, 32, sequenceHashPersonalization);
    return hash;
}

Data Transaction::getOutputsHash() const {
    auto data = Data{};
    for (auto& output : outputs) {
        output.encode(data);
    }
    auto hash = TW::Hash::blake2b(data, 32, outputsHashPersonalization);
    return hash;
}

Data Transaction::getJoinSplitsHash() const {
    Data vec(32, 0);
    return vec;
}

Data Transaction::getShieldedSpendsHash() const {
    Data vec(32, 0);
    return vec;
}

Data Transaction::getShieldedOutputsHash() const {
    Data vec(32, 0);
    return vec;
}

void Transaction::encode(Data& data) const {
    Bitcoin::encode32(version, data);
    Bitcoin::encode32(versionGroupId, data);

    // vin
    Bitcoin::writeCompactSize(inputs.size(), data);
    for (auto& input : inputs) {
        input.encode(data);
    }

    // vout
    Bitcoin::writeCompactSize(outputs.size(), data);
    for (auto& output : outputs) {
        output.encode(data);
    }

    Bitcoin::encode32(lockTime, data);
    Bitcoin::encode32(expiryHeight, data);
    Bitcoin::encode64(valueBalance, data);

    // vShieldedSpend
    Bitcoin::writeCompactSize(0, data);
    // vShieldedOutput
    Bitcoin::writeCompactSize(0, data);
    // vJoinSplit
    Bitcoin::writeCompactSize(0, data);
}

Data Transaction::getSignatureHash(const Bitcoin::Script& scriptCode, size_t index, uint32_t hashType, uint64_t amount, TWBitcoinSignatureVersion version) const {
    Data personalization;
    personalization.reserve(16);
    std::copy(sigHashPersonalization.begin(), sigHashPersonalization.begin() + 12, std::back_inserter(personalization));
    std::copy(saplingBranchID, saplingBranchID + 4, std::back_inserter(personalization));

    auto preimage = getPreImage(scriptCode, index, hashType, amount);
    auto hash = TW::Hash::blake2b(preimage, 32, personalization);
    return hash;
}

Bitcoin::Proto::Transaction Transaction::proto() const {
    auto protoTx = Bitcoin::Proto::Transaction();
    protoTx.set_version(version);
    protoTx.set_locktime(lockTime);

    for (const auto& input : inputs) {
        auto protoInput = protoTx.add_inputs();
        protoInput->mutable_previousoutput()->set_hash(input.previousOutput.hash.data(), input.previousOutput.hash.size());
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
