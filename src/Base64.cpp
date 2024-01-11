// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base64.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

namespace TW::Base64::internal {

std::string encode(const Data& val, bool is_url) {
    Rust::CStringWrapper res = Rust::encode_base64(val.data(), val.size(), is_url);
    return res.str;
}

Data decode(const std::string& val, bool is_url) {
    if (val.empty()) {
        return {};
    }
    Rust::CByteArrayResultWrapper res = Rust::decode_base64(val.c_str(), is_url);
    return res.unwrap_or_default().data;
}

} // namespace TW::Base64::internal

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
