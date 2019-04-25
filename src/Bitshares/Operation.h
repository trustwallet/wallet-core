// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <nlohmann/json.hpp>

#include "Asset.h"
#include "Memo.h"
#include "../Bravo/Operation.h"

namespace TW::Bitshares {
class TransferOperation: public Bravo::Operation {
public:
    TransferOperation(uint64_t senderId, 
                        uint64_t recipientId, 
                        const Asset& amount, 
                        const Asset& fee,
                        Memo *memo = nullptr);

    void serialize(Data& os) const noexcept override;
    nlohmann::json serialize() const noexcept override;
    void validate();
    static constexpr int operationId() { return 0; };

private:
    ObjectId from, to;
    std::unique_ptr<Memo> memo;
    Asset amount, fee;
};
} // namespace TW::Bitshares