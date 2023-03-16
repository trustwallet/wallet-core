// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "BinaryCoding.h"
#include "Data.h"

namespace TW::Algorand {

class BaseTransaction {
public:
    virtual ~BaseTransaction() noexcept = default;
    virtual Data serialize() const = 0;
    virtual Data serialize(const Data& signature) const {
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
};

} // namespace TW::Algorand
