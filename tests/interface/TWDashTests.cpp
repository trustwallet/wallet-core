#include "TWTestUtilities.h"

#include "Bitcoin/Address.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWPurpose.h>

#include <gtest/gtest.h>

TEST(Dash, LockScripts) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("XgkpWEFe59pX3aMhx6PrDawjNnoZKHeZbp").get(), TWCoinTypeDash));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a91442914f5b70c61619eca5359df57d0b9bdcf8ccff88ac");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("7eprxeVjKfVgS8p2RNsZ89K72YV61xg4gq").get(), TWCoinTypeDash));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "a9148835ae54f297ad069552a1401e535dfe5f396f6187");
}
