// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "BaseTransaction.h"


using namespace TW;
using namespace TW::Algorand;

Data BaseTransaction::serialize(const Data& signature) const {
    /* Algorand transaction and signature are encoded with msgpack:
    {
        "sig": <signature bytes>
        "txn": <encoded transaction object>,
    }
    */
    Data data;
    // encode map length
    data.push_back(0x80 + 2);
    // signature
    encodeString("sig", data);
    encodeBytes(signature, data);

    // transaction
    encodeString("txn", data);
    append(data, serialize());
    return data;
}