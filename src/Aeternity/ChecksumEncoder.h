// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Base58.h>
#include <Hash.h>
#include <HexCoding.h>
#include <string>

namespace TW::Aeternity {

class ChecksumEncoder {

  public:
    static const uint8_t checkSumSize = 4;

    /// Encode a byte array into base64 with checksum and a prefix
    static std::string encode(const std::string &prefix, const TW::Data &rawTx) {
        auto hexRawTx = hex(rawTx);
        std::vector<unsigned char> arrayOfByte(checkSumSize);
        auto checksum = Hash::sha256(Hash::sha256(rawTx));
        std::copy(std::begin(checksum), std::end(checksum), std::begin(arrayOfByte));

        auto data = Data();
        append(data, rawTx);
        append(data, arrayOfByte);

        return prefix + TW::Base64::encode(data);
    }

}; // namespace ChecksumEncoder
}