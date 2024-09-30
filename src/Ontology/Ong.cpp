// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Ong.h"
#include "Data.h"
#include "ParamsBuilder.h"

#include <list>

namespace TW::Ontology {

Transaction Ong::decimals(uint32_t nonce) {
    auto builder = ParamsBuilder();
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), version, "decimals", {Data()});
    auto tx =
        Transaction(version, txType, nonce, (uint64_t)0, (uint64_t)0, (std::string) "", invokeCode);
    return tx;
}

Transaction Ong::balanceOf(const Address& address, uint32_t nonce) {
    auto builder = ParamsBuilder();
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), version, "balanceOf", {address._data});
    auto tx =
        Transaction(version, txType, nonce, (uint64_t)0, (uint64_t)0, (std::string) "", invokeCode);
    return tx;
}

Transaction Ong::transfer(const Signer& from, const Address& to, uint64_t amount,
                          const Signer& payer, uint64_t gasPrice, uint64_t gasLimit,
                          uint32_t nonce) {
    NeoVmParamValue::ParamList transferParam{from.getAddress()._data, to._data, amount};
    NeoVmParamValue::ParamArray args{transferParam};
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), 0x00, "transfer", {args});
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit, payer.getAddress().string(),
                          invokeCode);
    from.sign(tx);
    payer.addSign(tx);
    return tx;
}

Transaction Ong::withdraw(const Signer& claimer, const Address& receiver, uint64_t amount,
                          const Signer& payer, uint64_t gasPrice, uint64_t gasLimit,
                          uint32_t nonce) {
    auto ontContract = Address("AFmseVrdL9f9oyCzZefL9tG6UbvhUMqNMV");
    NeoVmParamValue::ParamList args{claimer.getAddress()._data, ontContract._data, receiver._data, amount};
    auto invokeCode =
        ParamsBuilder::buildNativeInvokeCode(contractAddress(), 0x00, "transferFrom", {args});
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit, payer.getAddress().string(),
                          invokeCode);
    claimer.sign(tx);
    payer.addSign(tx);
    return tx;
}

Transaction Ong::unsignedTransfer(const Address &from, const Address &to, uint64_t amount, const Address &payer,
                                  uint64_t gasPrice, uint64_t gasLimit,uint32_t nonce) {
    NeoVmParamValue::ParamList transferParam{from._data, to._data, amount};
    NeoVmParamValue::ParamArray args{transferParam};
    auto invokeCode =
            ParamsBuilder::buildNativeInvokeCode(contractAddress(), 0x00, "transfer", {args});
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit,
                          payer.string(), invokeCode);
    return tx;
}

} // namespace TW::Ontology
