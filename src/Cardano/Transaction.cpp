// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

using namespace TW::Cardano;
using namespace TW;

Utxo fromProto(const Cardano::Proto::Utxo& proto) {
    auto ret = Utxo();
    ret.txHash = data(proto.tx_hash());
    ret.address = proto.address();
    ret.amount = proto.amount();
    ret.outputIndex = proto.output_index();
    return ret;
}
