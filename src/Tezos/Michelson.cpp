// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Michelson.h"

namespace TW::Tezos {

MichelsonValue::MichelsonVariant FA12ParameterToMichelson(const Proto::FA12Parameters& data) {
    MichelsonValue::MichelsonVariant address = StringValue{.string = data.from()};
    MichelsonValue::MichelsonVariant to = StringValue{.string = data.to()};
    MichelsonValue::MichelsonVariant amount = IntValue{._int = data.value()};
    auto primTransferInfos = PrimValue{.prim = "Pair", .args{{to}, {amount}}};
    return PrimValue{.prim = "Pair", .args{{address}, {primTransferInfos}}};
}

MichelsonValue::MichelsonVariant FA2ParameterToMichelson(const Proto::FA2Parameters& data) {
    auto& txObj = *data.txs_object().begin();
    MichelsonValue::MichelsonVariant from = StringValue{.string = txObj.from()};
    auto& txTransferInfos = txObj.txs(0);
    MichelsonValue::MichelsonVariant tokenId = IntValue{._int = txTransferInfos.token_id()};
    MichelsonValue::MichelsonVariant amount = IntValue{._int = txTransferInfos.amount()};
    auto primTransferInfos = PrimValue{.prim = "Pair", .args{{tokenId}, {amount}}};
    MichelsonValue::MichelsonVariant to = StringValue{.string = txTransferInfos.to()};
    MichelsonValue::MichelsonVariant txs = MichelsonValue::MichelsonArray{PrimValue{.prim = "Pair", .args{{to}, {primTransferInfos}}}};
    auto primTxs = PrimValue{.prim = "Pair", .args{{from}, {txs}}};
    return MichelsonValue::MichelsonArray{primTxs};
}

} // namespace TW::Tezos
