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

/// Computes an Authorization hash in [EIP-7702 format](https://eips.ethereum.org/EIPS/eip-7702)
/// `keccak256('0x05' || rlp([chain_id, address, nonce]))`.
///
/// \param chainId The chainId of the network
/// \param contractAddress The address of the contract to be authorized
/// \param nonce The nonce of the transaction
/// \return The authorization hash
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetAuthorizationHash(TWData* _Nonnull chainId, TWString* _Nonnull contractAddress, TWData* _Nonnull nonce);

/// Returns the signed authorization hash
///
/// \param chainId The chainId of the network
/// \param contractAddress The address of the contract to be authorized
/// \param nonce The nonce of the transaction
/// \param privateKey The private key
/// \return A json string of the signed authorization
TW_EXPORT_STATIC_METHOD
TWString *_Nonnull TWBarzSignAuthorization(TWData* _Nonnull chainId, TWString* _Nonnull contractAddress, TWData* _Nonnull nonce, TWString* _Nonnull privateKey);

/// Returns the encoded hash of the user operation
///
/// \param chainId The chainId of the network.
/// \param codeAddress The address of the Biz Smart Contract.
/// \param codeName The name of the Biz Smart Contract.
/// \param codeVersion The version of the Biz Smart Contract.
/// \param typeHash The type hash of the transaction.
/// \param domainSeparatorHash The domain separator hash of the wallet.
/// \param sender The address of the UserOperation sender.
/// \param userOpHash The hash of the user operation.
/// \return The encoded hash of the user operation
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetEncodedHash(
    TWData* _Nonnull chainId,
    TWString* _Nonnull codeAddress,
    TWString* _Nonnull codeName,
    TWString* _Nonnull codeVersion,
    TWString* _Nonnull typeHash,
    TWString* _Nonnull domainSeparatorHash,
    TWString* _Nonnull sender,
    TWString* _Nonnull userOpHash);

/// Signs a message using the private key
///
/// \param hash The hash to sign
/// \param privateKey The private key
/// \return The signature
TW_EXPORT_STATIC_METHOD
TWData *_Nonnull TWBarzGetSignedHash(TWString* _Nonnull hash, TWString* _Nonnull privateKey);

TW_EXTERN_C_END
