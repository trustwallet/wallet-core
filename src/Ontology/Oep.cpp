#include "Oep.h"
#include "Data.h"
#include "ParamsBuilder.h"

#include <list>

using namespace TW;
using namespace TW::Ontology;

Transaction Oep::decimals(uint32_t nonce) {
    std::vector<boost::any> emptyParam;
    std::string method = "decimals";
    std::vector<boost::any> params{emptyParam, method};
    auto invokeCode = ParamsBuilder::buildNeoVMInvokeCode(contractAddress(), params);
    auto tx = Transaction((uint8_t)0, txType, nonce, (uint64_t)0, (uint64_t)0, (std::string) "", invokeCode);
    return tx;
}

Transaction Oep::balanceOf(const Address &address, uint32_t nonce) {
    std::vector<boost::any> args{address.data};
    std::string method = "balanceOf";
    std::vector<boost::any> params{args, method};
    auto invokeCode = ParamsBuilder::buildNeoVMInvokeCode(contractAddress(), params);
    auto tx = Transaction((uint8_t)0, txType, nonce, (uint64_t)0, (uint64_t)0, (std::string) "", invokeCode);
    return tx;
}

Transaction Oep::transfer(const Signer &from, const Address &to, uint64_t amount,
                          const Signer &payer, uint64_t gasPrice, uint64_t gasLimit,
                          uint32_t nonce) {
    std::vector<boost::any> args{amount, to.data, from.getAddress().data,};
    std::string method = "transfer";
    std::vector<boost::any> params{args, method};
    auto invokeCode = ParamsBuilder::buildNeoVMInvokeCode(contractAddress(), params);
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit, payer.getAddress().string(), invokeCode);
    from.sign(tx);
    payer.addSign(tx);
    return tx;
}

Transaction Oep::unsignedTransfer(const Address &from, const Address &to, uint64_t amount,
                             uint64_t gasPrice, uint64_t gasLimit,uint32_t nonce) {
    std::vector<boost::any> args{amount, to.data, from.data};
    std::string method = "transfer";
    std::vector<boost::any> params{args, method};
    auto invokeCode = ParamsBuilder::buildNeoVMInvokeCode(contractAddress(), params);
    auto tx = Transaction(version, txType, nonce, gasPrice, gasLimit, from.string(), invokeCode);
    return tx;
}