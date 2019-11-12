// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base64.h"

#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace TW::Base64 {

using namespace TW;

Data decode(const std::string& val) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    return boost::algorithm::trim_right_copy_if(Data(It(std::begin(val)), It(std::end(val))),
                                                [](char c) { return c == '\0'; });
}

/// Convert from Base64Url format to regular
bool convertBase64Url(const std::string& base64Url_in, std::string& base64Std_out) {
    // Base64URL format ('+' and '/' are replaced by '-' and '_')
    if (base64Url_in.find('-') == std::string::npos &&
        base64Url_in.find('_') == std::string::npos) {
        // no Base64Url-specific characters
        return false;
    }
    // make copy, replace
    base64Std_out = std::string(base64Url_in.c_str());
    for (auto i = base64Std_out.begin(), n = base64Std_out.end(); i != n; ++i) {
        if (*i == '-') { *i = '+'; }
        else if (*i == '_') { *i = '/'; }
    }
    return true;
}

Data decodeBase64Url(const std::string& val) {
    Data bytes;
    try {
        return decode(val);
    } catch (const std::exception& ex) {
        // 2nd try: Base64URL format (replaced by '-' and '_' by '+' and '/' )
        std::string base64Url;
        if (!convertBase64Url(val, base64Url)) {
            // not valid base64url
            throw ex;
        }
        // looks to be URL format
        return decode(base64Url);
    }
}

std::string encode(const Data& val) {
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<Data::const_iterator, 6, 8>>;
    auto encoded = std::string(It(std::begin(val)), It(std::end(val)));
    return encoded.append((3 - val.size() % 3) % 3, '=');
}

} // namespace TW::Base64
