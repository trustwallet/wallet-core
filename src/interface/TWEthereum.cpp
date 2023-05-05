// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Data.h"
#include "Ethereum/EIP1014.h"
#include "Ethereum/EIP4337.h"
#include "Ethereum/EIP2645.h"
#include <TrustWalletCore/TWEthereum.h>

#include <string>

TWString* TWEthereumEip2645GetPath(TWString* ethAddress, TWString* layer, TWString* application, TWString* index) {
    const auto& ethAddressStr = *reinterpret_cast<const std::string*>(ethAddress);
    const auto& layerStr = *reinterpret_cast<const std::string*>(layer);
    const auto& applicationStr = *reinterpret_cast<const std::string*>(application);
    const auto& indexStr = *reinterpret_cast<const std::string*>(index);
    return new std::string(TW::Ethereum::accountPathFromAddress(ethAddressStr, layerStr, applicationStr, indexStr));
}

TWString* TWEthereumEip4337GetEIP4337AddressFromOwnerBytes(TWString* _Nonnull factoryAddress, TWString* _Nonnull bytecode, TWString* _Nonnull diamondCutFacetAddress, TWString* _Nonnull accountFacetAddress, TWString* _Nonnull verificationFacetAddress, TWString* _Nonnull entryPointAddress, TWString* _Nonnull securityManagerAddress, TWString* _Nonnull facetRegistryAddress, TWString* _Nonnull ownerPublicKey) {
    const auto& factoryAddressStr = *reinterpret_cast<const std::string*>(factoryAddress);
    const auto& bytecodeStr = *reinterpret_cast<const std::string*>(bytecode);
    const auto& diamondCutFacetAddressStr = *reinterpret_cast<const std::string*>(accountFacetAddress);
    const auto& verificationFacetAddressStr = *reinterpret_cast<const std::string*>(verificationFacetAddress);
    const auto& entryPointAddressStr = *reinterpret_cast<const std::string*>(entryPointAddress);
    const auto& securityManagerAddressStr = *reinterpret_cast<const std::string*>(securityManagerAddress);
    const auto& facetRegistryAddressStr = *reinterpret_cast<const std::string*>(facetRegistryAddress);
    const auto& ownerPublicKeyStr = *reinterpret_cast<const std::string*>(ownerPublicKey);

    return new std::string(TW::Ethereum::getEIP4337AddressFromOwnerBytes(factoryAddressStr, bytecodeStr, diamondCutFacetAddressStr, verificationFacetAddressStr, entryPointAddressStr, entryPointAddressStr, securityManagerAddressStr, facetRegistryAddressStr, ownerPublicKeyStr));
}

TWString* TWEthereumEip4337GetEIP4337AddressFromOwnerAddress(TWString* _Nonnull factoryAddress, TWString* _Nonnull bytecode, TWString* _Nonnull diamondCutFacetAddress, TWString* _Nonnull accountFacetAddress, TWString* _Nonnull verificationFacetAddress, TWString* _Nonnull entryPointAddress, TWString* _Nonnull securityManagerAddress, TWString* _Nonnull facetRegistryAddress, TWString* _Nonnull ownerAddress) {
    const auto& factoryAddressStr = *reinterpret_cast<const std::string*>(factoryAddress);
    const auto& bytecodeStr = *reinterpret_cast<const std::string*>(bytecode);
    const auto& diamondCutFacetAddressStr = *reinterpret_cast<const std::string*>(accountFacetAddress);
    const auto& verificationFacetAddressStr = *reinterpret_cast<const std::string*>(verificationFacetAddress);
    const auto& entryPointAddressStr = *reinterpret_cast<const std::string*>(entryPointAddress);
    const auto& securityManagerAddressStr = *reinterpret_cast<const std::string*>(securityManagerAddress);
    const auto& facetRegistryAddressStr = *reinterpret_cast<const std::string*>(facetRegistryAddress);
    const auto& ownerAddressStr = *reinterpret_cast<const std::string*>(ownerAddress);

    return new std::string(TW::Ethereum::getEIP4337AddressFromOwnerBytes(factoryAddressStr, bytecodeStr, diamondCutFacetAddressStr, verificationFacetAddressStr, entryPointAddressStr, entryPointAddressStr, securityManagerAddressStr, facetRegistryAddressStr, ownerAddressStr));
}
TWString* TWEthereumEip4337GetEIP4337AddressFromOwnerAttestationObject(TWString* _Nonnull factoryAddress, TWString* _Nonnull bytecode, TWString* _Nonnull diamondCutFacetAddress, TWString* _Nonnull accountFacetAddress, TWString* _Nonnull verificationFacetAddress, TWString* _Nonnull entryPointAddress, TWString* _Nonnull securityManagerAddress, TWString* _Nonnull facetRegistryAddress, TWString* _Nonnull ownerAttestationObject) {
    const auto& factoryAddressStr = *reinterpret_cast<const std::string*>(factoryAddress);
    const auto& bytecodeStr = *reinterpret_cast<const std::string*>(bytecode);
    const auto& diamondCutFacetAddressStr = *reinterpret_cast<const std::string*>(accountFacetAddress);
    const auto& verificationFacetAddressStr = *reinterpret_cast<const std::string*>(verificationFacetAddress);
    const auto& entryPointAddressStr = *reinterpret_cast<const std::string*>(entryPointAddress);
    const auto& securityManagerAddressStr = *reinterpret_cast<const std::string*>(securityManagerAddress);
    const auto& facetRegistryAddressStr = *reinterpret_cast<const std::string*>(facetRegistryAddress);
    const auto& ownerAttestationObjectStr = *reinterpret_cast<const std::string*>(ownerAttestationObject);

    return new std::string(TW::Ethereum::getEIP4337AddressFromOwnerBytes(factoryAddressStr, bytecodeStr, diamondCutFacetAddressStr, verificationFacetAddressStr, entryPointAddressStr, entryPointAddressStr, securityManagerAddressStr, facetRegistryAddressStr, ownerAttestationObjectStr));
}

TWString* TWEthereumEip4337GetDeploymentAddress(TWString* _Nonnull factoryAddress, TWString* _Nonnull logicAddress, TWString* _Nonnull ownerAddress) {
    const auto& factoryAddressStr = *reinterpret_cast<const std::string*>(factoryAddress);
    const auto& logicAddressStr = *reinterpret_cast<const std::string*>(logicAddress);
    const auto& ownerAddressStr = *reinterpret_cast<const std::string*>(ownerAddress);
    return new std::string(TW::Ethereum::getEIP4337DeploymentAddress(factoryAddressStr, logicAddressStr, ownerAddressStr));
}