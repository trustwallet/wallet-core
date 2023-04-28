// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Keystore/StoredKey.h"

#include "Bitcoin/Address.h"
#include "Coin.h"
#include "Data.h"
#include "HexCoding.h"
#include "Mnemonic.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>
#include <stdexcept>

extern std::string TESTS_ROOT;

namespace TW::Keystore::tests {

using namespace std;

const auto passwordString = "password";
const auto gPassword = TW::data(string(passwordString));
const auto gMnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
const TWCoinType coinTypeBc = TWCoinTypeBitcoin;
const TWCoinType coinTypeBnb = TWCoinTypeBinance;
const TWCoinType coinTypeBsc = TWCoinTypeSmartChain;
const TWCoinType coinTypeEth = TWCoinTypeEthereum;
const TWCoinType coinTypeBscLegacy = TWCoinTypeSmartChainLegacy;

const std::string testDataPath(const char* subpath) {
    return TESTS_ROOT + "/common/Keystore/Data/" + subpath;
}

TEST(StoredKey, CreateWithMnemonic) {
    auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    const Data& mnemo2Data = key.payload.decrypt(gPassword);
    EXPECT_EQ(string(mnemo2Data.begin(), mnemo2Data.end()), string(gMnemonic));
    EXPECT_EQ(key.accounts.size(), 0ul);
    EXPECT_EQ(key.wallet(gPassword).getMnemonic(), string(gMnemonic));

    const auto json = key.json();
    EXPECT_EQ(json["name"], "name");
    EXPECT_EQ(json["type"], "mnemonic");
    EXPECT_EQ(json["version"], 3);
}

TEST(StoredKey, CreateWithMnemonicInvalid) {
    try {
        auto key = StoredKey::createWithMnemonic("name", gPassword, "_THIS_IS_NOT_A_VALID_MNEMONIC_", TWStoredKeyEncryptionLevelDefault);
    } catch (std::invalid_argument&) {
        // expedcted exception OK
        return;
    }
    FAIL() << "Missing excpected excpetion";
}

TEST(StoredKey, CreateWithMnemonicRandom) {
    const auto key = StoredKey::createWithMnemonicRandom("name", gPassword, TWStoredKeyEncryptionLevelDefault);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    // random mnemonic: check only length and validity
    const Data& mnemo2Data = key.payload.decrypt(gPassword);
    EXPECT_TRUE(mnemo2Data.size() >= 36);
    EXPECT_TRUE(Mnemonic::isValid(string(mnemo2Data.begin(), mnemo2Data.end())));
    EXPECT_EQ(key.accounts.size(), 0ul);
}

TEST(StoredKey, CreateWithMnemonicAddDefaultAddress) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("name", gPassword, gMnemonic, coinTypeBc);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);

    const Data& mnemo2Data = key.payload.decrypt(gPassword);

    EXPECT_EQ(string(mnemo2Data.begin(), mnemo2Data.end()), string(gMnemonic));
    EXPECT_EQ(key.accounts.size(), 1ul);
    EXPECT_EQ(key.accounts[0].coin, coinTypeBc);
    EXPECT_EQ(key.accounts[0].address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    EXPECT_EQ(key.accounts[0].publicKey, "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006");
    EXPECT_EQ(key.accounts[0].extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
    EXPECT_EQ(hex(key.privateKey(coinTypeBc, gPassword).bytes), "d2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f");
}

TEST(StoredKey, CreateWithMnemonicAddDefaultAddressAes256) {
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("name", gPassword, gMnemonic, coinTypeBc, TWStoredKeyEncryptionAes256Ctr);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    auto header = key.payload;
    EXPECT_EQ(header.params.cipher(), "aes-256-ctr");
    const Data& mnemo2Data = key.payload.decrypt(gPassword);

    EXPECT_EQ(string(mnemo2Data.begin(), mnemo2Data.end()), string(gMnemonic));
    EXPECT_EQ(key.accounts.size(), 1ul);
    EXPECT_EQ(key.accounts[0].coin, coinTypeBc);
    EXPECT_EQ(key.accounts[0].address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    EXPECT_EQ(key.accounts[0].publicKey, "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006");
    EXPECT_EQ(key.accounts[0].extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
    EXPECT_EQ(hex(key.privateKey(coinTypeBc, gPassword).bytes), "d2568511baea8dc347f14c4e0479eb8ebe29eb5f664ed796e755896250ffd11f");
}

TEST(StoredKey, CreateWithPrivateKeyAddDefaultAddress) {
    const auto privateKey = parse_hex("3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266");
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("name", gPassword, coinTypeBc, privateKey);
    EXPECT_EQ(key.type, StoredKeyType::privateKey);
    EXPECT_EQ(key.accounts.size(), 1ul);
    EXPECT_EQ(key.accounts[0].coin, coinTypeBc);
    EXPECT_EQ(key.accounts[0].address, "bc1q375sq4kl2nv0mlmup3vm8znn4eqwu7mt6hkwhr");
    EXPECT_EQ(hex(key.privateKey(coinTypeBc, gPassword).bytes), hex(privateKey));

    const auto json = key.json();
    EXPECT_EQ(json["name"], "name");
    EXPECT_EQ(json["type"], "private-key");
    EXPECT_EQ(json["version"], 3);
}

TEST(StoredKey, CreateWithPrivateKeyAddDefaultAddressAes256) {
    const auto privateKey = parse_hex("3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266");
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("name", gPassword, coinTypeBc, privateKey, TWStoredKeyEncryptionAes256Ctr);
    auto header = key.payload;
    EXPECT_EQ(header.params.cipher(), "aes-256-ctr");
    EXPECT_EQ(key.type, StoredKeyType::privateKey);
    EXPECT_EQ(key.accounts.size(), 1ul);
    EXPECT_EQ(key.accounts[0].coin, coinTypeBc);
    EXPECT_EQ(key.accounts[0].address, "bc1q375sq4kl2nv0mlmup3vm8znn4eqwu7mt6hkwhr");
    EXPECT_EQ(hex(key.privateKey(coinTypeBc, gPassword).bytes), hex(privateKey));

    const auto json = key.json();
    EXPECT_EQ(json["name"], "name");
    EXPECT_EQ(json["type"], "private-key");
    EXPECT_EQ(json["version"], 3);
}

TEST(StoredKey, CreateWithPrivateKeyAddDefaultAddressInvalid) {
    try {
        const auto privateKeyInvalid = parse_hex("0001020304");
        auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("name", gPassword, coinTypeBc, privateKeyInvalid);
    } catch (std::invalid_argument&) {
        // expected exception ok
        return;
    }
    FAIL() << "Missing expected exception";
}

TEST(StoredKey, AccountGetCreate) {
    auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);
    EXPECT_EQ(key.accounts.size(), 0ul);

    // not exists
    EXPECT_FALSE(key.account(coinTypeBc).has_value());
    EXPECT_EQ(key.accounts.size(), 0ul);

    auto wallet = key.wallet(gPassword);
    // not exists, wallet null, not create
    EXPECT_FALSE(key.account(coinTypeBc, nullptr).has_value());
    EXPECT_EQ(key.accounts.size(), 0ul);

    // not exists, wallet nonnull, create
    std::optional<Account> acc3 = key.account(coinTypeBc, &wallet);
    EXPECT_TRUE(acc3.has_value());
    EXPECT_EQ(acc3->coin, coinTypeBc); 
    EXPECT_EQ(key.accounts.size(), 1ul);

    // exists
    std::optional<Account> acc4 = key.account(coinTypeBc);
    EXPECT_TRUE(acc4.has_value());
    EXPECT_EQ(acc4->coin, coinTypeBc); 
    EXPECT_EQ(key.accounts.size(), 1ul);

    // exists, wallet nonnull, not create
    std::optional<Account> acc5 = key.account(coinTypeBc, &wallet);
    EXPECT_TRUE(acc5.has_value());
    EXPECT_EQ(acc5->coin, coinTypeBc); 
    EXPECT_EQ(key.accounts.size(), 1ul);

    // exists, wallet null, not create
    std::optional<Account> acc6 = key.account(coinTypeBc, nullptr);
    EXPECT_TRUE(acc6.has_value());
    EXPECT_EQ(acc6->coin, coinTypeBc); 
    EXPECT_EQ(key.accounts.size(), 1ul);
}

TEST(StoredKey, AccountGetDoesntChange) {
    auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);
    auto wallet = key.wallet(gPassword);
    EXPECT_EQ(key.accounts.size(), 0ul);

    vector<TWCoinType> coins = {coinTypeBc, coinTypeEth, coinTypeBnb};
    // retrieve multiple accounts, which will be created
    vector<Account> accounts;
    for (auto coin : coins) {
        std::optional<Account> account = key.account(coin, &wallet);
        accounts.push_back(*account);

        // check
        ASSERT_TRUE(account.has_value());
        EXPECT_EQ(account->coin, coin);
    }

    // Check again; make sure returned references don't change
    for (auto i = 0ul; i < accounts.size(); ++i) {
        // check
        EXPECT_EQ(accounts[i].coin, coins[i]);
    }
}

TEST(StoredKey, AddRemoveAccount) {
    auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);
    EXPECT_EQ(key.accounts.size(), 0ul);

    {
        const auto derivationPath = DerivationPath("m/84'/0'/0'/0/0");
        key.addAccount("bc1qaucw06s3agez8tyyk4zj9kt0q2934e3mcewdpf", coinTypeBc, TWDerivationDefault, derivationPath, "", "zpub6rxtad3SPT1C5GUDjPiKQ5oJN5DBeMbdUR7LrdYt12VbU7TBSpGUkdLvfVYGuj1N5edkDoZ3bu1fdN1HprQYfCBdsSH5CaAAygHGsanwtTe");
        EXPECT_EQ(key.accounts.size(), 1ul);
    }
    {
        const auto derivationPath = DerivationPath("m/714'/0'/0'/0/0");
        key.addAccount("bnb1utrnnjym7ustgw7pgyvtmnxay4qmt3ahh276nu", coinTypeBnb, TWDerivationDefault, derivationPath, "", "");
        key.addAccount("0x23b02dC8f67eD6cF8DCa47935791954286ffe7c9", coinTypeBsc, TWDerivationDefault, derivationPath, "", "");
        EXPECT_EQ(key.accounts.size(), 3ul);
    }
    {
        const auto derivationPath = DerivationPath("m/60'/0'/0'/0/0");
        key.addAccount("0xC0d97f61A84A0708225F15d54978D628Fe2C5E62", coinTypeEth, TWDerivationDefault, derivationPath, "", "");
        key.addAccount("0xC0d97f61A84A0708225F15d54978D628Fe2C5E62", coinTypeBscLegacy, TWDerivationDefault, derivationPath, "", "");
        EXPECT_EQ(key.accounts.size(), 5ul);
    }

    key.removeAccount(coinTypeBc);
    key.removeAccount(coinTypeBnb);
    key.removeAccount(coinTypeBsc);
    key.removeAccount(coinTypeEth);
    key.removeAccount(coinTypeBscLegacy);
    EXPECT_EQ(key.accounts.size(), 0ul);
}

