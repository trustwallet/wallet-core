// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"

#include <TrustWalletCore/TWAccount.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWStoredKey.h>
#include <TrustWalletCore/TWData.h>
#include "../src/HexCoding.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <fstream>

extern std::string TESTS_ROOT;

using namespace TW;
using namespace std;


/// Return a StoredKey instance that can be used for further tests.  Needs to be deleted at the end.
struct std::shared_ptr<TWStoredKey> createAStoredKey(TWCoinType coin, TWData* password, TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr) {
    const auto mnemonic = WRAPS(TWStringCreateWithUTF8Bytes("team engine square letter hero song dizzy scrub tornado fabric divert saddle"));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto key = WRAP(TWStoredKey, TWStoredKeyImportHDWalletWithEncryption(mnemonic.get(), name.get(), password, coin, encryption));
    return key;
}

/// Return a StoredKey instance that can be used for further tests.  Needs to be deleted at the end.
struct std::shared_ptr<TWStoredKey> createDefaultStoredKey(TWStoredKeyEncryption encryption = TWStoredKeyEncryptionAes128Ctr) {
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    
    return createAStoredKey(TWCoinTypeBitcoin, password.get(), encryption);
}

TEST(TWStoredKey, loadPBKDF2Key) {
    const auto filename = WRAPS(TWStringCreateWithUTF8Bytes((TESTS_ROOT + "/common/Keystore/Data/pbkdf2.json").c_str()));
    const auto key = WRAP(TWStoredKey, TWStoredKeyLoad(filename.get()));
    const auto keyId = WRAPS(TWStoredKeyIdentifier(key.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(keyId.get())), "3198bc9c-6672-5ab3-d995-4942343ae5b6");
}

TEST(TWStoredKey, loadNonexistent) {
    const auto filenameInvalid = WRAPS(TWStringCreateWithUTF8Bytes((TESTS_ROOT + "_NO_/_SUCH_/_FILE_").c_str()));
    const auto nokey = WRAP(TWStoredKey, TWStoredKeyLoad(filenameInvalid.get()));
    EXPECT_EQ(nokey.get(), nullptr);
}

TEST(TWStoredKey, createWallet) {
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    const auto key = WRAP(TWStoredKey, TWStoredKeyCreateLevelAndEncryption(name.get(), password.get(), TWStoredKeyEncryptionLevelDefault, TWStoredKeyEncryptionAes128Ctr));
    const auto name2 = WRAPS(TWStoredKeyName(key.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(name2.get())), "name");
    const auto mnemonic = WRAPS(TWStoredKeyDecryptMnemonic(key.get(), password.get()));
    // mnemonic is random, check that it's reasonably long
    EXPECT_TRUE(TWStringSize(mnemonic.get()) > 36);
}

TEST(TWStoredKey, importPrivateKey) {
    const auto privateKeyHex = "3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266";
    const auto privateKey = WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes(privateKeyHex)).get()));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = WRAP(TWStoredKey, TWStoredKeyImportPrivateKey(privateKey.get(), name.get(), password.get(), coin));
    const auto privateKey2 = WRAPD(TWStoredKeyDecryptPrivateKey(key.get(), password.get()));
    EXPECT_EQ(hex(data(TWDataBytes(privateKey2.get()), TWDataSize(privateKey2.get()))), privateKeyHex);
    
    const auto privateKey3 = TWStoredKeyPrivateKey(key.get(), coin, password.get());
    const auto pkData3 = WRAPD(TWPrivateKeyData(privateKey3));
    EXPECT_EQ(hex(data(TWDataBytes(pkData3.get()), TWDataSize(pkData3.get()))), privateKeyHex);
    TWPrivateKeyDelete(privateKey3);
}

TEST(TWStoredKey, importPrivateKeyAes256) {
    const auto privateKeyHex = "3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266";
    const auto privateKey = WRAPD(TWDataCreateWithHexString(WRAPS(TWStringCreateWithUTF8Bytes(privateKeyHex)).get()));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = WRAP(TWStoredKey, TWStoredKeyImportPrivateKeyWithEncryption(privateKey.get(), name.get(), password.get(), coin, TWStoredKeyEncryptionAes256Ctr));
    const auto privateKey2 = WRAPD(TWStoredKeyDecryptPrivateKey(key.get(), password.get()));
    EXPECT_EQ(hex(data(TWDataBytes(privateKey2.get()), TWDataSize(privateKey2.get()))), privateKeyHex);

    const auto privateKey3 = TWStoredKeyPrivateKey(key.get(), coin, password.get());
    const auto pkData3 = WRAPD(TWPrivateKeyData(privateKey3));
    EXPECT_EQ(hex(data(TWDataBytes(pkData3.get()), TWDataSize(pkData3.get()))), privateKeyHex);
    TWPrivateKeyDelete(privateKey3);
}

