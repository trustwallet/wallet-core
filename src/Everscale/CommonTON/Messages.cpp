#include "Messages.h"

namespace TW::CommonTON {

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

CellBuilder StateInit::writeTo() const {
    CellBuilder builder;

    builder.appendBitZero(); // split_depth
    builder.appendBitZero(); // special
    builder.appendBitOne();  // code
    builder.appendReferenceCell(code);
    builder.appendBitOne(); // data
    builder.appendReferenceCell(data);
    builder.appendBitZero(); // library

    return builder;
}

} // namespace TW::CommonTON