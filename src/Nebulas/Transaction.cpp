// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "Transaction.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Nebulas;

const char *Transaction::TxPayloadBinaryType = "binary";
const char *Transaction::TxPayloadDeployType = "deploy";
const char *Transaction::TxPayloadCallType = "call";

Proto::Data* Transaction::newPayloadData() const{
    auto data = new Proto::Data();
    data->set_type(Transaction::TxPayloadBinaryType);
    return data;
}

void Transaction::serializeToRaw(){
    if(signature.empty()) {
        throw std::logic_error("The transaction is unsigned!");
    }

    auto tx = Proto::RawTransaction();
    auto data = newPayloadData();

    auto value = Data();
    auto gas_price = Data();
    auto gas_limit = Data();
    tx.set_hash(reinterpret_cast<const char *>(hash.data()),hash.size());
    tx.set_from(from.bytes.data(),from.size);
    tx.set_to(to.bytes.data(),to.size);
    encode256BE(value, amount, 128);
    tx.set_value(value.data(),value.size());
    tx.set_nonce((uint64_t)nonce);
    tx.set_timestamp((int64_t)timestamp);
    tx.set_allocated_data(data);
    tx.set_chain_id((uint32_t)chainID);
    encode256BE(gas_price, gasPrice, 128);
    tx.set_gas_price(gas_price.data(),gas_price.size());
    encode256BE(gas_limit, gasLimit, 128);
    tx.set_gas_limit(gas_limit.data(),gas_limit.size());
    
    tx.set_alg((uint32_t)algorithm);
    tx.set_sign(reinterpret_cast<const char *>(signature.data()),signature.size());

    raw.resize(tx.ByteSize());
    tx.SerializeToArray(raw.data(),raw.size());
}