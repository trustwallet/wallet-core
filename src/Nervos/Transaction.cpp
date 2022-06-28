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
#include <TrustWalletCore/TWHRP.h>

#include <algorithm>
#include <utility>
#include <vector>

using namespace TW;
using namespace TW::Nervos;

Data Transaction::hash() {
    Data data;
    std::vector<Data> dataArray;
    dataArray.reserve(6);

    // version
    Data versionData;
    encode32LE(version, versionData);
    dataArray.emplace_back(versionData);

    // cell deps
    Data cellDepsData;
    encode32LE(uint32_t(cellDeps.size()), cellDepsData);
    for (auto&& cellDep : cellDeps) {
        cellDep.encode(cellDepsData);
    }
    dataArray.emplace_back(cellDepsData);

    // header deps
    Data headerDepsData;
    encode32LE(uint32_t(headerDeps.size()), headerDepsData);
    for (auto&& headerDep : headerDeps) {
        headerDepsData.insert(headerDepsData.end(), headerDep.begin(), headerDep.end());
    }
    dataArray.emplace_back(headerDepsData);

    // inputs
    Data inputsData;
    encode32LE(uint32_t(inputs.size()), inputsData);
    for (auto&& input : inputs) {
        input.encode(inputsData);
    }
    dataArray.emplace_back(inputsData);

    // outputs
    Data outputsData1;
    std::vector<Data> outputsData1Array;
    outputsData1Array.reserve(outputs.size());
    for (auto&& output : outputs) {
        Data outputData1;
        output.encode(outputData1);
        outputsData1Array.emplace_back(outputData1);
    }
    Serialization::encodeDataArray(outputsData1Array, outputsData1);
    dataArray.emplace_back(outputsData1);

    // outputs data
    Data outputsData2;
    std::vector<Data> outputsData2Array;
    outputsData2Array.reserve(outputsData.size());
    for (auto&& outputData : outputsData) {
        Data outputData2;
        encode32LE(uint32_t(outputData.size()), outputData2);
        outputData2.insert(outputData2.end(), outputData.begin(), outputData.end());
        outputsData2Array.emplace_back(outputData2);
    }
    Serialization::encodeDataArray(outputsData2Array, outputsData2);
    dataArray.emplace_back(outputsData2);

    Serialization::encodeDataArray(dataArray, data);

    return Hash::blake2b(data, 32, Constants::gHashPersonalization);
}

void Transaction::build(const TransactionPlan& txPlan) {
    cellDeps = txPlan.cellDeps;
    headerDeps = txPlan.headerDeps;
    selectedCells = txPlan.selectedCells;
    outputs = txPlan.outputs;
    outputsData = txPlan.outputsData;
    for (auto&& cell : selectedCells) {
        inputs.emplace_back(cell.outPoint, 0);
    }
}

Common::Proto::SigningError Transaction::sign(const std::vector<PrivateKey>& privateKeys) {
    formGroups();
    return signGroups(privateKeys);
}

void Transaction::formGroups() {
    for (auto index = 0; index < selectedCells.size(); index++) {
        auto&& cell = selectedCells[index];
        auto lockHash = cell.lock.hash();
        int groupNum = -1;
        for (auto groupNum1 = 0; groupNum1 < m_groupNumToLockHash.size(); groupNum1++) {
            if (lockHash == m_groupNumToLockHash[groupNum1]) {
                // Group found. Add to existing group.
                groupNum = groupNum1;
                break;
            }
        }
        if (groupNum == -1) {
            // Group not found. Create new group.
            groupNum = int(m_groupNumToLockHash.size());
            m_groupNumToLockHash.emplace_back(lockHash);
            m_groupNumToInputIndices.emplace_back();
        }
        m_groupNumToInputIndices[groupNum].emplace_back(index);
        witnesses.emplace_back();
    }
}

Common::Proto::SigningError Transaction::signGroups(const std::vector<PrivateKey>& privateKeys) {
    const Data txHash = hash();
    for (auto groupNum = 0; groupNum < m_groupNumToLockHash.size(); groupNum++) {
        auto&& cell = selectedCells[m_groupNumToInputIndices[groupNum][0]];
        const PrivateKey* privateKey = nullptr;
        for (auto&& privateKey1 : privateKeys) {
            auto publicKey1 = privateKey1.getPublicKey(TWPublicKeyTypeSECP256k1);
            auto address = Address(publicKey1, HRP_NERVOS);
            auto script = Script(address);
            if (script == cell.lock) {
                privateKey = &privateKey1;
                break;
            }
        }
        if (!privateKey) {
            return Common::Proto::Error_missing_private_key;
        }
        Witnesses witnessesForThisGroup(m_groupNumToInputIndices[groupNum].size());
        auto result = signWitnesses(*privateKey, txHash, witnessesForThisGroup);
        if (result != Common::Proto::OK) {
            return result;
        }
        Data witnessData;
        witnessesForThisGroup[0].encode(witnessData);
        witnesses[m_groupNumToInputIndices[groupNum][0]] = witnessData;
    }
    return Common::Proto::OK;
}

Common::Proto::SigningError Transaction::signWitnesses(const PrivateKey& privateKey,
                                                       const Data& txHash, Witnesses& witnesses) {
    Data message;
    message.insert(message.end(), txHash.begin(), txHash.end());

    witnesses[0].lock = Data(Constants::gBlankWitnessBytes, 0);

    for (auto&& witness : witnesses) {
        Data serializedWitness;
        witness.encode(serializedWitness);
        encode64LE(serializedWitness.size(), message);
        message.insert(message.end(), serializedWitness.begin(), serializedWitness.end());
    }

    auto messageHash = Hash::blake2b(message, 32, Constants::gHashPersonalization);
    auto signature = privateKey.sign(messageHash, TWCurveSECP256k1);
    if (signature.empty()) {
        // Error: Failed to sign
        return Common::Proto::Error_signing;
    }
    witnesses[0].lock = signature;

    return Common::Proto::OK;
}
