// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWWebAuthn.h>
#include <string>
#include "WebAuthn.h"
#include "AsnParser.h"

struct TWPublicKey *_Nullable TWWebAuthnGetPublicKey(TWData *_Nonnull attestationObject) {
    const auto& attestationObjectData = *reinterpret_cast<const TW::Data*>(attestationObject);
    const auto publicKey = TW::WebAuthn::getPublicKey(attestationObjectData);
    if (publicKey.has_value()) {
        return new TWPublicKey{ TW::PublicKey(publicKey.value()) };
    } else {
        return nullptr;
    }
}

TWData *_Nonnull TWWebAuthnGetRSValues(TWData *_Nonnull signature) {
    const auto& signatureData = *reinterpret_cast<const TW::Data*>(signature);
    const auto& rsValues = TW::ASN::AsnParser::ecdsa_signature_from_der(signatureData);
    return TWDataCreateWithData(&rsValues);
}

TWData *_Nonnull TWWebAuthnReconstructOriginalMessage(TWData* _Nonnull authenticatorData, TWData* _Nonnull clientDataJSON) {
    const auto& authenticatorDataConverted = *reinterpret_cast<const TW::Data*>(authenticatorData);
    const auto& clientDataJSONConverted = *reinterpret_cast<const TW::Data*>(clientDataJSON);
    const auto& message = TW::WebAuthn::reconstructSignedMessage(authenticatorDataConverted, clientDataJSONConverted);
    return TWDataCreateWithData(&message);
}
