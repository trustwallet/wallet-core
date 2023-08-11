// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWPublicKey.h"

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
TWData *_Nonnull TWBarzGetInitCode(TWString* _Nonnull factory, struct TWPublicKey* _Nonnull publicKey, TWString* _Nonnull verificationFacet, uint32_t salt);

/// Converts the original ASN-encoded signature from webauthn to the format accepted by Barz
///
/// \param signature Original signature
/// \param challenge The original challenge that was signed
/// \param authenticatorData Returned from Webauthn API
/// \param clientDataJSON Returned from Webauthn API
/// \return Bytes of the formatted signature
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetFormattedSignature(TWData* _Nonnull signature, TWData* _Nonnull challenge, TWData* _Nonnull authenticatorData, TWString* _Nonnull clientDataJSON);
TW_EXTERN_C_END
