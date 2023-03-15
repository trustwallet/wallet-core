// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWBitcoinScript.h>

#include <gtest/gtest.h>

TEST(Doge, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("DLSSSUS3ex7YNDACJDxMER1ZMW579Vy8Zy").get(), TWCoinTypeDogecoin));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914a7d191ec42aa113e28cd858cceaa7c733ba2f77788ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("AETZJzedcmLM2rxCM6VqCGF3YEMUjA3jMw").get(), TWCoinTypeDogecoin));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a914f191149f72f235548746654f5b473c58258f7fb687");
}
