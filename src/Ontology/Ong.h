// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Asset.h"
#include "Data.h"

namespace TW::Ontology {

class Ong : public Asset {
  private:
    static const uint8_t version = 0x00;

    const Data ongContract = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};

  public:
    Data contractAddress() override { return ongContract; }

    Transaction decimals(uint32_t nonce) override;

    Transaction balanceOf(const Address &address, uint32_t nonce) override;

    Transaction transfer(const Signer &from, const Address &to, uint64_t amount,
                         const Signer &payer, uint64_t gasPrice, uint64_t gasLimit,
                         uint32_t nonce) override;

    Transaction withdraw(const Signer &claimer, const Address &receiver, uint64_t amount,
                         const Signer &payer, uint64_t gasPrice, uint64_t gasLimit, uint32_t nonce);

    Transaction unsignedTransfer(const Address &from, const Address &to, uint64_t amount, const Address &payer,
                                 uint64_t gasPrice, uint64_t gasLimit, uint32_t nonce) override;
};

} // namespace TW::Ontology
