// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

#include "ISerializable.h"
#include "PublicKey.h"
#include "TransactionType.h"
#include "TransactionAttribute.hpp"
#include "TransactionOutput.hpp"
#include "CoinReference.hpp"
#include "Witness.hpp"

namespace TW::NEO {
    class Transaction : public ISerializable {
    private:
        TransactionType type;
        byte version;

        std::vector<TransactionAttribute> attributes;
        std::vector<CoinReference> inInputs;
        std::vector<TransactionOutput> outputs;
        std::vector<Witness> witnesses;
    public:
        int64_t size() const override;
        void deserialize(const Data &data) override;
        Data serialize() const override;

        Data getHash() const;
    };

} // namespace TW::NEO
