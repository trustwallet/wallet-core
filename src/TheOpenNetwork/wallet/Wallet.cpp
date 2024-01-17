// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Wallet.h"

#include "HexCoding.h"

namespace TW::TheOpenNetwork {

static const uint32_t standard_wallet_id = 698983191;

Wallet::Wallet(PublicKey publicKey, int8_t workchainId, Data walletCode)
    : publicKey(std::move(publicKey))
    , workchainId(workchainId)
    , walletCode(std::move(walletCode))
    , walletId(standard_wallet_id + workchainId) {
}

Address Wallet::getAddress() const {
    const auto stateInit = this->createStateInit();
    return Address(workchainId, stateInit.writeTo().intoCell()->hash);
}

CommonTON::StateInit Wallet::createStateInit() const {
    Cell::Ref code = Cell::deserialize(walletCode.data(), walletCode.size());
    Cell::Ref data = this->createDataCell();
    return StateInit{code, data};
}

Cell::Ref Wallet::createSigningMessage(
    const Address& dest,
    uint64_t amount,
    uint32_t sequence_number,
    uint8_t mode,
    const Cell::Ref& queryPayload,
    uint32_t expireAt
) const {
    CellBuilder builder;
    this->writeSigningPayload(builder, sequence_number, expireAt);
    builder.appendU8(mode);

    { // Add internal message as a reference cell
        const auto header = std::make_shared<CommonTON::InternalMessageHeader>(true, dest.isBounceable, dest.addressData, amount);
        TheOpenNetwork::Message internalMessage = TheOpenNetwork::Message(MessageData(header));

        internalMessage.setBody(queryPayload);

        builder.appendReferenceCell(internalMessage.intoCell());
    }

    return builder.intoCell();
}

Cell::Ref Wallet::createQueryMessage(
    const PrivateKey& privateKey,
    const Address& dest,
    uint64_t amount,
    uint32_t sequence_number,
    uint8_t mode,
    const Cell::Ref& queryPayload,
    uint32_t expireAt
) const {
    const auto transferMessageHeader = std::make_shared<CommonTON::ExternalInboundMessageHeader>(this->getAddress().addressData);
    Message transferMessage = Message(MessageData(transferMessageHeader));
    if (sequence_number == 0) {
        const auto stateInit = this->createStateInit();
        transferMessage.setStateInit(stateInit);
    }

    { // Set body of transfer message
        CellBuilder bodyBuilder;
        const Cell::Ref signingMessage = this->createSigningMessage(dest, amount, sequence_number, mode, queryPayload, expireAt);
        Data data(signingMessage->hash.begin(), signingMessage->hash.end());
        const auto signature = privateKey.sign(data, TWCurveED25519);

        bodyBuilder.appendRaw(signature, static_cast<uint16_t>(signature.size()) * 8);
        bodyBuilder.appendCellSlice(CellSlice(signingMessage.get()));

        transferMessage.setBody(bodyBuilder.intoCell());
    }

    return transferMessage.intoCell();
}


Cell::Ref Wallet::createTransferMessage(
    const PrivateKey& privateKey,
    const Address& dest,
    uint64_t amount,
    uint32_t sequence_number,
    uint8_t mode,
    uint32_t expireAt,
    const std::string& comment
) const {
    CellBuilder bodyBuilder;
        if (!comment.empty()) {
            const auto& data = Data(comment.begin(), comment.end());
            bodyBuilder.appendU32(0);
            bodyBuilder.appendRaw(data, static_cast<uint16_t>(data.size()) * 8);
    }
    return createQueryMessage(privateKey, dest, amount, sequence_number, mode, bodyBuilder.intoCell(), expireAt);
}


} // namespace TW::TheOpenNetwork
