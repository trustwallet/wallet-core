// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
