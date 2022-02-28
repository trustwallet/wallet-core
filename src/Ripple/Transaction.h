// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "XAddress.h"
#include "../Data.h"
#include "../proto/Ripple.pb.h"

namespace TW::Ripple {

enum class FieldType: int {
    int16   = 1,
    int32   = 2,
    amount  = 6,
    vl      = 7,
    account = 8
};

enum class TransactionType { payment = 0 };

class Transaction {
    /// We only support transaction types other than the Payment transaction.
    /// Non-XRP currencies are not supported. Float and negative amounts are not supported.
    /// See https://github.com/trezor/trezor-core/tree/master/src/apps/ripple#transactions
  public:
    int64_t amount;
    int64_t fee;
    int64_t flags;
    int32_t sequence;
    int32_t last_ledger_sequence;
    Address account;
    Data destination;
    bool encode_tag;
    int64_t destination_tag;
    Data pub_key;
    Data signature;

    Transaction(int64_t amount, int64_t fee, int64_t flags, int32_t sequence,
                int32_t last_ledger_sequence, Address account, const std::string& destination,
                int64_t destination_tag)
        : amount(amount)
        , fee(fee)
        , flags(flags)
        , sequence(sequence)
        , last_ledger_sequence(last_ledger_sequence)
        , account(account) {
          try {
            auto address = Address(destination);
            encode_tag = destination_tag > 0;
            this->destination_tag = destination_tag;
            this->destination = Data(address.bytes.begin() + 1, address.bytes.end());
          } catch(const std::exception& e) {
            auto xAddress = XAddress(destination);
            encode_tag = xAddress.flag != TagFlag::none;
            this->destination_tag = xAddress.tag;
            this->destination = Data(xAddress.bytes.begin(), xAddress.bytes.end());
          }
        }

  public:
    /// simplified serialization format tailored for Payment transaction type
    /// exclusively.
    Data serialize() const;
    Data getPreImage() const;

    static Data serializeAmount(int64_t amount);
    static Data serializeAddress(Address address);
};

} // namespace TW::Ripple
