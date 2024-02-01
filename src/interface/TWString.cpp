// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.


#include <TrustWalletCore/TWString.h>
#include <TrezorCrypto/memzero.h>
#include <string>

TWString *_Nonnull TWStringCreateWithUTF8Bytes(const char *_Nonnull bytes) {
    auto* s = new std::string(bytes);
    return s;
}

TWString *_Nonnull TWStringCreateWithRawBytes(const uint8_t *_Nonnull bytes, size_t size) {
    auto* s = new std::string(bytes, bytes + size);
    return s;
}

size_t TWStringSize(TWString *_Nonnull string) {
    auto* s = reinterpret_cast<const std::string*>(string);
    return s->size();
}

char TWStringGet(TWString *_Nonnull string, size_t index) {
    auto* s = reinterpret_cast<const std::string*>(string);
    return (*s)[index];
}

const char *_Nonnull TWStringUTF8Bytes(TWString *_Nonnull string) {
    auto* s = reinterpret_cast<const std::string*>(string);
    return s->c_str();
}

void TWStringDelete(TWString *_Nonnull string) {
    auto *sConst = reinterpret_cast<const std::string*>(string);
    // `const_cast` is safe here despite that the pointer to the string is const
    // but `std::string` is not a constant value.
    auto *s = const_cast<std::string*>(sConst);
    memzero(s->data(), s->size());
    delete sConst;
}

bool TWStringEqual(TWString *_Nonnull lhs, TWString *_Nonnull rhs) {
    auto* lv = reinterpret_cast<const std::string*>(lhs);
    auto* rv = reinterpret_cast<const std::string*>(rhs);
    return *lv == *rv;
}
