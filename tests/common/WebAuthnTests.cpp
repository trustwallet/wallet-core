// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include "HexCoding.h"
#include <WebAuthn.h>
#include <TrustWalletCore/TWWebAuthn.h>

namespace TW::WebAuthn::tests {

TEST(WebAuthn, GetPublicKey) {
    // C++
    {
        const Data& attestationObject = parse_hex("0xa363666d74646e6f6e656761747453746d74a068617574684461746158981a70842af8c1feb7133b81e6a160a6a2be45ee057f0eb6d3f7f5126daa202e075d00000000000000000000000000000000000000000014c14f8a2dfd8f451581fad6e4e1c11821abcaacd6a5010203262001215820b173a6a812025c40c38bac46343646bd0a8137c807aae6e04aac238cc24d2ad2225820116ca14d23d357588ff2aabd7db29d5976f4ecc8037775db86f67e873a306b1f");
        ASSERT_EQ(hex(getPublicKey(attestationObject)->bytes), "04a620a8cfc88fd062b11eab31663e56cad95278bef612959be214d98779f645b84e7b905b42917570148b0432f99ba21f2e7eebe018cbf837247e38150a89f771");
    }

    // C
    {
        const auto attestationObject = DATA("0xa363666d74646e6f6e656761747453746d74a068617574684461746158a4f95bc73828ee210f9fd3bbe72d97908013b0a3759e9aea3d0ae318766cd2e1ad4500000000adce000235bcc60a648b0b25f1f055030020c720eb493e167ce93183dd91f5661e1004ed8cc1be23d3340d92381da5c0c80ca5010203262001215820a620a8cfc88fd062b11eab31663e56cad95278bef612959be214d98779f645b82258204e7b905b42917570148b0432f99ba21f2e7eebe018cbf837247e38150a89f771");
        const auto& publicKey = TWWebAuthnGetPublicKey(attestationObject.get());
        EXPECT_EQ(hex(publicKey->impl.bytes), "04a620a8cfc88fd062b11eab31663e56cad95278bef612959be214d98779f645b84e7b905b42917570148b0432f99ba21f2e7eebe018cbf837247e38150a89f771");
    }
}

}