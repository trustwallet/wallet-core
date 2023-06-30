// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWWebAuthn.h>
#include <string>
#include "WebAuthn.h"

struct TWPublicKey *_Nullable TWWebAuthnGetPublicKey(TWData *_Nonnull attestationObject) {
    const auto& attestationObjectData = *reinterpret_cast<const TW::Data*>(attestationObject);
    const auto publicKey = TW::WebAuthn::getPublicKey(attestationObjectData);
    if (publicKey.has_value()) {
        return new TWPublicKey{ TW::PublicKey(publicKey.value()) };
    } else {
        return nullptr;
    }
}
