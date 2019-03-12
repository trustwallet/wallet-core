// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"

#include "Stellar/Address.h"
#include "Stellar/Signer.h"
#include "Bitcoin/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Stellar;

TEST(StellarTransaction, signFromPhrase) {
    auto words = STRING("indicate rival expand cave giant same grocery burden ugly rose tuna blood");
    auto passphrase = STRING("");

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeStellar));
    auto input = Proto::SigningInput();
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_amount(10000000);
    input.set_fee(1000);
    input.set_sequence(2);
    input.set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.set_private_key(privateKey.get()->impl.bytes.data(), privateKey.get()->impl.bytes.size());

    const auto signer = Signer(input);

    const auto signature = signer.sign();
    ASSERT_EQ(signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAAxYC2MXoOs5v3/NT6PBn9q0uJu6u/YQle5FBa9uzteq4AAAAAAAAAAACYloAAAAAAAAAAARnfXKIAAABAocQZwTnVvGMQlpdGacWvgenxN5ku8YB8yhEGrDfEV48yDqcj6QaePAitDj/N2gxfYD9Q2pJ+ZpkQMsZZG4ACAg==");
}

TEST(StellarTransaction, signFromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("59a313f46ef1c23a9e4f71cea10fc0c56a2a6bb8a4b9ea3d5348823e5a478722"));
    auto input = Proto::SigningInput();
    input.set_account("GAE2SZV4VLGBAPRYRFV2VY7YYLYGYIP5I7OU7BSP6DJT7GAZ35OKFDYI");
    input.set_amount(10000000);
    input.set_fee(1000);
    input.set_sequence(2);
    input.set_destination("GDCYBNRRPIHLHG7X7TKPUPAZ7WVUXCN3VO7WCCK64RIFV5XM5V5K4A52");
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto signer = Signer(input);

    const auto signature = signer.sign();
    ASSERT_EQ(signature, "AAAAAAmpZryqzBA+OIlrquP4wvBsIf1H3U+GT/DTP5gZ31yiAAAD6AAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAAAEAAAAAxYC2MXoOs5v3/NT6PBn9q0uJu6u/YQle5FBa9uzteq4AAAAAAAAAAACYloAAAAAAAAAAARnfXKIAAABAocQZwTnVvGMQlpdGacWvgenxN5ku8YB8yhEGrDfEV48yDqcj6QaePAitDj/N2gxfYD9Q2pJ+ZpkQMsZZG4ACAg==");
}