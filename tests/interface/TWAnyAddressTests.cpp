// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include "HexCoding.h"
#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(TWAnyAddress, InvalidString) {
    auto string = STRING("0x4E5B2e1dc63F6b91cb6Cd759936495434C7e972F");
    auto btcAddress = TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoin);
    auto ethAaddress = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeEthereum));

    ASSERT_EQ(btcAddress, nullptr);
    ASSERT_NE(ethAaddress.get(), nullptr);
    ASSERT_EQ(TWAnyAddressCoin(ethAaddress.get()), TWCoinTypeEthereum);
}

TEST(TWAnyAddress, Data) {
    // ethereum
    {
        auto string = STRING("0x4E5B2e1dc63F6b91cb6Cd759936495434C7e972F");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeEthereum));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "4e5b2e1dc63f6b91cb6cd759936495434c7e972f");
    }
    // smartBCH
    {
        auto string = STRING("0x4E5B2e1dc63F6b91cb6Cd759936495434C7e972F");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeSmartBitcoinCash));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "4e5b2e1dc63f6b91cb6cd759936495434c7e972f");
    }
    // KuCoin Community Chain
    {
        auto string = STRING("0x4E5B2e1dc63F6b91cb6Cd759936495434C7e972F");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeKuCoinCommunityChain));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "4e5b2e1dc63f6b91cb6cd759936495434c7e972f");
    }
    // bnb address key hash
    {
        auto string = STRING("bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBinance));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "bffe47abfaede50419c577f1074fee6dd1535cd1");
    }
    // bitcoin segwit witness program
    {
        auto string = STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoin));
        auto witness = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(witness, "751e76e8199196d454941c45d1b3a323f1433bd6");
    }
    // bitcoin taproot (segwit v1, bech32m)
    {
        auto string = STRING("bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoin));
        auto witness = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(witness, "751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6");
    }
    // bitcoincashaddr
    {
        auto string = STRING("bitcoincash:qzxf0wl63ahx6jsxu8uuldcw7n5aatwppvnteraqaw");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeBitcoinCash));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "8c97bbfa8f6e6d4a06e1f9cfb70ef4e9deadc10b");
    }
    // base58 dogecoin
    {
        auto string = STRING("DQkiL71KkuGEgS9QFCKJkBeHmzM5YFYGkG");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeDogecoin));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "d726d32d9ff0560e7df35764987fcf01a6a343cf");
    }
    // decred
    {
        auto string = STRING("Dsb4fb7SfdLPhKVQFapGRGnokncgNiYmkAe");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeDecred));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "6ec8261586091a51ea706a62161eceba4b583b49");
    }
    // zcash
    {
        auto string = STRING("t3PkLgT71TnF112nSwBToXsD77yNbx2gJJY");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeZcash));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "38dfa4066f0032c86024bb5899adfa167c80189c");
    }
    // nano
    {
        auto string = STRING("nano_1ie9ffzo57nh51p53eaiqen8y6mxe1z5etcnpmucyts77qk7mqzm1x7yiyjk");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeNano));
        auto pubkey = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(pubkey, "41876b7f51968f182c30b110bb286f127d603e366954b4f6af6b252de459dff3");
    }
    // zilliqa
    {
        auto string = STRING("zil1l8ddxvejeam70qang54wnqkgtmlu5mwlgzy64z");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeZilliqa));

        auto expectedKeyHash = "F9dad33332CF77E783B3452aE982c85effCa6DDf";
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        auto checksumed = WRAPS(TWStringCreateWithRawBytes(TWDataBytes(keyHash.get()), strnlen(expectedKeyHash, 40)));
        assertStringsEqual(checksumed, expectedKeyHash);
    }
    // kusama
    {
        auto string = STRING("HewiDTQv92L2bVtkziZC8ASxrFUxr6ajQ62RXAnwQ8FDVmg");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeKusama));
        auto pubkey = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(pubkey, "e0b3fcccfe0283cc0f8c105c68b5690aab8c5c1692a868e55eaca836c8779085");
    }
    // polkadot
    {
        auto string = STRING("16fir1SPRAaWGtF4ZkKNDq3S6LnD9mbphGXqL923DoH85Exz");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypePolkadot));
        auto pubkey = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(pubkey, "fab55af5eaaa1a284b615946dc3dd403c44ce3448c15884d61b2dcba3ddb245a");
    }
    // cardano
    {
        auto string = STRING("addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeCardano));
        auto pubkey = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(pubkey, "01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b");
    }
    // neo
    {
        auto string = STRING("AKmrAHRD9ZDUnu4m3vWWonpsojo4vgSuqp");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeNEO));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "172bdf43265c0adfe105a1a8c45b3f406a38362f24");
    }
    // multiversx
    {
        auto string = STRING("erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeMultiversX));
        auto pubkey = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(pubkey, "fd691bb5e85d102687d81079dffce842d4dc328276d2d4c60d8fd1c3433c3293");
    }
    // near
    {
        auto string = STRING("NEARTDDWrUMdoC2rA1eU6gNrSU2zyGKdR71TNucTvsQHyfAXjKcJb");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeNEAR));
        auto pubkey = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(pubkey, "3b83b07cab54824a59c3d3f2e203a7cd913b7fcdc4439595983e2402c2cf791d");
    }
    // ecashaddr
    {
        auto string = STRING("ecash:qzxf0wl63ahx6jsxu8uuldcw7n5aatwppv2xdgx6me");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeECash));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "8c97bbfa8f6e6d4a06e1f9cfb70ef4e9deadc10b");
    }
    // solana
    {
        auto string = STRING("2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST");
        auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithString(string.get(), TWCoinTypeSolana));
        auto keyHash = WRAPD(TWAnyAddressData(addr.get()));
        assertHexEqual(keyHash, "18f9d8d877393bbbe8d697a8a2e52879cc7e84f467656d1cce6bab5a8d2637ec");
    }
}