TEST(TWStoredKey, importHDWallet) {
    const auto mnemonic = WRAPS(TWStringCreateWithUTF8Bytes("team engine square letter hero song dizzy scrub tornado fabric divert saddle"));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = WRAP(TWStoredKey, TWStoredKeyImportHDWallet(mnemonic.get(), name.get(), password.get(), coin));
    EXPECT_TRUE(TWStoredKeyIsMnemonic(key.get()));

    // invalid mnemonic
    const auto mnemonicInvalid = WRAPS(TWStringCreateWithUTF8Bytes("_THIS_IS_AN_INVALID_MNEMONIC_"));
    const auto nokey = WRAP(TWStoredKey, TWStoredKeyImportHDWallet(mnemonicInvalid.get(), name.get(), password.get(), coin));
    EXPECT_EQ(nokey.get(), nullptr);
}

TEST(TWStoredKey, importHDWalletAES256) {
    const auto mnemonic = WRAPS(TWStringCreateWithUTF8Bytes("team engine square letter hero song dizzy scrub tornado fabric divert saddle"));
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    const auto coin = TWCoinTypeBitcoin;
    const auto key = WRAP(TWStoredKey, TWStoredKeyImportHDWalletWithEncryption(mnemonic.get(), name.get(), password.get(), coin, TWStoredKeyEncryptionAes256Ctr));
    EXPECT_TRUE(TWStoredKeyIsMnemonic(key.get()));

    // invalid mnemonic
    const auto mnemonicInvalid = WRAPS(TWStringCreateWithUTF8Bytes("_THIS_IS_AN_INVALID_MNEMONIC_"));
    const auto nokey = WRAP(TWStoredKey, TWStoredKeyImportHDWalletWithEncryption(mnemonicInvalid.get(), name.get(), password.get(), coin, TWStoredKeyEncryptionAes256Ctr));
    EXPECT_EQ(nokey.get(), nullptr);
}

TEST(TWStoredKey, addressAddRemove) {
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));

    const auto coin = TWCoinTypeBitcoin;
    const auto key = createAStoredKey(coin, password.get());

    const auto wallet = WRAP(TWHDWallet, TWStoredKeyWallet(key.get(), password.get()));
    const auto accountCoin = WRAP(TWAccount, TWStoredKeyAccountForCoin(key.get(), coin, wallet.get()));
    const auto accountAddress = WRAPS(TWAccountAddress(accountCoin.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(accountAddress.get())), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");

    EXPECT_EQ(TWStoredKeyAccountCount(key.get()), 1ul);
    const auto accountIdx = WRAP(TWAccount, TWStoredKeyAccount(key.get(), 0));

    TWStoredKeyRemoveAccountForCoin(key.get(), coin);
    EXPECT_EQ(TWStoredKeyAccountCount(key.get()), 0ul);

    const auto addressAdd = "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny";
    const auto derivationPath = "m/84'/0'/0'/0/0";
    const auto extPubKeyAdd = "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn";
    const auto pubKey = "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006";

    TWStoredKeyAddAccount(key.get(),
        WRAPS(TWStringCreateWithUTF8Bytes(addressAdd)).get(),
        TWCoinTypeBitcoin,
        WRAPS(TWStringCreateWithUTF8Bytes(derivationPath)).get(),
        WRAPS(TWStringCreateWithUTF8Bytes(pubKey)).get(),
        WRAPS(TWStringCreateWithUTF8Bytes(extPubKeyAdd)).get());
    EXPECT_EQ(TWStoredKeyAccountCount(key.get()), 1ul);

    // invalid account index
    EXPECT_EQ(TWStoredKeyAccount(key.get(), 1001), nullptr);
}

TEST(TWStoredKey, addressAddRemoveDerivationPath) {
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));

    const auto coin = TWCoinTypeBitcoin;
    const auto key = createAStoredKey(coin, password.get());

    const auto wallet = WRAP(TWHDWallet, TWStoredKeyWallet(key.get(), password.get()));
    const auto accountCoin = WRAP(TWAccount, TWStoredKeyAccountForCoin(key.get(), coin, wallet.get()));
    const auto accountAddress = WRAPS(TWAccountAddress(accountCoin.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(accountAddress.get())), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");

    EXPECT_EQ(TWStoredKeyAccountCount(key.get()), 1ul);
    const auto accountIdx = WRAP(TWAccount, TWStoredKeyAccount(key.get(), 0));

    const auto derivationPath0 = "m/84'/0'/0'/0/0";
    const auto derivationPath1 = "m/84'/0'/0'/1/0";

    TWStoredKeyRemoveAccountForCoinDerivationPath(key.get(), coin, WRAPS(TWStringCreateWithUTF8Bytes(derivationPath1)).get());
    EXPECT_EQ(TWStoredKeyAccountCount(key.get()), 1ul);

    TWStoredKeyRemoveAccountForCoinDerivationPath(key.get(), coin, WRAPS(TWStringCreateWithUTF8Bytes(derivationPath0)).get());
    EXPECT_EQ(TWStoredKeyAccountCount(key.get()), 0ul);
}

