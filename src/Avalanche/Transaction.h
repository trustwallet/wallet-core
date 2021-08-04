// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Credential.h"
#include "TransferableInput.h"
#include "TransferableOp.h"
#include "TransferableOutput.h"
#include "../CoinEntry.h"
#include <stdexcept>

namespace TW::Avalanche {

const int BLOCKCHAIN_ID_SIZE = 32;
const int MAX_MEMO_SIZE = 256;
const int MAX_ASSET_NAME_CHARS = 128;
const int MAX_SYMBOL_CHARS = 4;

enum TransactionTypeID {
  Base = 0,
  CreateAsset = 1,
  Operation = 2, 
  Import = 3,
  Export = 4
};

class BaseTransaction {
  public:
    uint32_t TypeID;
    uint32_t NetworkID;
    Data BlockchainID;
    std::vector<TransferableInput> Inputs;
    std::vector<TransferableOutput> Outputs;
    Data Memo;

    void encode(Data& data) const;

    BaseTransaction(uint32_t typeID, uint32_t networkID, Data& blockchainID,
                    std::vector<TransferableInput>& inputs,
                    std::vector<TransferableOutput>& outputs, Data& memo)
      : TypeID(typeID), NetworkID(networkID), BlockchainID(blockchainID), Memo(memo) {
      if (blockchainID.size() != BLOCKCHAIN_ID_SIZE) {
        throw std::invalid_argument(std::string("BlockchainID must be ") +
                                    std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
      }
      if (memo.size() > MAX_MEMO_SIZE) {
        throw std::invalid_argument(std::string("Memo must be no longer than ") +
                                    std::to_string(MAX_MEMO_SIZE) + " bytes.");
      }
      Inputs = inputs;
      Outputs = outputs;
      std::sort(Inputs.begin(), Inputs.end());
      std::sort(Outputs.begin(), Outputs.end());
    }
};

class UnsignedOperationTransaction : public BaseTransaction {
  public:
    std::vector<TransferableOp> Operations;

    void encode(Data& data) const;

    UnsignedOperationTransaction(uint32_t networkID, Data& blockchainID,
                                 std::vector<TransferableInput>& inputs,
                                 std::vector<TransferableOutput>& outputs, Data& memo,
                                 std::vector<TransferableOp>& ops)
      : BaseTransaction(TransactionTypeID::Operation, networkID, blockchainID, inputs, outputs, memo), Operations(ops) {
      std::sort(Operations.begin(), Operations.end());
    }

    UnsignedOperationTransaction(BaseTransaction& baseTxn, std::vector<TransferableOp>& ops)
      : BaseTransaction(baseTxn), Operations(ops) {
      std::sort(Operations.begin(), Operations.end());
      TypeID = TransactionTypeID::Operation;
    }
};

class UnsignedImportTransaction : public BaseTransaction {
  public:
    Data SourceChain;
    std::vector<TransferableInput> ImportInputs;

    void encode(Data& data) const;

    UnsignedImportTransaction(uint32_t networkID, Data& blockchainID,
                              std::vector<TransferableInput>& inputs,
                              std::vector<TransferableOutput>& outputs, Data& memo, Data& source,
                              std::vector<TransferableInput>& importInputs)
      : BaseTransaction(TransactionTypeID::Import, networkID, blockchainID, inputs, outputs, memo)
      , SourceChain(source)
      , ImportInputs(importInputs) {
      if (source.size() != BLOCKCHAIN_ID_SIZE) {
        throw std::invalid_argument(std::string("SourceChain must be ") +
                                    std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
      }
      std::sort(ImportInputs.begin(), ImportInputs.end());
    }

    UnsignedImportTransaction(BaseTransaction& baseTxn, Data& source,
                              std::vector<TransferableInput> importInputs)
      : BaseTransaction(baseTxn), SourceChain(source), ImportInputs(importInputs) {
      if (source.size() != BLOCKCHAIN_ID_SIZE) {
        throw std::invalid_argument(std::string("SourceChain must be ") +
                                    std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
      }
      TypeID = TransactionTypeID::Import;
      std::sort(ImportInputs.begin(), ImportInputs.end());
    }
};

class UnsignedExportTransaction : public BaseTransaction {
  public:
    Data DestinationChain;
    std::vector<TransferableOutput> ExportOutputs;

    void encode(Data& data) const;

    UnsignedExportTransaction(uint32_t networkID, Data& blockchainID,
                              std::vector<TransferableInput>& inputs,
                              std::vector<TransferableOutput>& outputs, Data& memo, Data& dest,
                              std::vector<TransferableOutput>& exportOutputs)
      : BaseTransaction(TransactionTypeID::Export, networkID, blockchainID, inputs, outputs, memo)
      , DestinationChain(dest)
      , ExportOutputs(exportOutputs) {
      if (dest.size() != BLOCKCHAIN_ID_SIZE) {
        throw std::invalid_argument(std::string("DestinationChain must be ") +
                                    std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
      }
      std::sort(ExportOutputs.begin(), ExportOutputs.end());
    }

    UnsignedExportTransaction(BaseTransaction& baseTxn, Data& dest,
                              std::vector<TransferableOutput>& exportOutputs)
      : BaseTransaction(baseTxn), DestinationChain(dest), ExportOutputs(exportOutputs) {
      if (dest.size() != BLOCKCHAIN_ID_SIZE) {
        throw std::invalid_argument(std::string("DestinationChain must be ") +
                                    std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
      }
      TypeID = TransactionTypeID::Export;
      std::sort(ExportOutputs.begin(), ExportOutputs.end());
    }
};

class SignedTransaction {
  public:
    const uint16_t CodecID = 0;
    BaseTransaction UnsignedTransaction;
    std::vector<Credential> Credentials;

    void encode(Data& data) const;

    SignedTransaction(BaseTransaction& txn, std::vector<Credential>& credentials)
      : UnsignedTransaction(txn), Credentials(credentials) {}
};

} // namespace TW::Avalanche
