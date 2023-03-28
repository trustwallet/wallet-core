// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFilecoinAddressConverter.h>
#include <Filecoin/AddressConverter.h>

TWString* _Nonnull TWFilecoinAddressConverterConvertToEthereum(TWString* _Nonnull filecoinAddress) {
    const auto& address = *reinterpret_cast<const std::string*>(filecoinAddress);
    try {
        if (auto eth_opt = TW::Filecoin::AddressConverter::convertToEthereumString(address); eth_opt) {
            return TWStringCreateWithUTF8Bytes(eth_opt->c_str());
        }
    } catch (...) {
    }

    // Return an empty string if an error occurs.
    return TWStringCreateWithUTF8Bytes("");
}

TWString* _Nonnull TWFilecoinAddressConverterConvertFromEthereum(TWString* _Nonnull ethAddress) {
    const auto& address = *reinterpret_cast<const std::string*>(ethAddress);
    try {
        std::string filecoinAddress = TW::Filecoin::AddressConverter::convertFromEthereumString(address);
        return TWStringCreateWithUTF8Bytes(filecoinAddress.c_str());
    } catch (...) {
    }

    // Return an empty string if an error occurs.
    return TWStringCreateWithUTF8Bytes("");
}
