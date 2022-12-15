// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Wallet.h"

#include "HexCoding.h"

namespace TW::TheOpenNetwork {

Wallet::Wallet(PublicKey publicKey, int8_t workchainId, Data walletCode)
    : publicKey(publicKey)
    , workchainId(workchainId)
    , walletCode(walletCode)
    , walletId(698983191 + workchainId) {
}

Address Wallet::getAddress() const {
    const auto stateInit = this->createStateInit();
    auto address = Address(workchainId, stateInit.writeTo().intoCell()->hash);

    address.isUserFriendly = true;
    address.isBounceable = true;
    address.isTestOnly = false;

    return address;
}

Everscale::StateInit Wallet::createStateInit() const {
    Cell::Ref code = Cell::deserialize(walletCode.data(), walletCode.size());
    Cell::Ref data = this->createDataCell();
    return StateInit{code, data};
}

Cell::Ref Wallet::createSigningMessage(
    const Address& dest,
    uint64_t amount,
    uint32_t seqno,
    uint8_t mode,
    uint32_t expireAt,
    const std::string& comment
) const {
    const auto header = std::make_shared<Everscale::InternalMessageHeader>(true, dest.isBounceable, dest, amount);
    TheOpenNetwork::Message internalMessage = TheOpenNetwork::Message(header);

    CellBuilder bodyBuilder;
    if (comment.size() > 0) { // TODO(vbushev): test and finalize this branch
        const auto& data = Data(comment.begin(), comment.end());
        bodyBuilder.appendU32(0);
        bodyBuilder.appendRaw(data, data.size() * 8);
    }
    internalMessage.setBody(bodyBuilder.intoCell());

    CellBuilder builder;
    this->writeSigningPayload(builder, seqno, expireAt);
    builder.appendU8(mode);
    builder.appendReferenceCell(internalMessage.intoCell());
    return builder.intoCell();
}

Cell::Ref Wallet::createTransferMessage(
    const PrivateKey& privateKey,
    const Address& dest,
    uint64_t amount,
    uint32_t seqno,
    uint8_t mode,
    uint32_t expireAt,
    const std::string& comment
) const {
    const Cell::Ref signingMessage = this->createSigningMessage(dest, amount, seqno, mode, expireAt, comment);

    Data data(signingMessage->hash.begin(), signingMessage->hash.end());
    const auto signature = privateKey.sign(data, TWCurveED25519);

    CellBuilder bodyBuilder;
    bodyBuilder.appendRaw(signature, static_cast<uint16_t>(signature.size()) * 8);
    bodyBuilder.appendCellSlice(CellSlice(signingMessage.get()));

    const auto transferMessageHeader = std::make_shared<ExternalInboundMessageHeader>(this->getAddress());
    Message transferMessage = Message(transferMessageHeader);
    if (seqno == 0) {
        const auto stateInit = this->createStateInit();
        transferMessage.setStateInit(stateInit);
    }
    transferMessage.setBody(bodyBuilder.intoCell());

    return transferMessage.intoCell();
}

} // namespace TW::TheOpenNetwork
