// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
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

/// Returns the final hash to be signed by Barz for signing messages & typed data
///
/// \param msgHash Original msgHash
/// \param barzAddress The address of Barz wallet signing the message
/// \param chainId The chainId of the network the verification will happen
/// \return The final hash to be signed
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetPrefixedMsgHash(TWData* _Nonnull msgHash, TWString* _Nonnull barzAddress, uint32_t chainId);

/// Returns the encoded diamondCut function call for Barz contract upgrades
///
/// \param input The serialized data of DiamondCutInput
/// \return The encoded bytes of diamondCut function call
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetDiamondCutCode(TWData *_Nonnull input);
TW_EXTERN_C_END
