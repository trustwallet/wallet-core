// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Oep4.h"
#include <HexCoding.h>
#include <cstdio>

using namespace TW;

namespace TW::Ontology {
Oep4::Oep4(const Address addr) {
    oep4Contract = Data(std::begin(addr._data), std::end(addr._data));
}

Transaction Oep4::romethod0(std::string method_name, uint32_t nonce) {
    auto builder = ParamsBuilder();
    Address addr(std::vector<uint8_t>(oep4Contract.begin(), oep4Contract.end()));
    std::vector<boost::any> args{};
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(addr, method_name, args);
    auto tx = Transaction((uint8_t)0, (uint8_t)0xD1, nonce, (uint64_t)0, (uint64_t)0, (std::string) "",
                          invokeCode);
    return tx;
}

Transaction Oep4::name(uint32_t nonce) {
    return Oep4::romethod0("name", nonce);
}

Transaction Oep4::symbol(uint32_t nonce) {
    return Oep4::romethod0("symbol", nonce);
}

Transaction Oep4::decimals(uint32_t nonce) {
    return Oep4::romethod0("decimals", nonce);
}

Transaction Oep4::totalSupply(uint32_t nonce) {
    return Oep4::romethod0("totalSupply", nonce);
}

Transaction Oep4::balanceOf(const Address& user, uint32_t nonce) {
    auto builder = ParamsBuilder();
    Address contract(std::vector<uint8_t>(oep4Contract.begin(), oep4Contract.end()));
    Data d(std::begin(user._data), std::end(user._data));
    std::vector<boost::any> args{d};
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(contract, "balanceOf", args);
    auto tx = Transaction((uint8_t)0, (uint8_t)0xD1, nonce, (uint64_t)0, (uint64_t)0, (std::string) "",
                          invokeCode);
    return tx;
}

Transaction Oep4::transfer(const Signer& from, const Address& to, uint64_t amount,
                           const Signer& payer, uint64_t gasPrice, uint64_t gasLimit,
                           uint32_t nonce) {

    auto builder = ParamsBuilder();
    Address contract(std::vector<uint8_t>(oep4Contract.begin(), oep4Contract.end()));

    auto fromAddr = from.getAddress();
    std::vector<boost::any> args{fromAddr._data, to._data, amount};
    // yes, invoke neovm is not like ont transfer
    std::reverse(args.begin(), args.end());
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(contract, "transfer", args);

    auto tx = Transaction((uint8_t)0, (uint8_t)0xD1, nonce, gasPrice, gasLimit, payer.getAddress().string(), invokeCode);
    from.sign(tx);
    payer.addSign(tx);

    return tx;
}

} // namespace TW::Ontology
