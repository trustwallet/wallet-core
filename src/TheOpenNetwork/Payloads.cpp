// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Payloads.h"


namespace TW::TheOpenNetwork {

const uint64_t jettonTransferOperation = 0xf8a7ea5;

using TW::CommonTON::Cell;
using TW::CommonTON::CellBuilder;

Cell::Ref jettonTransferPayload(
    const Address& responseAddress,
    const Address& toOwner,
    uint64_t jettonAmount,
    uint64_t forwardAmount,
    const std::string& comment,
    uint64_t query_id
) {
    CellBuilder bodyBuilder;
    bodyBuilder.appendU32(jettonTransferOperation);
    bodyBuilder.appendU64(query_id);
    bodyBuilder.appendU128(jettonAmount);
    bodyBuilder.appendAddress(toOwner.addressData);
    bodyBuilder.appendAddress(responseAddress.addressData);
    bodyBuilder.appendBitZero(); // null custom_payload
    bodyBuilder.appendU128(forwardAmount); 
    bodyBuilder.appendBitZero(); // forward_payload in this slice, not separate cell
    if (!comment.empty()) {
        const auto& data = Data(comment.begin(), comment.end());
        bodyBuilder.appendU32(0);
        bodyBuilder.appendRaw(data, static_cast<uint16_t>(data.size()) * 8);
    }
    return bodyBuilder.intoCell();
}

} // namespace TW::TheOpenNetwork