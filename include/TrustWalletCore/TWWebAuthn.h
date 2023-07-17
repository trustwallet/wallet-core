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

/// Uses ASN parser to extract r and s values from a webauthn signature
///
/// \param signature ASN encoded webauthn signature: https://www.w3.org/TR/webauthn-2/#sctn-signature-attestation-types
/// \return Concatenated r and s values.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWWebAuthnGetRSValues(TWData *_Nonnull signature);

/// Reconstructs the original message that was signed via P256 curve. Can be used for signature validation.
///
/// \param authenticatorData Authenticator Data: https://www.w3.org/TR/webauthn-2/#authenticator-data
/// \param clientDataJSON clientDataJSON: https://www.w3.org/TR/webauthn-2/#dom-authenticatorresponse-clientdatajson
/// \return original messages.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWWebAuthnReconstructOriginalMessage(TWData* _Nonnull authenticatorData, TWData* _Nonnull clientDataJSON);
TW_EXTERN_C_END