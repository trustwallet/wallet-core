// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"
#include "TransferableInput.h"
#include "TransferableOutput.h"
#include "TransferableOp.h"
#include "InitialState.h"

namespace TW::Avalanche {

class BaseTransaction {
  public:
    uint32_t TypeID;
    uint32_t NetworkID;
    Data BlockchainID;
    std::vector<TransferableInput> Inputs;
    std::vector<TransferableOutput> Outputs;
    Data Memo;

    virtual void encode(Data& data) const;

  protected:
    BaseTransaction(uint32_t typeID, uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo)
    : TypeID(typeID), NetworkID(networkID), BlockchainID(blockchainID), Inputs(inputs), Outputs(outputs), Memo(memo) {}

    /// Encodes the BaseTransaction into the provided buffer.
    void baseEncode(Data& data) const;
};

class UnsignedCreateAssetTransaction : public BaseTransaction {
  public:
    std::string Name;
    std::string Symbol;
    uint8_t Denomination;
    std::vector<InitialState> InitialStates;

    void encode(Data& data) const;

    UnsignedCreateAssetTransaction(uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo, 
      std::string &name, std::string &symbol, uint8_t denomination, std::vector<InitialState> &states)
      : BaseTransaction(1, networkID, blockchainID, inputs, outputs, memo), 
      Name(name), Symbol(symbol), Denomination(denomination), InitialStates(states) {}
};

class UnsignedOperationTransaction : public BaseTransaction {
  public:
    std::vector<TransferableOp> Operations;

    void encode(Data& data) const;

    UnsignedOperationTransaction(uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo, 
      std::vector<TransferableOp> &ops)
      : BaseTransaction(2, networkID, blockchainID, inputs, outputs, memo), 
      Operations(ops) {}
};

class UnsignedImportTransaction : public BaseTransaction {
  public:
    Data SourceChain;
    std::vector<TransferableInput> ImportInputs;

    void encode(Data& data) const;

    UnsignedImportTransaction(uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo, 
      Data &source, std::vector<TransferableInput> &importInputs)
      : BaseTransaction(3, networkID, blockchainID, inputs, outputs, memo), 
      SourceChain(source), ImportInputs(inputs) {}
};

class UnsignedExportTransaction : public BaseTransaction {
  public:
    Data DestinationChain;
    std::vector<TransferableOutput> ExportOutputs;

    void encode(Data& data) const;

    UnsignedExportTransaction(uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo, 
      Data &dest, std::vector<TransferableOutput> &exportOutputs)
      : BaseTransaction(3, networkID, blockchainID, inputs, outputs, memo), 
      DestinationChain(dest), ExportOutputs(exportOutputs) {}
};

} // namespace TW::Avalanche
