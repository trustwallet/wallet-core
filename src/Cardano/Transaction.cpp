// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "Cbor.h"
#include "Hash.h"

using namespace TW::Cardano;
using namespace TW;

OutPoint OutPoint::fromProto(const Cardano::Proto::OutPoint& proto) {
    auto ret = OutPoint();
    ret.txHash = data(proto.tx_hash());
    ret.outputIndex = proto.output_index();
    return ret;
}

TxInput TxInput::fromProto(const Cardano::Proto::TxInput& proto) {
    auto ret = TxInput();
    ret.txHash = data(proto.out_point().tx_hash());
    ret.outputIndex = proto.out_point().output_index();
    ret.address = proto.address();
    ret.amount = proto.amount();
    return ret;
}

TransactionPlan TransactionPlan::fromProto(const Proto::TransactionPlan& proto) {
    auto ret = TransactionPlan();
    for (auto i = 0; i < proto.utxos_size(); ++i) {
        ret.utxos.push_back(TxInput::fromProto(proto.utxos(i)));
    }
    ret.availableAmount = proto.available_amount();
    ret.amount = proto.amount();
    ret.fee = proto.fee();
    ret.change = proto.change();
    ret.error = proto.error();
    return ret;
}

Cbor::Encode cborizeInputs(const std::vector<OutPoint>& inputs) {
    std::vector<Cbor::Encode> ii;
    for (const auto& i: inputs) {
        ii.push_back(Cbor::Encode::array({
            Cbor::Encode::bytes(i.txHash),
            Cbor::Encode::uint(i.outputIndex)
        }));
    }
    return Cbor::Encode::array(ii);
}

Cbor::Encode cborizeOutputs(const std::vector<TxOutput>& outputs) {
    std::vector<Cbor::Encode> oo;
    for (const auto& o: outputs) {
        oo.push_back(Cbor::Encode::array({
            Cbor::Encode::bytes(o.address),
            Cbor::Encode::uint(o.amount)
        }));
    }
    return Cbor::Encode::array(oo);
}

Data Transaction::encode() const {
    const auto ii = cborizeInputs(inputs);
    const auto oo = cborizeOutputs(outputs);

    // Encode elements in a map, with fixed numbers as keys
    Cbor::Encode encode = Cbor::Encode::map({
        std::make_pair(Cbor::Encode::uint(0), ii),
        std::make_pair(Cbor::Encode::uint(1), oo),
        std::make_pair(Cbor::Encode::uint(2), Cbor::Encode::uint(fee)),
        std::make_pair(Cbor::Encode::uint(3), Cbor::Encode::uint(ttl)),
    });
    // Note: following fields are not included:
    // 4 certificates, 5 withdrawals, 7 AUXILIARY_DATA_HASH, 8 VALIDITY_INTERVAL_START

    return encode.encoded();
}

Data Transaction::getId() const {
    const auto encoded = encode();
    const auto hash = Hash::blake2b(encoded, 32);
    return hash;
}