TEST(TWAnyAddress, createFromPubKey) {
    constexpr auto pubkey = "02753f5c275e1847ba4d2fd3df36ad00af2e165650b35fe3991e9c9c46f68b12bc";
    const auto pubkey_twstring = STRING(pubkey);
    const auto pubkey_data = WRAPD(TWDataCreateWithHexString(pubkey_twstring.get()));
    const auto pubkey_obj = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pubkey_data.get(), TWPublicKeyTypeSECP256k1));

    const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubkey_obj.get(), TWCoinTypeBitcoin));

    assertStringsEqual(WRAPS(TWAnyAddressDescription(addr.get())), "bc1qcj2vfjec3c3luf9fx9vddnglhh9gawmncmgxhz");
}

TEST(TWAnyAddress, createFromPubKeyDerivationBitcoin) {
    constexpr auto pubkey = "02753f5c275e1847ba4d2fd3df36ad00af2e165650b35fe3991e9c9c46f68b12bc";
    const auto pubkey_twstring = STRING(pubkey);
    const auto pubkey_data = WRAPD(TWDataCreateWithHexString(pubkey_twstring.get()));
    const auto pubkey_obj = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pubkey_data.get(), TWPublicKeyTypeSECP256k1));

    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(pubkey_obj.get(), TWCoinTypeBitcoin, TWDerivationDefault));
        assertStringsEqual(WRAPS(TWAnyAddressDescription(addr.get())), "bc1qcj2vfjec3c3luf9fx9vddnglhh9gawmncmgxhz");
    }
    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(pubkey_obj.get(), TWCoinTypeBitcoin, TWDerivationBitcoinLegacy));
        assertStringsEqual(WRAPS(TWAnyAddressDescription(addr.get())), "1JvRfEQFv5q5qy9uTSAezH7kVQf4hqnHXx");
    }
    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(pubkey_obj.get(), TWCoinTypeBitcoin, TWDerivationBitcoinTestnet));
        assertStringsEqual(WRAPS(TWAnyAddressDescription(addr.get())), "tb1qcj2vfjec3c3luf9fx9vddnglhh9gawmnjan4v3");
    }
}

TEST(TWAnyAddress, createFromPubKeyDerivationPactus) {
    constexpr auto pubkey = "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa";
    const auto pubkey_twstring = STRING(pubkey);
    const auto pubkey_data = WRAPD(TWDataCreateWithHexString(pubkey_twstring.get()));
    const auto pubkey_obj = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pubkey_data.get(), TWPublicKeyTypeED25519));

    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(pubkey_obj.get(), TWCoinTypePactus, TWDerivationDefault));
        assertStringsEqual(WRAPS(TWAnyAddressDescription(addr.get())), "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr");
    }
    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(pubkey_obj.get(), TWCoinTypePactus, TWDerivationPactusMainnet));
        assertStringsEqual(WRAPS(TWAnyAddressDescription(addr.get())), "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr");
    }
    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyDerivation(pubkey_obj.get(), TWCoinTypePactus, TWDerivationPactusTestnet));
        assertStringsEqual(WRAPS(TWAnyAddressDescription(addr.get())), "tpc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymzqkcrg");
    }
}

TEST(TWAnyAddress, createFromPubKeyFilecoinAddressType) {
    constexpr auto pubkey = "0419bf99082cf2fcdaa812d6eba1eba9036ff3a3d84c1817c84954d4e8ae283fec5313e427a0f5f68dec3169b2eda876b1d9f97b1ede7f958baee6a2ce78f6e94a";
    const auto pubkey_twstring = STRING(pubkey);
    const auto pubkey_data = WRAPD(TWDataCreateWithHexString(pubkey_twstring.get()));
    const auto pubkey_obj = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pubkey_data.get(), TWPublicKeyTypeSECP256k1Extended));

    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyFilecoinAddressType(pubkey_obj.get(), TWFilecoinAddressTypeDefault));
        const auto actual = WRAPS(TWAnyAddressDescription(addr.get()));
        assertStringsEqual(actual, "f1syn25x7infncgfvodhriq2dudvmudabtavm3wyy");
        ASSERT_TRUE(TWAnyAddressIsValid(actual.get(), TWCoinTypeFilecoin));
    }
    {
        const auto addr = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKeyFilecoinAddressType(pubkey_obj.get(), TWFilecoinAddressTypeDelegated));
        const auto actual = WRAPS(TWAnyAddressDescription(addr.get()));
        assertStringsEqual(actual, "f410fvak24cyg3saddajborn6idt7rrtfj2ptauk5pbq");
        ASSERT_TRUE(TWAnyAddressIsValid(actual.get(), TWCoinTypeFilecoin));
    }
}
