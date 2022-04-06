// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Bitcoin/Address.h"
#include "Bitcoin/Amount.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/Entry.h"
#include "Bitcoin/Script.h"
#include "TrustWalletCore/TWBitcoinSigHashType.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW::Bitcoin;

bool bitcoinAddressIsValid(TW::Data& data) {
    return Address::isValid(data);
}

bool bitcoinAddressIsValidString(const std::string& string) {
    return Address::isValid(string);
}

bool bitcoinAddressEqual(Address lhs, Address rhs) {
    return lhs == rhs;
}

uint8_t bitcoinAddressPrefix(Address address) {
    return address.bytes[0];
}

std::string bitcoinAddressToString(Address address) {
    return address.string();
}

EMSCRIPTEN_BINDINGS(wallet_bitcoin_module) {
    class_<Address>("BitcoinAddress")
        .constructor<const std::string&>()
        .constructor<const TW::PublicKey&, TW::byte>();

    function("bitcoinAddressIsValid", &bitcoinAddressIsValid);
    function("bitcoinAddressIsValidString", &bitcoinAddressIsValidString);
    function("bitcoinAddressEqual", &bitcoinAddressEqual);
    function("bitcoinAddressPrefix", &bitcoinAddressPrefix);
    function("bitcoinAddressToString", &bitcoinAddressToString);

    value_object<Amount>("BitcoinAmount");
    function("isValidAmount", &isValidAmount);

    class_<Script>("BitcoinScript")
        .property("bytes", &Script::bytes)
        .constructor<>()
        .constructor<const TW::Data&>()
        .function("empty", &Script::empty)
        .function("hash", &Script::hash)
        .function("isPayToScriptHash", &Script::isPayToScriptHash)
        .function("isPayToWitnessScriptHash", &Script::isPayToWitnessScriptHash)
        .function("isPayToWitnessPublicKeyHash", &Script::isPayToWitnessPublicKeyHash)
        .function("isWitnessProgram", &Script::isWitnessProgram)
        .function("matchPayToPublicKey", &Script::matchPayToPublicKey)
        .function("matchPayToPublicKeyHash", &Script::matchPayToPublicKeyHash)
        .function("matchPayToScriptHash", &Script::matchPayToScriptHash)
        .function("matchPayToWitnessPublicKeyHash", &Script::matchPayToWitnessPublicKeyHash)
        .function("matchPayToWitnessScriptHash", &Script::matchPayToWitnessScriptHash)
        .class_function("buildPayToPublicKey", &Script::buildPayToPublicKey)
        .class_function("buildPayToPublicKeyHash", &Script::buildPayToPublicKeyHash)
        .class_function("buildPayToScriptHash", &Script::buildPayToScriptHash)
        .class_function("buildPayToV0WitnessProgram", &Script::buildPayToV0WitnessProgram)
        .class_function("buildPayToV1WitnessProgram", &Script::buildPayToV1WitnessProgram)
        .class_function("buildPayToWitnessPublicKeyHash", &Script::buildPayToWitnessPublicKeyHash)
        .class_function("buildPayToWitnessScriptHash", &Script::buildPayToWitnessScriptHash)
        .class_function("lockScriptForAddress", &Script::lockScriptForAddress)
        .function("encode", &Script::encode)
        .class_function("encodeNumber", &Script::encodeNumber)
        .class_function("decodeNumber", &Script::decodeNumber)
        ;

    enum_<TWBitcoinSigHashType>("TWBitcoinSigHashType")
        .value("TWBitcoinSigHashTypeAll", TWBitcoinSigHashType::TWBitcoinSigHashTypeAll)
        .value("TWBitcoinSigHashType", TWBitcoinSigHashType::TWBitcoinSigHashTypeNone)
        .value("TWBitcoinSigHashTypeSingle", TWBitcoinSigHashType::TWBitcoinSigHashTypeSingle)
        .value("TWBitcoinSigHashTypeFork", TWBitcoinSigHashType::TWBitcoinSigHashTypeFork)
        .value("TWBitcoinSigHashTypeForkBTG", TWBitcoinSigHashType::TWBitcoinSigHashTypeForkBTG)
        .value("TWBitcoinSigHashTypeAnyoneCanPay",
               TWBitcoinSigHashType::TWBitcoinSigHashTypeAnyoneCanPay);
    function("TWBitcoinSigHashTypeIsSingle", &TWBitcoinSigHashTypeIsSingle);
    function("TWBitcoinSigHashTypeIsNone", &TWBitcoinSigHashTypeIsNone);
}
