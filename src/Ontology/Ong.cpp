// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ong.h"
#include "ParamsBuilder.h"

#include <unordered_map>

#include <TrezorCrypto/rand.h>

using namespace TW;
using namespace TW::Ontology;

Transaction Ong::decimals(const Address &address){
    auto builder = ParamsBuilder();
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(contractAddress(), version, "decimals", address.data);
    auto tx = Transaction(version, txType, random32(), (uint64_t) 0, (uint64_t) 0, (std::string) "", invokeCode);
    return tx;
}

Transaction Ong::balanceOf(const Address &address) {
    auto builder = ParamsBuilder();
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(contractAddress(), version, "balanceOf", address.data);
    auto tx = Transaction(version, txType, random32(), (uint64_t) 0, (uint64_t) 0, (std::string) "", invokeCode);
    return tx;
}

Transaction Ong::transfer(const Signer &from, const Address &to, uint64_t amount, const Signer &payer, uint64_t gasPrice, uint64_t gasLimit) {
    std::unordered_map<std::string, boost::any> transferParam{{"from", from.getAddress().data},{"to", to.data},{"amount", amount}};
    std::vector<boost::any> args{transferParam};
    auto invokeCode = ParamsBuilder::buildNativeInvokeCode(contractAddress(), 0x00, "transfer", args);
    auto tx = Transaction(version, txType, random32(), gasPrice, gasLimit, payer.getAddress().string(), invokeCode);
    from.sign(tx);
    payer.addSign(tx);
    return tx;
}

