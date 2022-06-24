// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "DerivationPath.h"
#include "Hash.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWCurve.h>
#include <TrustWalletCore/TWHDVersion.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWDerivation.h>

#include <array>
#include <optional>
#include <string>

namespace TW {

class HDWallet {
  public:
    static constexpr size_t seedSize = 64;
    static constexpr size_t maxMnemomincSize = 240;
    static constexpr size_t maxExtendedKeySize = 128;

  private:
    /// Wallet seed, derived one-way from the mnemonic and passphrase
    std::array<byte, seedSize> seed;

    /// Mnemonic word list (aka. recovery phrase).
    std::string mnemonic;

    /// Passphrase for mnemonic encryption.
    std::string passphrase;

    /// Entropy is the binary 1-to-1 representation of the mnemonic (11 bits from each word)
    TW::Data entropy;

  public:
    const std::array<byte, seedSize>& getSeed() const { return seed; }
    const std::string& getMnemonic() const { return mnemonic; }
    const std::string& getPassphrase() const { return passphrase; }
    const TW::Data& getEntropy() const { return entropy; }

  public:
    /// Initializes a new random HDWallet with the provided strength in bits.  
    /// Throws on invalid strength.
    HDWallet(int strength, const std::string& passphrase);

    /// Initializes an HDWallet from a BIP39 mnemonic and a passphrase, check English dict by default.
    /// Throws on invalid mnemonic.
    HDWallet(const std::string& mnemonic, const std::string& passphrase, const bool check = true);

    /// Initializes an HDWallet from an entropy.
    /// Throws on invalid data.
    HDWallet(const Data& entropy, const std::string& passphrase);

    HDWallet(const HDWallet& other) = default;
    HDWallet(HDWallet&& other) = default;
    HDWallet& operator=(const HDWallet& other) = default;
    HDWallet& operator=(HDWallet&& other) = default;

    virtual ~HDWallet();

    /// Returns master key.
    PrivateKey getMasterKey(TWCurve curve) const;

    /// Returns the master private key extension (32 byte).
    PrivateKey getMasterKeyExtension(TWCurve curve) const;

    /// Returns the private key with the given derivation.
    PrivateKey getKey(const TWCoinType coin, TWDerivation derivation) const;

    /// Returns the private key at the given derivation path.
    PrivateKey getKey(const TWCoinType coin, const DerivationPath& derivationPath) const;

    /// Derives the address for a coin (default derivation).
    std::string deriveAddress(TWCoinType coin) const;

    /// Derives the address for a coin with given derivation.
    std::string deriveAddress(TWCoinType coin, TWDerivation derivation) const;

    /// Returns the extended private key for default 0 account with the given derivation.
    std::string getExtendedPrivateKeyDerivation(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version) const {
        return getExtendedPrivateKeyAccount(purpose, coin, derivation, version, 0);
    }

    /// Returns the extended public key for default 0 account with the given derivation.
    std::string getExtendedPublicKeyDerivation(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version) const {
        return getExtendedPublicKeyAccount(purpose, coin, derivation, version, 0);
    }

    /// Returns the extended private key for default 0 account; derivation path used is "m/purpose'/coin'/0'".
    std::string getExtendedPrivateKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
        return getExtendedPrivateKeyAccount(purpose, coin, TWDerivationDefault, version, 0);
    }

    /// Returns the extended public key for default 0 account; derivation path used is "m/purpose'/coin'/0'".
    std::string getExtendedPublicKey(TWPurpose purpose, TWCoinType coin, TWHDVersion version) const {
        return getExtendedPublicKeyAccount(purpose, coin, TWDerivationDefault, version, 0);
    }

    /// Returns the extended private key for a custom account; derivation path used is "m/purpose'/coin'/account'".
    std::string getExtendedPrivateKeyAccount(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) const;

    /// Returns the extended public key for a custom account; derivation path used is "m/purpose'/coin'/account'".
    std::string getExtendedPublicKeyAccount(TWPurpose purpose, TWCoinType coin, TWDerivation derivation, TWHDVersion version, uint32_t account) const;

    /// Returns the BIP32 Root Key (private)
    std::string getRootKey(TWCoinType coin, TWHDVersion version) const;

    /// Computes the public key from an extended public key representation.
    static std::optional<PublicKey> getPublicKeyFromExtended(const std::string& extended, TWCoinType coin, const DerivationPath& path);

    /// Computes the private key from an extended private key representation.
    static std::optional<PrivateKey> getPrivateKeyFromExtended(const std::string& extended, TWCoinType coin, const DerivationPath& path);

  public:
    // Private key type (later could be moved out of HDWallet)
    enum PrivateKeyType {
      PrivateKeyTypeDefault32 = 0, // 32-byte private key
      PrivateKeyTypeDoubleExtended = 1, // used by Cardano
    };
    
    // obtain privateKeyType used by the coin/curve
    static PrivateKeyType getPrivateKeyType(TWCurve curve);

  private:
    void updateSeedAndEntropy(bool check = true);

    // For Cardano, derive 2nd, staking derivation path from the primary one
    static DerivationPath cardanoStakingDerivationPath(const DerivationPath& path);
};

} // namespace TW

/// Wrapper for C interface.
struct TWHDWallet {
    TW::HDWallet impl;
};
