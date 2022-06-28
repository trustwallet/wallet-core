// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TransactionPlan.h"
#include "Constants.h"
#include "Serialization.h"
#include "Witness.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include <TrustWalletCore/TWCoinType.h>

#include <algorithm>
#include <utility>
#include <vector>

using namespace TW;
using namespace TW::Nervos;

void TransactionPlan::plan(const Proto::SigningInput& signingInput) {
    error = Common::Proto::OK;

    m_useMaxAmount = signingInput.use_max_amount();
    m_amount = Serialization::decodeUint256(data(signingInput.amount()));
    m_byteFee = signingInput.byte_fee();
    for (auto&& cell : signingInput.cell()) {
        m_availableCells.emplace_back(cell);
    }

    if ((m_amount == 0) && !m_useMaxAmount) {
        error = Common::Proto::Error_zero_amount_requested;
        return;
    }
    if (signingInput.cell_size() == 0) {
        error = Common::Proto::Error_missing_input_utxos;
        return;
    }

    switch (signingInput.operation_oneof_case()) {
    case Proto::SigningInput::kNativeTransfer: {
        planNativeTransfer(signingInput);
        break;
    }
    case Proto::SigningInput::kSudtTransfer: {
        planSudtTransfer(signingInput);
        break;
    }
    default: {
        error = Common::Proto::Error_invalid_params;
    }
    }
}

void TransactionPlan::planNativeTransfer(const Proto::SigningInput& signingInput) {
    cellDeps.emplace_back(Constants::gSecp256k1CellDep);

    outputs.emplace_back(uint64_t(m_amount),
                         Script(Address(signingInput.native_transfer().to_address())), Script());
    outputsData.emplace_back();

    if (m_useMaxAmount) {
        selectMaximumCapacity();
    } else {
        auto changeAddress = Address(signingInput.native_transfer().change_address());
        selectRequiredCapacity(changeAddress);
    }
}

void TransactionPlan::planSudtTransfer(const Proto::SigningInput& signingInput) {
    cellDeps.emplace_back(Constants::gSecp256k1CellDep);
    cellDeps.emplace_back(Constants::gSUDTCellDep);

    outputs.emplace_back(Constants::gMinCellCapacityForSUDT,
                         Script(Address(signingInput.sudt_transfer().to_address())),
                         Script(Constants::gSUDTCodeHash, HashType::Type1,
                                data(signingInput.sudt_transfer().sudt_address())));
    outputsData.emplace_back();

    auto changeAddress = Address(signingInput.sudt_transfer().change_address());
    selectSudtTokens(changeAddress);
    selectRequiredCapacity(changeAddress);
}

void TransactionPlan::selectMaximumCapacity() {
    uint64_t selectedCapacity =
        std::accumulate(m_availableCells.begin(), m_availableCells.end(), uint64_t(0),
                        [&](const uint64_t total, const Cell& cell) {
                            if (cell.type.empty()) {
                                selectedCells.emplace_back(cell);
                                return total + cell.capacity;
                            } else {
                                return total;
                            }
                        });
    uint64_t fee = calculateFee();
    outputs[0].capacity = selectedCapacity - fee;
}

void TransactionPlan::selectRequiredCapacity(const Address& changeAddress) {
    uint64_t requiredCapacity = getRequiredCapacity();
    uint64_t fee = calculateFee();
    uint64_t feeForChangeOutput = sizeOfSingleOutput(changeAddress) * m_byteFee;
    uint64_t selectedCapacity = getSelectedCapacity();
    uint64_t requiredCapacityPlusFees = requiredCapacity + fee + feeForChangeOutput;
    if (selectedCapacity >= requiredCapacityPlusFees + Constants::gMinCellCapacityForNativeToken) {
        outputs.emplace_back(selectedCapacity - requiredCapacityPlusFees, Script(changeAddress),
                             Script());
        outputsData.emplace_back();
        return;
    }
    sortAccordingToCapacity();
    bool gotEnough = false;
    for (auto&& cell : m_availableCells) {
        if (!cell.type.empty()) {
            continue;
        }
        selectedCells.emplace_back(cell);
        selectedCapacity += cell.capacity;
        fee += sizeOfSingleInputAndWitness() * m_byteFee;
        if (selectedCapacity >= requiredCapacity + fee) {
            gotEnough = true;
            uint64_t remainingCapacity = selectedCapacity - requiredCapacity - fee;
            if (remainingCapacity >=
                feeForChangeOutput + Constants::gMinCellCapacityForNativeToken) {
                // If change is enough, add it to the change address
                outputs.emplace_back(remainingCapacity - feeForChangeOutput, Script(changeAddress),
                                     Script());
                outputsData.emplace_back();
            } else {
                // If change is not enough, add it to the destination address
                outputs[outputs.size() - 1].capacity += remainingCapacity;
            }
            break;
        }
    }
    if (!gotEnough) {
        error = Common::Proto::Error_not_enough_utxos;
    }
}

