// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "StoredKey.h"

#include "Coin.h"
#include "HexCoding.h"
#include "Mnemonic.h"
#include "PrivateKey.h"

#include <nlohmann/json.hpp>
#include <TrezorCrypto/memzero.h>

#include <cassert>
#include <fstream>
#include <stdexcept>

using namespace TW;

namespace TW::Keystore {

StoredKey StoredKey::createWithMnemonic(const std::string& name, const Data& password, const std::string& mnemonic, TWStoredKeyEncryptionLevel encryptionLevel, TWStoredKeyEncryption encryption) {
    if (!Mnemonic::isValid(mnemonic)) {
        throw std::invalid_argument("Invalid mnemonic");
    }

    Data mnemonicData = TW::Data(mnemonic.begin(), mnemonic.end());
    StoredKey key(StoredKeyType::mnemonicPhrase, name, password, mnemonicData, encryptionLevel, encryption);
    memzero(mnemonicData.data(), mnemonic.size());
    return key;
}

StoredKey StoredKey::createWithMnemonicRandom(const std::string& name, const Data& password, TWStoredKeyEncryptionLevel encryptionLevel, TWStoredKeyEncryption encryption) {
    const auto wallet = TW::HDWallet<>(128, "");
    const auto& mnemonic = wallet.getMnemonic();
    assert(Mnemonic::isValid(mnemonic));
    Data mnemonicData = TW::Data(mnemonic.begin(), mnemonic.end());
    StoredKey key(StoredKeyType::mnemonicPhrase, name, password, mnemonicData, encryptionLevel, encryption);
    memzero(mnemonicData.data(), mnemonic.size());
    return key;
}

StoredKey StoredKey::createWithMnemonicAddDefaultAddress(const std::string& name, const Data& password, const std::string& mnemonic, TWCoinType coin, TWStoredKeyEncryption encryption) {
    StoredKey key = createWithMnemonic(name, password, mnemonic, TWStoredKeyEncryptionLevelDefault, encryption);
    const auto wallet = key.wallet(password);
    key.account(coin, &wallet);
    return key;
}

StoredKey StoredKey::createWithPrivateKey(const std::string& name, const Data& password, const Data& privateKeyData, TWStoredKeyEncryption encryption) {
    return StoredKey(StoredKeyType::privateKey, name, password, privateKeyData, TWStoredKeyEncryptionLevelDefault, encryption);
}

StoredKey StoredKey::createWithPrivateKeyAddDefaultAddress(
    const std::string& name,
    const Data& password,
    TWCoinType coin,
    const Data& privateKeyData,
    TWStoredKeyEncryption encryption,
    TWDerivation derivation
) {
    const auto curve = TW::curve(coin);
    if (!PrivateKey::isValid(privateKeyData, curve)) {
        throw std::invalid_argument("Invalid private key data");
    }

    StoredKey key = createWithPrivateKey(name, password, privateKeyData, encryption);
    const auto derivationPath = TW::derivationPath(coin, derivation);
    const auto pubKeyType = TW::publicKeyType(coin);
    const auto pubKey = PrivateKey(privateKeyData, TWCoinTypeCurve(coin)).getPublicKey(pubKeyType);
    const auto address = TW::deriveAddress(coin, PrivateKey(privateKeyData), derivation);
    key.accounts.emplace_back(address, coin, derivation, derivationPath, hex(pubKey.bytes), "");
    return key;
}

StoredKey StoredKey::createWithEncodedPrivateKeyAddDefaultAddress(
    const std::string& name,
    const Data& password,
    TWCoinType coin,
    const std::string& encodedPrivateKey,
    TWStoredKeyEncryption encryption,
    TWDerivation derivation
) {
    const auto curve = TW::curve(coin);
    const auto privateKey = TW::decodePrivateKey(coin, encodedPrivateKey);
    StoredKey key = StoredKey(StoredKeyType::privateKey, name, password, privateKey.bytes, TWStoredKeyEncryptionLevelDefault, encryption, encodedPrivateKey);
    const auto derivationPath = TW::derivationPath(coin, derivation);
    const auto pubKeyType = TW::publicKeyType(coin);
    const auto pubKey = privateKey.getPublicKey(pubKeyType);
    const auto address = TW::deriveAddress(coin, privateKey, derivation);
    key.accounts.emplace_back(address, coin, derivation, derivationPath, hex(pubKey.bytes), "");
    return key;
}

StoredKey::StoredKey(StoredKeyType type, std::string name, const Data& password, const Data& data, TWStoredKeyEncryptionLevel encryptionLevel, TWStoredKeyEncryption encryption, const std::optional<std::string>& encodedStr)
    : type(type), id(), name(std::move(name)), accounts() {
    const auto encryptionParams = EncryptionParameters::getPreset(encryptionLevel, encryption);
    payload = EncryptedPayload(password, data, encryptionParams);
    if (encodedStr) {
        const auto bytes = reinterpret_cast<const uint8_t*>(encodedStr->c_str());
        const auto encodedData = Data(bytes, bytes + encodedStr->size());
        encodedPayload = EncryptedPayload(password, encodedData, encryptionParams);
    }
    const char* uuid_ptr = Rust::tw_uuid_random();
    id = std::make_optional<std::string>(uuid_ptr);
    Rust::free_string(uuid_ptr);
}

const HDWallet<> StoredKey::wallet(const Data& password) const {
    if (type != StoredKeyType::mnemonicPhrase) {
        throw std::invalid_argument("Invalid account requested.");
    }
    const auto data = payload.decrypt(password);
    const auto mnemonic = std::string(reinterpret_cast<const char*>(data.data()), data.size());
    return HDWallet<>(mnemonic, "");
}

std::vector<Account> StoredKey::getAccounts(TWCoinType coin) const {
    std::vector<Account> result;
    for (auto& account : accounts) {
        if (account.coin == coin) {
            result.push_back(account);
        }
    }
    return result;
}

std::optional<Account> StoredKey::getDefaultAccount(TWCoinType coin, const HDWallet<>* wallet) const {
    // there are multiple, try to look for default
    if (wallet != nullptr) {
        const auto address = wallet->deriveAddress(coin);
        const auto defaultAccount = getAccount(coin, address);
        if (defaultAccount.has_value()) {
            return defaultAccount;
        }
    }
    // no wallet or not found, rely on derivation=0 condition
    const auto coinAccounts = getAccounts(coin);
    for (auto& account : coinAccounts) {
        if (account.derivation == TWDerivationDefault) {
            return account;
        }
    }
    return std::nullopt;
}

std::optional<Account> StoredKey::getDefaultAccountOrAny(TWCoinType coin, const HDWallet<>* wallet) const {
    const auto defaultAccount = getDefaultAccount(coin, wallet);
    if (defaultAccount.has_value()) {
        return defaultAccount;
    }
    // return any
    const auto coinAccounts = getAccounts(coin);
    if (coinAccounts.size() > 0) {
        return coinAccounts[0];
    }
    return std::nullopt;
}

std::optional<Account> StoredKey::getAccount(TWCoinType coin, const std::string& address) const {
    for (auto& account : accounts) {
        if (account.coin == coin && account.address == address) {
            return account;
        }
    }
    return std::nullopt;
}

std::optional<Account> StoredKey::getAccount(TWCoinType coin, TWDerivation derivation, const HDWallet<>& wallet) const {
    // obtain address
    const auto address = wallet.deriveAddress(coin, derivation);
    return getAccount(coin, address);
}

Account StoredKey::fillAddressIfMissing(Account& account, const HDWallet<>* wallet) const {
    if (account.address.empty() && wallet != nullptr) {
        account.address = wallet->deriveAddress(account.coin, account.derivation);
    }
    if (account.publicKey.empty() && wallet != nullptr) {
        const auto pubKeyType = TW::publicKeyType(account.coin);
        const auto pubKey = wallet->getKey(account.coin, account.derivationPath).getPublicKey(pubKeyType);
        account.publicKey = hex(pubKey.bytes);
    }
    return account;
}

void StoredKey::updateAddressForAccount(const PrivateKey& privKey, Account& account) {
    const auto pubKey = privKey.getPublicKey(TW::publicKeyType(account.coin));
    account.address = TW::deriveAddress(account.coin, pubKey, account.derivation);
    account.publicKey = hex(pubKey.bytes);
}

std::optional<const Account> StoredKey::account(TWCoinType coin, const HDWallet<>* wallet) {
    const auto account = getDefaultAccountOrAny(coin, wallet);
    if (account.has_value()) {
        Account accountLval = account.value();
        return fillAddressIfMissing(accountLval, wallet);
    }
    // not found, add
    if (wallet == nullptr) {
        return std::nullopt;
    }
    assert(wallet != nullptr);
    const auto derivationPath = TW::derivationPath(coin);
    const auto address = wallet->deriveAddress(coin);
    const auto version = TW::xpubVersion(coin);
    const auto extendedPublicKey = wallet->getExtendedPublicKey(derivationPath.purpose(), coin, version);
    const auto pubKeyType = TW::publicKeyType(coin);
    const auto pubKey = wallet->getKey(coin, derivationPath).getPublicKey(pubKeyType);

    addAccount(address, coin, TWDerivationDefault, derivationPath, hex(pubKey.bytes), extendedPublicKey);
    return accounts.back();
}

Account StoredKey::account(TWCoinType coin, TWDerivation derivation, const HDWallet<>& wallet) {
    const auto coinAccount = getAccount(coin, derivation, wallet);
    if (coinAccount.has_value()) {
        Account accountLval = coinAccount.value();
        return fillAddressIfMissing(accountLval, &wallet);
    }
    // not found, add
    const auto derivationPath = TW::derivationPath(coin, derivation);
    const auto address = wallet.deriveAddress(coin, derivation);
    const auto version = TW::xpubVersionDerivation(coin, derivation);
    const auto extendedPublicKey = wallet.getExtendedPublicKey(derivationPath.purpose(), coin, version);
    const auto pubKeyType = TW::publicKeyType(coin);
    const auto pubKey = wallet.getKey(coin, derivationPath).getPublicKey(pubKeyType);

    addAccount(address, coin, derivation, derivationPath, hex(pubKey.bytes), extendedPublicKey);
    return accounts.back();
}

std::optional<const Account> StoredKey::account(TWCoinType coin) const {
    return getDefaultAccountOrAny(coin, nullptr);
}

std::optional<const Account> StoredKey::account(TWCoinType coin, TWDerivation derivation, const HDWallet<>& wallet) const {
    const auto account = getAccount(coin, derivation, wallet);
    if (account.has_value()) {
        Account accountLval = account.value();
        return fillAddressIfMissing(accountLval, &wallet);
    }
    return std::nullopt;
}

void StoredKey::addAccount(
    const std::string& address,
    TWCoinType coin,
    TWDerivation derivation,
    const DerivationPath& derivationPath,
    const std::string& publicKey,
    const std::string& extendedPublicKey) {
    if (getAccount(coin, address).has_value()) {
        // address already present
        return;
    }
    accounts.emplace_back(address, coin, derivation, derivationPath, publicKey, extendedPublicKey);
}

void StoredKey::removeAccount(TWCoinType coin) {
    accounts.erase(
        std::remove_if(accounts.begin(), accounts.end(), [coin](Account& account) -> bool { return account.coin == coin; }),
        accounts.end());
}

void StoredKey::removeAccount(TWCoinType coin, TWDerivation derivation) {
    accounts.erase(
        std::remove_if(accounts.begin(), accounts.end(), [coin, derivation](Account& account) -> bool {
            return account.coin == coin && account.derivation == derivation;
        }),
        accounts.end());
}

void StoredKey::removeAccount(TWCoinType coin, DerivationPath derivationPath) {
    accounts.erase(
        std::remove_if(accounts.begin(), accounts.end(), [coin, derivationPath](Account& account) -> bool {
            return account.coin == coin && account.derivationPath == derivationPath;
        }),
        accounts.end());
}

const PrivateKey StoredKey::privateKey(TWCoinType coin, const Data& password) {
    return privateKey(coin, TWDerivationDefault, password);
}

const PrivateKey StoredKey::privateKey(TWCoinType coin, [[maybe_unused]] TWDerivation derivation, const Data& password) {
    if (type == StoredKeyType::mnemonicPhrase) {
        const auto wallet = this->wallet(password);
        const Account& account = this->account(coin, derivation, wallet);
        return wallet.getKey(coin, account.derivationPath);
    }
    // type == StoredKeyType::privateKey
    return PrivateKey(payload.decrypt(password), TWCoinTypeCurve(coin));
}

void StoredKey::fixAddresses(const Data& password) {
    switch (type) {
    case StoredKeyType::mnemonicPhrase: {
        const auto wallet = this->wallet(password);
        for (auto& account : accounts) {
            if (!account.address.empty() && !account.publicKey.empty() &&
                TW::validateAddress(account.coin, account.address)) {
                continue;
            }
            const auto& derivationPath = account.derivationPath;
            const auto key = wallet.getKey(account.coin, derivationPath);
            updateAddressForAccount(key, account);
        }
    } break;

    case StoredKeyType::privateKey: {
        auto key = PrivateKey(payload.decrypt(password));
        for (auto& account : accounts) {
            if (!account.address.empty() && !account.publicKey.empty() &&
                TW::validateAddress(account.coin, account.address)) {
                continue;
            }
            updateAddressForAccount(key, account);
        }
    } break;
    }
}

bool StoredKey::updateAddress(TWCoinType coin) {
    bool addressUpdated = false;
    const auto publicKeyType = TW::publicKeyType(coin);

    for (auto& account : accounts) {
        // Update the address for the given chain if only `publicKey` is set.
        if (account.coin == coin && !account.publicKey.empty()) {
            const auto publicKeyBytes = parse_hex(account.publicKey);
            const PublicKey publicKey(publicKeyBytes, publicKeyType);
            account.address = TW::deriveAddress(account.coin, publicKey, account.derivation);

            addressUpdated = true;
        }
    }

    return addressUpdated;
}

const std::string StoredKey::decryptPrivateKeyEncoded(const Data& password) const {
    if (encodedPayload) {
        auto data = encodedPayload->decrypt(password);
        return std::string(reinterpret_cast<const char*>(data.data()), data.size());
    } else {
        auto data = payload.decrypt(password);
        return TW::hex(data);
    }
}

// -----------------
// Encoding/Decoding
// -----------------

StoredKey StoredKey::createWithJson(const nlohmann::json& json) {
    StoredKey storedKey;
    storedKey.loadJson(json);
    return storedKey;
}

namespace CodingKeys::SK {

static const auto address = "address";
static const auto type = "type";
static const auto name = "name";
static const auto id = "id";
static const auto crypto = "crypto";
static const auto encodedCrypto = "encodedCrypto";
static const auto activeAccounts = "activeAccounts";
static const auto version = "version";
static const auto coin = "coin";

} // namespace CodingKeys::SK

namespace UppercaseCodingKeys {
static const auto crypto = "Crypto";
} // namespace UppercaseCodingKeys

namespace TypeString {
static const auto privateKey = "private-key";
static const auto mnemonic = "mnemonic";
} // namespace TypeString

void StoredKey::loadJson(const nlohmann::json& json) {
    if (json.count(CodingKeys::SK::type) != 0 &&
        json[CodingKeys::SK::type].get<std::string>() == TypeString::mnemonic) {
        type = StoredKeyType::mnemonicPhrase;
    } else {
        type = StoredKeyType::privateKey;
    }

    if (json.count(CodingKeys::SK::name) != 0) {
        name = json[CodingKeys::SK::name].get<std::string>();
    }

    if (json.count(CodingKeys::SK::id) != 0) {
        id = json[CodingKeys::SK::id].get<std::string>();
    }

    if (json.count(CodingKeys::SK::crypto) != 0) {
        payload = EncryptedPayload(json[CodingKeys::SK::crypto]);
    } else if (json.count(UppercaseCodingKeys::crypto) != 0) {
        // Workaround for myEtherWallet files
        payload = EncryptedPayload(json[UppercaseCodingKeys::crypto]);
    } else {
        throw DecryptionError::invalidKeyFile;
    }

    if (json.count(CodingKeys::SK::encodedCrypto) != 0) {
        encodedPayload = EncryptedPayload(json[CodingKeys::SK::encodedCrypto]);
    } else { 
        encodedPayload = std::nullopt;
    }

    if (json.count(CodingKeys::SK::activeAccounts) != 0 &&
        json[CodingKeys::SK::activeAccounts].is_array()) {
        for (auto& accountJSON : json[CodingKeys::SK::activeAccounts]) {
            accounts.emplace_back(accountJSON);
        }
    }

    if (accounts.empty() && json.count(CodingKeys::SK::address) != 0 &&
        json[CodingKeys::SK::address].is_string()) {
        TWCoinType coin = TWCoinTypeEthereum;
        if (json.count(CodingKeys::SK::coin) != 0) {
            coin = json[CodingKeys::SK::coin].get<TWCoinType>();
        }
        auto address = json[CodingKeys::SK::address].get<std::string>();
        accounts.emplace_back(address, coin, TWDerivationDefault, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(coin), 0, 0, 0), "", "");
    }
}

