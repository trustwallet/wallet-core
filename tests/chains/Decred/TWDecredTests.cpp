// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWBitcoinScript.h>

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
    auto pubKey0 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(dpub.get(), TWCoinTypeDecred, STRING("m/44'/42'/0'/0/0").get()));
    auto address0 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeDecred, pubKey0.get()));

    assertStringsEqual(address0, "DsksmLD2wDoA8g8QfFvm99ASg8KsZL8eJFd");
}

TEST(Decred, Lockscripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("Dcur2mcGjmENx4DhNqDctW5wJCVyT3Qeqkx").get(), TWCoinTypeDecred));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "a914f5916158e3e2c4551c1796708db8367207ed13bb87");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("DsfD7KYsJtRraYXPZM61ui7779oYJCakYvH").get(), TWCoinTypeDecred));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a9149c417596dea6570f8e546674555b5ce5087ce2c288ac");
}
