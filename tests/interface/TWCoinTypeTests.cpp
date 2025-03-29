// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <gtest/gtest.h>

TEST(TWCoinType, TWPurpose) {
    ASSERT_EQ(TWPurposeBIP84, TWCoinTypePurpose(TWCoinTypeBitcoin));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeBitcoinCash));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeBinance));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeCosmos));
    ASSERT_EQ(TWPurposeBIP84, TWCoinTypePurpose(TWCoinTypeDigiByte));
    ASSERT_EQ(TWPurposeBIP84, TWCoinTypePurpose(TWCoinTypeLitecoin));
    ASSERT_EQ(TWPurposeBIP84, TWCoinTypePurpose(TWCoinTypeGroestlcoin));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeIoTeX));
    ASSERT_EQ(TWPurposeBIP84, TWCoinTypePurpose(TWCoinTypeViacoin));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeQtum));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeZilliqa));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeTerra));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeMonacoin));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeKava));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeBandChain));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeBluzelle));
    ASSERT_EQ(TWPurposeBIP1852, TWCoinTypePurpose(TWCoinTypeCardano));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeMultiversX));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeOasis));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeTHORChain));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeCryptoOrg));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeOsmosis));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeECash));

    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeAion));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeCallisto));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeDash));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeDecred));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeDogecoin));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeEOS));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeEthereum));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeEthereumClassic));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeGoChain));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeICON));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeKin));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeNULS));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeNano));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeNimiq));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeOntology));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypePOANetwork));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeXRP));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeStellar));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeTezos));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeTheta));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeThunderCore));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeViction));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeTron));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeVeChain));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeWanchain));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeZcash));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeFiro));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeZelcash));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeRavencoin));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeWaves));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeNEO));
    ASSERT_EQ(TWPurposeBIP44, TWCoinTypePurpose(TWCoinTypeNebl));
}

TEST(TWCoinType, TWHDVersion) {
    ASSERT_EQ(TWHDVersionZPUB, TWCoinTypeXpubVersion(TWCoinTypeBitcoin));
    ASSERT_EQ(TWHDVersionZPRV, TWCoinTypeXprvVersion(TWCoinTypeBitcoin));

    ASSERT_EQ(TWHDVersionXPUB, TWCoinTypeXpubVersion(TWCoinTypeBitcoinCash));
    ASSERT_EQ(TWHDVersionXPRV, TWCoinTypeXprvVersion(TWCoinTypeBitcoinCash));

    ASSERT_EQ(TWHDVersionDPUB, TWCoinTypeXpubVersion(TWCoinTypeDecred));
    ASSERT_EQ(TWHDVersionDPRV, TWCoinTypeXprvVersion(TWCoinTypeDecred));

    ASSERT_EQ(TWHDVersionDGUB, TWCoinTypeXpubVersion(TWCoinTypeDogecoin));
    ASSERT_EQ(TWHDVersionDGPV, TWCoinTypeXprvVersion(TWCoinTypeDogecoin));
}

TEST(TWCoinType, TWPublicKeyType) {
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeBitcoin));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeBitcoinCash));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeBinance));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeCosmos));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeDigiByte));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeLitecoin));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeGroestlcoin));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeIoTeX));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeViacoin));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeQtum));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeZilliqa));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeTerra));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeMonacoin));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeKava));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeBandChain));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeBluzelle));
    ASSERT_EQ(TWPublicKeyTypeED25519Cardano, TWCoinTypePublicKeyType(TWCoinTypeCardano));
    ASSERT_EQ(TWPublicKeyTypeED25519, TWCoinTypePublicKeyType(TWCoinTypeMultiversX));
    ASSERT_EQ(TWPublicKeyTypeED25519, TWCoinTypePublicKeyType(TWCoinTypeOasis));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeTHORChain));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeCryptoOrg));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeOsmosis));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeECash));

    ASSERT_EQ(TWPublicKeyTypeED25519, TWCoinTypePublicKeyType(TWCoinTypeAion));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeCallisto));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeDash));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeDecred));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeDogecoin));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeEOS));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeEthereum));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeEthereumClassic));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeGoChain));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeICON));
    ASSERT_EQ(TWPublicKeyTypeED25519, TWCoinTypePublicKeyType(TWCoinTypeKin));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeNULS));
    ASSERT_EQ(TWPublicKeyTypeED25519Blake2b, TWCoinTypePublicKeyType(TWCoinTypeNano));
    ASSERT_EQ(TWPublicKeyTypeED25519, TWCoinTypePublicKeyType(TWCoinTypeNimiq));
    ASSERT_EQ(TWPublicKeyTypeNIST256p1, TWCoinTypePublicKeyType(TWCoinTypeOntology));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypePOANetwork));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeXRP));
    ASSERT_EQ(TWPublicKeyTypeED25519, TWCoinTypePublicKeyType(TWCoinTypeStellar));
    ASSERT_EQ(TWPublicKeyTypeED25519, TWCoinTypePublicKeyType(TWCoinTypeTezos));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeTheta));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeThunderCore));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeViction));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeTron));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeVeChain));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1Extended, TWCoinTypePublicKeyType(TWCoinTypeWanchain));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeZcash));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeFiro));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeZelcash));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeRavencoin));
    ASSERT_EQ(TWPublicKeyTypeCURVE25519, TWCoinTypePublicKeyType(TWCoinTypeWaves));
    ASSERT_EQ(TWPublicKeyTypeNIST256p1, TWCoinTypePublicKeyType(TWCoinTypeNEO));
    ASSERT_EQ(TWPublicKeyTypeSECP256k1, TWCoinTypePublicKeyType(TWCoinTypeNebl));
}

