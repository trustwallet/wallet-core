// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <nlohmann/json.hpp>

#include "ObjectId.h"
#include "../Bravo/Serialization.h"

namespace TW {
namespace Bitshares {
    class Asset {
    public:
        int64_t amount;
        ObjectId id;

        Asset(int64_t amount, uint64_t assetId) : amount(amount), id({1, 3, assetId}) { }

        void serialize(Data& os) const {
            encode64LE(amount, os);
            Bravo::encodeVarInt64(id.instance, os);
        }

        nlohmann::json serialize() const {
            nlohmann::json obj;
            obj["amount"] = amount;
            obj["asset_id"] = id.string();
        }
    };
}} // namespace