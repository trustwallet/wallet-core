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
#include "Credential.h"

namespace TW::Avalanche {

class BaseTransaction {
  public:
    uint32_t TypeID;
    uint32_t NetworkID;
    Data BlockchainID;
    std::vector<TransferableInput> Inputs;
    std::vector<TransferableOutput> Outputs;
    Data Memo;

    void encode(Data& data) const;

    BaseTransaction(uint32_t typeID, uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo)
    : TypeID(typeID), NetworkID(networkID), BlockchainID(blockchainID), Memo(memo) {
      //TODO guard rails and validation would be nice here; for example BlockchainID must be 32 bytes; assetID must be 4 bytes, networkID 4 bytes.
      Inputs = inputs;
      Outputs = outputs;
      std::sort(Inputs.begin(), Inputs.end());
      std::sort(Outputs.begin(), Outputs.end());
    }
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
      Name(name), Symbol(symbol), Denomination(denomination), InitialStates(states) {
        std::sort(InitialStates.begin(), InitialStates.end());
      }

    UnsignedCreateAssetTransaction(BaseTransaction &baseTxn, std::string &name, std::string &symbol, uint8_t denomination, std::vector<InitialState> &states)
      : BaseTransaction(baseTxn), 
      Name(name), Symbol(symbol), Denomination(denomination), InitialStates(states) {
        std::sort(InitialStates.begin(), InitialStates.end());
      }
};

class UnsignedOperationTransaction : public BaseTransaction {
  public:
    std::vector<TransferableOp> Operations;

    void encode(Data& data) const;

    UnsignedOperationTransaction(uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo, 
      std::vector<TransferableOp> &ops)
      : BaseTransaction(2, networkID, blockchainID, inputs, outputs, memo), 
      Operations(ops) {
        std::sort(Operations.begin(), Operations.end());
      }

    UnsignedOperationTransaction(BaseTransaction &baseTxn, std::vector<TransferableOp> &ops)
    : BaseTransaction(baseTxn), Operations(ops) {
      std::sort(Operations.begin(), Operations.end());
    }
};

class UnsignedImportTransaction : public BaseTransaction {
  public:
    Data SourceChain;
    std::vector<TransferableInput> ImportInputs;

    void encode(Data& data) const;

    UnsignedImportTransaction(uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo, 
      Data &source, std::vector<TransferableInput> &importInputs)
      : BaseTransaction(3, networkID, blockchainID, inputs, outputs, memo), 
      SourceChain(source), ImportInputs(importInputs) {
        std::sort(ImportInputs.begin(), ImportInputs.end());
      }

    UnsignedImportTransaction(BaseTransaction &baseTxn, Data &source, std::vector<TransferableInput> importInputs)
      : BaseTransaction(baseTxn), SourceChain(source), ImportInputs(importInputs) {
        std::sort(ImportInputs.begin(), ImportInputs.end());
      }
};

class UnsignedExportTransaction : public BaseTransaction {
  public:
    Data DestinationChain;
    std::vector<TransferableOutput> ExportOutputs;

    void encode(Data& data) const;

    UnsignedExportTransaction(uint32_t networkID, Data &blockchainID, std::vector<TransferableInput> &inputs, std::vector<TransferableOutput> &outputs, Data &memo, 
      Data &dest, std::vector<TransferableOutput> &exportOutputs)
      : BaseTransaction(3, networkID, blockchainID, inputs, outputs, memo), 
      DestinationChain(dest), ExportOutputs(exportOutputs) {
        std::sort(ExportOutputs.begin(), ExportOutputs.end());
      }

    UnsignedExportTransaction(BaseTransaction &baseTxn, Data &dest, std::vector<TransferableOutput> &exportOutputs)
      : BaseTransaction(baseTxn), DestinationChain(dest), ExportOutputs(exportOutputs) {
        std::sort(ExportOutputs.begin(), ExportOutputs.end());
      }
};

class SignedTransaction {
  public: 
    const uint16_t CodecID = 0; // TODO EJR x-chain is on codecID 1 now but docs are on 0
    BaseTransaction UnsignedTransaction;
    std::vector<Credential> Credentials;

    void encode(Data& data) const;

    SignedTransaction(BaseTransaction &txn, std::vector<Credential> &credentials)
    : UnsignedTransaction(txn), Credentials(credentials) {}    
};

} // namespace TW::Avalanche
