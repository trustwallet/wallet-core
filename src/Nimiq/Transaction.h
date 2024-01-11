// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"

namespace TW::Nimiq {

class Transaction {
  public:
    // Sender public key
    std::array<uint8_t, 32> sender_pub_key;
    // Recipient address
    Address destination;
    // Transaction value
    uint64_t amount;
    // Transaction fee
    uint64_t fee;
    // Validity start (block) height
    uint32_t vsh;
    // Sender signature
    std::array<uint8_t, 64> signature;

    Transaction(const std::array<uint8_t, 32>& sender, const Address& dest, uint64_t amount,
                uint64_t fee, uint32_t vsh)
        : sender_pub_key(sender), destination(dest), amount(amount), fee(fee), vsh(vsh) {}

  public:
    std::vector<uint8_t> serialize() const;
    std::vector<uint8_t> getPreImage() const;
};

} // namespace TW::Nimiq
