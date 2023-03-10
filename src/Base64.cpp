// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"

namespace TW::Base64::internal {

std::string encode(const Data& val, bool is_url) {
    char* encoded = Rust::encode_base64(val.data(), val.size(), is_url);
    std::string encoded_str(encoded);
    Rust::free_string(encoded);
    return encoded_str;
}

Data decode(const std::string& val, bool is_url) {
    if (val.empty()) {
        return Data();
    }
    auto decoded = Rust::decode_base64(val.c_str(), is_url);
    if (decoded.data == nullptr || decoded.size == 0) {
        return Data();
    }
    std::vector<uint8_t> decoded_vec(&decoded.data[0], &decoded.data[decoded.size]);
    std::free(decoded.data);
    return decoded_vec;
}

}

namespace TW::Base64 {

using namespace TW;
using namespace std;

static bool isBase64Any(const string& val, const char* alphabet) {
    if (val.length() % 4 != 0) {
        return false;
    }
    size_t first_non_alphabet = val.find_first_not_of(alphabet);
    size_t first_non_padding = val.find_first_not_of("=", first_non_alphabet);

    if (first_non_alphabet == std::string::npos ||
        (first_non_padding == std::string::npos && (val.length() - first_non_alphabet < 3))) {
        return true;
    }
    return false;
}

bool isBase64orBase64Url(const string& val) {
    const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const char* base64_url_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    return isBase64Any(val, base64_chars) || isBase64Any(val, base64_url_chars);
}

Data decodeBase64Url(const string& val) {
    return internal::decode(val, true);
}

string encodeBase64Url(const Data& val) {
    return internal::encode(val, true);
}

std::string encode(const Data& val) {
   return internal::encode(val, false);
}

Data decode(const string& val) {
   return internal::decode(val, false);
}

} // namespace TW::Base64