TEST(TWCoinType, ValidateAddress) {
    ASSERT_TRUE(TWCoinTypeValidate(TWCoinTypeBitcoin, STRING("12dNaXQtN5Asn2YFwT1cvciCrJa525fAe4").get()));
    ASSERT_TRUE(TWCoinTypeValidate(TWCoinTypeBitcoin, STRING("bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4").get()));
}

TEST(TWCoinType, DeriveAddress) {
    auto pkData = DATA("0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
    auto publicKey = WRAP(TWPublicKey, TWPublicKeyCreateWithData(pkData.get(), TWPublicKeyTypeSECP256k1));

    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKeyAndDerivation(TWCoinTypeBitcoin, publicKey.get(), TWDerivationBitcoinSegwit));
    assertStringsEqual(address, "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4");
}

TEST(TWCoinType, TWCoinTypeDerivationPath) {
    auto res = TWCoinTypeDerivationPath(TWCoinTypeBitcoin);
    auto result = *reinterpret_cast<const std::string *>(res);
    ASSERT_EQ(result, "m/84'/0'/0'/0/0");
    TWStringDelete(res);
}

TEST(TWCoinType, TWCoinTypeDerivationPathWithDerivation) {
    auto res = TWCoinTypeDerivationPathWithDerivation(TWCoinTypeBitcoin, TWDerivationBitcoinLegacy);
    auto result = *reinterpret_cast<const std::string *>(res);
    ASSERT_EQ(result, "m/44'/0'/0'/0/0");
    TWStringDelete(res);
}

TEST(TWCoinType, TWCoinTypeDerivationPathWithDerivationSolana) {
    auto res = TWCoinTypeDerivationPathWithDerivation(TWCoinTypeSolana, TWDerivationSolanaSolana);
    auto result = *reinterpret_cast<const std::string *>(res);
    ASSERT_EQ(result, "m/44'/501'/0'/0'");
    TWStringDelete(res);
}

TEST(TWCoinType, TWCoinTypeDerivationPathPactus) {
    auto res = TWCoinTypeDerivationPath(TWCoinTypePactus);
    auto result = *reinterpret_cast<const std::string *>(res);
    ASSERT_EQ(result, "m/44'/21888'/3'/0'");
    TWStringDelete(res);
}

TEST(TWCoinType, TWCoinTypeDerivationPathWithDerivationPactusMainnet) {
    auto res = TWCoinTypeDerivationPathWithDerivation(TWCoinTypePactus, TWDerivationPactusMainnet);
    auto result = *reinterpret_cast<const std::string *>(res);
    ASSERT_EQ(result, "m/44'/21888'/3'/0'");
    TWStringDelete(res);
}

TEST(TWCoinType, TWCoinTypeDerivationPathWithDerivationPactusTestnet) {
    auto res = TWCoinTypeDerivationPathWithDerivation(TWCoinTypePactus, TWDerivationPactusTestnet);
    auto result = *reinterpret_cast<const std::string *>(res);
    ASSERT_EQ(result, "m/44'/21777'/3'/0'");
    TWStringDelete(res);
}
