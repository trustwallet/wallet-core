// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace TW::Elrond {

class Transaction {
public:
    uint64_t nonce;
    std::string sender;
    std::string senderUsername;
    std::string receiver;
    std::string receiverUsername;
    std::string value;
    std::string data;
    uint64_t gasPrice;
    uint64_t gasLimit;
    std::string chainID;
    uint32_t version;
    uint32_t options;

    Transaction();
};

} // namespace
