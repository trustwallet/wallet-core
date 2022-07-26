#include "Messages.h"

using namespace TW;
using namespace TW::Everscale;

void ExternalInboundMessageHeader::writeTo(CellBuilder& builder) const {
    builder.appendBitOne();
    builder.appendBitZero();

    // addr src (none)
    builder.appendRaw(Data{0x00}, 2);

    // addr dst
    Data dst_addr;
    dst_addr.reserve(32);
    std::copy(dst_.second.begin(), dst_.second.end(), std::begin(dst_addr));

    builder.appendBitZero();
    builder.appendi8(dst_.first);
    builder.appendRaw(dst_addr, 256);

    // ihr fee
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
    Data dst_addr(dst_.second.begin(), dst_.second.end());

    builder.appendBitZero();
    builder.appendi8(dst_.first);
    builder.appendRaw(dst_addr, 256);

    // value
    builder.appendU128(value_);
    builder.appendU32(0);

    // fee
    builder.appendU128(ihrFee_);
    builder.appendU128(fwdFee_);

    // created
    builder.appendU64(createdLt_);
    builder.appendU32(createdAt_);
}

Cell::Ref Message::intoCell() const {
    CellBuilder builder;

    // write header
    header_->writeTo(builder);

    CellBuilder initBuilder;
    if (init_.has_value()) {
        initBuilder.appendReferenceCell(init_.value().intoCell());
    }

    // write StateInit
    if (init_.has_value()) {
        builder.appendBitOne();
        builder.appendReferenceCell(init_.value().intoCell());
    } else {
        builder.appendBitZero();
    }

    // write body
    if (body_.has_value()) {
        builder.appendBitZero();
        builder.appendCellSlice(body_.value());
    } else {
        builder.appendBitZero();
    }

    return builder.intoCell();
}
