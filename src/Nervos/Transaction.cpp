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
#include <vector>

using namespace TW;
using namespace TW::Nervos;

const uint64_t transactionBaseSize = 72;
const uint64_t cellDepSize = 37;
const uint64_t headerDepSize = 32;
const uint64_t singleInputAndWitnessBaseSize = 44;
const uint64_t blankWitnessBytes = 65;
const uint64_t uint32Size = 4;
const uint64_t minCellCapacityForNativeToken = 6100000000;
const uint64_t minCellCapacityForSUDT = 14400000000;

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
    if (signingInput.amount == 0 && !signingInput.useMaxAmount) {
        return Common::Proto::Error_zero_amount_requested;
    }
    if (signingInput.cells.empty()) {
        return Common::Proto::Error_missing_input_utxos;
    }

    cellDeps.emplace_back(Constants::getSecp256k1CellDep());
    outputsData.emplace_back();

    Common::Proto::SigningError result;
    if (signingInput.sudtAddress.empty()) {
        outputs.emplace_back(uint64_t(signingInput.useMaxAmount ? 0 : signingInput.amount),
                             Script(Address(signingInput.toAddress)), Script());
        result = planNativeTokenTransfer(signingInput.cells, Address(signingInput.changeAddress),
                                         signingInput.byteFee);
    } else {
        cellDeps.emplace_back(Constants::getSUDTCellDep());
        outputs.emplace_back(
            minCellCapacityForSUDT, Script(Address(signingInput.toAddress)),
            Script(Constants::getSUDTCodeHash(), HashType::Type1, signingInput.sudtAddress));
        result = planSudtTransfer(signingInput.useMaxAmount ? 0 : signingInput.amount,
                                  signingInput.cells, Address(signingInput.changeAddress),
                                  signingInput.byteFee);
    }
    if (result != Common::Proto::OK) {
        return result;
    }

    for (auto& cell : inputCells) {
        inputs.emplace_back(cell.outPoint, 0);
    }

    return Common::Proto::OK;
}

Common::Proto::SigningError Transaction::planNativeTokenTransfer(const Cells& signingInputCells,
                                                                 const Address& changeAddress,
                                                                 uint64_t byteFee) {
    uint64_t requiredAmount =
        std::accumulate(outputs.begin(), outputs.end(), uint64_t(0),
                        [](const uint64_t total, const CellOutput& cellOutput) {
                            return total + cellOutput.capacity;
                        });
    if (requiredAmount == 0) {
        // Transfer maximum available tokens
        uint64_t availableAmount =
            std::accumulate(signingInputCells.begin(), signingInputCells.end(), uint64_t(0),
                            [&](const uint64_t total, const Cell& cell) {
                                if (cell.type.empty()) {
                                    inputCells.emplace_back(std::move(cell));
                                    return total + cell.capacity;
                                } else {
                                    return total;
                                }
                            });
        uint64_t fee =
            (sizeWithoutInputs() + inputCells.size() * sizeOfSingleInputAndWitness()) * byteFee;
        outputs[outputs.size() - 1].capacity += availableAmount - fee;
    } else {
        // Transfer exact number of tokens
        auto sorted = signingInputCells;
        std::sort(sorted.begin(), sorted.end(),
                  [](const Cell& lhs, const Cell& rhs) { return lhs.capacity < rhs.capacity; });
        uint64_t fee =
            (sizeWithoutInputs() + inputCells.size() * sizeOfSingleInputAndWitness()) * byteFee;
        uint64_t feeForChangeOutput = sizeOfSingleOutput(changeAddress);
        uint64_t sum = std::accumulate(
            inputCells.begin(), inputCells.end(), uint64_t(0),
            [&](const uint64_t total, const Cell& cell) { return total + cell.capacity; });
        bool gotEnough = false;
        for (auto& cell : sorted) {
            if (!cell.type.empty()) {
                continue;
            }
            inputCells.emplace_back(std::move(cell));
            sum += cell.capacity;
            fee += sizeOfSingleInputAndWitness() * byteFee;
            if (sum >= requiredAmount + fee) {
                gotEnough = true;
                uint64_t remainingAmount = sum - requiredAmount - fee;
                if (remainingAmount >= feeForChangeOutput + minCellCapacityForNativeToken) {
                    // If change is enough, add it to the change address
                    outputs.emplace_back(remainingAmount - feeForChangeOutput,
                                         Script(changeAddress), Script());
                    outputsData.emplace_back();
                } else {
                    // If change is not enough, add it to the destination address
                    outputs[outputs.size() - 1].capacity += remainingAmount;
                }
                break;
            }
        }
        if (!gotEnough) {
            return Common::Proto::Error_not_enough_utxos;
        }
    }
    return Common::Proto::OK;
}