TEST(StoredKey, AddRemoveAccountDerivation) {
    auto key = StoredKey::createWithMnemonic("name", Data(), gMnemonic, TWStoredKeyEncryptionLevelDefault);
    EXPECT_EQ(key.accounts.size(), 0ul);

    const auto derivationPath = DerivationPath("m/84'/0'/0'/0/0");
    {
        key.addAccount("bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny", coinTypeBc, TWDerivationDefault, derivationPath, "", "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
        EXPECT_EQ(key.accounts.size(), 1ul);
    }
    {
        key.addAccount("1NyRyFewhZcWMa9XCj3bBxSXPXyoSg8dKz", coinTypeBc, TWDerivationBitcoinLegacy, derivationPath, "", "xpub6CR52eaUuVb4kXAVyHC2i5ZuqJ37oWNPZFtjXaazFPXZD45DwWBYEBLdrF7fmCR9pgBuCA9Q57zZfyJjDUBDNtWkhWuGHNYKLgDHpqrHsxV");
        EXPECT_EQ(key.accounts.size(), 2ul);
    }

    key.removeAccount(coinTypeBc, TWDerivationDefault);
    EXPECT_EQ(key.accounts.size(), 1ul);
    key.removeAccount(coinTypeBc, TWDerivationDefault); // try 2nd time
    EXPECT_EQ(key.accounts.size(), 1ul);
    key.removeAccount(coinTypeBc, TWDerivationBitcoinLegacy);
    EXPECT_EQ(key.accounts.size(), 0ul);
    key.removeAccount(coinTypeBc, TWDerivationBitcoinLegacy); // try 2nd time
    EXPECT_EQ(key.accounts.size(), 0ul);
}

TEST(StoredKey, AddRemoveAccountDerivationPath) {
    auto key = StoredKey::createWithMnemonic("name", Data(), gMnemonic, TWStoredKeyEncryptionLevelDefault);
    EXPECT_EQ(key.accounts.size(), 0ul);

    const auto derivationPath0 = DerivationPath("m/84'/0'/0'/0/0");
    {
        key.addAccount("bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny", coinTypeBc, TWDerivationDefault, derivationPath0, "", "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
        EXPECT_EQ(key.accounts.size(), 1ul);
    }
    const auto derivationPath1 = DerivationPath("m/84'/0'/0'/1/0");
    {
        key.addAccount("bc1qumuzptwdr6jlsqum8jnzz80rdg8nx6x29m2qpu", coinTypeBc, TWDerivationDefault, derivationPath1, "", "zpub6rxtad3SPT1C5GUDjPiKQ5oJN5DBeMbdUR7LrdYt12VbU7TBSpGUkdLvfVYGuj1N5edkDoZ3bu1fdN1HprQYfCBdsSH5CaAAygHGsanwtTe");
        EXPECT_EQ(key.accounts.size(), 2ul);
    }

    key.removeAccount(coinTypeBc, derivationPath0);
    EXPECT_EQ(key.accounts.size(), 1ul);
    key.removeAccount(coinTypeBc, derivationPath0); // try 2nd time
    EXPECT_EQ(key.accounts.size(), 1ul);
    key.removeAccount(coinTypeBc, derivationPath1);
    EXPECT_EQ(key.accounts.size(), 0ul);
    key.removeAccount(coinTypeBc, derivationPath1); // try 2nd time
    EXPECT_EQ(key.accounts.size(), 0ul);
}

TEST(StoredKey, FixAddress) {
    {
        auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);
        key.fixAddresses(gPassword);
    }
    {
        const auto privateKey = parse_hex("3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266");
        auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("name", gPassword, coinTypeBc, privateKey);
        key.fixAddresses(gPassword);
    }
}

TEST(StoredKey, WalletInvalid) {
    const auto privateKey = parse_hex("3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266");
    auto key = StoredKey::createWithPrivateKeyAddDefaultAddress("name", gPassword, coinTypeBc, privateKey);
    try {
        auto wallet = key.wallet(gPassword);
    } catch (std::invalid_argument&) {
        // expected exception ok
        return;
    }
    FAIL() << "Missing expected exception";
}

TEST(StoredKey, LoadNonexistent) {
    ASSERT_THROW(StoredKey::load(testDataPath("nonexistent.json")), invalid_argument);
}

TEST(StoredKey, LoadLegacyPrivateKey) {
    const auto key = StoredKey::load(testDataPath("legacy-private-key.json"));
    EXPECT_EQ(key.type, StoredKeyType::privateKey);
    EXPECT_EQ(key.id, "3051ca7d-3d36-4a4a-acc2-09e9083732b0");
    EXPECT_EQ(key.accounts[0].coin, TWCoinTypeEthereum);
    EXPECT_EQ(hex(key.payload.decrypt(TW::data("testpassword"))), "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");
}

TEST(StoredKey, LoadLivepeerKey) {
    const auto key = StoredKey::load(testDataPath("livepeer.json"));
    EXPECT_EQ(key.type, StoredKeyType::privateKey);
    EXPECT_EQ(key.id, "70ea3601-ee21-4e94-a7e4-66255a987d22");
    EXPECT_EQ(key.accounts[0].coin, TWCoinTypeEthereum);
    EXPECT_EQ(hex(key.payload.decrypt(TW::data("Radchenko"))), "09b4379d9a41a71d94ee36357bccb4d77b45e7fd9307e2c0f673dd54c0558c73");
}

TEST(StoredKey, LoadPBKDF2Key) {
    const auto key = StoredKey::load(testDataPath("pbkdf2.json"));
    EXPECT_EQ(key.type, StoredKeyType::privateKey);
    EXPECT_EQ(key.id, "3198bc9c-6672-5ab3-d995-4942343ae5b6");

    const auto& payload = key.payload;
    ASSERT_TRUE(std::holds_alternative<PBKDF2Parameters>(payload.params.kdfParams));
    EXPECT_EQ(std::get<PBKDF2Parameters>(payload.params.kdfParams).desiredKeyLength, 32ul);
    EXPECT_EQ(std::get<PBKDF2Parameters>(payload.params.kdfParams).iterations, 262144ul);
    EXPECT_EQ(hex(std::get<PBKDF2Parameters>(payload.params.kdfParams).salt), "ae3cd4e7013836a3df6bd7241b12db061dbe2c6785853cce422d148a624ce0bd");

    EXPECT_EQ(hex(payload.decrypt(TW::data("testpassword"))), "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");

    auto j = std::get<PBKDF2Parameters>(payload.params.kdfParams).json();
    auto expected = R"|({"c":262144,"dklen":32,"salt":"ae3cd4e7013836a3df6bd7241b12db061dbe2c6785853cce422d148a624ce0bd"})|";
    EXPECT_EQ(j.dump(), expected);
}

TEST(StoredKey, RandomPBKDF2Param) {
    auto p = PBKDF2Parameters();
    ASSERT_TRUE(p.salt.size() == 32ul);
}

TEST(StoredKey, LoadLegacyMnemonic) {
    const auto key = StoredKey::load(testDataPath("legacy-mnemonic.json"));
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    EXPECT_EQ(key.id, "629aad29-0b22-488e-a0e7-b4219d4f311c");

    const auto data = key.payload.decrypt(gPassword);
    const auto mnemonic = string(reinterpret_cast<const char*>(data.data()));
    EXPECT_EQ(mnemonic, "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn back");

    EXPECT_EQ(key.accounts[0].coin, TWCoinTypeEthereum);
    EXPECT_EQ(key.accounts[0].derivationPath.string(), "m/44'/60'/0'/0/0");
    EXPECT_EQ(key.accounts[0].address, "");
    EXPECT_EQ(key.accounts[1].coin, coinTypeBc);
    EXPECT_EQ(key.accounts[1].derivationPath.string(), "m/84'/0'/0'/0/0");
    EXPECT_EQ(key.accounts[1].address, "");
    EXPECT_EQ(key.accounts[1].extendedPublicKey, "zpub6r97AegwVxVbJeuDAWP5KQgX5y4Q6KyFUrsFQRn8yzSXrnmpwg1ZKHSWwECR1Kiqgr4h93WN5kdS48KC6hVFniuZHqVFXjULZZkCwurqyPn");
}

TEST(StoredKey, LoadFromWeb3j) {
    const auto key = StoredKey::load(testDataPath("web3j.json"));
    EXPECT_EQ(key.type, StoredKeyType::privateKey);
    EXPECT_EQ(key.id, "86066d8c-8dba-4d81-afd4-934e2a2b72a2");
    const auto password = parse_hex("2d6eefbfbd4622efbfbdefbfbd516718efbfbdefbfbdefbfbdefbfbd59efbfbd30efbfbdefbfbd3a4348efbfbd2aefbfbdefbfbd49efbfbd27efbfbd0638efbfbdefbfbdefbfbd4cefbfbd6befbfbdefbfbd6defbfbdefbfbd63efbfbd5aefbfbd61262b70efbfbdefbfbdefbfbdefbfbdefbfbdc7aa373163417cefbfbdefbfbdefbfbd44efbfbdefbfbd1d10efbfbdefbfbdefbfbd61dc9e5b124befbfbd11efbfbdefbfbd2fefbfbdefbfbd3d7c574868efbfbdefbfbdefbfbd37043b7b5c1a436471592f02efbfbd18efbfbdefbfbd2befbfbdefbfbd7218efbfbd6a68efbfbdcb8e5f3328773ec48174efbfbd67efbfbdefbfbdefbfbdefbfbdefbfbd2a31efbfbd7f60efbfbdd884efbfbd57efbfbd25efbfbd590459efbfbd37efbfbd2bdca20fefbfbdefbfbdefbfbdefbfbd39450113efbfbdefbfbdefbfbd454671efbfbdefbfbdd49fefbfbd47efbfbdefbfbdefbfbdefbfbd00efbfbdefbfbdefbfbdefbfbd05203f4c17712defbfbd7bd1bbdc967902efbfbdc98a77efbfbd707a36efbfbd12efbfbdefbfbd57c78cefbfbdefbfbdefbfbd10efbfbdefbfbdefbfbde1a1bb08efbfbdefbfbd26efbfbdefbfbd58efbfbdefbfbdc4b1efbfbd295fefbfbd0eefbfbdefbfbdefbfbd0e6eefbfbd");
    const auto data = key.payload.decrypt(password);
    EXPECT_EQ(hex(data), "043c5429c7872502531708ec0d821c711691402caf37ef7ba78a8c506f10653b");
}

TEST(StoredKey, ReadWallet) {
    const auto key = StoredKey::load(testDataPath("key.json"));

    EXPECT_EQ(key.type, StoredKeyType::privateKey);
    EXPECT_EQ(key.id, "e13b209c-3b2f-4327-bab0-3bef2e51630d");
    EXPECT_EQ(key.name, "Test Account");

    const auto header = key.payload;

    EXPECT_EQ(header.params.cipher(), "aes-128-ctr");
    EXPECT_EQ(hex(header.encrypted), "d172bf743a674da9cdad04534d56926ef8358534d458fffccd4e6ad2fbde479c");
    EXPECT_EQ(hex(header._mac), "2103ac29920d71da29f15d75b4a16dbe95cfd7ff8faea1056c33131d846e3097");
    EXPECT_EQ(hex(header.params.cipherParams.iv), "83dbcc02d8ccb40e466191a123791e0e");

    ASSERT_TRUE(std::holds_alternative<ScryptParameters>(header.params.kdfParams));
    EXPECT_EQ(std::get<ScryptParameters>(header.params.kdfParams).desiredKeyLength, 32ul);
    EXPECT_EQ(std::get<ScryptParameters>(header.params.kdfParams).n, 262144ul);
    EXPECT_EQ(std::get<ScryptParameters>(header.params.kdfParams).p, 8ul);
    EXPECT_EQ(std::get<ScryptParameters>(header.params.kdfParams).r, 1ul);
    EXPECT_EQ(hex(std::get<ScryptParameters>(header.params.kdfParams).salt), "ab0c7876052600dd703518d6fc3fe8984592145b591fc8fb5c6d43190334ba19");
}

TEST(StoredKey, ReadMyEtherWallet) {
    ASSERT_NO_THROW(StoredKey::load(testDataPath("myetherwallet.uu")));
}

TEST(StoredKey, InvalidPassword) {
    const auto key = StoredKey::load(testDataPath("key.json"));

    ASSERT_THROW(key.payload.decrypt(gPassword), DecryptionError);
}

TEST(StoredKey, EmptyAccounts) {
    const auto key = StoredKey::load(testDataPath("empty-accounts.json"));

    ASSERT_NO_THROW(key.payload.decrypt(TW::data("testpassword")));
}

TEST(StoredKey, Decrypt) {
    const auto key = StoredKey::load(testDataPath("key.json"));
    const auto privateKey = key.payload.decrypt(TW::data("testpassword"));

    EXPECT_EQ(hex(privateKey), "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d");
}

TEST(StoredKey, CreateWallet) {
    const auto privateKey = parse_hex("3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266");
    const auto key = StoredKey::createWithPrivateKey("name", gPassword, privateKey);
    const auto decrypted = key.payload.decrypt(gPassword);

    EXPECT_EQ(hex(decrypted), hex(privateKey));
}

TEST(StoredKey, CreateAccounts) {
    string mnemonicPhrase = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
    auto key = StoredKey::createWithMnemonic("name", gPassword, mnemonicPhrase, TWStoredKeyEncryptionLevelDefault);
    const auto wallet = key.wallet(gPassword);
    
    EXPECT_EQ(key.account(TWCoinTypeEthereum, &wallet)->address, "0x494f60cb6Ac2c8F5E1393aD9FdBdF4Ad589507F7");
    EXPECT_EQ(key.account(TWCoinTypeEthereum, &wallet)->publicKey, "04cc32a479080d83fdcf69966713f0aad1bc1dc3ecf873b034894e84259841bc1c9b122717803e68905220ff54952d3f5ea2ab2698ca31f843addf94ae73fae9fd");
    EXPECT_EQ(key.account(TWCoinTypeEthereum, &wallet)->extendedPublicKey, "");

    EXPECT_EQ(key.account(coinTypeBc, &wallet)->address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    EXPECT_EQ(key.account(coinTypeBc, &wallet)->publicKey, "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006");
    EXPECT_EQ(key.account(coinTypeBc, &wallet)->extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
}

TEST(StoredKey, CreateAccountsAes256) {
    string mnemonicPhrase = "team engine square letter hero song dizzy scrub tornado fabric divert saddle";
    auto key = StoredKey::createWithMnemonic("name", gPassword, mnemonicPhrase, TWStoredKeyEncryptionLevelDefault, TWStoredKeyEncryptionAes256Ctr);
    auto header = key.payload;
    const auto wallet = key.wallet(gPassword);

    EXPECT_EQ(header.params.cipher(), "aes-256-ctr");
    EXPECT_EQ(key.account(TWCoinTypeEthereum, &wallet)->address, "0x494f60cb6Ac2c8F5E1393aD9FdBdF4Ad589507F7");
    EXPECT_EQ(key.account(TWCoinTypeEthereum, &wallet)->publicKey, "04cc32a479080d83fdcf69966713f0aad1bc1dc3ecf873b034894e84259841bc1c9b122717803e68905220ff54952d3f5ea2ab2698ca31f843addf94ae73fae9fd");
    EXPECT_EQ(key.account(TWCoinTypeEthereum, &wallet)->extendedPublicKey, "");

    EXPECT_EQ(key.account(coinTypeBc, &wallet)->address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny");
    EXPECT_EQ(key.account(coinTypeBc, &wallet)->publicKey, "02df6fc590ab3101bbe0bb5765cbaeab9b5dcfe09ac9315d707047cbd13bc7e006");
    EXPECT_EQ(key.account(coinTypeBc, &wallet)->extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
}

TEST(StoredKey, DecodingEthereumAddress) {
    const auto key = StoredKey::load(testDataPath("key.json"));

    EXPECT_EQ(key.accounts[0].address, "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b");
}

TEST(StoredKey, DecodingBitcoinAddress) {
    const auto key = StoredKey::load(testDataPath("key_bitcoin.json"));

    EXPECT_EQ(key.accounts[0].address, "3PWazDi9n1Hfyq9gXFxDxzADNL8RNYyK2y");
}

TEST(StoredKey, RemoveAccount) {
    auto key = StoredKey::load(testDataPath("legacy-mnemonic.json"));
    EXPECT_EQ(key.accounts.size(), 2ul);
    key.removeAccount(TWCoinTypeEthereum);
    EXPECT_EQ(key.accounts.size(), 1ul);
    EXPECT_EQ(key.accounts[0].coin, coinTypeBc);
}

TEST(StoredKey, MissingAddressFix) {
    auto key = StoredKey::load(testDataPath("missing-address.json"));
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);

    const auto wallet = key.wallet(gPassword);
    EXPECT_EQ(wallet.getMnemonic(), "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
    EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));

    EXPECT_EQ(key.account(TWCoinTypeBitcoin)->address, "");
    EXPECT_EQ(key.account(TWCoinTypeEthereum)->address, "");

    key.fixAddresses(gPassword);

    EXPECT_EQ(key.account(TWCoinTypeEthereum, nullptr)->address, "0xA3Dcd899C0f3832DFDFed9479a9d828c6A4EB2A7");
    EXPECT_EQ(key.account(TWCoinTypeEthereum, nullptr)->publicKey, "0448a9ffac8022f1c7eb5253746e24d11d9b6b2737c0aecd48335feabb95a179916b1f3a97bed6740a85a2d11c663d38566acfb08af48a47ce0c835c65c9b23d0d");
    EXPECT_EQ(key.account(coinTypeBc, nullptr)->address, "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd");
    EXPECT_EQ(key.account(coinTypeBc, nullptr)->publicKey, "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac");
}

TEST(StoredKey, MissingAddressReadd) {
    auto key = StoredKey::load(testDataPath("missing-address.json"));
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);

    const auto wallet = key.wallet(gPassword);
    EXPECT_EQ(wallet.getMnemonic(), "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal");
    EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));

    EXPECT_EQ(key.account(TWCoinTypeBitcoin)->address, "");
    EXPECT_EQ(key.account(TWCoinTypeEthereum)->address, "");

    // get accounts, this will also fill addresses as they are empty
    const auto btcAccount = key.account(TWCoinTypeBitcoin, &wallet);
    const auto ethAccount = key.account(TWCoinTypeEthereum, &wallet);

    EXPECT_TRUE(btcAccount.has_value());
    EXPECT_EQ(btcAccount->address, "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd");
    EXPECT_TRUE(ethAccount.has_value());
    EXPECT_EQ(ethAccount->address, "0xA3Dcd899C0f3832DFDFed9479a9d828c6A4EB2A7");
}

