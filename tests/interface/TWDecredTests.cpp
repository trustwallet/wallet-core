// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWDecredAddress.h>

#include <gtest/gtest.h>

TEST(Decred, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto dpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDecred, TWHDVersionDPUB));
    auto dprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDecred, TWHDVersionDPRV));

    assertStringsEqual(dpub, "dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP");
    assertStringsEqual(dprv, "dprv3oggQ2FQ1chcr18hbW7Aur5x8SxQdES3FGa4WqeTZnFY88SNMzLdB7LkZLroF4bGAqWS8sDm3w4DKyYV7sDKfC6JMSVHnVJdpDLgHioq1vq");
}

TEST(Decred, DerivePubkeyFromDpub) {
    auto dpub = STRING("dpubZFUmm9oh5zmQkR2Tr2AXS4tCkTWg4B27SpCPFkapZrrAqgU1EwgEFgrmi6EnLGXhak86yDHhXPxFAnGU58W5S4e8NCKG1ASUVaxwRqqNdfP");
    auto pubKey0 = TWHDWalletGetPublicKeyFromExtended(dpub.get(), TWCoinTypeDecred, TWHDVersionDPUB, TWHDVersionDPRV, 0, 0);
    auto address0 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeDecred, pubKey0));

    assertStringsEqual(address0, "DsksmLD2wDoA8g8QfFvm99ASg8KsZL8eJFd");
}
