#include "Address.h"
#include "../HexCoding.h"
#include "../../trezor-crypto/include/TrezorCrypto/hasher.h"
#include "../../trezor-crypto/include/TrezorCrypto/ecdsa.h"
#include "../Bech32.h"

using namespace TW;
using namespace TW::TERRA;

// проверка адресса по префиксу
bool Address::isValid(const std::string &string) {
    std::string prefix = "terra";
    return std::equal(prefix.begin(), prefix.end(), string.begin());
}


//вызываем метод элептических кривых. в который передаем публичный ключ, функцию SHA256, хеш публичного ключа
Address::Address(std::string hrp, const PublicKey& publicKey) : hrp(std::move(hrp)), keyHash() {
    keyHash.resize(20);
    ecdsa_get_pubkeyhash(publicKey.compressed().bytes.data(), HASHER_SHA2_RIPEMD, keyHash.data());
}


Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address");
    }

    auto dec = Bech32::decode(string).second;
}

std::string Address::string() const {
    Data enc;
    Bech32::convertBits<8, 5, true>(enc, keyHash);
    std::string result = Bech32::encode(hrp, enc);
    if (!Address::isValid(result)) {
        return nullptr;
    }
    return result;
}