TEST(StoredKey, EtherWalletAddressNo0x) {
    auto key = StoredKey::load(testDataPath("ethereum-wallet-address-no-0x.json"));
    key.fixAddresses(TW::data("15748c4e3dca6ae2110535576ab0c398cb79d985707c68ee6c9f9df9d421dd53"));
    const auto account = key.account(TWCoinTypeEthereum, nullptr);
    EXPECT_EQ(account->address, "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309");
    EXPECT_EQ(account->publicKey, "0499c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c166b489a4b7c491e7688e6ebea3a71fc3a1a48d60f98d5ce84c93b65e423fde91");
}

TEST(StoredKey, CreateMinimalEncryptionParameters) {
    const auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelMinimal);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    const Data& mnemo2Data = key.payload.decrypt(gPassword);
    EXPECT_EQ(string(mnemo2Data.begin(), mnemo2Data.end()), string(gMnemonic));
    EXPECT_EQ(key.accounts.size(), 0ul);
    EXPECT_EQ(key.wallet(gPassword).getMnemonic(), string(gMnemonic));

    const auto json = key.json();

    EXPECT_EQ(json["crypto"]["kdf"], "scrypt");
    EXPECT_EQ(json["crypto"]["kdfparams"]["n"], 4096);

    // load it back
    const auto key2 = StoredKey::createWithJson(json);
    EXPECT_EQ(key2.wallet(gPassword).getMnemonic(), string(gMnemonic));
}

