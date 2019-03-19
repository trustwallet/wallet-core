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
std::string OperationList::forgeBranch() const {
    std::array<byte, 2> prefix = {1, 52};

    const auto decoded = Base58::bitcoin.decodeCheck(branch);
    if (decoded.size() < prefix.size() || !std::equal(prefix.begin(), prefix.end(), decoded.begin())) {
        return "";
    }

    return hex(decoded.begin() + prefix.size(), decoded.end());
}

std::string OperationList::forge() const {
    std::string result = forgeBranch();

    for (auto operation : operation_list) {
      result += operation.forge();
    }
    return result;
}
