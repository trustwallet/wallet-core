// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWStarkWare.h>
#include <ImmutableX/StarkKey.h>
#include <HexCoding.h>
#include <DerivationPath.h>
#include <string>

struct TWPrivateKey* TWStarkWareGetStarkKeyFromSignature(const struct TWDerivationPath* derivationPath, TWString* signature) {
    using namespace TW;
    const auto& ethSignatureStr = *reinterpret_cast<const std::string*>(signature);
    return new TWPrivateKey{ ImmutableX::getPrivateKeyFromRawSignature(parse_hex(ethSignatureStr), derivationPath->impl)};
}
