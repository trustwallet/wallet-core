// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ont.h"
#include "Data.h"
#include "ParamsBuilder.h"

#include <unordered_map>

namespace TW::Ontology {

Transaction Ont::decimals(uint32_t nonce) {
    auto builder = ParamsBuilder();
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), version, "decimals", {Data()});
    auto tx = Transaction((uint8_t)0, txType, nonce, (uint64_t)0, (uint64_t)0, (std::string) "",
                          invokeCode);
    return tx;
}

Transaction Ont::balanceOf(const Address& address, uint32_t nonce) {
    auto builder = ParamsBuilder();
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), version, "balanceOf", {address._data});
    auto tx = Transaction((uint8_t)0, txType, nonce, (uint64_t)0, (uint64_t)0,
                          (std::string) "", invokeCode);
    return tx;
}

Transaction Ont::transfer(const Signer& from, const Address& to, uint64_t amount,
                          const Signer& payer, uint64_t gasPrice, uint64_t gasLimit,
                          uint32_t nonce) {
    NeoVmParamValue::ParamList transferParam{from.getAddress()._data, to._data, amount};
    NeoVmParamValue::ParamArray args{transferParam};
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), 0x00, "transfer", {args});
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit,
                          payer.getAddress().string(), invokeCode);
    from.sign(tx);
    payer.addSign(tx);
    return tx;
}

Transaction Ont::unsignedTransfer(const Address& from, const Address& to, uint64_t amount,
                                  const Address& payer, uint64_t gasPrice, uint64_t gasLimit, 
                                  uint32_t nonce) {
    NeoVmParamValue::ParamList transferParam{from._data, to._data, amount};
    NeoVmParamValue::ParamArray args{transferParam};
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), 0x00, "transfer", {args});
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit,
                          payer.string(), invokeCode);
    return tx;
}

} // namespace TW::Ontology
