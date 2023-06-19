// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once
#include "../proto/NULS.pb.h"

#include "Data.h"
#include "../uint256.h"
#include <algorithm>
#include <stdint.h>

namespace TW::NULS {

/// Helper class that performs NULS transaction signing.
class Signer {
  public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;

  public:
    static const uint16_t TRANSACTION_FIX_SIZE = 11; //type size 2, time size 4, txData size 1, hash size 4
    static const uint16_t TRANSACTION_SIG_MAX_SIZE = 110;
    static const uint16_t TRANSACTION_INPUT_SIZE = 70;
    static const uint16_t TRANSACTION_OUTPUT_SIZE = 70;
    /// Transaction size must less that 300KB
    static const uint64_t MAX_TRANSACTION_SIZE = 300 * 1024;
    /// 0.001 NULS per KB
    static const uint64_t MIN_PRICE_PRE_1024_BYTES = 100000;

    static const uint16_t TX_TYPE = 2;

    Proto::Transaction tx;

    Proto::SigningInput input;

    /// Initializes a transaction signer.
    Signer(const Proto::SigningInput& input);

    /// Signs the transaction.
    ///
    /// \returns the transaction signature or an empty vector if there is an error.
    Data sign() const;

    Data buildUnsignedTx() const;

    Data buildSignedTx(const std::vector<Data> publicKeys,
                       const std::vector<Data> signatures,
                       const Data unsignedTxBytes) const;

private:
    uint64_t CalculatorTransactionFee(uint64_t size) const;
    int32_t CalculatorMaxInput(uint32_t remarkSize) const;
    uint32_t CalculatorTransactionSize(uint32_t inputCount, uint32_t outputCount, uint32_t remarkSize) const;
};

} // namespace TW::NULS
