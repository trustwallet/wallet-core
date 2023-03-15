// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <StarkEx/MessageSigner.h>
#include <HexCoding.h>

namespace TW::StarkEx {

std::string MessageSigner::signMessage(const TW::PrivateKey& privateKey, const std::string& message) {
    auto digest = parse_hex(message, true);
    return hex(privateKey.sign(digest, TWCurveStarkex));
}

bool MessageSigner::verifyMessage(const PublicKey& publicKey, const std::string& message, const std::string& signature) noexcept {
    auto starkSignature = parse_hex(signature, true);
    auto digest = parse_hex(message, true);
    return publicKey.verify(starkSignature, digest);
}

} // namespace TW::StarkEx
