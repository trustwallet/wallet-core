// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Barz functions
TW_EXPORT_STRUCT
struct TWBarz;

/// Calculate a counterfactual address for the smart contract wallet
///
/// \param input The serialized data of ContractAddressInput.
/// \return The address.
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBarzGetCounterfactualAddress(TWData *_Nonnull input);

/// Returns the init code parameter of ERC-4337 User Operation
///
/// \param factory Wallet factory address (BarzFactory)
/// \param publicKey Public key for the verification facet
/// \param verificationFacet Verification facet address
/// \return The address.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetInitCodeFromPublicKey(TWString* _Nonnull factory, TWString* _Nonnull publicKey, TWString* _Nonnull verificationFacet);

/// Returns the init code parameter of ERC-4337 User Operation
///
/// \param factory Wallet factory address (BarzFactory)
/// \param attestationObject Attestation object from created webauthn credentials
/// \param verificationFacet Verification facet address
/// \return The address.
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetInitCodeFromAttestationObject(TWString* _Nonnull factory, TWString* _Nonnull attestationObject, TWString* _Nonnull verificationFacet);

/// Converts the original ASN-encoded signature from webauthn to the format accepted by Barz
///
/// \param signature Original signature
/// \param authenticatorData Hex encoded authenticator data
/// \param origin URL of the origin from clientDataJSON
/// \return Bytes of the formatted signature
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetFormattedSignature(TWData* _Nonnull signature, TWData* _Nonnull authenticatorData, TWString* _Nonnull origin);
TW_EXTERN_C_END
