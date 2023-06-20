// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
