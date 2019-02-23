// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Ripple.pb.h"
#include "../Data.h"
#include "Address.h"

namespace TW {
namespace Ripple {

enum FieldType: int {
    FieldTypeInt16 = 1,
    FieldTypeInt32 = 2,
    FieldTypeAmount = 6,
    FieldTypeVL = 7,
    FieldTypeAccount = 8
};

enum TransactionType {
    TransactionTypePayment = 0
};

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
    Address destination;
    int64_t destination_tag;
    Data pub_key;
    Data signature;

    Transaction() = default;
    Transaction(int64_t amount, int64_t fee, int64_t flags, int32_t sequence, int32_t last_ledger_sequence, Address account, Address destination, int64_t destination_tag)
    : amount(amount)
    , fee(fee)
    , flags(flags)
    , sequence(sequence)
    , last_ledger_sequence(last_ledger_sequence)
    , account(account)
    , destination(destination)
    , destination_tag(destination_tag)
    {}

public:
    /// simplified serialization format tailored for Payment transaction type exclusively.
    Data serialize() const;
    Data getPreImage() const;

    static Data serializeAmount(int64_t amount);
    static Data serializeAddress(Address address);
};

}} // namespace
