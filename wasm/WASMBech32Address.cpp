// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Bech32Address.h"
#include <TrezorCrypto/hasher.h>
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

EMSCRIPTEN_BINDINGS(wallet_Bech32Address_module) {
    class_<Bech32Address>("Bech32Address")
        .constructor<const std::string&>()
        .constructor<const std::string&, const Data&>()
        .constructor<const std::string&, HasherType, const PublicKey&>()
        .class_function("isValid",
                        select_overload<bool(const std::string&)>(&Bech32Address::isValid))
        .class_function("isValid", select_overload<bool(const std::string&, const std::string&)>(
                                       &Bech32Address::isValid))
        .class_function("decode", &Bech32Address::decode)
        .function("setHrp", &Bech32Address::setHrp)
        .function("setKey", &Bech32Address::setKey)
        .function("getHrp", &Bech32Address::getHrp)
        .function("getKeyHash", &Bech32Address::getKeyHash)
        .function("string", &Bech32Address::string);
}
