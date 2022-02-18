// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

#include "Cbor.h"

using namespace TW::Cardano;
using namespace TW;

OutPoint OutPoint::fromProto(const Cardano::Proto::OutPoint& proto) {
    auto ret = OutPoint();
    ret.txHash = proto.tx_hash();
    ret.outputIndex = proto.output_index();
    return ret;
}

TxInput TxInput::fromProto(const Cardano::Proto::TxInput& proto) {
    auto ret = TxInput();
    ret.txHash = proto.out_point().tx_hash();
    ret.outputIndex = proto.out_point().output_index();
    ret.amount = proto.amount();
    return ret;
}

Data Transaction::encode() const {
    std::vector<Cbor::Encode> ii;
    for (const auto& i: inputs) {
        ii.push_back(Cbor::Encode::array({
            Cbor::Encode::string(i.txHash),
            Cbor::Encode::uint(i.outputIndex)
        }));
    }
    std::vector<Cbor::Encode> oo;
    for (const auto& o: outputs) {
        oo.push_back(Cbor::Encode::array({
            Cbor::Encode::bytes(o.address),
            Cbor::Encode::uint(o.amount)
        }));
    }

    // Encode elements in a map, with fixed numbers as keys
    Cbor::Encode encode = Cbor::Encode::map({
        std::make_pair(Cbor::Encode::uint(0), Cbor::Encode::array(ii)),
        std::make_pair(Cbor::Encode::uint(1), Cbor::Encode::array(oo)),
        std::make_pair(Cbor::Encode::uint(2), Cbor::Encode::uint(fee)),
        std::make_pair(Cbor::Encode::uint(3), Cbor::Encode::uint(ttl)),
    });
    // Note: following fields are not included:
    // 4 certificates, 5 withdrawals, 7 AUXILIARY_DATA_HASH, 8 VALIDITY_INTERVAL_START

    return encode.encoded();
}
