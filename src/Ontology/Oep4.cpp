// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Oep4.h"
#include <HexCoding.h>

namespace TW::Ontology {
Oep4::Oep4(const Address addr) noexcept
    : oep4Contract(addr._data.begin(), addr._data.end()) {
}

Oep4::Oep4(const Data bin) noexcept
    : oep4Contract(bin) {
}

Transaction Oep4::readOnlyMethod(std::string method_name, uint32_t nonce) {
    Address addr(oep4Contract);
    NeoVmParamValue::ParamArray args{};
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(addr, method_name, {args});

    return Transaction(0, 0xD1, nonce, 0, 0, "", invokeCode);
}

Transaction Oep4::name(uint32_t nonce) {
    return Oep4::readOnlyMethod("name", nonce);
}

Transaction Oep4::symbol(uint32_t nonce) {
    return Oep4::readOnlyMethod("symbol", nonce);
}

Transaction Oep4::decimals(uint32_t nonce) {
    return Oep4::readOnlyMethod("decimals", nonce);
}

Transaction Oep4::totalSupply(uint32_t nonce) {
    return Oep4::readOnlyMethod("totalSupply", nonce);
}

Transaction Oep4::balanceOf(const Address& user, uint32_t nonce) {
    Address contract(oep4Contract);
    Data d(std::begin(user._data), std::end(user._data));
    NeoVmParamValue::ParamArray args{d};
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(contract, "balanceOf", {args});
    return Transaction(0, 0xD1, nonce, 0, 0, "", invokeCode);
}

Transaction Oep4::transfer(const Signer& from, const Address& to, uint64_t amount,
                           const Signer& payer, uint64_t gasPrice, uint64_t gasLimit,
                           uint32_t nonce) {
    Address contract(oep4Contract);

    auto fromAddr = from.getAddress();
    NeoVmParamValue::ParamArray args{fromAddr._data, to._data, amount};
    // yes, invoke neovm is not like ont transfer
    std::reverse(args.begin(), args.end());
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(contract, "transfer", {args});

    auto tx = Transaction(0, 0xD1, nonce, gasPrice, gasLimit, payer.getAddress().string(), invokeCode);
    from.sign(tx);
    payer.addSign(tx);

    return tx;
}

Transaction Oep4::unsignedTransfer(const Address& from, const Address& to, uint64_t amount,
                                   const Address& payer, uint64_t gasPrice, uint64_t gasLimit, 
                                   uint32_t nonce) {
    Address contract(oep4Contract);

    NeoVmParamValue::ParamArray args{from._data, to._data, amount};
    // yes, invoke neovm is not like ont transfer
    std::reverse(args.begin(), args.end());
    auto invokeCode = ParamsBuilder::buildOep4InvokeCode(contract, "transfer", {args});

    auto tx = Transaction(0, 0xD1, nonce, gasPrice, gasLimit, payer.string(), invokeCode);
    return tx;
}

} // namespace TW::Ontology
