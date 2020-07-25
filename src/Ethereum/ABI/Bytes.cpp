// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bytes.h"
#include "ParamNumber.h"
#include "ValueEncoder.h"

namespace TW::Ethereum::ABI {

void ParamByteArray::encodeBytes(const Data& bytes, Data& data) {
    ValueEncoder::encodeUInt256(uint256_t(bytes.size()), data);

    const auto count = bytes.size();
    const auto padding = ValueEncoder::padNeeded32(count);
    data.insert(data.end(), bytes.begin(), bytes.begin() + count);
    append(data, Data(padding));
}

bool ParamByteArray::decodeBytes(const Data& encoded, Data& decoded, size_t& offset_inout) {
    size_t origOffset = offset_inout;
    // read len
    uint256_t len256;
    if (!ABI::decode(encoded, len256, offset_inout)) {
        return false;
    }
    // check if length is in the size_t range
    size_t len = static_cast<size_t>(len256);
    if (len256 != uint256_t(len)) {
        return false;
    }
    // check if there is enough data
    if (encoded.size() < offset_inout + len) {
        return false;
    }
    // read data
    decoded = Data(encoded.begin() + offset_inout, encoded.begin() + offset_inout + len);
    offset_inout += len;
    // padding
    offset_inout = origOffset + ValueEncoder::paddedTo32(offset_inout - origOffset);
    return true;
}

void ParamByteArrayFix::encode(Data& data) const {
    const auto count = _bytes.size();
    const auto padding = ValueEncoder::padNeeded32(count);
    data.insert(data.end(), _bytes.begin(), _bytes.begin() + count);
    append(data, Data(padding));
}

bool ParamByteArrayFix::decodeBytesFix(const Data& encoded, size_t n, Data& decoded,
                                       size_t& offset_inout) {
    size_t origOffset = offset_inout;
    if (encoded.size() < offset_inout + n) {
        // not enough data
        return false;
    }
    if (decoded.size() < n) {
        append(decoded, Data(n - decoded.size()));
    }
    std::copy(encoded.begin() + offset_inout, encoded.begin() + (offset_inout + n),
              decoded.begin());
    offset_inout += n;
    // padding
    offset_inout = origOffset + ValueEncoder::paddedTo32(offset_inout - origOffset);
    return true;
}

void ParamString::encodeString(const std::string& decoded, Data& data) {
    auto bytes = Data(decoded.begin(), decoded.end());
    ParamByteArray::encodeBytes(bytes, data);
}

bool ParamString::decodeString(const Data& encoded, std::string& decoded, size_t& offset_inout) {
    Data decodedData;
    if (!ParamByteArray::decodeBytes(encoded, decodedData, offset_inout)) {
        return false;
    }
    decoded = std::string(decodedData.begin(), decodedData.end());
    return true;
}

} // namespace TW::Ethereum::ABI
