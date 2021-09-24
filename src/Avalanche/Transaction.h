// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Credential.h"
#include "TransferableInput.h"
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