TEST(TWStoredKey, addressAddDerivation) {
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));

    const auto coin = TWCoinTypeBitcoin;
    const auto key = createAStoredKey(coin, password.get());
    const auto wallet = WRAP(TWHDWallet, TWStoredKeyWallet(key.get(), password.get()));

    const auto accountCoin1 = WRAP(TWAccount, TWStoredKeyAccountForCoinDerivation(key.get(), coin, TWDerivationDefault, wallet.get()));
    const auto accountAddress1 = WRAPS(TWAccountAddress(accountCoin1.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(accountAddress1.get())), "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");

    const auto accountCoin2 = WRAP(TWAccount, TWStoredKeyAccountForCoinDerivation(key.get(), coin, TWDerivationBitcoinLegacy, wallet.get()));
    const auto accountAddress2 = WRAPS(TWAccountAddress(accountCoin2.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(accountAddress2.get())), "1NyRyFewhZcWMa9XCj3bBxSXPXyoSg8dKz");

    EXPECT_EQ(TWStoredKeyAccountCount(key.get()), 2ul);
}

TEST(TWStoredKey, exportJSON) {
    const auto key = createDefaultStoredKey();
    const auto json = WRAPD(TWStoredKeyExportJSON(key.get()));
    // check size and first character
    EXPECT_TRUE(TWDataSize(json.get()) > 100);
    EXPECT_EQ(TWDataGet(json.get(), 0), '{');
}

TEST(TWStoredKey, storeAndImportJSONAES256) {
    const auto key = createDefaultStoredKey(TWStoredKeyEncryptionAes256Ctr);
    const auto outFileName = string(getTestTempDir() + "/TWStoredKey_store.json");
    const auto outFileNameStr = WRAPS(TWStringCreateWithUTF8Bytes(outFileName.c_str()));
    EXPECT_TRUE(TWStoredKeyStore(key.get(), outFileNameStr.get()));

    // read contents of file
    ifstream ifs(outFileName);
    // get length of file:
    ifs.seekg (0, ifs.end);
    auto length = ifs.tellg();
    ifs.seekg (0, ifs.beg);
    EXPECT_TRUE(length > 20);

    Data json(length);
    size_t idx = 0;
    // read the slow way, ifs.read gave some false warnings with codacy
    while (!ifs.eof() && idx < static_cast<std::size_t>(length)) { char c = ifs.get(); json[idx++] = (uint8_t)c; }

    const auto key2 = WRAP(TWStoredKey, TWStoredKeyImportJSON(WRAPD(TWDataCreateWithData(&json)).get()));
    const auto name2 = WRAPS(TWStoredKeyName(key2.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(name2.get())), "name");
}

TEST(TWStoredKey, storeAndImportJSON) {
    const auto key = createDefaultStoredKey();
    const auto outFileName = string(getTestTempDir() + "/TWStoredKey_store.json");
    const auto outFileNameStr = WRAPS(TWStringCreateWithUTF8Bytes(outFileName.c_str()));
    EXPECT_TRUE(TWStoredKeyStore(key.get(), outFileNameStr.get()));
    //EXPECT_TRUE(filesystem::exists(outFileName));  // some linker issues with filesystem
    
    // read contents of file
    ifstream ifs(outFileName);
    // get length of file:
    ifs.seekg (0, ifs.end);
    auto length = ifs.tellg();
    ifs.seekg (0, ifs.beg);
    EXPECT_TRUE(length > 20);

    Data json(length);
    size_t idx = 0;
    // read the slow way, ifs.read gave some false warnings with codacy 
    while (!ifs.eof() && idx < static_cast<std::size_t>(length)) { char c = ifs.get(); json[idx++] = (uint8_t)c; }

    const auto key2 = WRAP(TWStoredKey, TWStoredKeyImportJSON(WRAPD(TWDataCreateWithData(&json)).get()));
    const auto name2 = WRAPS(TWStoredKeyName(key2.get()));
    EXPECT_EQ(string(TWStringUTF8Bytes(name2.get())), "name");
}

TEST(TWStoredKey, importJsonInvalid) {
    const string jsonInvalidStr = "]]]}}}_THIS_IS_AN_INVALID_JSON_}}}]]]";
    auto json = WRAPD(TWDataCreateWithBytes((const uint8_t*)jsonInvalidStr.c_str(), jsonInvalidStr.length()));
    const auto nokey = WRAP(TWStoredKey, TWStoredKeyImportJSON(json.get()));
    EXPECT_EQ(nokey, nullptr);
}

