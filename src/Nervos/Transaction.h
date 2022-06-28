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
#include "Script.h"
#include "TransactionPlan.h"
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

class Transaction {
public:
    /// Transaction data format version (note, this is signed)
    int32_t version = 0;

    // List of cell deps
    CellDeps cellDeps;

    // List of header deps
    HeaderDeps headerDeps;

    // List of cell inputs
    CellInputs inputs;

    // List of cell outputs
    CellOutputs outputs;

    // List of outputs data
    std::vector<Data> outputsData;

    // List of witnesses
    std::vector<Data> witnesses;

    // List of cells selected for this transaction
    Cells selectedCells;

    Transaction() = default;

    /// Initializes a transaction from a Protobuf transaction.
    Transaction(const Proto::Transaction& tx) {
        version = tx.version();
        cellDeps.reserve(tx.cell_deps_size());
        for (auto&& cellDep : tx.cell_deps()) {
            cellDeps.emplace_back(cellDep);
        }
        headerDeps.reserve(tx.header_deps_size());
        for (auto&& headerDep : tx.header_deps()) {
            Data data;
            data.insert(data.end(), headerDep.begin(), headerDep.end());
            headerDeps.emplace_back(data);
        }
        inputs.reserve(tx.inputs_size());
        for (auto&& input : tx.inputs()) {
            inputs.emplace_back(input);
        }
        outputs.reserve(tx.outputs_size());
        for (auto&& output : tx.outputs()) {
            outputs.emplace_back(output);
        }
        outputsData.reserve(tx.outputs_data_size());
        for (auto&& outputData : tx.outputs_data()) {
            Data data;
            data.insert(data.end(), outputData.begin(), outputData.end());
            outputsData.emplace_back(data);
        }
        witnesses.reserve(tx.witnesses_size());
        for (auto&& witness : tx.witnesses()) {
            Data data;
            data.insert(data.end(), witness.begin(), witness.end());
            witnesses.emplace_back(data);
        }
    }

    /// Converts to Protobuf model
    Proto::Transaction proto() const {
        auto tx = Proto::Transaction();
        tx.set_version(version);
        for (auto&& cellDep : cellDeps) {
            *tx.add_cell_deps() = cellDep.proto();
        }
        for (auto&& headerDep : headerDeps) {
            tx.add_header_deps(headerDep.data(), headerDep.size());
        }
        for (auto&& input : inputs) {
            *tx.add_inputs() = input.proto();
        }
        for (auto&& output : outputs) {
            *tx.add_outputs() = output.proto();
        }
        for (auto&& outputData : outputsData) {
            tx.add_outputs_data(outputData.data(), outputData.size());
        }
        for (auto&& witness : witnesses) {
            tx.add_witnesses(witness.data(), witness.size());
        }
        return tx;
    }

    Data hash();
    void build(const TransactionPlan& txPlan);
    Common::Proto::SigningError sign(const std::vector<PrivateKey>& privateKeys);

private:
    std::vector<Data> m_groupNumToLockHash;
    std::vector<std::vector<int>> m_groupNumToInputIndices;

    void formGroups();
    Common::Proto::SigningError signGroups(const std::vector<PrivateKey>& privateKeys);
    Common::Proto::SigningError signWitnesses(const PrivateKey& privateKey, const Data& txHash,
                                              Witnesses& witnesses);
};

} // namespace TW::Nervos
