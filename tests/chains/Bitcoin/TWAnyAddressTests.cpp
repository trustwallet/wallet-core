// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include "Hash.h"
#include "PublicKey.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Bitcoin::tests {

void testBitcoinAddressFromPublicKeyDerivation(const char* privateKey, TWDerivation derivation, const char* expectedAddr) {
    const auto data = DATA(privateKey);
    const auto privkey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(data.get()));
    const auto pubkey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyByType(privkey.get(), TWPublicKeyTypeSECP256k1));
    const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(pubkey.get(), TWCoinTypeBitcoin, derivation));
    const auto addrDescription = WRAPS(TWAnyAddressDescription(addr.get()));
    assertStringsEqual(addrDescription, expectedAddr);
}

TEST(TWBitcoinAnyAddress, CreateFromPublicKey) {
    testBitcoinAddressFromPublicKeyDerivation(
        "a26b7ffda8ad29cf3aba066cc43ce255cb13b3fba5fa9b638f4685333e3670fd",
        TWDerivationDefault,
        "bc1qj7uu67l9zkajuhx976d8fvj2ylc29gnq3kjm3r"
    );
}

TEST(TWBitcoinAnyAddress, CreateFromPublicKeyWithDerivationLegacy) {
    testBitcoinAddressFromPublicKeyDerivation(
        "a8212108f1f2a42b18b642c2dbe6e388e75de89ca7be9b6f7648bab06cdedabf",
        TWDerivationBitcoinLegacy,
        "1JspsmYcHGLjRDavb62hodTb7BW8GGia7v"
    );
}

TEST(TWBitcoinAnyAddress, CreateFromPublicKeyWithDerivationTaproot) {
    testBitcoinAddressFromPublicKeyDerivation(
        "e5bbc9cab0ff9ec86889bcd84d79ca484b017763614cd4fa3bf4aa49ad9d55a9",
        TWDerivationBitcoinTaproot,
        "bc1pkup60ng64cthqujp38l57y0eewkyqy35s86tmqjcc5yzgmz9l2gscz37tx"
    );
    testBitcoinAddressFromPublicKeyDerivation(
        "ae39965b1e73944763eb8ddaeb126d9cbde772a5c141e3ad5791f8e7f094a089",
        TWDerivationBitcoinTaproot,
        "bc1plvzy756f4t2p32n29jy9vrqcrj46g5yka7u7e89tkeufx5urgyss38y98d"
    );
    testBitcoinAddressFromPublicKeyDerivation(
        "e0721275d0f94bf4d0f59bf0bef42179f680e38dfb306e1d0e3c13ab1d797d20",
        TWDerivationBitcoinTaproot,
        "bc1puu933ez5v6w99w9dqsmwdeajpnrvknc65tlnrdx3xhxwd954mudsa39gum"
    );
}

} // namespace TW::Bitcoin::tests
