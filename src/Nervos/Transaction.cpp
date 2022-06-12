// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "Constants.h"
#include "Serialization.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>
#include <utility>

using namespace TW;
using namespace TW::Nervos;

const uint64_t transactionBaseSize = 72;
const uint64_t cellDepSize = 37;
const uint64_t headerDepSize = 32;
const uint64_t singleInputAndWitnessBaseSize = 44;
const uint64_t blankWitnessBytes = 65;
const uint64_t uint32Size = 4;
const int64_t minChangeValue = 6100000000;

uint64_t Transaction::sizeWithoutInputs() {
    uint64_t size = transactionBaseSize;
    size += cellDepSize * cellDeps.size();
    size += headerDepSize * headerDeps.size();
    for (auto& output : outputs) {
        Data outputData1;
        output.encode(outputData1);
        size += outputData1.size() + uint32Size;
    }
    for (auto& outputData : outputsData) {
        size += uint32Size + outputData.size() + uint32Size;
    }
    return size;
}

uint64_t Transaction::sizeOfSingleInputAndWitness() {
    uint64_t size = singleInputAndWitnessBaseSize;
    Witness witness;
    witness.lock = Data(blankWitnessBytes, 0);
    Data witnessData;
    witness.encode(witnessData);
    size += uint32Size + witnessData.size() + uint32Size;
    return size;
}

uint64_t Transaction::sizeOfSingleOutput(const Address& address) {
    uint64_t size = 0;
    auto output = CellOutput(0, Script(address), Script());
    Data outputData1;
    output.encode(outputData1);
    size += outputData1.size() + uint32Size; // output
    size += uint32Size + 0 + uint32Size;     // blank outputData
    return size;
}

Data Transaction::hash() {
    Data data;
    std::vector<Data> dataArray;

    // version
    Data versionData;
    encode32LE(version, versionData);
    dataArray.emplace_back(versionData);

    // cell deps
    Data cellDepsData;
    encode32LE(uint32_t(cellDeps.size()), cellDepsData);
    for (auto& cellDep : cellDeps) {
        cellDep.encode(cellDepsData);
    }
    dataArray.emplace_back(cellDepsData);

    // header deps
    Data headerDepsData;
    encode32LE(uint32_t(headerDeps.size()), headerDepsData);
    for (auto& headerDep : headerDeps) {
        headerDepsData.insert(headerDepsData.end(), headerDep.begin(), headerDep.end());
    }
    dataArray.emplace_back(headerDepsData);

    // inputs
    Data inputsData;
    encode32LE(uint32_t(inputs.size()), inputsData);
    for (auto& input : inputs) {
        input.encode(inputsData);
    }
    dataArray.emplace_back(inputsData);

    // outputs
    Data outputsData1;
    std::vector<Data> outputsData1Array;
    for (auto& output : outputs) {
        Data outputData1;
        output.encode(outputData1);
        outputsData1Array.emplace_back(outputData1);
    }
    Serialization::encodeDataArray(outputsData1Array, outputsData1);
    dataArray.emplace_back(outputsData1);

    // outputs data
    Data outputsData2;
    std::vector<Data> outputsData2Array;
    for (auto& outputData : outputsData) {
        Data outputData2;
        encode32LE(uint32_t(outputData.size()), outputData2);
        outputData2.insert(outputData2.end(), outputData.begin(), outputData.end());
        outputsData2Array.emplace_back(outputData2);
    }
    Serialization::encodeDataArray(outputsData2Array, outputsData2);
    dataArray.emplace_back(outputsData2);

    Serialization::encodeDataArray(dataArray, data);

    return TW::Hash::blake2b(data, 32, Constants::getHashPersonalization());
}

