// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPublicKey.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWWebAuthn;

/// Converts attestation object to the public key on P256 curve
///
/// \param attestationObject Attestation object retrieved from webuthn.get method
/// \return Public key.
TW_EXPORT_STATIC_METHOD
struct TWPublicKey *_Nullable TWWebAuthnGetPublicKey(TWData *_Nonnull attestationObject);
TW_EXTERN_C_END