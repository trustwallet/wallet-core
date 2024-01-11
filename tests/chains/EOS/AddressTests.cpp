// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "EOS/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWPublicKeyType.h>

#include <gtest/gtest.h>

using namespace TW;

namespace TW::EOS::tests {

TEST(EOSAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"));
    ASSERT_FALSE(Address::isValid("EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjT"));
    ASSERT_FALSE(Address::isValid("PUB_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe"));
    ASSERT_FALSE(Address::isValid("PUB_K1_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe"));

    ASSERT_THROW(Address("PUB_K1_65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF"), std::invalid_argument);
    ASSERT_THROW(EOS::Address(Data(0)), std::invalid_argument);
}

TEST(EOSAddress, Base58) {
    ASSERT_EQ(
        Address("EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF").string(),
        "EOS65QzSGJ579GPNKtZoZkChTzsxR4B48RCfiS82m2ymJR6VZCjTF");
    ASSERT_EQ(
        Address("EOS55hdeEZHoArE8LLTv6drj2yR1K1AH8wAPT4kjTVSnkmQc3nzwQ").string(),
        "EOS55hdeEZHoArE8LLTv6drj2yR1K1AH8wAPT4kjTVSnkmQc3nzwQ");
    ASSERT_EQ(
        Address("PUB_R1_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe").string(),
        "PUB_R1_5hieQEFWh68h6bjaYAY25Ptd2bmqLCaFsunaneh9gZsmSgUBUe");
    ASSERT_EQ(
        Address("PUB_R1_7M9ckjr6p5CmS3N3yLPg9vcTB5NHmLcMHwZ3iGccEVfbjJRHv3").string(),
        "PUB_R1_7M9ckjr6p5CmS3N3yLPg9vcTB5NHmLcMHwZ3iGccEVfbjJRHv3");
}

TEST(EOSAddress, FromPrivateKey) {
    std::string privArray[]{"8e14ef506fee5e0aaa32f03a45242d32d0eb993ffe25ce77542ef07219db667c",
                            "e2bfd815c5923f404388a3257aa5527f0f52e92ce364e1e26a04d270c901edda",
                            "e6b783120a21cb234d8e15077ce186c47261d1043781ab8b16b84f2acd377668",
                            "bb96c0a4a6ec9c93ccc0b2cbad6b0e8110b9ca4731aef9c6937b99552a319b03"};

    Type privTypes[]{Type::Legacy, Type::Legacy, Type::ModernR1, Type::ModernK1};

    std::string pubArray[]{"EOS6TFKUKVvtvjRq9T4fV9pdxNUuJke92nyb4rzSFtZfdR5ssmVuY",
                           "EOS5YtaCcbPJ3BknNBTDezE9eJoGNnAVuUwT8bnxhSRS5dqRvyfxr",
                           "PUB_R1_67itCyDj42CRgtpyP4fLbAccBYnVHGeZQujQAeK3fyNbvfvZM6",
                           "PUB_K1_6enPVMggisfqVVRZ1tj47d9UeHK46CBssoCmAz6sLDMBdtZk78"};

    for (int i = 0; i < 4; i++) {
        const auto privateKey = PrivateKey(parse_hex(privArray[i]));
        const auto publicKey = PublicKey(privateKey.getPublicKey(privTypes[i] == Type::ModernR1 ? TWPublicKeyTypeNIST256p1 : TWPublicKeyTypeSECP256k1));
        const auto address = Address(publicKey, privTypes[i]);

        ASSERT_EQ(address.string(), pubArray[i]);
    }
}

TEST(EOSAddress, IsValid) {
    ASSERT_TRUE(Address::isValid("EOS6Vm7RWMS1KKAM9kDXgggpu4sJkFMEpARhmsWA84tk4P22m29AV"));
    ASSERT_TRUE(Address::isValid("PUB_R1_6pQRUVU5vdneRnmjSiZPsvu3zBqcptvg6iK2Vz4vKo4ugnzow3"));
    ASSERT_TRUE(Address::isValid("EOS5mGcPvsqFDe8YRrA3yMMjQgjrCa6yiCho79KViDhvxh4ajQjgS"));
    ASSERT_TRUE(Address::isValid("PUB_R1_82dMu3zSSfyHYc4cvWJ6SPsHZWB5mBNAyhL53xiM5xpqmfqetN"));
    ASSERT_TRUE(Address::isValid("PUB_K1_6enPVMggisfqVVRZ1tj47d9UeHK46CBssoCmAz6sLDMBdtZk78"));

    ASSERT_NO_THROW(Address(parse_hex("039d91164ea04f4e751762643ef4ae520690af361b8e677cf341fd213419956b356cb721b7"), Type::ModernR1));
    ASSERT_NO_THROW(Address(parse_hex("02d3c8e736a9a50889766caf3c37bd16e2fecc7340b3130e25d4c01b153f996a10a78afc0e"), Type::Legacy));
}

} // namespace TW::EOS::tests
