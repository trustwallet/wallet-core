// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

namespace TW::ASN {

struct AsnParser {
    static std::optional<Data> ecdsa_signature_from_der(const Data& derEncoded) {
        Rust::CByteArrayResultWrapper res = Rust::ecdsa_signature_from_asn_der(derEncoded.data(), derEncoded.size());
        if (!res.isOk()) {
            return std::nullopt;
        }
        return res.unwrap().data;
    }
};

} // namespace TW::ASN
