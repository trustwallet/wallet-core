#include "Messages.h"
#include "WorkchainType.h"

using namespace TW;
using namespace TW::Everscale;

void ExternalInboundMessageHeader::writeTo(CellBuilder& builder) const {
    builder.appendBitOne();
    builder.appendBitZero();

    // addr src (none)
    builder.appendRaw(Data{0x00}, 2);

    // addr dst
    Data dstAddr(dst_.second.begin(), dst_.second.end());

    Data prefix{0x80};
    builder.appendRaw(prefix, 2);

    builder.appendBitZero();
    builder.appendi8(dst_.first);
    builder.appendRaw(dstAddr, 256);

    // fee
    builder.appendU128(importFee_);
}

void InternalMessageHeader::writeTo(CellBuilder& builder) const {
    //tag
    builder.appendBitZero();

    builder.appendBitBool(ihrDisabled_);
    builder.appendBitBool(bounce_);
    builder.appendBitBool(bounced_);

    // addr src (none)
    builder.appendRaw(Data{0x00}, 2);

    // addr dst
    Data dstAddr(dst_.second.begin(), dst_.second.end());

    Data prefix{0x80};
    builder.appendRaw(prefix, 2);

    builder.appendBitZero();
    builder.appendi8(dst_.first);
    builder.appendRaw(dstAddr, 256);

    // value
    builder.appendU128(value_);
    builder.appendBitZero();

    // fee
    builder.appendU128(ihrFee_);
    builder.appendU128(fwdFee_);

    // created
    builder.appendU64(createdLt_);
    builder.appendU32(createdAt_);
}

Cell::Ref Message::intoCell() const {
    CellBuilder builder;

    // write Header
    header_->writeTo(builder);

    // write StateInit
    if (init_.has_value()) {
        auto initBuilder = init_.value().writeTo();

        builder.appendBitOne();
        builder.appendBitZero();
        builder.appendBuilder(initBuilder);
    } else {
        builder.appendBitZero();
    }

    // write Body
    if (body_.has_value()) {
        builder.appendBitZero();
        builder.appendCellSlice(body_.value());
    } else {
        builder.appendBitZero();
    }

    return builder.intoCell();
}

Data TW::Everscale::createSignedMessage(PublicKey& publicKey, PrivateKey& key, bool bounce, uint32_t flags, uint64_t amount, uint32_t expiredAt,
    std::optional<MsgAddressInt>& destination, std::optional<Data>& stateInit)
{
    auto getInitData = [&](){
        if (stateInit.has_value()) {
            auto cell = Cell::deserialize(stateInit.value().data(), stateInit.value().size());
            auto cellSlice = CellSlice(cell.get());
            return std::make_pair(InitData(cellSlice), true);
        } else {
            return std::make_pair(InitData(publicKey), false);
        }
    };

    auto getDstAddress = [&](InitData& initData) {
        if (!destination.has_value()) {
            return initData.computeAddr(WorkchainType::Basechain);
        } else {
            return destination.value();
        }
    };

    auto [initData, withInitState] = getInitData();

    auto gift = Wallet::Gift {
        .bounce = bounce,
        .amount = amount,
        .destination = getDstAddress(initData),
        .flags =  static_cast<uint8_t>(flags),
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

    return message.intoCell()->data;
}
