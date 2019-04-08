// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "HexCoding.h"
#include "OperationList.h"
#include "Transaction.h"
#include "../Base58.h"

using namespace TW;
using namespace TW::Tezos;

OperationList::OperationList(const std::string& str) {
    branch = str;
}

void OperationList::addOperation(const Transaction& transaction) {
    operation_list.push_back(transaction);
}

// Forge the given branch to a hex encoded string.
Data OperationList::forgeBranch() const {
    std::array<byte, 2> prefix = {1, 52};
    const auto decoded = Base58::bitcoin.decodeCheck(branch);
    if (decoded.size() != 34 || !std::equal(prefix.begin(), prefix.end(), decoded.begin())) {
        throw std::invalid_argument("Invalid branch for forge");
    }
    auto forged = Data();
    forged.insert(forged.end(), decoded.begin() + prefix.size(), decoded.end());
    return forged;
}

Data OperationList::forge() const {
    auto forged = forgeBranch();

    for (auto operation : operation_list) {
        append(forged, operation.forge());
    }
    return forged;
}
