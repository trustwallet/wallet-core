// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDVersion.h>
#include <TrustWalletCore/TWPurpose.h>

#include <array>
#include <optional>
#include <string>

namespace TW {

class HDWallet {
public:
    static constexpr size_t seedSize = 64;
    static constexpr size_t maxMnemomincSize = 240;
    static constexpr size_t maxExtendedKeySize = 128;

    /// Wallet seed.
    std::array<byte, seedSize> seed;

    /// Mnemonic word list.
    std::string mnemonic;

    /// Mnemonic passphrase.
    std::string passphrase;

public:
    /// Determines if a mnemonic phrase is valid.
    static bool isValid(const std::string& mnemonic);

    /// Initializes a new random HDWallet with the provided strength in bits.
    HDWallet(int strength, const std::string& passphrase);

    /// Initializes an HDWallet from a mnemonic seed.
    HDWallet(const std::string& mnemonic, const std::string& passphrase);

    /// Initializes an HDWallet from a seed.
    HDWallet(const Data& data, const std::string& passphrase);

    virtual ~HDWallet();

    /// Returns the private key at the given derivation path.
    PrivateKey getKey(TWPurpose purpose, TWCoinType coin, uint32_t account, uint32_t change, uint32_t address) const;

    /// Returns the extended private key.
    std::string getExtendedPrivateKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const;

    /// Returns the exteded public key.
    std::string getExtendedPublicKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const;

    /// Computes the public key from an exteded public key representation.
    static PublicKey getPublicKeyFromExtended(const std::string& extended, enum TWHDVersion versionPublic, enum TWHDVersion versionPrivate, uint32_t change, uint32_t address);

    /// Generates an address from an exteded public key representation, coin type, and change and address indices.
    static std::optional<std::string> getAddressFromExtended(const std::string& extended, TWCoinType coinType, uint32_t change, uint32_t address);
};

} // namespace

/// Wrapper for C interface.
struct TWHDWallet {
    TW::HDWallet impl;
};
