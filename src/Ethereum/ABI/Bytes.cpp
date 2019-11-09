// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bytes.h"

namespace TW::Ethereum {

void encode(const Data& bytes, Data& data) {
    encode(uint256_t(bytes.size()), data);

    const auto count = bytes.size();
    const auto padding = padNeeded32(count);
    data.insert(data.end(), bytes.begin(), bytes.begin() + count);
    append(data, Data(padding));
}

bool decode(const Data& encoded, Data& decoded, size_t& offset_inout) {
    size_t origOffset = offset_inout;
    // read len
    uint256_t len256;
    if (!decode(encoded, len256, offset_inout)) { return false; }
    // check if length is in the size_t range
    size_t len = static_cast<size_t>(len256);
    if (len256 != static_cast<uint256_t>(len)) { return false; }
    // check if there is enough data
    if (encoded.size() < offset_inout + len) { return false; }
    // read data
    decoded = Data(encoded.begin() + offset_inout, encoded.begin() + offset_inout + len);
    offset_inout += len;
    // padding
    offset_inout = origOffset + paddedTo32(offset_inout - origOffset);
    return true;
}

bool decode(const Data& encoded, std::string& decoded, size_t& offset_inout) {
    Data decodedData;
    if (!decode(encoded, decodedData, offset_inout)) { return false; }
    decoded = std::string(decodedData.begin(), decodedData.end());
    return true;
}

} // namespace TW::Ethereum