nlohmann::json StoredKey::json() const {
    nlohmann::json j;
    j[CodingKeys::SK::version] = 3;

    switch (type) {
    case StoredKeyType::privateKey:
        j[CodingKeys::SK::type] = TypeString::privateKey;
        break;
    case StoredKeyType::mnemonicPhrase:
        j[CodingKeys::SK::type] = TypeString::mnemonic;
        break;
    }

    if (id) {
        j[CodingKeys::SK::id] = *id;
    }

    j[CodingKeys::SK::name] = name;
    j[CodingKeys::SK::crypto] = payload.json();
    if (encodedPayload) {
        j[CodingKeys::SK::encodedCrypto] = encodedPayload->json();
    }

    nlohmann::json accountsJSON = nlohmann::json::array();
    for (const auto& account : accounts) {
        accountsJSON.push_back(account.json());
    }
    j[CodingKeys::SK::activeAccounts] = accountsJSON;

    return j;
}

// File operations

void StoredKey::store(const std::string& path) {
    auto stream = std::ofstream(path);
    stream << json();
}

StoredKey StoredKey::load(const std::string& path) {
    std::ifstream stream(path);
    if (!stream.is_open()) {
        throw std::invalid_argument("Can't open file");
    }
    nlohmann::json j;
    stream >> j;

    return createWithJson(j);
}

} // namespace TW::Keystore
