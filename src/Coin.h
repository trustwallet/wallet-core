// Copyright © 2017-2019 Trust Wallet.
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

#include <TrustWalletCore/TWBlockchain.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWCurve.h>
#include <TrustWalletCore/TWHDVersion.h>
#include <TrustWalletCore/TWPurpose.h>

#include <string>

namespace TW {

/// Validates an address for a particular coin.
bool validateAddress(TWCoinType coin, const std::string& address);

/// Returns the blockchain for a coin type.
TWBlockchain blockchain(TWCoinType coin);

/// Returns the purpose for a coin type.
TWPurpose purpose(TWCoinType coin);

/// Returns the curve that should be used for a coin type.
TWCurve curve(TWCoinType coin);

/// Returns the xpub HD version that should be used for a coin type.
TWHDVersion xpubVersion(TWCoinType coin);

/// Returns the xprv HD version that should be used for a coin type.
TWHDVersion xprvVersion(TWCoinType coin);

/// Returns the default derivation path for a particular coin.
DerivationPath derivationPath(TWCoinType coin);

/// Returns the public key type for a particular coin.
enum TWPublicKeyType publicKeyType(TWCoinType coin);

/// Derives the address for a particular coin from the private key.
std::string deriveAddress(TWCoinType coin, const PrivateKey& privateKey);

/// Derives the address for a particular coin from the private key.
std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey);

/// Hasher for deriving the public key hash.
Hash::Hasher publicKeyHasher(TWCoinType coin);

/// Hasher to use for base 58 checksums.
Hash::Hasher base58Hasher(TWCoinType coin);

} // namespace TW
