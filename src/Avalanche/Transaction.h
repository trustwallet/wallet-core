// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"
#include "TransferableInput.h"
#include "TransferableOutput.h"

namespace TW::Avalanche {

class BaseTransaction {
  public:
    uint32_t TypeID;
    uint32_t NetworkID;
    Data BlockchainID;
    std::vector<TransferableInput> Inputs;
    std::vector<TransferableOutput> Outputs;
    Data Memo;

    /// Encodes the BaseTransaction into the provided buffer.
    void encode(Data& data) const;

  protected:
    BaseTransaction(uint32_t typeID, uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo)
    : TypeID(typeID), NetworkID(networkID), BlockchainID(blockchainID), Inputs(inputs), Outputs(outputs), Memo(memo) {}

};

} // namespace TW::Avalanche
