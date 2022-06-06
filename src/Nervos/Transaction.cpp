// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "CommonFunc.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>
#include <utility>

using namespace TW;
using namespace TW::Nervos;

// Above this number of Cells a simplified selection is used (optimization)
const auto SimpleModeLimit = 1000;

// The maximum number of Cells to consider.  Cells above this limit are cut off because it cak take
// very long
const size_t MaxCellsHardLimit = 3000;

uint64_t Transaction::sizeWithoutInputs() {
    uint64_t size = 0;
    size += 68 + 4;
    size += 37 * cellDeps.size();
    size += 32 * headerDeps.size();
    for (auto& output : outputs) {
        Data outputData1;
        output.encode(outputData1);
        size += outputData1.size() + 4;
    }
    for (auto& outputData : outputsData) {
        size += 4 + outputData.size() + 4;
    }
    return size;
}

uint64_t Transaction::sizeOfSingleInputAndWitness() {
    uint64_t size = 0;
    size += 44;
    Witness witness;
    witness.lock = Data(65, 0);
    Data witnessData;
    witness.encode(witnessData);
    size += 4 + witnessData.size() + 4;
    return size;
}

Data Transaction::hash() {
    Data data;
    std::vector<Data> dataArray;

    // version
    Data versionData;
    encode32LE(version, versionData);
    dataArray.push_back(versionData);

    // cell deps
    Data cellDepsData;
    encode32LE((uint32_t)cellDeps.size(), cellDepsData);
    for (auto& cellDep : cellDeps) {
        cellDep.encode(cellDepsData);
    }
    dataArray.push_back(cellDepsData);

    // header deps
    Data headerDepsData;
    encode32LE((uint32_t)headerDeps.size(), headerDepsData);
    for (auto& headerDep : headerDeps) {
        std::copy(std::begin(headerDep), std::end(headerDep), std::back_inserter(headerDepsData));
    }
    dataArray.push_back(headerDepsData);

    // inputs
    Data inputsData;
    encode32LE((uint32_t)inputs.size(), inputsData);
    for (auto& input : inputs) {
        input.encode(inputsData);
    }
    dataArray.push_back(inputsData);

    // outputs
    Data outputsData1;
    std::vector<Data> outputsData1Array;
    for (auto& output : outputs) {
        Data outputData1;
        output.encode(outputData1);
        outputsData1Array.push_back(outputData1);
    }
    CommonFunc::encodeDataArray(outputsData1Array, outputsData1);
    dataArray.push_back(outputsData1);

    // outputs data
    Data outputsData2;
    std::vector<Data> outputsData2Array;
    for (auto& outputData : outputsData) {
        Data outputData2;
        encode32LE((uint32_t)outputData.size(), outputData2);
        std::copy(std::begin(outputData), std::end(outputData), std::back_inserter(outputData2));
        outputsData2Array.push_back(outputData2);
    }
    CommonFunc::encodeDataArray(outputsData2Array, outputsData2);
    dataArray.push_back(outputsData2);

    CommonFunc::encodeDataArray(dataArray, data);

    return TW::Hash::blake2b(data, 32, CommonFunc::getHashPersonalization());
}

