// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
