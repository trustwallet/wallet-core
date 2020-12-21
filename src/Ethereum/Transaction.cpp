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

Transaction Transaction::buildERC20Transfer(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit,
                const Data& tokenContract, const Data& toAddress, uint256_t amount) {
    return Transaction(nonce, gasPrice, gasLimit, tokenContract, 0, buildERC20TransferCall(toAddress, amount));
}

Transaction Transaction::buildERC20Approve(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit,
                const Data& tokenContract, const Data& spenderAddress, uint256_t amount) {
    return Transaction(nonce, gasPrice, gasLimit, tokenContract, 0, buildERC20ApproveCall(spenderAddress, amount));
}

Transaction Transaction::buildERC721Transfer(uint256_t nonce, uint256_t gasPrice, uint256_t gasLimit,
                const Data& tokenContract, const Data& from, const Data& to, uint256_t tokenId) {
    return Transaction(nonce, gasPrice, gasLimit, {}, 0, buildERC721TransferFromCall(from, to, tokenId));
}

Data Transaction::buildERC20TransferCall(const Data& to, uint256_t amount) {
    auto func = Function("transfer", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(to),
        std::make_shared<ParamUInt256>(amount)
    });
    Data payload;
    func.encode(payload);
    return payload;
}

Data Transaction::buildERC20ApproveCall(const Data& spender, uint256_t amount) {
    auto func = Function("approve", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(spender),
        std::make_shared<ParamUInt256>(amount)
    });
    Data payload;
    func.encode(payload);
    return payload;
}

Data Transaction::buildERC721TransferFromCall(const Data& from, const Data& to, uint256_t tokenId) {
    auto func = Function("transferFrom", std::vector<std::shared_ptr<ParamBase>>{
        std::make_shared<ParamAddress>(from),
        std::make_shared<ParamAddress>(to),
        std::make_shared<ParamUInt256>(tokenId)
    });
    Data payload;
    func.encode(payload);
    return payload;
}
