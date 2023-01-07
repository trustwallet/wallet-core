#include "Messages.h"
#include "WorkchainType.h"

namespace TW::CommonTON {

using CellSlice = Everscale::CellSlice;
using InitData = Everscale::InitData;
using Wallet = Everscale::Wallet;

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

    // currency collections
    builder.appendBitZero();

    // fee
    builder.appendU128(_ihrFee);
    builder.appendU128(_fwdFee);

    // created
    builder.appendU64(_createdLt);
    builder.appendU32(_createdAt);
}

MessageData createSignedMessage(PublicKey& publicKey, PrivateKey& key, bool bounce, uint32_t flags, uint64_t amount, uint32_t expiredAt,
                         AddressData to, const Cell::Ref& contractData) {
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

} // namespace TW::CommonTON