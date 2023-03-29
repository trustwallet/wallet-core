// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Messages.h"

#include "Wallet.h"
#include "WorkchainType.h"

using namespace TW;

namespace TW::Everscale {

Cell::Ref Message::intoCell() const {
    CellBuilder builder;

    // write Header
    _messageData.header->writeTo(builder);

    // write StateInit
    if (_messageData.init.has_value()) {
        auto initBuilder = _messageData.init.value().writeTo();

        builder.appendBitOne();
        builder.appendBitZero();
        builder.appendBuilder(initBuilder);
    } else {
        builder.appendBitZero();
    }

    // write Body
    if (_messageData.body.has_value()) {
        builder.appendBitZero();
        builder.appendCellSlice(CellSlice(_messageData.body.value().get()));
    } else {
        builder.appendBitZero();
    }

    return builder.intoCell();
}

MessageData createSignedMessage(PublicKey& publicKey, PrivateKey& key, bool bounce, uint32_t flags, uint64_t amount, uint32_t expiredAt,
                                Address to, const Cell::Ref& contractData) {
    auto getInitData = [](const PublicKey& publicKey, const Cell::Ref& contractData) {
        if (contractData != nullptr) {
            auto cellSlice = CellSlice(contractData.get());
            return std::make_pair(InitData(cellSlice), /* withInitState */ false);
        } else {
            return std::make_pair(InitData(publicKey), /* withInitState */ true);
        }
    };

    auto [initData, withInitState] = getInitData(publicKey, contractData);

    auto gift = Wallet::Gift{
        .bounce = bounce,
        .amount = amount,
        .to = to,
        .flags = static_cast<uint8_t>(flags),
    };

    auto payload = initData.makeTransferPayload(expiredAt, gift);

    auto payloadCopy = payload;
    auto payloadCell = payloadCopy.intoCell();

    Data data(payloadCell->hash.begin(), payloadCell->hash.end());
    auto signature = key.sign(data, TWCurveED25519);
    payload.prependRaw(signature, static_cast<uint16_t>(signature.size()) * 8);

    auto header = std::make_shared<ExternalInboundMessageHeader>(InitData(publicKey).computeAddr(WorkchainType::Basechain));
    auto message = MessageData(header);

    if (withInitState) {
        message.init = initData.makeStateInit();
    }

    message.body = payload.intoCell();

    return message;
}

} // namespace TW::Everscale