TEST(StoredKey, CreateWeakEncryptionParameters) {
    const auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelWeak);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    const Data& mnemo2Data = key.payload.decrypt(gPassword);
    EXPECT_EQ(string(mnemo2Data.begin(), mnemo2Data.end()), string(gMnemonic));
    EXPECT_EQ(key.accounts.size(), 0ul);
    EXPECT_EQ(key.wallet(gPassword).getMnemonic(), string(gMnemonic));

    const auto json = key.json();

    EXPECT_EQ(json["crypto"]["kdf"], "scrypt");
    EXPECT_EQ(json["crypto"]["kdfparams"]["n"], 16384);

    // load it back
    const auto key2 = StoredKey::createWithJson(json);
    EXPECT_EQ(key2.wallet(gPassword).getMnemonic(), string(gMnemonic));
}

TEST(StoredKey, CreateStandardEncryptionParameters) {
    const auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelStandard);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    const Data& mnemo2Data = key.payload.decrypt(gPassword);
    EXPECT_EQ(string(mnemo2Data.begin(), mnemo2Data.end()), string(gMnemonic));
    EXPECT_EQ(key.accounts.size(), 0ul);
    EXPECT_EQ(key.wallet(gPassword).getMnemonic(), string(gMnemonic));

    const auto json = key.json();

    EXPECT_EQ(json["crypto"]["kdf"], "scrypt");
    EXPECT_EQ(json["crypto"]["kdfparams"]["n"], 262144);

    // load it back
    const auto key2 = StoredKey::createWithJson(json);
    EXPECT_EQ(key2.wallet(gPassword).getMnemonic(), string(gMnemonic));
}

