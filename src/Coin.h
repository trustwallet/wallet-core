// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "Hash.h"
#include "DerivationPath.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "uint256.h"
#include "CoinEntry.h"

#include <TrustWalletCore/TWBlockchain.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWCurve.h>
#include <TrustWalletCore/TWHDVersion.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWDerivation.h>

#include <string>
#include <vector>

namespace TW {

// Return the set of supported coin types.
std::vector<TWCoinType> getCoinTypes();

/// Validates an address for a particular coin.
bool validateAddress(TWCoinType coin, const std::string& address);

/// Validates an address for a particular coin.
bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& prefix);

/// Validates and normalizes an address for a particular coin.
std::string normalizeAddress(TWCoinType coin, const std::string& address);
std::string normalizeAddress(TWCoinType coin, const std::string& address, const PrefixVariant& prefix);

/// Returns the blockchain for a coin type.
TWBlockchain blockchain(TWCoinType coin);

/// Returns the purpose for a coin type.
TWPurpose purpose(TWCoinType coin);

/// Returns the curve that should be used for a coin type.
TWCurve curve(TWCoinType coin);

/// Returns the default xpub HD version that should be used for a coin type.
TWHDVersion xpubVersion(TWCoinType coin);

/// Returns the default xprv HD version that should be used for a coin type.
TWHDVersion xprvVersion(TWCoinType coin);

/// Returns the xpub HD version for a TWDerivation.
TWHDVersion xpubVersionDerivation(TWCoinType coin, TWDerivation derivation);

/// Returns the xprv HD version for a TWDerivation.
TWHDVersion xprvVersionDerivation(TWCoinType coin, TWDerivation derivation);

/// Returns the default derivation path for a particular coin.
DerivationPath derivationPath(TWCoinType coin);

/// Returns an alternative derivation path for a particular coin, TWDerivationDefault for default.
DerivationPath derivationPath(TWCoinType coin, TWDerivation derivation);

/// Returns the string name of a derivation for a particular coin.
const char* derivationName(TWCoinType coin, TWDerivation derivation);

/// Returns the public key type for a particular coin.
enum TWPublicKeyType publicKeyType(TWCoinType coin);

/// Derives the address for a particular coin from the private key.
std::string deriveAddress(TWCoinType coin, const PrivateKey& privateKey);

/// Derives the address for a particular coin from the private key, with given derivation.
std::string deriveAddress(TWCoinType coin, const PrivateKey& privateKey, TWDerivation derivation);

/// Derives the address for a particular coin from the public key, with given derivation and addressPrefix.
std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation = TWDerivationDefault, const PrefixVariant& addressPrefix = std::monostate());

/// Returns the binary representation of a string address
Data addressToData(TWCoinType coin, const std::string& address);

/// Hasher for deriving the extended public key
Hash::Hasher publicKeyHasher(TWCoinType coin);

/// Hasher to use for base 58 checksums in keys (extended private, public)
Hash::Hasher base58Hasher(TWCoinType coin);

/// Hasher used inside address generation (hash of public key)
Hash::Hasher addressHasher(TWCoinType coin);

/// Returns static prefix for a coin type.
byte staticPrefix(TWCoinType coin);

/// Returns P2PKH prefix for a coin type.
byte p2pkhPrefix(TWCoinType coin);

/// Returns P2SH prefix for a coin type.
byte p2shPrefix(TWCoinType coin);

/// Returns human readable part for a coin type.
enum TWHRP hrp(TWCoinType coin);

/// Returns the ss58 prefix of a coin type.
std::uint32_t ss58Prefix(TWCoinType coin);

/// Returns chain ID.
const char* chainId(TWCoinType coin);

// Note: use output parameter to avoid unneeded copies
void anyCoinSign(TWCoinType coinType, const Data& dataIn, Data& dataOut);

uint32_t slip44Id(TWCoinType coin);

std::string anySignJSON(TWCoinType coinType, const std::string& json, const Data& key);

bool supportsJSONSigning(TWCoinType coinType);

void anyCoinPlan(TWCoinType coinType, const Data& dataIn, Data& dataOut);

Data anyCoinPreImageHashes(TWCoinType coinType, const Data& txInputData);

void anyCoinCompileWithSignatures(TWCoinType coinType, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& txOutputOut);

Data anyCoinBuildTransactionInput(TWCoinType coinType, const std::string& from, const std::string& to, const uint256_t& amount, const std::string& asset, const std::string& memo, const std::string& chainId);

// Describes a derivation: path + optional format + optional name
struct Derivation {
    TWDerivation name = TWDerivationDefault;
    const char* path = "";
    const char* nameString = "";
    TWHDVersion xpubVersion = TWHDVersionNone;
    TWHDVersion xprvVersion = TWHDVersionNone;
};

// Contains only simple types.
struct CoinInfo {
    const char* id;
    const char* name;
    TWBlockchain blockchain;
    TWPurpose purpose;
    TWCurve curve;
    std::vector<Derivation> derivation;
    TWPublicKeyType publicKeyType;
    byte staticPrefix;
    byte p2pkhPrefix;
    byte p2shPrefix;
    TWHRP hrp;
    const char* chainId;
    Hash::Hasher publicKeyHasher;
    Hash::Hasher base58Hasher;
    Hash::Hasher addressHasher;
    const char* symbol;
    int decimals;
    const char* explorerTransactionUrl;
    const char* explorerAccountUrl;
    uint32_t slip44;
    std::uint32_t ss58Prefix;

    // returns default derivation
    const Derivation defaultDerivation() const {
        return (derivation.size() > 0) ? derivation[0] : Derivation();
    }
    const Derivation derivationByName(TWDerivation name) const;
};

} // namespace TW
