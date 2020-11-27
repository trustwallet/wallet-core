// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSolanaProgram.h>
#include "Solana/Address.h"
#include "Solana/Program.h"

using namespace TW::Solana;
using namespace TW;

/// Wrapper for C interface, empty as all methods are static
struct TWSolanaProgram {
    uint8_t unused; // C doesn't allow zero-sized struct
};

TWString *_Nullable TWSolanaProgramDefaultTokenAddress(TWString *_Nonnull mainAddress, TWString* _Nonnull tokenMintAddress) {
    try {
        std::string defaultAddress = TokenProgram::defaultTokenAddress(TWStringUTF8Bytes(mainAddress), TWStringUTF8Bytes(tokenMintAddress));
        return TWStringCreateWithUTF8Bytes(defaultAddress.c_str());
    } catch (...) {
        return nullptr;
    }
}
