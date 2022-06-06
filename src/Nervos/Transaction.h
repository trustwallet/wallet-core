// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "Cell.h"
#include "CellDep.h"
#include "CellInput.h"
#include "CellOutput.h"
#include "HeaderDep.h"
#include "InputSelector.h"
#include "Script.h"
#include "SignatureVersion.h"
#include "SigningInput.h"
#include "Transaction.h"
#include "Witness.h"

#include "../Coin.h"
#include "../CoinEntry.h"
#include "../Data.h"
#include "../Hash.h"
#include "../KeyPair.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../Result.h"
#include "../proto/Nervos.pb.h"

#include <vector>

namespace TW::Nervos {

struct Transaction {
  public:
    /// Transaction data format version (note, this is signed)
    int32_t version = 0;

    // List of cell deps
    CellDeps cellDeps;

    // List of header deps
    HeaderDeps headerDeps;

    // List of inputs in the form of Cell
    Cells inputCells;

    // List of inputs in the form of OutPoint
    CellInputs inputs;

    // List of witnesses
    std::vector<Data> witnesses;

    // List of cell outputs
    CellOutputs outputs;

    // List of outputs data
    std::vector<Data> outputsData;

    Transaction() = default;

    Transaction(int32_t version)
        : version(version)
        , cellDeps()
        , headerDeps()
        , inputCells()
        , inputs()
        , witnesses()
        , outputs()
        , outputsData() {}

    /// Initializes a transaction from a Protobuf transaction.
    Transaction(const Proto::Transaction& tx) {
        version = tx.version();
        for (auto cellDep : tx.cell_deps()) {
            cellDeps.push_back(CellDep(cellDep));
        }
        for (auto headerDep : tx.header_deps()) {
            Data data;
            std::copy(headerDep.begin(), headerDep.end(), std::back_inserter(data));
            headerDeps.push_back(data);
        }
        for (auto inputCell : tx.input_cells()) {
            inputCells.push_back(Cell(inputCell));
        }
        for (auto input : tx.inputs()) {
            inputs.push_back(CellInput(input));
        }
        for (auto witness : tx.witnesses()) {
            Data data;
            std::copy(witness.begin(), witness.end(), std::back_inserter(data));
            outputsData.push_back(data);
        }
        for (auto output : tx.outputs()) {
            outputs.push_back(CellOutput(output));
        }
        for (auto outputData : tx.outputs_data()) {
            Data data;
            std::copy(outputData.begin(), outputData.end(), std::back_inserter(data));
            outputsData.push_back(data);
        }
    }

    /// Converts to Protobuf model
    Proto::Transaction proto() const {
        auto tx = Proto::Transaction();
        tx.set_version(version);
        for (const auto& cellDep : cellDeps) {
            *tx.add_cell_deps() = cellDep.proto();
        }
        for (const auto& headerDep : headerDeps) {
            tx.add_header_deps(headerDep.data(), headerDep.size());
        }
        for (const auto& inputCell : inputCells) {
            *tx.add_input_cells() = inputCell.proto();
        }
        for (const auto& input : inputs) {
            *tx.add_inputs() = input.proto();
        }
        for (const auto& output : outputs) {
            *tx.add_outputs() = output.proto();
        }
        for (const auto& outputData : outputsData) {
            tx.add_outputs_data(outputData.data(), outputData.size());
        }
        for (const auto& witness : witnesses) {
            tx.add_witnesses(witness.data(), witness.size());
        }
        return tx;
    }

    uint64_t sizeWithoutInputs();
    uint64_t sizeOfSingleInputAndWitness();
    Data hash();
    Common::Proto::SigningError plan(const SigningInput& signingInput);
    Common::Proto::SigningError sign(const std::vector<PrivateKey>& privateKeys);

  private:
    Common::Proto::SigningError signWitnesses(const PrivateKey& privateKey, const Data& txHash,
                                              Witnesses& witnesses);
};

} // namespace TW::Nervos
