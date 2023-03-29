// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ScryptParameters.h"

#include <TrezorCrypto/rand.h>
#include <limits>

using namespace TW;

namespace TW::Keystore {

ScryptParameters ScryptParameters::Minimal = ScryptParameters(Data(), minimalN, defaultR, minimalP, defaultDesiredKeyLength);
ScryptParameters ScryptParameters::Weak = ScryptParameters(Data(), weakN, defaultR, weakP, defaultDesiredKeyLength);
ScryptParameters ScryptParameters::Standard = ScryptParameters(Data(), standardN, defaultR, standardP, defaultDesiredKeyLength);

ScryptParameters::ScryptParameters()
    : salt(32) {
    random_buffer(salt.data(), salt.size());
}

#pragma GCC diagnostic ignored "-Wtautological-constant-out-of-range-compare"

std::optional<ScryptValidationError> ScryptParameters::validate() const {
    if (desiredKeyLength > ((1ULL << 32) - 1) * 32) { // depending on size_t size on platform, may be always false
        return ScryptValidationError::desiredKeyLengthTooLarge;
    }
    if (static_cast<uint64_t>(r) * static_cast<uint64_t>(p) >= (1 << 30)) {
        return ScryptValidationError::blockSizeTooLarge;
    }
    if ((n & (n - 1)) != 0 || n < 2) {
        return ScryptValidationError::invalidCostFactor;
    }
    if ((r > std::numeric_limits<uint32_t>::max() / 128 / p) ||
        (n > std::numeric_limits<uint32_t>::max() / 128 / r)) {
        return ScryptValidationError::overflow;
    }
    return {};
}

// -----------------
// Encoding/Decoding
// -----------------

namespace CodingKeys::SP {

static const auto salt = "salt";
static const auto desiredKeyLength = "dklen";
static const auto n = "n";
static const auto p = "p";
static const auto r = "r";

} // namespace CodingKeys::SP

ScryptParameters::ScryptParameters(const nlohmann::json& json) {
    salt = parse_hex(json[CodingKeys::SP::salt].get<std::string>());
    desiredKeyLength = json[CodingKeys::SP::desiredKeyLength];
    if (json.count(CodingKeys::SP::n) != 0)
        n = json[CodingKeys::SP::n];
    if (json.count(CodingKeys::SP::n) != 0)
        p = json[CodingKeys::SP::p];
    if (json.count(CodingKeys::SP::n) != 0)
        r = json[CodingKeys::SP::r];
}

/// Saves `this` as a JSON object.
nlohmann::json ScryptParameters::json() const {
    nlohmann::json j;
    j[CodingKeys::SP::salt] = hex(salt);
    j[CodingKeys::SP::desiredKeyLength] = desiredKeyLength;
    j[CodingKeys::SP::n] = n;
    j[CodingKeys::SP::p] = p;
    j[CodingKeys::SP::r] = r;
    return j;
}

} // namespace TW::Keystore
