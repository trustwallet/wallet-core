// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <TrustWalletCore/TWAnyAddress.h>
#include <emscripten/bind.h>

#include "TWData.h"
#include "TWString.h"

using namespace emscripten;

namespace TW::Wasm {

class AnyAddress {
  public:
    static auto isValid(const std::string& string, enum TWCoinType coin) {
        return TWAnyAddressIsValid(&string, coin);
    }

    static auto create(const std::string& string, enum TWCoinType coin) {
        return new AnyAddress(TWAnyAddressCreateWithString(&string, coin));
    }

    AnyAddress(TWAnyAddress* address) : address(address) {}

    ~AnyAddress() { TWAnyAddressDelete(address); }

    auto description() {
        return TWStringToStd(TWAnyAddressDescription(address));
    }

    auto coin() { 
        return TWAnyAddressCoin(address);
    }

    auto data() {
        return TWDataToStd(TWAnyAddressData(address));
    }

  protected:
    TWAnyAddress* address;
};

EMSCRIPTEN_BINDINGS(WASM_TWAnyAddress) {
    class_<AnyAddress>("AnyAddress")
        .class_function("create", &AnyAddress::create, allow_raw_pointers())
        .class_function("isValid", &AnyAddress::isValid)
        .function("description", &AnyAddress::description)
        .function("coin", &AnyAddress::coin)
        .function("data", &AnyAddress::data);
}

} // namespace TW::Wasm
