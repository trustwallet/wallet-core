// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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