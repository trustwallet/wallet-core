// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <string>
#include "ABI/ValueEncoder.h"
#include "ABI/Function.h"
#include "AddressChecksum.h"
#include "EIP1014.h"
#include "Hash.h"
#include "HexCoding.h"
#include "../proto/Barz.pb.h"
#include "AsnParser.h"
#include "Base64.h"

namespace TW::Barz {

std::string getCounterfactualAddress(const Proto::ContractAddressInput input) {
    auto encodedData = Ethereum::ABI::Function::encodeParams(Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.account_facet()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.verification_facet()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.entry_point()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.facet_registry()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.default_fallback()),
        std::make_shared<Ethereum::ABI::ProtoByteArray>(parse_hex(input.public_key())),
    });
    if (!encodedData.has_value()) {
        return {};
    }

    Data initCode = parse_hex(input.bytecode());
    append(initCode, encodedData.value());

    const Data initCodeHash = Hash::keccak256(initCode);
    Data salt = store(input.salt(), 32);
    return Ethereum::checksumed(Ethereum::Address(hexEncoded(Ethereum::create2Address(input.factory(), salt, initCodeHash))));
}

Data getInitCode(const std::string& factoryAddress, const PublicKey& publicKey, const std::string& verificationFacet, const uint32_t salt) {
    auto createAccountFuncEncoded = Ethereum::ABI::Function::encodeFunctionCall("createAccount", Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoAddress>(verificationFacet),
        std::make_shared<Ethereum::ABI::ProtoByteArray>(publicKey.bytes),
        std::make_shared<Ethereum::ABI::ProtoUInt256>(salt),
    });
    if (!createAccountFuncEncoded.has_value()) {
        return {};
    }

    Data envelope;
    append(envelope, parse_hex(factoryAddress));
    append(envelope, createAccountFuncEncoded.value());
    return envelope;
}

Data getFormattedSignature(const Data& signature, const Data challenge, const Data& authenticatorData, const std::string& clientDataJSON) {
    std::string challengeBase64 = TW::Base64::encodeBase64Url(challenge);
    while (challengeBase64.back() == '=') {
        challengeBase64.pop_back();
    }
    size_t challengePos = clientDataJSON.find(challengeBase64);
    if (challengePos == std::string::npos) {
        return Data();
    }

    const std::string clientDataJSONPre = clientDataJSON.substr(0, challengePos);
    const std::string clientDataJSONPost = clientDataJSON.substr(challengePos + challengeBase64.size());

    const auto parsedSignatureOptional = ASN::AsnParser::ecdsa_signature_from_der(signature);
    if (!parsedSignatureOptional.has_value()) {
        return {};
    }
    const Data parsedSignature = parsedSignatureOptional.value();
    const Data rValue = subData(parsedSignature, 0, 32);
    const Data sValue = subData(parsedSignature, 32, 64);

    auto encoded = Ethereum::ABI::Function::encodeParams(Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoUInt256>(rValue),
        std::make_shared<Ethereum::ABI::ProtoUInt256>(sValue),
        std::make_shared<Ethereum::ABI::ProtoByteArray>(authenticatorData),
        std::make_shared<Ethereum::ABI::ProtoString>(clientDataJSONPre),
        std::make_shared<Ethereum::ABI::ProtoString>(clientDataJSONPost),
    });

    if (encoded.has_value()) {
        return encoded.value();
    }
    return {};
}

Data getPrefixedMsgHash(const Data msgHash, const std::string& barzAddress, const uint32_t chainId) {
    // keccak256("EIP712Domain(uint256 chainId,address verifyingContract)");
    const Data& domainSeparatorTypeHashData = parse_hex("0x47e79534a245952e8b16893a336b85a3d9ea9fa8c573f3d803afb92a79469218");
    // keccak256("BarzMessage(bytes message)")
    const Data& barzMsgHashData = parse_hex("0xb1bcb804a4a3a1af3ee7920d949bdfd417ea1b736c3552c8d6563a229a619100");
    const auto signedDataPrefix = "0x1901";

    auto encodedDomainSeparatorData = Ethereum::ABI::Function::encodeParams(Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoBytes32>(domainSeparatorTypeHashData),
        std::make_shared<Ethereum::ABI::ProtoUInt256>(chainId),
        std::make_shared<Ethereum::ABI::ProtoAddress>(barzAddress)
    });
    if (!encodedDomainSeparatorData.has_value()) {
        return {};
    }

    Data domainSeparator = encodedDomainSeparatorData.value();
    const Data domainSeparatorHash = Hash::keccak256(domainSeparator);

    auto encodedRawMessageData = Ethereum::ABI::Function::encodeParams(Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoBytes32>(barzMsgHashData),
        std::make_shared<Ethereum::ABI::ProtoBytes32>(Hash::keccak256(msgHash)),
    });

    Data rawMessageData = encodedRawMessageData.value();

    auto encodedMsg = Ethereum::ABI::Function::encodeParams(Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoBytes32>(domainSeparatorHash),
        std::make_shared<Ethereum::ABI::ProtoBytes32>(Hash::keccak256(rawMessageData))
    });
    auto encodedMsgData = signedDataPrefix + hex(encodedMsg.value());

    Data finalEncodedMsgData = parse_hex(encodedMsgData);

    const Data encodedMsgHash = Hash::keccak256(finalEncodedMsgData);

    Data envelope;
    append(envelope, encodedMsgHash);

    return envelope;
}

