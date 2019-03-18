// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "OperationList.h"

#include "BinaryCoding.h"
#include "HexCoding.h"
#include "Transaction.h"
#include "../Base58.h"

using namespace TW;
using namespace TW::Tezos;

OperationList::OperationList(const std::string& str) {
    branch = str;
}

void OperationList::add_operation(Transaction transaction) {
    operation_list.push_back(transaction);
}

// Forge the given branch to a hex encoded string.
Data OperationList::forgeBranch() const {
    size_t capacity = 128;
    uint8_t decoded[capacity];
    size_t prefixLength = 2;
    uint8_t prefix[] = {1, 52};

    int decodedLength = base58CheckDecodePrefix(branch, prefixLength, prefix, decoded);
    return parse_hex(hex(decoded, decoded + decodedLength));
}

std::string OperationList::forge() const {
    std::string result = hex(forgeBranch());

    for (auto operation : operation_list) {
      result += hex(operation.forge());
    }
    return result;
}
