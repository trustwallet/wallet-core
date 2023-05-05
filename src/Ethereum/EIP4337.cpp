// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ABI.h"
#include "AddressChecksum.h"
#include "EIP1014.h"
#include "Hash.h"
#include "HexCoding.h"
#include <Webauthn.h>

namespace TW::Ethereum {

using ParamBasePtr = std::shared_ptr<ABI::ParamBase>;
using ParamCollection = std::vector<ParamBasePtr>;

// https://github.com/thomas-waite/bundler/blob/b083680059a52d3121c5e33cea67c86652370562/packages/sdk/src/SimpleAccountAPI.ts#L63-L75
Data getEIP4337AccountInitializeBytecode(const std::string& ownerAddress, const std::string& factoryAddress) {
    auto createAccountFunc = ABI::Function("createAccount", ParamCollection{
                                                                std::make_shared<ABI::ParamAddress>(parse_hex(ownerAddress)),
                                                                std::make_shared<ABI::ParamUInt256>(0)});
    Data createAccountFuncEncoded;
    createAccountFunc.encode(createAccountFuncEncoded);

    Data envelope;
    append(envelope, parse_hex(factoryAddress));
    append(envelope, createAccountFuncEncoded);
    return envelope;
}

// https://github.com/eth-infinitism/account-abstraction/blob/5a1ad4072438d9e9f7c934b66464dc05a4b37d02/contracts/samples/SimpleAccountFactory.sol#L48
Data getEIP4337LogicInitializeBytecode(const std::string& ownerAddress) {
    auto initializeFunc = ABI::Function("initialize", ParamCollection{
                                                          std::make_shared<ABI::ParamAddress>(parse_hex(ownerAddress))});
    Data initializeFuncEncoded;
    initializeFunc.encode(initializeFuncEncoded);
    return initializeFuncEncoded;
}

// https://github.com/thomas-waite/bundler/blob/b083680059a52d3121c5e33cea67c86652370562/packages/sdk/src/SimpleAccountAPI.ts#L91-L100
Data getEIP4337ExecuteBytecode(const Data& toAddress, const uint256_t& value, const Data& data) {
    auto executeFunc = ABI::Function("execute", ParamCollection{
                                                    std::make_shared<ABI::ParamAddress>(toAddress),
                                                    std::make_shared<ABI::ParamUInt256>(value),
                                                    std::make_shared<ABI::ParamByteArray>(data)});
    Data executeFuncEncoded;
    executeFunc.encode(executeFuncEncoded);
    return executeFuncEncoded;
}

// https://github.com/eth-infinitism/account-abstraction/blob/5a1ad4072438d9e9f7c934b66464dc05a4b37d02/contracts/samples/SimpleAccountFactory.sol#L43-L51
std::string getEIP4337DeploymentAddress(const std::string& factoryAddress, const std::string& logicAddress, const std::string& ownerAddress) {
    const Data logicInitializeBytecode = getEIP4337LogicInitializeBytecode(ownerAddress);
    const Data proxyInitCode = getEIP1967ProxyInitCode(logicAddress, logicInitializeBytecode);
    const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
    const Data initCodeHash = Hash::keccak256(proxyInitCode);
    return Ethereum::checksumed(Address(hexEncoded(create2Address(factoryAddress, salt, initCodeHash))));
}

std::string getEIP4337AddressFromOwnerBytes(const std::string& factoryAddress, const std::string& bytecode, const std::string& diamondCutFacetAddress, const std::string& accountFacetAddress, const std::string& verificationFacetAddress, const std::string& entryPointAddress, const std::string& securityManagerAddress, const std::string& facetRegistryAddress, const std::string& ownerPublicKey) {
    Data initCode = parse_hex(bytecode);

    auto params = ABI::ParamTuple();
    params.addParam(std::make_shared<ABI::ParamAddress>(parse_hex(diamondCutFacetAddress)));
    params.addParam(std::make_shared<ABI::ParamAddress>(parse_hex(accountFacetAddress)));
    params.addParam(std::make_shared<ABI::ParamAddress>(parse_hex(verificationFacetAddress)));
    params.addParam(std::make_shared<ABI::ParamAddress>(parse_hex(entryPointAddress)));
    params.addParam(std::make_shared<ABI::ParamAddress>(parse_hex(securityManagerAddress)));
    params.addParam(std::make_shared<ABI::ParamAddress>(parse_hex(facetRegistryAddress)));
    params.addParam(std::make_shared<ABI::ParamByteArray>(parse_hex(ownerPublicKey)));

    Data encoded;
    params.encode(encoded);

    append(initCode, encoded);

    const Data initCodeHash = Hash::keccak256(initCode);
    const Data salt = parse_hex("0x0000000000000000000000000000000000000000000000000000000000000000");
    return Ethereum::checksumed(Address(hexEncoded(create2Address(factoryAddress, salt, initCodeHash))));
}

std::string getEIP4337AddressFromOwnerAttestationObject(const std::string& factoryAddress, const std::string& bytecode, const std::string& diamondCutFacetAddress, const std::string& accountFacetAddress, const std::string& verificationFacetAddress, const std::string& entryPointAddress, const std::string& securityManagerAddress, const std::string& facetRegistryAddress, const std::string& ownerAttestationObject) {
    const std::string& ownerPublicKey = hex(TW::Webauthn::getPublicKey(parse_hex(ownerAttestationObject)));
    return getEIP4337AddressFromOwnerBytes(factoryAddress, bytecode, diamondCutFacetAddress, accountFacetAddress, verificationFacetAddress, entryPointAddress, securityManagerAddress, facetRegistryAddress, ownerPublicKey);
}

} // namespace TW::Ethereum
