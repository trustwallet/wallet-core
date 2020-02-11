#include "Address.h"

#include <stdexcept>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ripemd160.h>

#include "../BinaryCoding.h"
#include "../Base58.h"
#include "../Hash.h"

using namespace TW::Bravo;
using namespace TW;

const std::vector<std::string> Address::prefixes = {"BRV", "TST"};

const auto ripemd = static_cast<Hash::HasherSimpleType>(&Hash::ripemd);

bool Address::isValid(const std::string& string, const std::vector<std::string>& validPrefixes) {
    return extractKeyData(string, validPrefixes);
}

/// Extracts and verifies the key data from a base58 string.
/// If address is non-null, the bytes and the prefix are
/// copied to that object.
bool Address::extractKeyData(const std::string& string, const std::vector<std::string>& validPrefixes, Address *address) {
    // verify if the string has one of the valid prefixes
    std::string prefix;
    int i = 0;
    while (i < validPrefixes.size()) {
        prefix = validPrefixes[i];
        if (string.substr(0, prefix.size()) == prefix) {
            break;
        }
        i++;
    }

    if (i == validPrefixes.size()) {
        return false;
    }

    // the binary data will easily fit in a buffer of the size of base58 data
    Data bytes = Base58::bitcoin.decodeCheck(string.substr(prefix.size()), ripemd);

    if (bytes.size() != Size) {
        return false;
    }

    if (address) {
        address->bytes = bytes;
        address->prefix = prefix;
    }

    return true;
}

/// Initializes a Bravo address from a string representation.
Address::Address(const std::string& string, const std::vector<std::string>& validPrefixes) {
    if (!Address::extractKeyData(string, validPrefixes, this)) {
        throw std::invalid_argument("Invalid address string!");
    }
}

/// Initializes a Bravo address from raw bytes
Address::Address(const Data& data, std::string prefix) : bytes(data), prefix(prefix) { }

/// Initializes a Bravo address from a public key.
Address::Address(const PublicKey& publicKey, std::string prefix) : prefix(prefix) {
    static_assert(Size == TW::PublicKey::secp256k1Size);

    // copy the raw, compressed key data
    bytes = publicKey.compressed().bytes;
}

/// Returns a string representation of the Bravo address.
std::string Address::string() const {
    return prefix + Base58::bitcoin.encodeCheck(bytes, ripemd);
}