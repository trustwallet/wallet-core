// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CashAddress.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/cash_addr.h>

using namespace TW::Bitcoin;

/// Cash address human-readable part
static const char* const cashHRP = "bitcoincash";

static const size_t maxHRPSize = 20;
static const size_t maxDataSize = 104;

bool CashAddress::isValid(const std::string& string) {
    char hrp[maxHRPSize + 1];
    uint8_t data[maxDataSize];
    size_t dataLen;
    if (cash_decode(hrp, data, &dataLen, string.c_str()) == 0 || dataLen != CashAddress::size) {
        return false;
    }
    if (strcmp(hrp, cashHRP) != 0) {
        return false;
    }
    return true;
}

CashAddress::CashAddress(const std::string& string) {
    uint8_t data[maxDataSize];
    char hrp[maxHRPSize + 1];
    size_t dataLen;
    auto success = cash_decode(hrp, data, &dataLen, string.c_str()) != 0;
    assert(success && "Invalid cash address string");
    assert(strcmp(hrp, cashHRP) == 0);
    assert(dataLen == CashAddress::size);
    memcpy(bytes, data, CashAddress::size);
}

CashAddress::CashAddress(const std::vector<uint8_t>& data) {
    assert(isValid(data));
    std::copy(data.begin(), data.end(), bytes);
}

CashAddress::CashAddress(const PublicKey& publicKey) {
    uint8_t payload[21];
    payload[0] = 0;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, payload + 1);

    size_t outlen = 0;
    auto success = cash_addr_to_data(bytes, &outlen, payload, 21) != 0;
    assert(success && outlen == CashAddress::size);
}

std::string CashAddress::string() const {
    char result[129];
    cash_encode(result, cashHRP, bytes, CashAddress::size);
    return result;
}

Address CashAddress::legacyAddress() const {
    std::vector<uint8_t> result(Address::size);
    size_t outlen = 0;
    cash_data_to_addr(result.data(), &outlen, bytes, CashAddress::size);
    assert(outlen == 21 && "Invalid length");
    return Address(result);
}