Common::Proto::SigningError Transaction::plan(const SigningInput& signingInput) {
    bool maxAmount = signingInput.useMaxAmount;

    if (signingInput.amount == 0 && !maxAmount) {
        return Common::Proto::Error_zero_amount_requested;
    }
    if (signingInput.cells.empty()) {
        return Common::Proto::Error_missing_input_utxos;
    }

    cellDeps.emplace_back(Constants::getSecp256k1CellDep());

    outputs.emplace_back(signingInput.amount, Script(Address(signingInput.toAddress)), Script());
    outputsData.emplace_back();

    if (maxAmount) {
        inputCells = signingInput.cells;
        int64_t availableAmount = std::accumulate(
            inputCells.begin(), inputCells.end(), int64_t(0),
            [](const int64_t total, const Cell& cell) { return total + cell.capacity; });
        int64_t fee = (sizeWithoutInputs() + inputCells.size() * sizeOfSingleInputAndWitness()) *
                      signingInput.byteFee;
        outputs[0].capacity = availableAmount - fee;
    } else {
        auto sorted = signingInput.cells;
        std::sort(sorted.begin(), sorted.end(),
                  [](const Cell& lhs, const Cell& rhs) { return lhs.capacity < rhs.capacity; });
        int64_t targetValue = outputs[0].capacity + sizeWithoutInputs() * signingInput.byteFee;
        int64_t feeForChangeOutput = sizeOfSingleOutput(Address(signingInput.changeAddress));
        int64_t sum = 0;
        bool gotEnough = false;
        for (auto& cell : sorted) {
            inputCells.emplace_back(std::move(cell));
            sum += cell.capacity;
            targetValue += sizeOfSingleInputAndWitness() * signingInput.byteFee;
            if (sum >= targetValue) {
                gotEnough = true;
                int64_t changeValue = sum - targetValue - feeForChangeOutput;
                if (changeValue >= minChangeValue) {
                    // If change is enough, add it to the change address
                    outputs.emplace_back(changeValue, Script(Address(signingInput.changeAddress)),
                                         Script());
                    outputsData.emplace_back();
                } else {
                    // If change is not enough, add it to the destination address
                    outputs[0].capacity += sum - targetValue;
                }
                break;
            }
        }
        if (!gotEnough) {
            return Common::Proto::Error_not_enough_utxos;
        }
    }

    for (auto& cell : inputCells) {
        inputs.emplace_back(cell.outPoint, 0);
    }

    return Common::Proto::OK;
}

Common::Proto::SigningError Transaction::sign(const std::vector<PrivateKey>& privateKeys) {
    const Data txHash = hash();
    const auto* hrp = stringForHRP(TW::hrp(TWCoinTypeNervos));
    std::vector<int> inputIndexToGroupNum;
    std::vector<Data> groupNumToLockHash;
    std::vector<std::vector<int>> groupNumToInputIndices;
    for (auto index = 0; index < inputs.size(); index++) {
        auto& cell = inputCells[index];
        auto lockHash = cell.lock.hash();
        int groupNum = -1;
        for (auto groupNum1 = 0; groupNum1 < groupNumToLockHash.size(); groupNum1++) {
            if (lockHash == groupNumToLockHash[groupNum1]) {
                groupNum = groupNum1;
                break;
            }
        }
        if (groupNum == -1) {
            groupNum = int(groupNumToLockHash.size());
            groupNumToLockHash.emplace_back(lockHash);
            groupNumToInputIndices.emplace_back();
        }
        groupNumToInputIndices[groupNum].emplace_back(index);
        inputIndexToGroupNum.emplace_back(groupNum);
    }
    for (auto groupNum = 0; groupNum < groupNumToLockHash.size(); groupNum++) {
        while (this->witnesses.size() < groupNumToInputIndices[groupNum][0]) {
            Witness emptyWitness;
            Data emptyWitnessData;
            emptyWitness.encode(emptyWitnessData);
            this->witnesses.emplace_back(emptyWitnessData);
        }
        Witnesses witnesses;
        for (auto i = 0; i < groupNumToInputIndices[groupNum].size(); i++) {
            witnesses.emplace_back();
        }
        auto& cell = inputCells[this->witnesses.size()];
        const PrivateKey* privateKey = nullptr;
        for (auto& privateKey1 : privateKeys) {
            auto publicKey1 = privateKey1.getPublicKey(TWPublicKeyTypeSECP256k1);
            auto address = Address(publicKey1, hrp);
            auto script = Script(address);
            if (script == cell.lock) {
                privateKey = &privateKey1;
                break;
            }
        }
        if (!privateKey) {
            return Common::Proto::Error_missing_private_key;
        }
        auto result = signWitnesses(*privateKey, txHash, witnesses);
        if (result != Common::Proto::OK) {
            return result;
        }
        Data witnessData;
        witnesses[0].encode(witnessData);
        this->witnesses.emplace_back(witnessData);
    }
    return Common::Proto::OK;
}

Common::Proto::SigningError Transaction::signWitnesses(const PrivateKey& privateKey,
                                                       const Data& txHash, Witnesses& witnesses) {
    Data message;
    message.insert(message.end(), txHash.begin(), txHash.end());

    witnesses[0].lock = Data(blankWitnessBytes, 0);

    for (auto& witness : witnesses) {
        Data serializedWitness;
        witness.encode(serializedWitness);
        encode64LE(serializedWitness.size(), message);
        message.insert(message.end(), serializedWitness.begin(), serializedWitness.end());
    }

    auto messageHash = TW::Hash::blake2b(message, 32, Constants::getHashPersonalization());
    auto signature = privateKey.sign(messageHash, TWCurveSECP256k1);
    if (signature.empty()) {
        // Error: Failed to sign
        return Common::Proto::Error_signing;
    }
    witnesses[0].lock = signature;

    return Common::Proto::OK;
}
