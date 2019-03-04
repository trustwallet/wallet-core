// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"

namespace TW {
namespace Nimiq {

class Transaction {
    /// TODO: Only supports Nimiq basic transactions atm
public:
    uint64_t amount;
    uint64_t fee;
    std::array<uint8_t, 32> sender_pub_key;
    uint8_t sender_type;
    Address destination;
    uint8_t destination_type;
    std::vector<uint8_t> signature;

    Transaction() = default;
    Transaction(uint64_t amount, uint64_t fee, uint8_t sender_type, Address destination, uint8_t destination_type)
        : amount(amount)
        , fee(fee)
        , sender_type(sender_type)
        , destination(destination)
        , destination_type(destination_type)
        {}

public:
    std::vector<uint8_t> serialize() const;
};

}} // namespace

