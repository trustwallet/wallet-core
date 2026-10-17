// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Sui.pb.h"
#include "PublicKey.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Sui::tests {

TEST(SuiSigner, Transfer) {
    // Successfully broadcasted https://explorer.sui.io/txblock/HkPo6rYPyDY53x1MBszvSZVZyixVN7CHvCJGX381czAh?network=devnet
    Proto::SigningInput input;
    auto txMsg = "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA";
    input.mutable_sign_direct_message()->set_unsigned_tx_msg(txMsg);
    auto privateKey = PrivateKey(parse_hex("3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSui);
    ASSERT_EQ(output.unsigned_tx(), "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA");
    ASSERT_EQ(output.signature(), "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

TEST(SuiSigner, RawJsonV2) {
    // A transaction serialized by `Transaction.toJSON()` of the `@mysten/sui` 2.x SDK - the
    // release that replaces the removed JSON-RPC client with `SuiGrpcClient`.
    const auto rawJson = R"({
        "version": 2,
        "sender": "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2",
        "expiration": null,
        "gasData": {
            "budget": "28457880",
            "price": "750",
            "owner": null,
            "payment": [
                {
                    "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
                    "version": "486126455",
                    "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
                }
            ]
        },
        "inputs": [{ "Pure": { "bytes": "AMqaOwAAAAA=" } }],
        "commands": [{ "SplitCoins": { "coin": { "GasCoin": true }, "amounts": [{ "Input": 0 }] } }]
    })";

    Proto::SigningInput input;
    input.set_raw_json(rawJson);
    // The signer key must own the `sender` address, otherwise signing is rejected.
    auto privateKey = PrivateKey(parse_hex("7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSui);
    ASSERT_EQ(output.error(), Common::Proto::OK);
    ASSERT_EQ(output.unsigned_tx(), "AAABAAgAypo7AAAAAAECAAEBAABU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgEHlL4/MBbHPmdhIDLog5ffxDeYuiCxwPZnaadEVaVJR3ez+RwAAAAAIP7KIfEr2V4Bz92nxrEELP5DrjTzjSNxK1xIMDaHYr7LVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rLuAgAAAAAAAJg7sgEAAAAAAA==");
    ASSERT_EQ(output.signature(), "AI1PXL87wq4i3c2SICbFCDUB8l8CW8TACA00tCdy4qWjjGKqSUNj/J4YxFMjS5OJ1P2ieTcozHEb2FdXx/9u0A6F69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

TEST(SuiSigner, RawJsonUnsupportedVersion) {
    Proto::SigningInput input;
    input.set_raw_json(R"({"version": 3})");
    auto privateKey = PrivateKey(parse_hex("7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSui);
    ASSERT_EQ(output.error(), Common::Proto::Error_invalid_params);
}

} // namespace TW::Sui::tests