// Function to encode the diamondCut function call using protobuf message as input
Data getDiamondCutCode(const Proto::DiamondCutInput& input) {
    const auto diamondCutSelector = "1f931c1c";
    const auto dataLocationChunk = "60";
    const char defaultPadding = '0';
    Data encoded;

    //    function diamondCut(
    //        FacetCut[] calldata diamondCut,
    //        address init,
    //        bytes calldata _calldata // Note that Barz does not use the _calldata for initialization.
    //    )
    Data encodedSignature = parse_hex(diamondCutSelector); // diamondCut() function selector
    encoded.insert(encoded.end(), encodedSignature.begin(), encodedSignature.end());

    // First argument Data Location `diamondCut`
    Data dataLocation = parse_hex(dataLocationChunk);
    pad_left(dataLocation, 32);
    append(encoded, dataLocation);

    // Encode second Parameter `init`
    Data initAddress = parse_hex(input.init_address());
    pad_left(initAddress, 32);
    append(encoded, initAddress);

    // Third Argument Data location `_calldata`
    auto callDataDataLocation = int(hex(encoded).size()) / 2;

    Ethereum::ABI::ValueEncoder::encodeUInt256(input.facet_cuts_size(), encoded);

    // Prepend the function selector for the diamondCut function
    int instructChunk = 0;
    int totalInstructChunk = 0;
    int prevDataPosition = 0;
    const auto encodingChunk = 32;
    const auto bytesChunkLine = 5;
    int chunkLocation;
    Data dataPosition;
    // Encode each FacetCut from the input
    for (const auto& facetCut : input.facet_cuts()) {
        if (instructChunk == 0) {
            prevDataPosition = input.facet_cuts_size() * encodingChunk;
            Ethereum::ABI::ValueEncoder::encodeUInt256(prevDataPosition, encoded);
            chunkLocation = int(hex(encoded).size()) / 2;
        } else {
            prevDataPosition = prevDataPosition + (instructChunk * encodingChunk);
            Ethereum::ABI::ValueEncoder::encodeUInt256(prevDataPosition, dataPosition);
            instructChunk = 0;

            encoded.insert(encoded.begin() + chunkLocation, dataPosition.begin(), dataPosition.end());
            ++instructChunk;
        }
        Ethereum::ABI::ValueEncoder::encodeAddress(parse_hex(facetCut.facet_address()), encoded); // facet address
        ++instructChunk;
        Ethereum::ABI::ValueEncoder::encodeUInt256(facetCut.action(), encoded); // FacetAction enum
        ++instructChunk;
        append(encoded, dataLocation); // adding 0x60 DataStorage position
        ++instructChunk;
        Ethereum::ABI::ValueEncoder::encodeUInt256(facetCut.function_selectors_size(), encoded); // Number of FacetSelector
        ++instructChunk;
        // Encode and append function selectors
        for (const auto& selector : facetCut.function_selectors()) {
            Ethereum::ABI::ValueEncoder::encodeBytes(parse_hex(hex(selector)), encoded);
            ++instructChunk;
        }
        totalInstructChunk += instructChunk;
    }

    Data calldataLength;
    Ethereum::ABI::ValueEncoder::encodeUInt256((totalInstructChunk * encodingChunk) + (bytesChunkLine * encodingChunk), calldataLength);

    encoded.insert(encoded.begin() + callDataDataLocation, calldataLength.begin(), calldataLength.end());

    auto initDataSize = int(hex(parse_hex(input.init_data())).size());
    if (initDataSize == 0 || initDataSize % 2 != 0)
        return {};

    auto initDataLength = initDataSize / 2; // 1 byte is encoded into 2 char
    Ethereum::ABI::ValueEncoder::encodeUInt256(initDataLength, encoded);

    append(encoded, parse_hex(input.init_data()));

    const int paddingLength = (encodingChunk * 2) - (initDataSize % (encodingChunk * 2));
    const std::string padding(paddingLength, defaultPadding);
    append(encoded, parse_hex(padding));

    return encoded;
}

} // namespace TW::Barz