TEST(TWStoredKey, fixAddresses) {
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    
    const auto key = createAStoredKey(TWCoinTypeBitcoin, password.get());
    EXPECT_TRUE(TWStoredKeyFixAddresses(key.get(), password.get()));
}

TEST(TWStoredKey, importInvalidKey) {
    auto bytes = TW::parse_hex("fffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");
    auto data = WRAPD(TWDataCreateWithBytes(bytes.data(), bytes.size()));
    auto name = WRAPS(TWStringCreateWithUTF8Bytes("test"));
    auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(name.get())), TWStringSize(name.get())));

    auto eth = WRAP(TWStoredKey, TWStoredKeyImportPrivateKey(data.get(), name.get(), password.get(), TWCoinTypeEthereum));
    auto ont = WRAP(TWStoredKey, TWStoredKeyImportPrivateKey(data.get(), name.get(), password.get(), TWCoinTypeOntology));
    auto tezos = WRAP(TWStoredKey, TWStoredKeyImportPrivateKey(data.get(), name.get(), password.get(), TWCoinTypeTezos));

    ASSERT_EQ(eth, nullptr);
    ASSERT_EQ(ont, nullptr);
    ASSERT_NE(tezos, nullptr);
}

TEST(TWStoredKey, removeAccountForCoin) {
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("Test KeyStore"));
    auto key = WRAP(TWStoredKey, TWStoredKeyCreate(name.get(), password.get()));
    auto wallet = WRAP(TWHDWallet, TWStoredKeyWallet(key.get(), password.get()));
    
    ASSERT_NE(WRAP(TWAccount, TWStoredKeyAccountForCoin(key.get(), TWCoinTypeEthereum, wallet.get())).get(), nullptr);
    ASSERT_NE(WRAP(TWAccount, TWStoredKeyAccountForCoin(key.get(), TWCoinTypeBitcoin, wallet.get())).get(), nullptr);
    
    ASSERT_EQ(TWStoredKeyAccountCount(key.get()), 2ul);
    
    TWStoredKeyRemoveAccountForCoin(key.get(), TWCoinTypeBitcoin);
    
    ASSERT_EQ(TWStoredKeyAccountCount(key.get()), 1ul);
    
    ASSERT_NE(WRAP(TWAccount, TWStoredKeyAccountForCoin(key.get(), TWCoinTypeEthereum, nullptr)).get(), nullptr);
    ASSERT_EQ(WRAP(TWAccount, TWStoredKeyAccountForCoin(key.get(), TWCoinTypeBitcoin, nullptr)).get(), nullptr);
}

TEST(TWStoredKey, getWalletPasswordInvalid) {
    const auto name = WRAPS(TWStringCreateWithUTF8Bytes("name"));
    const auto passwordString = WRAPS(TWStringCreateWithUTF8Bytes("password"));
    const auto password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(passwordString.get())), TWStringSize(passwordString.get())));
    
    const auto invalidString = WRAPS(TWStringCreateWithUTF8Bytes("_THIS_IS_INVALID_PASSWORD_"));
    const auto passwordInvalid = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(TWStringUTF8Bytes(invalidString.get())), TWStringSize(invalidString.get())));
    
    auto key = WRAP(TWStoredKey, TWStoredKeyCreate(name.get(), password.get()));
    ASSERT_NE(WRAP(TWHDWallet, TWStoredKeyWallet(key.get(), password.get())).get(), nullptr);
    ASSERT_EQ(WRAP(TWHDWallet, TWStoredKeyWallet(key.get(), passwordInvalid.get())).get(), nullptr);
}

TEST(TWStoredKey, encryptionParameters) {
    const auto key = createDefaultStoredKey();
    const auto params = WRAPS(TWStoredKeyEncryptionParameters(key.get()));

    nlohmann::json jsonParams = nlohmann::json::parse(string(TWStringUTF8Bytes(params.get())));

    // compare some specific parameters
    EXPECT_EQ(jsonParams["kdfparams"]["n"], 16384);
    EXPECT_EQ(std::string(jsonParams["cipherparams"]["iv"]).length(), 32ul);

    // compare all keys, except dynamic ones (like cipherparams/iv)
    jsonParams["cipherparams"] = {};
    jsonParams["ciphertext"] = "<ciphertext>";
    jsonParams["kdfparams"]["salt"] = "<salt>";
    jsonParams["mac"] = "<mac>";
    const auto params2 = jsonParams.dump();
    assertJSONEqual(params2, R"(
        {
            "cipher": "aes-128-ctr",
            "cipherparams": null,
            "ciphertext": "<ciphertext>",
            "kdf": "scrypt",
            "kdfparams": {
                "dklen": 32,
                "n": 16384,
                "p": 4,
                "r": 8,
                "salt": "<salt>"
            },
            "mac": "<mac>"
        }        
    )");
}