Common::Proto::SigningError Transaction::planSudtTransfer(uint256_t amount,
                                                          const Cells& signingInputCells,
                                                          const Address& changeAddress,
                                                          uint64_t byteFee) {
    uint64_t availableNativeAmount = 0;
    uint256_t availableSudtAmount = 0;
    Cells sorted;
    std::copy_if(signingInputCells.begin(), signingInputCells.end(), std::back_inserter(sorted),
                 [&](const Cell& cell) { return cell.type == outputs[0].type; });
    std::sort(sorted.begin(), sorted.end(), [&](const Cell& lhs, const Cell& rhs) {
        return Serialization::decodeUint256(lhs.data) < Serialization::decodeUint256(rhs.data);
    });
    bool gotEnough = false;
    for (auto& cell : sorted) {
        availableSudtAmount += Serialization::decodeUint256(cell.data);
        availableNativeAmount += cell.capacity;
        inputCells.emplace_back(std::move(cell));
        if (amount == 0) {
            // Transfer maximum available tokens
            gotEnough = true;
        } else if (availableSudtAmount >= amount) {
            // Transfer exact number of tokens
            gotEnough = true;
            uint256_t changeValue = availableSudtAmount - amount;
            if (changeValue > 0) {
                outputs.emplace_back(minCellCapacityForSUDT, Script(changeAddress),
                                     Script(outputs[0].type));
                outputsData.emplace_back(std::move(Serialization::encodeUint256(changeValue, 16)));
            }
            break;
        }
    }
    if (!gotEnough) {
        return Common::Proto::Error_not_enough_utxos;
    }
    outputsData[0] = Serialization::encodeUint256(amount == 0 ? availableSudtAmount : amount, 16);
    uint64_t fee = (sizeWithoutInputs() + inputCells.size() * sizeOfSingleInputAndWitness() +
                    sizeOfSingleOutput(changeAddress)) *
                   byteFee;
    uint64_t requiredNativeAmount =
        minCellCapacityForSUDT * outputs.size() + minCellCapacityForNativeToken + fee;
    if (availableNativeAmount >= requiredNativeAmount) {
        outputs.emplace_back(availableNativeAmount - requiredNativeAmount +
                                 minCellCapacityForNativeToken,
                             Script(changeAddress), Script());
        outputsData.emplace_back();
        return Common::Proto::OK;
    } else {
        Cells remainingCells;
        std::copy_if(signingInputCells.begin(), signingInputCells.end(),
                     std::back_inserter(remainingCells), [&](const Cell& cell1) {
                         return !std::any_of(
                             inputCells.begin(), inputCells.end(),
                             [&](const Cell& cell2) { return cell1.outPoint == cell2.outPoint; });
                     });
        return planNativeTokenTransfer(remainingCells, changeAddress, byteFee);
    }
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
        witnesses.emplace_back();
    }
    for (auto groupNum = 0; groupNum < groupNumToLockHash.size(); groupNum++) {
        auto& cell = inputCells[groupNumToInputIndices[groupNum][0]];
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
        Witnesses witnessesForThisGroup(groupNumToInputIndices[groupNum].size());
        auto result = signWitnesses(*privateKey, txHash, witnessesForThisGroup);
        if (result != Common::Proto::OK) {
            return result;
        }
        Data witnessData;
        witnessesForThisGroup[0].encode(witnessData);
        witnesses[groupNumToInputIndices[groupNum][0]] = witnessData;
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
