// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bravo/Serialization.h"

namespace TW {
namespace Bitshares {
    struct ObjectId: Bravo::Serializable {
        uint8_t spaceId, typeId;
        uint64_t instance;

        void serialize(const Data& os) {
            Bravo::encodeVarInt64(os, instance);
        }
    };
}} // namespace