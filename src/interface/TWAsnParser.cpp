// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAsnParser.h>

#include "AsnParser.h"

using namespace TW;

TWData *_Nullable TWAsnParserEcdsaSignatureFromDer(TWData *_Nonnull encoded) {
    const Data& encodedData = *reinterpret_cast<const Data*>(encoded);
    try {
        auto decoded = ASN::AsnParser::ecdsa_signature_from_der(encodedData);
        if (!decoded) {
            return nullptr;
        }
        return TWDataCreateWithBytes(decoded.value().data(), decoded.value().size());
    } catch (...) {
        return nullptr;
    }
}
