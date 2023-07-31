// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Everscale/CommonTON/CellBuilder.h"

#include "TheOpenNetwork/Address.h"

namespace TW::TheOpenNetwork {
    TW::CommonTON::Cell::Ref jettonTransferPayload(
        const Address& responseAddress,
        const Address& toOwner,
        uint64_t jettonAmount,
        uint64_t forwardAmount,
        const std::string& comment,
        uint64_t query_id);
} // namespace TW::TheOpenNetwork