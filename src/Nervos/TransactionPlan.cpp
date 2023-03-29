// Copyright © 2017-2023 Trust Wallet.
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
namespace TW::Nervos {

void TransactionPlan::plan(const Proto::SigningInput& signingInput) {
    error = Common::Proto::OK;

    m_byteFee = signingInput.byte_fee();

    if (signingInput.cell_size() == 0) {
        error = Common::Proto::Error_missing_input_utxos;
        return;
    }
    for (auto&& cell : signingInput.cell()) {
        m_availableCells.emplace_back(cell);
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
    case Proto::SigningInput::kDaoDeposit: {
        planDaoDeposit(signingInput);
        break;
    }
    case Proto::SigningInput::kDaoWithdrawPhase1: {
        planDaoWithdrawPhase1(signingInput);
        break;
    }
    case Proto::SigningInput::kDaoWithdrawPhase2: {
        planDaoWithdrawPhase2(signingInput);
        break;
    }
    default: {
        error = Common::Proto::Error_invalid_params;
    }
    }
}

void TransactionPlan::planNativeTransfer(const Proto::SigningInput& signingInput) {
    auto useMaxAmount = signingInput.native_transfer().use_max_amount();
    auto amount = signingInput.native_transfer().amount();
    if ((amount == 0) && !useMaxAmount) {
        error = Common::Proto::Error_zero_amount_requested;
        return;
    }

    cellDeps.emplace_back(Constants::gSecp256k1CellDep);

    outputs.emplace_back(amount, Script(Address(signingInput.native_transfer().to_address())),
                         Script());
    outputsData.emplace_back();

    if (useMaxAmount) {
        selectMaximumCapacity();
    } else {
        auto changeAddress = Address(signingInput.native_transfer().change_address());
        selectRequiredCapacity(changeAddress);
    }
}

void TransactionPlan::planSudtTransfer(const Proto::SigningInput& signingInput) {
    auto useMaxAmount = signingInput.sudt_transfer().use_max_amount();
    uint256_t amount = uint256_t(signingInput.sudt_transfer().amount());
    if ((amount == 0) && !useMaxAmount) {
        error = Common::Proto::Error_zero_amount_requested;
        return;
    }

    cellDeps.emplace_back(Constants::gSecp256k1CellDep);
    cellDeps.emplace_back(Constants::gSUDTCellDep);

    outputs.emplace_back(Constants::gMinCellCapacityForSUDT,
                         Script(Address(signingInput.sudt_transfer().to_address())),
                         Script(Constants::gSUDTCodeHash, HashType::Type1,
                                data(signingInput.sudt_transfer().sudt_address())));
    outputsData.emplace_back();

    auto changeAddress = Address(signingInput.sudt_transfer().change_address());
    selectSudtTokens(useMaxAmount, amount, changeAddress);
    selectRequiredCapacity(changeAddress);
}

void TransactionPlan::planDaoDeposit(const Proto::SigningInput& signingInput) {
    auto amount = signingInput.dao_deposit().amount();

    cellDeps.emplace_back(Constants::gSecp256k1CellDep);
    cellDeps.emplace_back(Constants::gDAOCellDep);

    outputs.emplace_back(amount, Script(Address(signingInput.dao_deposit().to_address())),
                         Script(Constants::gDAOCodeHash, HashType::Type1, Data()));
    outputsData.emplace_back();
    encode64LE(0, outputsData[outputsData.size() - 1]);

    auto changeAddress = Address(signingInput.dao_deposit().change_address());
    selectRequiredCapacity(changeAddress);
}

void TransactionPlan::planDaoWithdrawPhase1(const Proto::SigningInput& signingInput) {
    cellDeps.emplace_back(Constants::gSecp256k1CellDep);
    cellDeps.emplace_back(Constants::gDAOCellDep);

    auto depositCell = Cell(signingInput.dao_withdraw_phase1().deposit_cell());
    selectedCells.emplace_back(depositCell);
    m_availableCells.erase(std::remove_if(
        m_availableCells.begin(), m_availableCells.end(),
        [&depositCell](const Cell& cell) { return cell.outPoint == depositCell.outPoint; }));

    headerDeps.emplace_back(depositCell.blockHash);

    outputs.emplace_back(depositCell.capacity, Script(depositCell.lock), Script(depositCell.type));
    outputsData.emplace_back();
    encode64LE(depositCell.blockNumber, outputsData[outputsData.size() - 1]);

    auto changeAddress = Address(signingInput.dao_withdraw_phase1().change_address());
    selectRequiredCapacity(changeAddress);
}

void TransactionPlan::planDaoWithdrawPhase2(const Proto::SigningInput& signingInput) {
    cellDeps.emplace_back(Constants::gSecp256k1CellDep);
    cellDeps.emplace_back(Constants::gDAOCellDep);

    auto depositCell = Cell(signingInput.dao_withdraw_phase2().deposit_cell());
    auto withdrawingCell = Cell(signingInput.dao_withdraw_phase2().withdrawing_cell());
    selectedCells.emplace_back(withdrawingCell);
    encode64LE(0, selectedCells[selectedCells.size() - 1].inputType);

    headerDeps.emplace_back(depositCell.blockHash);
    headerDeps.emplace_back(withdrawingCell.blockHash);

    outputs.emplace_back(signingInput.dao_withdraw_phase2().amount(), Script(withdrawingCell.lock),
                         Script());
    outputsData.emplace_back();

    outputs[0].capacity -= calculateFee();
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
        fee += sizeOfSingleInputAndWitness(cell.inputType, cell.outputType) * m_byteFee;
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

void TransactionPlan::selectSudtTokens(const bool useMaxAmount, const uint256_t amount,
                                       const Address& changeAddress) {
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
        if (useMaxAmount) {
            // Transfer maximum available tokens
            gotEnough = true;
        } else if (selectedSudtAmount >= amount) {
            // Transfer exact number of tokens
            gotEnough = true;
            uint256_t changeValue = selectedSudtAmount - amount;
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
    outputsData[0] = Serialization::encodeUint256(useMaxAmount ? selectedSudtAmount : amount, 16);
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

uint64_t TransactionPlan::sizeOfSingleInputAndWitness(const Data& inputType,
                                                      const Data& outputType) {
    uint64_t size = Constants::gSingleInputAndWitnessBaseSize;
    auto witness = Witness(Data(Constants::gBlankWitnessBytes, 0), inputType, outputType);
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
    uint64_t size = sizeWithoutInputs();
    size += std::accumulate(selectedCells.begin(), selectedCells.end(), uint64_t(0),
                            [&](const uint64_t total, const Cell& cell) {
                                return total +
                                       sizeOfSingleInputAndWitness(cell.inputType, cell.outputType);
                            });
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

} // namespace TW::Nervos
