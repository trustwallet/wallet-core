// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "FIO/Encryption.h"
#include "FIO/Address.h"
#include "Data.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

#include <stdlib.h>

namespace TW::FIO::EncryptionTests {

using namespace std;

TEST(FIOEncryption, checkEncrypt) {
    // test derived from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-check.test.ts
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    const Data plaintext = TW::data("secret message");
    Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");

    Data result = Encryption::checkEncrypt(secret, plaintext, iv);
    EXPECT_EQ(hex(result), "f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224da7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab");
}

TEST(FIOEncryption, checkDecrypt) {
    // test derived from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-check.test.ts
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    const Data encrypted = parse_hex("f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224da7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab");
    const Data expectedPlaintext = TW::data("secret message");

    Data result = Encryption::checkDecrypt(secret, encrypted);
    EXPECT_EQ(hex(result), hex(expectedPlaintext));
}

TEST(FIOEncryption, checkEncryptInvalidIvLength) {
    try {
        Data iv = Data(5); // wrong size
        Encryption::checkEncrypt(Data(64), Data(100), iv);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(FIOEncryption, checkDecryptInvalidMessageHMAC) {
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    const Data encrypted = parse_hex("f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224da7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab00");
    try {
        Encryption::checkDecrypt(secret, encrypted);
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

TEST(FIOEncryption, checkDecryptMessageTooShort) {
    const Data secret = parse_hex("02332627b9325cb70510a70f0f6be4bcb008fbbc7893ca51dedf5bf46aa740c0fc9d3fbd737d09a3c4046d221f4f1a323f515332c3fef46e7f075db561b1a2c9");
    try {
        Encryption::checkDecrypt(secret, Data(60));
    } catch (std::invalid_argument&) {
        // expected exception, OK
        return;
    }
    ADD_FAILURE() << "Missed expected exeption";
}

Data randomBuffer(size_t size) {
    Data d(size);
    for (auto i = 0ul; i < size; ++i) {
        d[i] = (TW::byte)(256.0 * rand() / RAND_MAX);
    }
    return d;
}

void checkEncryptDecryptRandomOne(size_t messageLen) {
    // Encrypt, decrypt, and obtain the same
    // random (but reproducible) input
    Data secret = randomBuffer(64);
    Data message = randomBuffer(messageLen);
    Data iv = randomBuffer(16);

    Data encrypted = Encryption::checkEncrypt(secret, message, iv);
    Data decrypted = Encryption::checkDecrypt(secret, encrypted);

    EXPECT_EQ(message.size(), decrypted.size());
}

TEST(FIOEncryption, checkEncryptDecryptRandom) {
    // Encrypt, decrypt, and obtain the same
    // Random, but reproducible input
    srand(1);
    for (auto i = 0; i < 5; ++i) {
        // random message length
        size_t messageLen = 1 + (rand() % 60);
        checkEncryptDecryptRandomOne(messageLen);
    }
    // some even-block lengths
    checkEncryptDecryptRandomOne(16);
    checkEncryptDecryptRandomOne(32);
    checkEncryptDecryptRandomOne(64);
}

TEST(FIOEncryption, getSharedSecret) {
    // tests extracted from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-fio.test.ts
    // See also https://github.com/fioprotocol/fiojs/blob/master/docs/message_encryption.md
    {
        const PrivateKey privateKey(parse_hex("2bd806c97f0e00af1a1fc3328fa763a9269723c8db8fac4f93af71db186d6e90"));
        const PublicKey publicKey(parse_hex("024edfcf9dfe6c0b5c83d1ab3f78d1b39a46ebac6798e08e19761f5ed89ec83c10"), TWPublicKeyTypeSECP256k1);
        Data secret = Encryption::getSharedSecret(privateKey, publicKey);
        EXPECT_EQ(secret.size(), 64ul);
        EXPECT_EQ(hex(secret), "a71b4ec5a9577926a1d2aa1d9d99327fd3b68f6a1ea597200a0d890bd3331df300a2d49fec0b2b3e6969ce9263c5d6cf47c191c1ef149373ecc9f0d98116b598");
    }
    {
        const PrivateKey privateKey(parse_hex("81b637d8fcd2c6da6359e6963113a1170de795e4b725b84d1e0b4cfd9ec58ce9"));
        const PublicKey publicKey(parse_hex("039997a497d964fc1a62885b05a51166a65a90df00492c8d7cf61d6accf54803be"), TWPublicKeyTypeSECP256k1);
        Data secret = Encryption::getSharedSecret(privateKey, publicKey);
        EXPECT_EQ(secret.size(), 64ul);
        EXPECT_EQ(hex(secret), "a71b4ec5a9577926a1d2aa1d9d99327fd3b68f6a1ea597200a0d890bd3331df300a2d49fec0b2b3e6969ce9263c5d6cf47c191c1ef149373ecc9f0d98116b598");
    }
    {
        const PrivateKey privateKey(parse_hex("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
        const PublicKey publicKey(parse_hex("03a34b99f22c790c4e36b2b3c2c35a36db06226e41c692fc82b8b56ac1c540c5bd"), TWPublicKeyTypeSECP256k1);
        Data secret = Encryption::getSharedSecret(privateKey, publicKey);
        EXPECT_EQ(secret.size(), 64ul);
        EXPECT_EQ(hex(secret), "3f0840df1912e24d85f39008a56550c31403e096fce7fa9d7886fab8e5c2ceb66b4139c8f4f4172fd9f455e76c2e8913a3d734f51a1951090ce9ec660671957d");
    }
}

TEST(FIOEncryption, encryptAndEncode) {
    // tests extracted from https://github.com/fioprotocol/fiojs/blob/master/src/tests/encryption-fio.test.ts
    {
        const PrivateKey privateKey(parse_hex("2bd806c97f0e00af1a1fc3328fa763a9269723c8db8fac4f93af71db186d6e90"));
        const PublicKey publicKey(parse_hex("024edfcf9dfe6c0b5c83d1ab3f78d1b39a46ebac6798e08e19761f5ed89ec83c10"), TWPublicKeyTypeSECP256k1);
        const Data message = parse_hex("0b70757273652e616c69636501310a66696f2e7265716f6274000000");
        const Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");
        const Data encrypted = Encryption::encrypt(privateKey, publicKey, message, iv);
        EXPECT_EQ(hex(encrypted), "f300888ca4f512cebdc0020ff0f7224c0db2984c4ad9afb12629f01a8c6a76328bbde17405655dc4e3cb30dad272996fb1dea8e662e640be193e25d41147a904c571b664a7381ab41ef062448ac1e205");
        const string encoded = Encryption::encode(encrypted);
        EXPECT_EQ(encoded, "8wCIjKT1Es69wAIP8PciTA2ymExK2a+xJinwGoxqdjKLveF0BWVdxOPLMNrScplvsd6o5mLmQL4ZPiXUEUepBMVxtmSnOBq0HvBiRIrB4gU=");
    }
    {
        const PrivateKey privateKey(parse_hex("81b637d8fcd2c6da6359e6963113a1170de795e4b725b84d1e0b4cfd9ec58ce9"));
        const PublicKey publicKey(parse_hex("039997a497d964fc1a62885b05a51166a65a90df00492c8d7cf61d6accf54803be"), TWPublicKeyTypeSECP256k1);
        const Data message = parse_hex("0b70757273652e616c69636501310a66696f2e7265716f6274000000");
        const Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");
        const Data encrypted = Encryption::encrypt(privateKey, publicKey, message, iv);
        EXPECT_EQ(hex(encrypted), "f300888ca4f512cebdc0020ff0f7224c0db2984c4ad9afb12629f01a8c6a76328bbde17405655dc4e3cb30dad272996fb1dea8e662e640be193e25d41147a904c571b664a7381ab41ef062448ac1e205");
        const string encoded = Encryption::encode(encrypted);
        EXPECT_EQ(encoded, "8wCIjKT1Es69wAIP8PciTA2ymExK2a+xJinwGoxqdjKLveF0BWVdxOPLMNrScplvsd6o5mLmQL4ZPiXUEUepBMVxtmSnOBq0HvBiRIrB4gU=");
    }
}

TEST(FIOEncryption, encryptEncodeDecodeDecrypt) {
    const PrivateKey privateKeyAlice(parse_hex("2bd806c97f0e00af1a1fc3328fa763a9269723c8db8fac4f93af71db186d6e90"));
    const PublicKey publicKeyAlice = privateKeyAlice.getPublicKey(TWPublicKeyTypeSECP256k1);
    const Address addressAlice(publicKeyAlice);
    EXPECT_EQ(addressAlice.string(), "FIO7zsqi7QUAjTAdyynd6DVe8uv4K8gCTRHnAoMN9w9CA1xLCTDVv");
    const PrivateKey privateKeyBob(parse_hex("81b637d8fcd2c6da6359e6963113a1170de795e4b725b84d1e0b4cfd9ec58ce9"));
    const PublicKey publicKeyBob = privateKeyBob.getPublicKey(TWPublicKeyTypeSECP256k1);
    const Address addressBob(publicKeyBob);
    EXPECT_EQ(addressBob.string(), "FIO5VE6Dgy9FUmd1mFotXwF88HkQN1KysCWLPqpVnDMjRvGRi1YrM");
    const Data message = parse_hex("0b70757273652e616c69636501310a66696f2e7265716f6274000000");
    const Data iv = parse_hex("f300888ca4f512cebdc0020ff0f7224c");

    const Data encrypted = Encryption::encrypt(privateKeyAlice, publicKeyBob, message, iv);
    EXPECT_EQ(hex(encrypted), "f300888ca4f512cebdc0020ff0f7224c0db2984c4ad9afb12629f01a8c6a76328bbde17405655dc4e3cb30dad272996fb1dea8e662e640be193e25d41147a904c571b664a7381ab41ef062448ac1e205");
    const string encoded = Encryption::encode(encrypted);
    EXPECT_EQ(encoded, "8wCIjKT1Es69wAIP8PciTA2ymExK2a+xJinwGoxqdjKLveF0BWVdxOPLMNrScplvsd6o5mLmQL4ZPiXUEUepBMVxtmSnOBq0HvBiRIrB4gU=");

    const Data decoded = Encryption::decode(encoded);
    EXPECT_EQ(hex(decoded), "f300888ca4f512cebdc0020ff0f7224c0db2984c4ad9afb12629f01a8c6a76328bbde17405655dc4e3cb30dad272996fb1dea8e662e640be193e25d41147a904c571b664a7381ab41ef062448ac1e205");
    const Data decrypted = Encryption::decrypt(privateKeyBob, publicKeyAlice, decoded);
    // verify that decrypted is the same as the original
    EXPECT_EQ(hex(decrypted), hex(message));
}

} // namespace TW::FIO::EncryptionTests
