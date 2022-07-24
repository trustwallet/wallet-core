// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "CellBuilder.h"
#include "HexCoding.h"
#include "Wallet.h"
#include "WorkchainType.h"

using namespace TW;
using namespace TW::Everscale;

bool Address::isValid(const std::string& string) {
    auto parsed = parseWorkchainId(string);
    if (!parsed.has_value())
        return false;

    auto [workchainId, pos] = *parsed;

    if (workchainId != WorkchainType::Basechain && workchainId != WorkchainType::Masterchain) {
        return false;
    }

    if (string.size() != pos + 64) {
        return false;
    }

    std::string addr = string.substr(pos);
    return parse_hex(addr).size() == size;
}

Address::Address(const std::string& string) {
    if (!Address::isValid(string)) {
        throw std::invalid_argument("Invalid address string!");
    }

    auto parsed = parseWorkchainId(string);
    auto [workchainId, pos] = *parsed;

    wc = workchainId;

    const auto acc = parse_hex(string.substr(pos));
    std::copy(std::begin(acc), std::end(acc), std::begin(address));
}

Address::Address(const PublicKey& publicKey, int8_t workchainId) {
    wc = workchainId;
    address = computeContractAddress(publicKey);
}

std::string Address::string() const {
    std::string string = std::to_string(wc) + ":" + hex(address);
    return string;
}

std::array<byte, Address::size> Address::computeContractAddress(const PublicKey& publicKey) {
    CellBuilder dataBuilder;
    dataBuilder.appendU32(0);
    dataBuilder.appendU32(WALLET_ID);
    dataBuilder.appendRaw(publicKey.bytes, 256);

    const auto data = dataBuilder.intoCell();
    const auto code = Cell::deserialize(Wallet::code, sizeof(Wallet::code));

    CellBuilder stateInitBuilder;
    stateInitBuilder.appendBitZero(); // split_depth
    stateInitBuilder.appendBitZero(); // special
    stateInitBuilder.appendBitOne();  // code
    stateInitBuilder.appendReferenceCell(code);
    stateInitBuilder.appendBitOne(); // data
    stateInitBuilder.appendReferenceCell(data);
    stateInitBuilder.appendBitZero(); // library

    auto stateInit = stateInitBuilder.intoCell();
    return stateInit->hash;
}

std::optional<std::pair<int8_t, int32_t>> Address::parseWorkchainId(const std::string& string) {
    int8_t workchainId = WorkchainType::Basechain;

    auto pos = string.find(':');
    if (pos != std::string::npos) {
        auto tmp = string.substr(0, pos);
        workchainId = static_cast<int8_t>(std::stoi(tmp));
        ++pos;
    } else {
        return {};
    }

    return std::make_pair(workchainId, pos);
}
