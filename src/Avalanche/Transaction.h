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

// clang-format off

enum TransactionTypeID {
  Base = 0,
  CreateAsset = 1,
  Operation = 2, 
  Import = 3,
  Export = 4
};

// clang-format on

class BaseTransaction {
  public:
    uint32_t typeID;
    uint32_t networkID;
    Data blockchainID;
    std::vector<TransferableInput> inputs;
    std::vector<TransferableOutput> outputs;
    Data memo;

    void encode(Data& data) const;

    BaseTransaction(uint32_t typeID, uint32_t networkID, Data& blockchainID,
                    std::vector<TransferableInput>& inputs,
                    std::vector<TransferableOutput>& outputs, Data& memo)
        : typeID(typeID), networkID(networkID), blockchainID(blockchainID), memo(memo) {
        if (blockchainID.size() != BLOCKCHAIN_ID_SIZE) {
            throw std::invalid_argument(std::string("BlockchainID must be ") +
                                        std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
        }
        if (memo.size() > MAX_MEMO_SIZE) {
            throw std::invalid_argument(std::string("Memo must be no longer than ") +
                                        std::to_string(MAX_MEMO_SIZE) + " bytes.");
        }
        this->inputs = inputs;
        this->outputs = outputs;
        std::sort(this->inputs.begin(), this->inputs.end());
        std::sort(this->outputs.begin(), this->outputs.end());
    }
};

class UnsignedOperationTransaction : public BaseTransaction {
  public:
    std::vector<TransferableOp> operations;

    void encode(Data& data) const;

    UnsignedOperationTransaction(uint32_t networkID, Data& blockchainID,
                                 std::vector<TransferableInput>& inputs,
                                 std::vector<TransferableOutput>& outputs, Data& memo,
                                 std::vector<TransferableOp>& ops)
        : BaseTransaction(TransactionTypeID::Operation, networkID, blockchainID, inputs, outputs,
                          memo)
        , operations(ops) {
        std::sort(this->operations.begin(), this->operations.end());
    }

    UnsignedOperationTransaction(BaseTransaction& baseTxn, std::vector<TransferableOp>& ops)
        : BaseTransaction(baseTxn), operations(ops) {
        std::sort(this->operations.begin(), this->operations.end());
        typeID = TransactionTypeID::Operation;
    }
};

class UnsignedImportTransaction : public BaseTransaction {
  public:
    Data sourceChain;
    std::vector<TransferableInput> importInputs;

    void encode(Data& data) const;

    UnsignedImportTransaction(uint32_t networkID, Data& blockchainID,
                              std::vector<TransferableInput>& inputs,
                              std::vector<TransferableOutput>& outputs, Data& memo, Data& source,
                              std::vector<TransferableInput>& importInputs)
        : BaseTransaction(TransactionTypeID::Import, networkID, blockchainID, inputs, outputs, memo)
        , sourceChain(source)
        , importInputs(importInputs) {
        if (source.size() != BLOCKCHAIN_ID_SIZE) {
            throw std::invalid_argument(std::string("SourceChain must be ") +
                                        std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
        }
        std::sort(this->importInputs.begin(), this->importInputs.end());
    }

    UnsignedImportTransaction(BaseTransaction& baseTxn, Data& source,
                              std::vector<TransferableInput> importInputs)
        : BaseTransaction(baseTxn), sourceChain(source), importInputs(importInputs) {
        if (source.size() != BLOCKCHAIN_ID_SIZE) {
            throw std::invalid_argument(std::string("SourceChain must be ") +
                                        std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
        }
        typeID = TransactionTypeID::Import;
        std::sort(this->importInputs.begin(), this->importInputs.end());
    }
};

class UnsignedExportTransaction : public BaseTransaction {
  public:
    Data destinationChain;
    std::vector<TransferableOutput> exportOutputs;

    void encode(Data& data) const;

    UnsignedExportTransaction(uint32_t networkID, Data& blockchainID,
                              std::vector<TransferableInput>& inputs,
                              std::vector<TransferableOutput>& outputs, Data& memo, Data& dest,
                              std::vector<TransferableOutput>& exportOutputs)
        : BaseTransaction(TransactionTypeID::Export, networkID, blockchainID, inputs, outputs, memo)
        , destinationChain(dest)
        , exportOutputs(exportOutputs) {
        if (dest.size() != BLOCKCHAIN_ID_SIZE) {
            throw std::invalid_argument(std::string("DestinationChain must be ") +
                                        std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
        }
        std::sort(this->exportOutputs.begin(), this->exportOutputs.end());
    }

    UnsignedExportTransaction(BaseTransaction& baseTxn, Data& dest,
                              std::vector<TransferableOutput>& exportOutputs)
        : BaseTransaction(baseTxn), destinationChain(dest), exportOutputs(exportOutputs) {
        if (dest.size() != BLOCKCHAIN_ID_SIZE) {
            throw std::invalid_argument(std::string("DestinationChain must be ") +
                                        std::to_string(BLOCKCHAIN_ID_SIZE) + " bytes.");
        }
        typeID = TransactionTypeID::Export;
        std::sort(this->exportOutputs.begin(), this->exportOutputs.end());
    }
};

class SignedTransaction {
  public:
    const uint16_t codecID = 0;
    BaseTransaction unsignedTransaction;
    std::vector<Credential> Credentials;

    void encode(Data& data) const;

    SignedTransaction(BaseTransaction& txn, std::vector<Credential>& credentials)
        : unsignedTransaction(txn), Credentials(credentials) {}
};

} // namespace TW::Avalanche