void TransactionPlan::selectSudtTokens(const Address& changeAddress) {
    uint256_t selectedSudtAmount = 0;
    sortAccordingToTypeAndData(outputs[0].type);
    bool gotEnough = false;
    auto cell = m_availableCells.begin();
    while (cell != m_availableCells.end()) {
        if (cell->type != outputs[0].type) {
            cell++;
            continue;
        }
        selectedCells.emplace_back(*cell);
        selectedSudtAmount += Serialization::decodeUint256(cell->data);
        cell = m_availableCells.erase(cell);
        if (m_useMaxAmount) {
            // Transfer maximum available tokens
            gotEnough = true;
        } else if (selectedSudtAmount >= m_amount) {
            // Transfer exact number of tokens
            gotEnough = true;
            uint256_t changeValue = selectedSudtAmount - m_amount;
            if (changeValue > 0) {
                outputs.emplace_back(Constants::gMinCellCapacityForSUDT, Script(changeAddress),
                                     Script(outputs[0].type));
                outputsData.emplace_back(Serialization::encodeUint256(changeValue, 16));
            }
            break;
        }
    }
    if (!gotEnough) {
        error = Common::Proto::Error_not_enough_utxos;
        return;
    }
    outputsData[0] =
        Serialization::encodeUint256(m_useMaxAmount ? selectedSudtAmount : m_amount, 16);
}

uint64_t TransactionPlan::sizeWithoutInputs() {
    uint64_t size = Constants::gTransactionBaseSize;
    size += Constants::gCellDepSize * cellDeps.size();
    size += Constants::gHeaderDepSize * headerDeps.size();
    size += std::accumulate(outputs.begin(), outputs.end(), 0,
                            [](const uint64_t size, const CellOutput& output) {
                                Data outputData1;
                                output.encode(outputData1);
                                return size + outputData1.size() + Constants::gUint32Size;
                            });
    size += std::accumulate(
        outputsData.begin(), outputsData.end(), 0, [](const uint64_t size, const Data& outputData) {
            return size + Constants::gUint32Size + outputData.size() + Constants::gUint32Size;
        });
    return size;
}

uint64_t TransactionPlan::sizeOfSingleInputAndWitness() {
    uint64_t size = Constants::gSingleInputAndWitnessBaseSize;
    Witness witness;
    witness.lock = Data(Constants::gBlankWitnessBytes, 0);
    Data witnessData;
    witness.encode(witnessData);
    size += Constants::gUint32Size + witnessData.size() + Constants::gUint32Size;
    return size;
}

uint64_t TransactionPlan::sizeOfSingleOutput(const Address& address) {
    uint64_t size = 0;
    auto output = CellOutput(0, Script(address), Script());
    Data outputData1;
    output.encode(outputData1);
    size += outputData1.size() + Constants::gUint32Size;         // output
    size += Constants::gUint32Size + 0 + Constants::gUint32Size; // blank outputData
    return size;
}

uint64_t TransactionPlan::calculateFee() {
    uint64_t size = sizeWithoutInputs() + selectedCells.size() * sizeOfSingleInputAndWitness();
    return size * m_byteFee;
}

void TransactionPlan::sortAccordingToCapacity() {
    std::sort(m_availableCells.begin(), m_availableCells.end(),
              [](const Cell& lhs, const Cell& rhs) { return lhs.capacity < rhs.capacity; });
}

void TransactionPlan::sortAccordingToTypeAndData(const Script& type) {
    std::sort(
        m_availableCells.begin(), m_availableCells.end(), [&](const Cell& lhs, const Cell& rhs) {
            uint256_t lhsAmount = (lhs.type == type) ? Serialization::decodeUint256(lhs.data) : 0;
            uint256_t rhsAmount = (rhs.type == type) ? Serialization::decodeUint256(rhs.data) : 0;
            return lhsAmount < rhsAmount;
        });
}

uint64_t TransactionPlan::getRequiredCapacity() {
    return std::accumulate(outputs.begin(), outputs.end(), uint64_t(0),
                           [](const uint64_t total, const CellOutput& cellOutput) {
                               return total + cellOutput.capacity;
                           });
}

uint64_t TransactionPlan::getSelectedCapacity() {
    return std::accumulate(
        selectedCells.begin(), selectedCells.end(), uint64_t(0),
        [](const uint64_t total, const Cell& cell) { return total + cell.capacity; });
}
