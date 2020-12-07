// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSolanaAddress.h>
#include "Solana/Address.h"

using namespace TW::Solana;
using namespace TW;

struct TWSolanaAddress* _Nullable TWSolanaAddressCreateWithString(TWString* _Nonnull string) {
    auto& str = *reinterpret_cast<const std::string*>(string);
    return new TWSolanaAddress{Address(str)};
}

void TWSolanaAddressDelete(struct TWSolanaAddress* _Nonnull address) {
    delete address;
}

TWString *_Nullable TWSolanaAddressDefaultTokenAddress(struct TWSolanaAddress* _Nonnull address, TWString* _Nonnull tokenMintAddress) {
    try {
        if (address == nullptr || tokenMintAddress == nullptr) {
            return nullptr;
        }
        Address tokenMint = Address(TWStringUTF8Bytes(tokenMintAddress));
        std::string defaultAddress = address->impl.defaultTokenAddress(tokenMint).string();
        return TWStringCreateWithUTF8Bytes(defaultAddress.c_str());
    } catch (...) {
        return nullptr;
    }
}

TWString* _Nonnull TWSolanaAddressDescription(struct TWSolanaAddress* _Nonnull address) {
    return TWStringCreateWithUTF8Bytes(address->impl.string().c_str());
}
