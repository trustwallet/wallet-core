// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Cell.h"
#include "CellDep.h"
#include "CellInput.h"
#include "CellOutput.h"
#include "HeaderDep.h"
#include "Script.h"

#include "../Coin.h"
#include "../CoinEntry.h"
#include "Data.h"
#include "../Hash.h"
#include "../KeyPair.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../Result.h"
#include "../proto/Nervos.pb.h"

#include <vector>

namespace TW::Nervos {

class TransactionPlan {
public:
    // List of cell deps
    CellDeps cellDeps;

    // List of header deps
    HeaderDeps headerDeps;

    // List of cells selected for this transaction
    Cells selectedCells;

    // List of cell outputs
    CellOutputs outputs;

    // List of outputs data
    std::vector<Data> outputsData;

    // Error during transaction planning
    Common::Proto::SigningError error;

    TransactionPlan() = default;

    /// Initializes a transaction from a Protobuf transaction.
    TransactionPlan(const Proto::TransactionPlan& txPlan) {
        for (auto&& cellDep : txPlan.cell_deps()) {
            cellDeps.emplace_back(cellDep);
        }
        for (auto&& headerDep : txPlan.header_deps()) {
            Data data;
            data.insert(data.end(), headerDep.begin(), headerDep.end());
            headerDeps.emplace_back(data);
        }
        for (auto&& cell : txPlan.selected_cells()) {
            selectedCells.emplace_back(cell);
        }
        for (auto&& output : txPlan.outputs()) {
            outputs.emplace_back(output);
        }
        for (auto&& outputData : txPlan.outputs_data()) {
            Data data;
            data.insert(data.end(), outputData.begin(), outputData.end());
            outputsData.emplace_back(data);
        }
        error = txPlan.error();
    }

    /// Converts to Protobuf model
    Proto::TransactionPlan proto() const {
        auto txPlan = Proto::TransactionPlan();
        for (auto&& cellDep : cellDeps) {
            *txPlan.add_cell_deps() = cellDep.proto();
        }
        for (auto&& headerDep : headerDeps) {
            txPlan.add_header_deps(headerDep.data(), headerDep.size());
        }
        for (auto&& cell : selectedCells) {
            *txPlan.add_selected_cells() = cell.proto();
        }
        for (auto&& output : outputs) {
            *txPlan.add_outputs() = output.proto();
        }
        for (auto&& outputData : outputsData) {
            txPlan.add_outputs_data(outputData.data(), outputData.size());
        }
        return txPlan;
    }

    void plan(const Proto::SigningInput& signingInput);

private:
    uint64_t m_byteFee;
    Cells m_availableCells;

    void planNativeTransfer(const Proto::SigningInput& signingInput);
    void planSudtTransfer(const Proto::SigningInput& signingInput);
    void planDaoDeposit(const Proto::SigningInput& signingInput);
    void planDaoWithdrawPhase1(const Proto::SigningInput& signingInput);
    void planDaoWithdrawPhase2(const Proto::SigningInput& signingInput);
    void selectMaximumCapacity();
    void selectRequiredCapacity(const Address& changeAddress);
    void selectSudtTokens(const bool useMaxAmount, const uint256_t amount,
                          const Address& changeAddress);
    uint64_t sizeWithoutInputs();
    uint64_t sizeOfSingleInputAndWitness(const Data& inputType, const Data& outputType);
    uint64_t sizeOfSingleOutput(const Address& address);
    uint64_t calculateFee();
    void sortAccordingToCapacity();
    void sortAccordingToTypeAndData(const Script& type);
    uint64_t getRequiredCapacity();
    uint64_t getSelectedCapacity();
};

} // namespace TW::Nervos
