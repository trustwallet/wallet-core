// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Messages.h"
#include "WorkchainType.h"

using namespace TW;

namespace TW::Everscale {

void ExternalInboundMessageHeader::writeTo(CellBuilder& builder) const {
    builder.appendBitOne();
    builder.appendBitZero();

    // addr src (none)
    builder.appendRaw(Data{0x00}, 2);

    // addr dst
    Data dstAddr(_dst.hash.begin(), _dst.hash.end());

    Data prefix{0x80};
    builder.appendRaw(prefix, 2);

    builder.appendBitZero();
    builder.appendI8(_dst.workchainId);
    builder.appendRaw(dstAddr, 256);

    // fee
    builder.appendU128(_importFee);
}

void InternalMessageHeader::writeTo(CellBuilder& builder) const {
    // tag
    builder.appendBitZero();

    builder.appendBitBool(_ihrDisabled);
    builder.appendBitBool(_bounce);
    builder.appendBitBool(_bounced);

    // addr src (none)
    builder.appendRaw(Data{0x00}, 2);

    // addr dst
    Data dstAddr(_dst.hash.begin(), _dst.hash.end());

    Data prefix{0x80};
    builder.appendRaw(prefix, 2);

    builder.appendBitZero();
    builder.appendI8(_dst.workchainId);
    builder.appendRaw(dstAddr, 256);

    // value
    builder.appendU128(_value);
    builder.appendBitZero();

    // fee
    builder.appendU128(_ihrFee);
    builder.appendU128(_fwdFee);

    // created
    builder.appendU64(_createdLt);
    builder.appendU32(_createdAt);
}

Cell::Ref Message::intoCell() const {
    CellBuilder builder;

    // write Header
    _header->writeTo(builder);

    // write StateInit
    if (_init.has_value()) {
        auto initBuilder = _init.value().writeTo();

        builder.appendBitOne();
        builder.appendBitZero();
        builder.appendBuilder(initBuilder);
    } else {
        builder.appendBitZero();
    }

    // write Body
    if (_body.has_value()) {
        builder.appendBitZero();
        builder.appendCellSlice(_body.value());
    } else {
        builder.appendBitZero();
    }

    return builder.intoCell();
}

Data createSignedMessage(PublicKey& publicKey, PrivateKey& key, bool bounce, uint32_t flags, uint64_t amount, uint32_t expiredAt,
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
    auto message = Message(header);

    if (withInitState) {
        message.setStateInit(initData.makeStateInit());
    }

    auto cell = payload.intoCell();
    auto body = CellSlice(cell.get());

    message.setBody(body);

    const auto messageCell = message.intoCell();

    Data result{};
    messageCell->serialize(result);

    return result;
}

} // namespace TW::Everscale
