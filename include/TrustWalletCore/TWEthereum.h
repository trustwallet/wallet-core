// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWEthereum;

/// Generate a layer 2 eip2645 derivation path from eth address, layer, application and given index.
///
/// \param wallet non-null TWHDWallet
/// \param ethAddress non-null Ethereum address
/// \param layer  non-null layer 2 name (E.G starkex)
/// \param application non-null layer 2 application (E.G immutablex)
/// \param index non-null layer 2 index (E.G 1)
/// \return a valid eip2645 layer 2 derivation path as a string
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumEip2645GetPath(TWString* _Nonnull ethAddress, TWString* _Nonnull layer,  TWString* _Nonnull application,  TWString* _Nonnull index);

/// Generates a deployment address for a ERC-4337 compatible smart contract wallet
///
/// \param factoryAddress non-null address of the account factory
/// \param logicAddress non-null address of the wallet's logic smart contract
/// \param ownerAddress  non-null address of the signing key that controls the smart contract wallet
/// \return Ethereum resulting address
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWEthereumEip4337GetDeploymentAddress(TWString* _Nonnull factoryAddress, TWString* _Nonnull logicAddress, TWString* _Nonnull ownerAddress);

/// Generates a deployment address for a ERC-4337 compatible smart contract wallet with a generic ownership scheme
///
/// \param factoryAddress non-null address of the account factory
/// \param bytecode non-null bytecode of the smart contract wallet
/// \param diamondCutFacetAddress  non-null address of the diamond cut facet
/// \param accountFacetAddress  non-null address of the account facet
/// \param verificationFacetAddress  non-null address of the verification facet
/// \param entryPointAddress  non-null address of the entry point
/// \param securityManagerAddress  non-null address of the security manager
/// \param facetRegistryAddress  non-null address of the facet registry
/// \param ownerPublicKey  non-null bytes of the public key that controls the account
/// \return Ethereum resulting address
TW_EXPORT_STATIC_METHOD
TWString* TWEthereumEip4337GetEIP4337AddressFromOwnerBytes(TWString* _Nonnull factoryAddress, TWString* _Nonnull bytecode, TWString* _Nonnull diamondCutFacetAddress, TWString* _Nonnull accountFacetAddress, TWString* _Nonnull verificationFacetAddress, TWString* _Nonnull entryPointAddress, TWString* _Nonnull securityManagerAddress, TWString* _Nonnull facetRegistryAddress, TWString* _Nonnull ownerPublicKey);

/// Generates a deployment address for a ERC-4337 compatible smart contract wallet with a EOA ownership scheme
///
/// \param factoryAddress non-null address of the account factory
/// \param bytecode non-null bytecode of the smart contract wallet
/// \param diamondCutFacetAddress  non-null address of the diamond cut facet
/// \param accountFacetAddress  non-null address of the account facet
/// \param verificationFacetAddress  non-null address of the verification facet
/// \param entryPointAddress  non-null address of the entry point
/// \param securityManagerAddress  non-null address of the security manager
/// \param facetRegistryAddress  non-null address of the facet registry
/// \param ownerAddress  non-null EOA address of the account owner
/// \return Ethereum resulting address
TW_EXPORT_STATIC_METHOD
TWString* TWEthereumEip4337GetEIP4337AddressFromOwnerAddress(TWString* _Nonnull factoryAddress, TWString* _Nonnull bytecode, TWString* _Nonnull diamondCutFacetAddress, TWString* _Nonnull accountFacetAddress, TWString* _Nonnull verificationFacetAddress, TWString* _Nonnull entryPointAddress, TWString* _Nonnull securityManagerAddress, TWString* _Nonnull facetRegistryAddress, TWString* _Nonnull ownerAddress);

/// Generates a deployment address for a ERC-4337 compatible smart contract wallet with a Webauthn based ownership scheme
///
/// \param factoryAddress non-null address of the account factory
/// \param bytecode non-null bytecode of the smart contract wallet
/// \param diamondCutFacetAddress  non-null address of the diamond cut facet
/// \param accountFacetAddress  non-null address of the account facet
/// \param verificationFacetAddress  non-null address of the verification facet
/// \param entryPointAddress  non-null address of the entry point
/// \param securityManagerAddress  non-null address of the security manager
/// \param facetRegistryAddress  non-null address of the facet registry
/// \param ownerAttestationObject  non-null bytes of the webauthn attestation object
/// \return Ethereum resulting address
TW_EXPORT_STATIC_METHOD
TWString* TWEthereumEip4337GetEIP4337AddressFromOwnerAttestationObject(TWString* _Nonnull factoryAddress, TWString* _Nonnull bytecode, TWString* _Nonnull diamondCutFacetAddress, TWString* _Nonnull accountFacetAddress, TWString* _Nonnull verificationFacetAddress, TWString* _Nonnull entryPointAddress, TWString* _Nonnull securityManagerAddress, TWString* _Nonnull facetRegistryAddress, TWString* _Nonnull ownerAttestationObject);
TW_EXTERN_C_END
