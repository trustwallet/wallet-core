// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "Base64.h"
#include "Crc.h"

#include "WorkchainType.h"

namespace TW::TheOpenNetwork {

using AddressImpl = TW::CommonTON::RawAddress;

static inline Data decodeUserFriendlyAddress(const std::string& string) {
    Data decoded;
    if (string.find('-') != std::string::npos || string.find('_') != std::string::npos) {
        decoded = Base64::decodeBase64Url(string);
    } else {
        decoded = Base64::decode(string);
    }
    return decoded;
}

bool Address::isValid(const std::string& string) noexcept {
    if (AddressImpl::isValid(string)) {
        return true;
    }

    if (string.size() != b64UserFriendlyAddressLen) {
        return false;
    }

    if (!Base64::isBase64orBase64Url(string)) {
        return false;
    }
    Data decoded = decodeUserFriendlyAddress(string);

    if (decoded.size() != userFriendlyAddressLen) {
        return false;
    }

    byte tag = decoded[0];
    if (tag & AddressTag::TEST_ONLY) {
        tag ^= AddressTag::TEST_ONLY;
    }

    if (tag != AddressTag::BOUNCEABLE && tag != AddressTag::NON_BOUNCEABLE) {
        return false;
    }

    int8_t workchainId = decoded[1];
    if (workchainId != WorkchainType::Basechain && workchainId != WorkchainType::Masterchain) {
        return false;
    }

    Data data(decoded.begin(), decoded.end() - 2);
    Data givenCrc(decoded.end() - 2, decoded.end());

    const uint16_t crc16 = Crc::crc16(data.data(), (uint32_t) data.size());
    const byte b1 = (crc16 >> 8) & 0xff;
    const byte b2 = crc16 & 0xff;
    if (b1 != givenCrc[0] || b2 != givenCrc[1]) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string, std::optional<bool> bounceable) {
    if (!Address::isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }

    if (string.find(':') != std::string::npos) {
        addressData = AddressImpl::splitAddress(string);
    } else {
        isUserFriendly = true;

        Data decoded = decodeUserFriendlyAddress(string);
        addressData.workchainId = decoded[1];

        if (!bounceable.has_value()) {
            byte tag = decoded[0];
            if (tag & AddressTag::TEST_ONLY) {
                isTestOnly = true;
                tag ^= AddressTag::TEST_ONLY;
            }
            isBounceable = (tag == AddressTag::BOUNCEABLE);
        } else {
            isBounceable = *bounceable;
        }

        std::copy(decoded.begin() + 2, decoded.end() - 2, addressData.hash.begin());
    }
}

std::string Address::string() const {
    return this->string(isUserFriendly, isBounceable, isTestOnly);
}

std::string Address::string(bool userFriendly, bool bounceable, bool testOnly)  const {
    if (!userFriendly) {
        return AddressImpl::to_string(addressData);
    }

    Data data;
    Data hashData(addressData.hash.begin(), addressData.hash.end());

    byte tag = bounceable ? AddressTag::BOUNCEABLE : AddressTag::NON_BOUNCEABLE;
    if (testOnly) {
        tag |= AddressTag::TEST_ONLY;
    }

    append(data, tag);
    append(data, addressData.workchainId);
    append(data, hashData);

    const uint16_t crc16 = Crc::crc16(data.data(), (uint32_t) data.size());
    append(data, (crc16 >> 8) & 0xff);
    append(data, crc16 & 0xff);

    return Base64::encodeBase64Url(data);
}

} // namespace TW::TheOpenNetwork
