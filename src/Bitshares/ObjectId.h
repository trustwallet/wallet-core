// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

namespace TW {
namespace Bitshares {
    struct ObjectId {
        uint8_t spaceId, typeId;
        uint64_t instance;

        ObjectId(uint8_t spaceId, uint8_t typeId, uint64_t instance): spaceId(spaceId), 
                                                                        typeId(typeId),
                                                                        instance(instance) { }

        std::string string() const {
            return std::to_string(spaceId) + "."
                    + std::to_string(typeId) + "."
                    + std::to_string(instance);
        }

        friend bool operator==(const ObjectId& lhs, const ObjectId rhs);
        friend bool operator!=(const ObjectId& lhs, const ObjectId rhs);
    };

    inline bool operator==(const ObjectId& lhs, const ObjectId rhs) {
        return lhs.spaceId == rhs.spaceId 
                && lhs.typeId == rhs.typeId 
                && lhs.instance == rhs.instance;
    }

    inline bool operator!=(const ObjectId& lhs, const ObjectId rhs) {
        return ! (lhs == rhs);
    }
}} // namespace