Common::Proto::SigningError Transaction::plan(const SigningInput& signingInput) {
    bool maxAmount = signingInput.useMaxAmount;

    if (signingInput.amount == 0 && !maxAmount) {
        return Common::Proto::Error_zero_amount_requested;
    }
    if (signingInput.cells.empty()) {
        return Common::Proto::Error_missing_input_utxos;
    }

    auto inputSelector = InputSelector(signingInput.cells);
    auto inputSum = InputSelector::sum(signingInput.cells);

    // if amount requested is the same or more than available amount, it cannot be satisifed, but
    // treat this case as MaxAmount, and send maximum available (which will be less)
    if (!maxAmount && signingInput.amount > inputSum) {
        return Common::Proto::Error_not_enough_utxos;
    }

    cellDeps.push_back(CommonFunc::getSecp256k1CellDep());

    outputs.push_back(
        CellOutput(signingInput.amount, Script(Address(signingInput.toAddress)), Script()));
    outputsData.push_back(Data());
    Cells selectedInputs;
    if (!maxAmount) {
        // output + change
        outputs.push_back(CellOutput(0, Script(Address(signingInput.changeAddress)), Script()));
        outputsData.push_back(Data());
        if (signingInput.cells.size() <= SimpleModeLimit &&
            signingInput.cells.size() <= MaxCellsHardLimit) {
            selectedInputs =
                inputSelector.select(outputs[0].capacity, sizeWithoutInputs(),
                                     sizeOfSingleInputAndWitness(), signingInput.byteFee);
        } else {
            selectedInputs =
                inputSelector.selectSimple(outputs[0].capacity, sizeWithoutInputs(),
                                           sizeOfSingleInputAndWitness(), signingInput.byteFee);
        }
    } else {
        // output, no change
        selectedInputs =
            inputSelector.selectMaxAmount(sizeOfSingleInputAndWitness(), signingInput.byteFee);
    }
    if (selectedInputs.size() <= MaxCellsHardLimit) {
        inputCells = selectedInputs;
    } else {
        // truncate to limit number of selected Cells
        inputCells.clear();
        for (auto i = 0; i < MaxCellsHardLimit; ++i) {
            inputCells.push_back(selectedInputs[i]);
        }
    }

    if (inputCells.size() == 0) {
        return Common::Proto::Error_not_enough_utxos;
    }

    int64_t availableAmount = InputSelector::sum(inputCells);

    // Compute fee.
    // must preliminary set change so that there is a second output
    if (!maxAmount) {
        if (signingInput.amount > availableAmount) {
            return Common::Proto::Error_general;
        }
        outputs[0].capacity = signingInput.amount;
    } else {
        outputs[0].capacity = availableAmount;
    }
    int64_t fee = (sizeWithoutInputs() + inputCells.size() * sizeOfSingleInputAndWitness()) *
                  signingInput.byteFee;
    // If fee is larger then availableAmount (can happen in special maxAmount case), we reduce it
    // (and hope it will go through)
    fee = std::min(availableAmount, fee);
    if (fee < 0 || fee > availableAmount) {
        return Common::Proto::Error_general;
    }

    // adjust/compute amount
    if (!maxAmount) {
        // reduce amount if needed
        outputs[0].capacity =
            std::max(int64_t(0), std::min(outputs[0].capacity, availableAmount - fee));
    } else {
        // max available amount
        outputs[0].capacity = std::max(int64_t(0), availableAmount - fee);
    }

    // compute change
    if (!maxAmount) {
        outputs[1].capacity = availableAmount - outputs[0].capacity - fee;
    }

    for (auto& cell : inputCells) {
        inputs.emplace_back(cell.outPoint, 0);
    }

    if ((outputs[0].capacity < 0) || (outputs[0].capacity > availableAmount)) {
        return Common::Proto::Error_general;
    }
    if (!maxAmount) {
        if ((outputs[1].capacity < 0) || (outputs[1].capacity > availableAmount)) {
            return Common::Proto::Error_general;
        }
    }
    return Common::Proto::OK;
}

Common::Proto::SigningError Transaction::sign(const std::vector<PrivateKey>& privateKeys) {
    const Data txHash = hash();
    auto p2pkh = TW::p2pkhPrefix(TWCoinTypeNervos);
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
            groupNum = (int)groupNumToLockHash.size();
            groupNumToLockHash.push_back(lockHash);
            groupNumToInputIndices.push_back(std::vector<int>());
        }
        groupNumToInputIndices[groupNum].push_back(index);
        inputIndexToGroupNum.push_back(groupNum);
    }
    int index = 0;
    for (auto groupNum = 0; groupNum < groupNumToLockHash.size(); groupNum++) {
        while (index < groupNumToInputIndices[groupNum][0]) {
            Witness emptyWitness;
            Data emptyWitnessData;
            emptyWitness.encode(emptyWitnessData);
            witnesses.push_back(emptyWitnessData);
            index++;
        }
        Witnesses witnesses;
        for (auto i = 0; i < groupNumToInputIndices[groupNum].size(); i++) {
            witnesses.push_back(Witness());
        }
        auto& cell = inputCells[index];
        const PrivateKey* privateKey = NULL;
        for (auto& privateKey1 : privateKeys) {
            auto publicKey1 = privateKey1.getPublicKey(TWPublicKeyTypeSECP256k1);
            auto address = Address(publicKey1, p2pkh, hrp);
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
        this->witnesses.push_back(witnessData);
    }
    return Common::Proto::OK;
}

Common::Proto::SigningError Transaction::signWitnesses(const PrivateKey& privateKey,
                                                       const Data& txHash, Witnesses& witnesses) {
    Data message;
    message.insert(message.end(), txHash.begin(), txHash.end());

    witnesses[0].lock = Data(65, 0);

    for (auto& witness : witnesses) {
        Data serializedWitness;
        witness.encode(serializedWitness);
        encode64LE(serializedWitness.size(), message);
        std::copy(std::begin(serializedWitness), std::end(serializedWitness),
                  std::back_inserter(message));
    }

    auto messageHash = TW::Hash::blake2b(message, 32, CommonFunc::getHashPersonalization());
    auto signature = privateKey.sign(messageHash, TWCurveSECP256k1);
    if (signature.empty()) {
        // Error: Failed to sign
        return Common::Proto::Error_signing;
    }
    witnesses[0].lock = signature;

    return Common::Proto::OK;
}