TEST(StoredKey, CreateMultiAccounts) { // Multiple accounts for the same coin
    auto key = StoredKey::createWithMnemonic("name", gPassword, gMnemonic, TWStoredKeyEncryptionLevelDefault);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);
    const Data& mnemo2Data = key.payload.decrypt(gPassword);
    EXPECT_EQ(string(mnemo2Data.begin(), mnemo2Data.end()), string(gMnemonic));
    EXPECT_EQ(key.wallet(gPassword).getMnemonic(), string(gMnemonic));
    EXPECT_EQ(key.accounts.size(), 0ul);

    const auto expectedBtc1 = "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny";
    const auto expectedBtc2 = "1NyRyFewhZcWMa9XCj3bBxSXPXyoSg8dKz";
    const auto expectedSol1 = "HiipoCKL8hX2RVmJTz3vaLy34hS2zLhWWMkUWtw85TmZ";
    const auto wallet = key.wallet(gPassword);
    auto expectedAccounts = 0ul;

    { // Create default Bitcoin account
        const auto coin = TWCoinTypeBitcoin;

        const auto btc1 = key.account(coin, &wallet);

        EXPECT_TRUE(btc1.has_value());
        EXPECT_EQ(btc1->address, expectedBtc1);
        EXPECT_EQ(btc1->derivationPath.string(), "m/84'/0'/0'/0/0");
        EXPECT_EQ(btc1->extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");
        EXPECT_EQ(key.accounts.size(), ++expectedAccounts);
        EXPECT_EQ(key.accounts[expectedAccounts - 1].address, expectedBtc1);
        EXPECT_EQ(key.account(coin)->address, expectedBtc1);
        EXPECT_EQ(key.getAccounts(coin).size(), 1ul);
        EXPECT_EQ(key.getAccounts(coin)[0].address, expectedBtc1);
    }
    { // Create default Solana account
        const auto coin = TWCoinTypeSolana;

        const auto sol1 = key.account(coin, &wallet);

        EXPECT_TRUE(sol1.has_value());
        EXPECT_EQ(sol1->address, expectedSol1);
        EXPECT_EQ(sol1->derivationPath.string(), "m/44'/501'/0'");
        EXPECT_EQ(key.accounts.size(), ++expectedAccounts);
        EXPECT_EQ(key.accounts[expectedAccounts - 1].address, expectedSol1);
        EXPECT_EQ(key.account(coin)->address, expectedSol1);
        EXPECT_EQ(key.getAccounts(coin).size(), 1ul);
        EXPECT_EQ(key.getAccounts(coin)[0].address, expectedSol1);
    }
    { // Create alternative P2PK Bitcoin account (different address format)
        const auto coin = TWCoinTypeBitcoin;

        const auto btc2 = key.account(coin, TWDerivationBitcoinLegacy, wallet);

        EXPECT_EQ(btc2.address, expectedBtc2);
        EXPECT_EQ(btc2.derivationPath.string(), "m/44'/0'/0'/0/0");
        EXPECT_EQ(btc2.extendedPublicKey, "xpub6CR52eaUuVb4kXAVyHC2i5ZuqJ37oWNPZFtjXaazFPXZD45DwWBYEBLdrF7fmCR9pgBuCA9Q57zZfyJjDUBDNtWkhWuGHNYKLgDHpqrHsxV");
        EXPECT_EQ(key.accounts.size(), ++expectedAccounts);
        EXPECT_EQ(key.accounts[expectedAccounts - 1].address, expectedBtc2);
        EXPECT_EQ(key.account(coin)->address, expectedBtc1);
        EXPECT_EQ(key.account(coin, TWDerivationBitcoinLegacy, wallet).address, expectedBtc2);
        EXPECT_EQ(key.getAccounts(coin).size(), 2ul);
        EXPECT_EQ(key.getAccounts(coin)[0].address, expectedBtc1);
        EXPECT_EQ(key.getAccounts(coin)[1].address, expectedBtc2);
    }
    { // Create alternative Solana account with non-default derivation path (different derivation path and address)
        const auto coin = TWCoinTypeSolana;

        const auto sol2 = key.account(coin, TWDerivationSolanaSolana, wallet);

        const auto expectedSol2 = "CgWJeEWkiYqosy1ba7a3wn9HAQuHyK48xs3LM4SSDc1C";
        EXPECT_EQ(sol2.address, expectedSol2);
        EXPECT_EQ(sol2.derivationPath.string(), "m/44'/501'/0'/0'");
        EXPECT_EQ(key.accounts.size(), ++expectedAccounts);
        EXPECT_EQ(key.accounts[expectedAccounts - 1].address, expectedSol2);
        // Now we have 2 Solana addresses, 1st is returned here
        EXPECT_EQ(key.account(coin)->address, expectedSol1);
        EXPECT_EQ(key.account(coin, TWDerivationSolanaSolana, wallet).address, expectedSol2);
        EXPECT_EQ(key.getAccounts(coin).size(), 2ul);
        EXPECT_EQ(key.getAccounts(coin)[0].address, expectedSol1);
        EXPECT_EQ(key.getAccounts(coin)[1].address, expectedSol2);
    }
    { // Create CUSTOM account with alternative Bitcoin address. Note: this is not recommended.
        const auto coin = TWCoinTypeBitcoin;
        const auto customPath = DerivationPath("m/44'/2'/0'/0/0");
        const auto btcPrivateKey = wallet.getKey(coin, customPath);
        EXPECT_NE(TW::deriveAddress(coin, btcPrivateKey), expectedBtc1);
        const auto btcPublicKey = btcPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        const auto p2pkhBtcAddress = Bitcoin::Address(btcPublicKey, TWCoinTypeP2pkhPrefix(coin)).string();
        const auto expectedBtc3 = "1C43YUWSYTgaoBEsRffAkzF6HruJegEqP5";
        EXPECT_EQ(p2pkhBtcAddress, expectedBtc3);
        const auto extendedPublicKey = wallet.getExtendedPublicKey(TW::purpose(coin), coin, TWHDVersionZPUB);
        EXPECT_EQ(extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn");

        key.addAccount(p2pkhBtcAddress, coin, TWDerivationCustom, customPath, hex(btcPublicKey.bytes), extendedPublicKey);

        EXPECT_EQ(key.accounts.size(), ++expectedAccounts);
        EXPECT_EQ(key.accounts[expectedAccounts - 1].address, expectedBtc3);
        // Now we have 2 Bitcoin addresses, 1st is returned here
        EXPECT_EQ(key.account(coin)->address, expectedBtc1);
        EXPECT_EQ(key.getAccounts(coin).size(), 3ul);
        EXPECT_EQ(key.getAccounts(coin)[0].address, expectedBtc1);
        EXPECT_EQ(key.getAccounts(coin)[1].address, expectedBtc2);
        EXPECT_EQ(key.getAccounts(coin)[2].address, expectedBtc3);
        EXPECT_EQ(key.getAccounts(coin)[2].derivationPath.string(), "m/44'/2'/0'/0/0");
    }
}

TEST(StoredKey, CreateWithMnemonicAlternativeDerivation) {
    const auto coin = TWCoinTypeSolana;
    auto key = StoredKey::createWithMnemonicAddDefaultAddress("name", gPassword, gMnemonic, coin);
    EXPECT_EQ(key.type, StoredKeyType::mnemonicPhrase);

    ASSERT_EQ(key.accounts.size(), 1ul);
    EXPECT_EQ(key.accounts[0].coin, coin);
    EXPECT_EQ(key.accounts[0].address, "HiipoCKL8hX2RVmJTz3vaLy34hS2zLhWWMkUWtw85TmZ");
    EXPECT_EQ(key.accounts[0].publicKey, "f86b18399096c8134dd185f1e72dd7e26528772a2a998abfd81c5f8c547223d0");
    EXPECT_EQ(hex(key.privateKey(coin, gPassword).bytes), "d81b5c525979e487736b69cb84ed8331559de17294f38491b304555c26687e83");
    EXPECT_EQ(hex(key.privateKey(coin, TWDerivationDefault, gPassword).bytes), "d81b5c525979e487736b69cb84ed8331559de17294f38491b304555c26687e83");
    ASSERT_EQ(key.accounts.size(), 1ul);

    // alternative derivation, different keys
    EXPECT_EQ(hex(key.privateKey(coin, TWDerivationSolanaSolana, gPassword).bytes), "d49a5fa7f77593534c7afd2ba8dc8e9d8b007bc6ec65fe8df25ffe6fafc57151");

    ASSERT_EQ(key.accounts.size(), 2ul);
    EXPECT_EQ(key.accounts[1].coin, coin);
    EXPECT_EQ(key.accounts[1].address, "CgWJeEWkiYqosy1ba7a3wn9HAQuHyK48xs3LM4SSDc1C");
    EXPECT_EQ(key.accounts[1].publicKey, "ad8f57924dce62f9040f93b4f6ce3c3d39afde7e29bcb4013dad59db7913c4c7");
    EXPECT_EQ(hex(key.privateKey(coin, TWDerivationSolanaSolana, gPassword).bytes), "d49a5fa7f77593534c7afd2ba8dc8e9d8b007bc6ec65fe8df25ffe6fafc57151");
}

} // namespace TW::Keystore
