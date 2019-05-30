// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "../BinaryCoding.h"
#include "../Hash.h"

#include <TrustWalletCore/TWBitcoin.h>

#include <cassert>

using namespace TW::Iocoin;
/*
std::vector<uint8_t> Transaction::getPreImage(const Bitcoin::Script& scriptCode, size_t index,
                                              uint32_t hashType, uint64_t amount) const {
    assert(index < inputs.size());

    auto data = std::vector<uint8_t>{};

    // Version
    encode32LE(version, data);

    // Input prevouts (none/all, depending on flags)
    if ((hashType & TWSignatureHashTypeAnyoneCanPay) == 0) {
        auto hashPrevouts = getPrevoutHash();
        std::copy(std::begin(hashPrevouts), std::end(hashPrevouts), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // Input nSequence (none/all, depending on flags)
    if ((hashType & TWSignatureHashTypeAnyoneCanPay) == 0 &&
        !TWSignatureHashTypeIsSingle(hashType) && !TWSignatureHashTypeIsNone(hashType)) {
        auto hashSequence = getSequenceHash();
        std::copy(std::begin(hashSequence), std::end(hashSequence), std::back_inserter(data));
    } else {
        std::fill_n(back_inserter(data), 32, 0);
    }

    // The input being signed (replacing the scriptSig with scriptCode + amount)
    // The prevout may already be contained in hashPrevout, and the nSequence
    // may already be contain in hashSequence.
    reinterpret_cast<const TW::Iocoin::OutPoint&>(inputs[index].previousOutput).encode(data);
    scriptCode.encode(data);

    encode64LE(amount, data);

    // Outputs (none/one/all, depending on flags)
    if (!TWSignatureHashTypeIsSingle(hashType) && !TWSignatureHashTypeIsNone(hashType)) {
        auto hashOutputs = getOutputsHash();
        copy(begin(hashOutputs), end(hashOutputs), back_inserter(data));
    } else if (TWSignatureHashTypeIsSingle(hashType) && index < outputs.size()) {
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
*/
std::vector<uint8_t> Transaction::getPrevoutHash() const {
    auto data = std::vector<uint8_t>{};
    for (auto& input : inputs) {
        auto& outpoint = reinterpret_cast<const TW::Iocoin::OutPoint&>(input.previousOutput);
        outpoint.encode(data);
    }
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}

std::vector<uint8_t> Transaction::getSequenceHash() const {
    auto data = std::vector<uint8_t>{};
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

void Transaction::encode(std::vector<uint8_t>& data) const {
    encode32LE(version, data);

    encode32LE(nTime, data);

    encodeVarInt(inputs.size(), data);
    for (auto& input : inputs) {
        input.encode(data);
    }

    encodeVarInt(outputs.size(), data);
    for (auto& output : outputs) {
        output.encode(data);
    }

    encode32LE(lockTime, data);
}

std::vector<uint8_t> Transaction::getSignatureHash(const Bitcoin::Script& scriptCode, size_t index,
                                                       uint32_t hashType) const {
    assert(index < inputs.size());

    auto data = std::vector<uint8_t>{};
   
    Transaction tmp(*this);

    for(auto i=0; i<tmp.inputs.size(); i++)
      tmp.inputs[i].script = Bitcoin::Script();

    tmp.inputs[index].script = scriptCode;

    if((hashType & 0x1f) == TWSignatureHashTypeNone)
    {
      tmp.outputs.clear();
      for(auto i=0; i<tmp.inputs.size(); i++)
      {
        if(i != index) 
          tmp.inputs[i].sequence = 0;
      }
    }
    else if((hashType & 0x1f) == TWSignatureHashTypeSingle)
    {
      if(index >= tmp.outputs.size())
        throw std::invalid_argument("Iocoin::Transaction index out of range.");
    
      int indexOut = index;
      tmp.outputs.resize(indexOut+1);
      for(auto i=0; i<indexOut; i++)
      {
        tmp.outputs[i] = Bitcoin::TransactionOutput(-1,{});
      }
      
      for(auto i=0; i<tmp.inputs.size(); i++)
      {
        if(i != index) 
          tmp.inputs[i].sequence=0;
      }
    }

    tmp.encode(data);
    encode32LE(hashType, data);
     
    auto hash = TW::Hash::hash(hasher, data);
    return hash;
}
std::vector<uint8_t> Transaction::getSignatureHash_(const Bitcoin::Script& scriptCode, size_t index,
                                                       uint32_t hashType) const {
    assert(index < inputs.size());

    auto data = std::vector<uint8_t>{};

    encode32LE(version, data);
    encode32LE(nTime, data);

    auto serializedInputCount =
        (hashType & TWSignatureHashTypeAnyoneCanPay) != 0 ? 1 : inputs.size();
    encodeVarInt(serializedInputCount, data);
    for (auto subindex = 0; subindex < serializedInputCount; subindex += 1) {
        serializeInput(subindex, scriptCode, index, hashType, data);
    }

    auto hashNone = (hashType & 0x1f) == TWSignatureHashTypeNone;
    auto hashSingle = (hashType & 0x1f) == TWSignatureHashTypeSingle;
    auto serializedOutputCount = hashNone ? 0 : (hashSingle ? index + 1 : outputs.size());
    encodeVarInt(serializedOutputCount, data);
    for (auto subindex = 0; subindex < serializedOutputCount; subindex += 1) {
        if (hashSingle && subindex != index) {
            auto output = Bitcoin::TransactionOutput(-1, {});
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

void Transaction::serializeInput(size_t subindex, const Bitcoin::Script& scriptCode, size_t index,
                                 uint32_t hashType, std::vector<uint8_t>& data) const {
    // In case of SIGHASH_ANYONECANPAY, only the input being signed is
    // serialized
    if ((hashType & TWSignatureHashTypeAnyoneCanPay) != 0) {
        subindex = index;
    }

    reinterpret_cast<const TW::Iocoin::OutPoint&>(inputs[subindex].previousOutput).encode(data);

    // Serialize the script
    if (subindex != index) {
        //encodeVarInt(0, data);
	    Bitcoin::Script s;
        s.encode(data);
    } else {
        scriptCode.encode(data);
    }

    // Serialize the nSequence
    auto hashNone = (hashType & 0x1f) == TWSignatureHashTypeNone;
    auto hashSingle = (hashType & 0x1f) == TWSignatureHashTypeSingle;
    if (subindex != index && (hashSingle || hashNone)) {
        encode32LE(0, data);
    } else {
    }
}

Proto::Transaction Transaction::proto() const {
    auto protoTx = Proto::Transaction();
    protoTx.set_version(version);
    //XXXX protoTx.set_time(nTime);

    for (const auto& input : inputs) {
        auto protoInput = protoTx.add_inputs();
        protoInput->mutable_previousoutput()->set_hash(input.previousOutput.hash.data(),
                                                       input.previousOutput.hash.size());
        protoInput->mutable_previousoutput()->set_index(input.previousOutput.index);
        protoInput->set_script(input.script.bytes.data(), input.script.bytes.size());
    }

    for (const auto& output : outputs) {
        auto protoOutput = protoTx.add_outputs();
        protoOutput->set_value(output.value);
        protoOutput->set_script(output.script.bytes.data(), output.script.bytes.size());
    }

    protoTx.set_locktime(lockTime);

    return protoTx;
}
