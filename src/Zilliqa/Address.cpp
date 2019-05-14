#include "Address.h"
#include "AddressChecksum.h"
#include "stdint.h"
#include "../HexCoding.h"

using namespace TW::Zilliqa;

bool Address::isValid(const std::string& string) {
    if (string.size() != 42 || string[0] != '0' || string[1] != 'x') {
        return false;
    }
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Ziliqa::Address needs SECP256k1 public key");
    }
    const auto data =
        publicKey.hash({}, static_cast<Data (*)(const byte*, const byte*)>(Hash::sha256), false);
    std::copy(data.end() - Address::size, data.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

std::string Address::string() const {
    return checksumed(*this);
}
