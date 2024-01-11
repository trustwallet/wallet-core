// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Solana/Address.h"
#include "Solana/VersionedMessage.h"
#include "Data.h"
#include "BinaryCoding.h"

#include <vector>
#include <string>

namespace TW::Solana {

class VersionedTransaction {
  public:
    // Signatures
    std::vector<Data> signatures;
    // The message to sign
    VersionedMessage message;

    VersionedTransaction(const VersionedMessage& message) : message(message) {
        this->signatures.resize(header(message).numRequiredSignatures, defaultSignature);
    }

    // Default basic transfer transaction
    VersionedTransaction(const Address& from, const Address& to, uint64_t value, Data recentBlockhash, std::string memo = "", std::vector<Address> references = {})
        : message(VersionedMessage(LegacyMessage::createTransfer(from, to, value, recentBlockhash, memo, references))) {
        this->signatures.resize(1, defaultSignature);
    }

  public:
    std::string serialize() const;
    std::vector<uint8_t> messageData() const;
    uint8_t getAccountIndex(Address publicKey);

  private:
    TW::Data defaultSignature = TW::Data(64);
};

} // namespace TW::Solana
