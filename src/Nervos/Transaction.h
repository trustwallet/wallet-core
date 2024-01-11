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
#include "TransactionPlan.h"
#include "Witness.h"

#include "../Coin.h"
#include "../CoinEntry.h"
#include "Data.h"
#include "../Hash.h"
#include "../KeyPair.h"
#include "../PrivateKey.h"
#include "../PublicKey.h"
#include "../Result.h"

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

    // List of serialized witnesses
    std::vector<Data> serializedWitnesses;

    // List of cells selected for this transaction
    Cells selectedCells;

    Transaction() = default;

    Data hash() const;
    nlohmann::json json() const;
    void build(const TransactionPlan& txPlan);
    Common::Proto::SigningError sign(const std::vector<PrivateKey>& privateKeys);

private:
    std::vector<Data> m_groupNumToLockHash;
    std::vector<std::vector<int>> m_groupNumToInputIndices;
    std::vector<Witnesses> m_groupNumToWitnesses;

    void formGroups();
    Common::Proto::SigningError signGroups(const std::vector<PrivateKey>& privateKeys);
    Common::Proto::SigningError signWitnesses(const PrivateKey& privateKey, const Data& txHash,
                                              Witnesses& witnesses);
};

} // namespace TW::Nervos
