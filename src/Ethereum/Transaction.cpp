// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"
#include "ABI/Function.h"
#include "ABI/ParamBase.h"
#include "ABI/ParamAddress.h"

using namespace TW::Ethereum::ABI;
using namespace TW::Ethereum;
using namespace TW;

Transaction Transaction::buildERC20Transfer(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit, const Data& to, const Address& tokenContract, uint256_t amount) {
    auto func = Function("transfer", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(TW::data(tokenContract.bytes.data(), tokenContract.bytes.size())),
        std::make_shared<ParamUInt256>(amount)
    });
    Data payload;
    func.encode(payload);
    return Transaction(nonce, gasPrice, gasLimit, to, 0, payload);
}
