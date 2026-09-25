// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "HexCoding.h"
#include "TrustWalletCore/TWTONMessageSigner.h"

namespace TW::TheOpenNetwork::tests {

TEST(TWTONMessageSigner, SignMessage) {
    const auto privateKeyBytes = DATA("112d4e2e700a468f1eae699329202f1ee671d6b665caa2d92dea038cf3868c18");
    const auto privateKey = WRAP(TWPrivateKey, TWPrivateKeyCreateWithData(privateKeyBytes.get()));
    const auto message = STRING("Hello world");

    const auto signature = WRAPS(TWTONMessageSignerSignMessage(privateKey.get(), message.get()));
    assertStringsEqual(signature, "2490fbaa72aec0b77b19162bbbe0b0e3f7afd42cc9ef469f0494cd4a366a4bf76643300cd5991f66bce6006336742b8d1d435d541d244dcc013d428472e89504");
}

} // namespace TW::TheOpenNetwork::tests
