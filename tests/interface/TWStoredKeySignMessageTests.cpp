// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// Sealed EVM message signing (Security Layer, Phase 2): personal_sign, EIP-712, Biz and EIP-7702
// authorization through the TWStoredKey C ABI, each byte-identical to the app's current
// extract-key-then-sign path (TWStoredKeyPrivateKey + the matching public API).

#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

#include <TrustWalletCore/TWAccount.h>
#include <TrustWalletCore/TWBiz.h>
#include <TrustWalletCore/TWEip7702.h>
#include <TrustWalletCore/TWEthereumAbi.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWStoredKey.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

#include <string>

namespace TW::SealedMessageTests {

// Private key from the EIP-7702 SignAuthorization fixture (TWBarzTests), so the sealed result can
// also be checked against the viem-verified values.
static const char* gSealedMsgKeyHex = "947dd69af402e7f48da1b845dfc1df6be593d01a0d8274bd03ec56712e7164e8";
static const char* gSealedMsgTypedData = R"({"types":{"EIP712Domain":[{"name":"name","type":"string"},{"name":"version","type":"string"},{"name":"chainId","type":"uint256"},{"name":"verifyingContract","type":"address"}],"Person":[{"name":"name","type":"string"},{"name":"wallet","type":"address"}],"Mail":[{"name":"from","type":"Person"},{"name":"to","type":"Person"},{"name":"contents","type":"string"}]},"primaryType":"Mail","domain":{"name":"Ether Mail","version":"1","chainId":1,"verifyingContract":"0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"},"message":{"from":{"name":"Cow","wallet":"0xCD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"},"to":{"name":"Bob","wallet":"0xbBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB"},"contents":"Hello, Bob!"}})";

struct SealedMsgFixture {
    std::shared_ptr<TWData> password = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>("password"), 8));
    std::shared_ptr<TWStoredKey> key;
    std::shared_ptr<TWString> address;
    std::shared_ptr<TWPrivateKey> privateKey;

    explicit SealedMsgFixture(TWCoinType coin) {
        const auto keyData = DATA(gSealedMsgKeyHex);
        key = WRAP(TWStoredKey, TWStoredKeyImportPrivateKey(keyData.get(), STRING("n").get(), password.get(), coin));
        const auto account = WRAP(TWAccount, TWStoredKeyAccountForCoin(key.get(), coin, nullptr));
        address = WRAPS(TWAccountAddress(account.get()));
        privateKey = WRAP(TWPrivateKey, TWStoredKeyPrivateKey(key.get(), coin, password.get()));
    }
};

// What the app does for personal_sign / EIP-712 today: sign the digest, then add 27 to v.
static Data sealedMsgAppSignDigest(TWPrivateKey* privateKey, const Data& digest) {
    const auto digestData = WRAPD(TWDataCreateWithBytes(digest.data(), digest.size()));
    const auto signature = WRAPD(TWPrivateKeySign(privateKey, digestData.get(), TWCurveSECP256k1));
    auto result = *reinterpret_cast<const Data*>(signature.get());
    result[64] += 27;
    return result;
}

// Null results must not be wrapped: the WRAP deleters do not accept nullptr. (TWData and TWString
// are the same C type, hence two names.)
static bool sealedMsgIsNull(TWData* result) {
    if (result == nullptr) {
        return true;
    }
    TWDataDelete(result);
    return false;
}

static bool sealedMsgIsNullString(TWString* result) {
    if (result == nullptr) {
        return true;
    }
    TWStringDelete(result);
    return false;
}

// Copies and frees a possibly-null TWData result.
static Data sealedMsgData(TWData* result) {
    if (result == nullptr) {
        return {};
    }
    auto copy = *reinterpret_cast<const Data*>(result);
    TWDataDelete(result);
    return copy;
}

TEST(TWStoredKeySignMessage, PersonalMessageByteIdentical) {
    const SealedMsgFixture f(TWCoinTypeEthereum);
    for (const std::string text : {"Hello Trust Wallet", ""}) {
        const auto message = TW::data(text);
        const auto messageData = WRAPD(TWDataCreateWithBytes(message.data(), message.size()));
        const auto sealed = sealedMsgData(TWStoredKeySignEthereumMessage(f.key.get(), TWCoinTypeEthereum, f.address.get(), f.password.get(), messageData.get()));

        const std::string prefix = "\x19" "Ethereum Signed Message:\n" + std::to_string(message.size());
        Data prefixed(prefix.begin(), prefix.end());
        append(prefixed, message);
        const auto prefixedData = WRAPD(TWDataCreateWithBytes(prefixed.data(), prefixed.size()));
        const auto digest = sealedMsgData(TWHashKeccak256(prefixedData.get()));
        ASSERT_EQ(sealed.size(), 65ul);
        EXPECT_EQ(hex(sealed), hex(sealedMsgAppSignDigest(f.privateKey.get(), digest)));
    }

    // Binary (hex-encoded in the app) messages are signed as raw bytes.
    const auto binary = parse_hex("deadbeef00ff");
    const auto binaryData = WRAPD(TWDataCreateWithBytes(binary.data(), binary.size()));
    EXPECT_EQ(sealedMsgData(TWStoredKeySignEthereumMessage(f.key.get(), TWCoinTypeEthereum, f.address.get(), f.password.get(), binaryData.get())).size(), 65ul);
}

TEST(TWStoredKeySignMessage, TypedDataByteIdentical) {
    const SealedMsgFixture f(TWCoinTypeEthereum);
    const auto json = STRING(gSealedMsgTypedData);
    const auto sealed = sealedMsgData(TWStoredKeySignEthereumTypedData(f.key.get(), TWCoinTypeEthereum, f.address.get(), f.password.get(), json.get()));
    const auto digest = sealedMsgData(TWEthereumAbiEncodeTyped(json.get()));
    ASSERT_EQ(digest.size(), 32ul);
    ASSERT_EQ(sealed.size(), 65ul);
    EXPECT_EQ(hex(sealed), hex(sealedMsgAppSignDigest(f.privateKey.get(), digest)));

    const auto malformed = STRING("{not json");
    EXPECT_TRUE(sealedMsgIsNull(TWStoredKeySignEthereumTypedData(f.key.get(), TWCoinTypeEthereum, f.address.get(), f.password.get(), malformed.get())));
}

// The app's FlexGas / Biz path: TWBizGetEncodedHash (sender = account) then TWBizGetSignedHash.
TEST(TWStoredKeySignMessage, BizUserOpHashByteIdentical) {
    const SealedMsgFixture f(TWCoinTypeSmartChain);
    const auto chainIdBytes = store(uint256_t(56));
    const auto chainId = WRAPD(TWDataCreateWithBytes(chainIdBytes.data(), chainIdBytes.size()));
    const auto codeAddress = STRING("0x2e234DAe75C793f67A35089C9d99245E1C58470b");
    const auto codeName = STRING("Biz");
    const auto codeVersion = STRING("v1.0.0");
    const auto typeHash = STRING("0x4f51e7a567f083a31264743067875fc6a7ae45c32c5bd71f6a998c4625b13867");
    const auto domainSeparatorHash = STRING("0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472");
    const auto userOpHash = STRING("0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb");

    const auto sealed = sealedMsgData(TWStoredKeySignBizUserOpHash(f.key.get(), TWCoinTypeSmartChain, f.address.get(), f.password.get(), chainId.get(),
                                                                         codeAddress.get(), codeName.get(), codeVersion.get(), typeHash.get(), domainSeparatorHash.get(), userOpHash.get()));

    const auto encoded = sealedMsgData(TWBizGetEncodedHash(chainId.get(), codeAddress.get(), codeName.get(), codeVersion.get(), typeHash.get(),
                                                                 domainSeparatorHash.get(), f.address.get(), userOpHash.get()));
    ASSERT_EQ(encoded.size(), 32ul);
    const auto baseline = sealedMsgData(TWBizGetSignedHash(STRING(hexEncoded(encoded).c_str()).get(), STRING(gSealedMsgKeyHex).get()));
    ASSERT_EQ(sealed.size(), 65ul);
    EXPECT_EQ(hex(sealed), hex(baseline));
}

// The app's eip7702SignAuthorization path: TWEip7702SignAuthorization with the exported key. The
// fixture also matches the viem-verified signature from TWBarzTests.
TEST(TWStoredKeySignMessage, Eip7702AuthorizationByteIdentical) {
    const SealedMsgFixture f(TWCoinTypeEthereum);
    const auto contract = STRING("0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1");
    for (const uint64_t nonceValue : {0ull, 1ull, 300ull}) {
        const auto chainIdBytes = store(uint256_t(1));
        const auto nonceBytes = store(uint256_t(nonceValue));
        const auto chainId = WRAPD(TWDataCreateWithBytes(chainIdBytes.data(), chainIdBytes.size()));
        const auto nonce = WRAPD(TWDataCreateWithBytes(nonceBytes.data(), nonceBytes.size()));

        TWString* sealedRaw = TWStoredKeySignEip7702Authorization(f.key.get(), TWCoinTypeEthereum, f.address.get(), f.password.get(), chainId.get(), contract.get(), nonce.get());
        TWString* baselineRaw = TWEip7702SignAuthorization(chainId.get(), contract.get(), nonce.get(), STRING(gSealedMsgKeyHex).get());
        ASSERT_NE(sealedRaw, nullptr);
        ASSERT_NE(baselineRaw, nullptr);
        const auto sealed = WRAPS(sealedRaw);
        const auto baseline = WRAPS(baselineRaw);
        EXPECT_EQ(std::string(TWStringUTF8Bytes(sealed.get())), std::string(TWStringUTF8Bytes(baseline.get()))) << nonceValue;
        if (nonceValue == 1) {
            EXPECT_NE(std::string(TWStringUTF8Bytes(sealed.get())).find("0x2c39f2f64441dd38c364ee175dc6b9a87f34ca330bce968f6c8e22811e3bb710"), std::string::npos);
        }
    }

    const auto chainIdBytes = store(uint256_t(1));
    const auto chainId = WRAPD(TWDataCreateWithBytes(chainIdBytes.data(), chainIdBytes.size()));
    const auto invalidContract = STRING("0x1234");
    EXPECT_TRUE(sealedMsgIsNullString(TWStoredKeySignEip7702Authorization(f.key.get(), TWCoinTypeEthereum, f.address.get(), f.password.get(), chainId.get(), invalidContract.get(), chainId.get())));
}

// Fail closed: non-EVM coin, unknown address, wrong password.
TEST(TWStoredKeySignMessage, RejectsNonEvmUnknownAddressAndWrongPassword) {
    const SealedMsgFixture f(TWCoinTypeEthereum);
    const auto message = DATA("68656c6c6f");

    const SealedMsgFixture btc(TWCoinTypeBitcoin);
    EXPECT_TRUE(sealedMsgIsNull(TWStoredKeySignEthereumMessage(btc.key.get(), TWCoinTypeBitcoin, btc.address.get(), btc.password.get(), message.get())));

    const auto otherAddress = STRING("0x3535353535353535353535353535353535353535");
    EXPECT_TRUE(sealedMsgIsNull(TWStoredKeySignEthereumMessage(f.key.get(), TWCoinTypeEthereum, otherAddress.get(), f.password.get(), message.get())));

    const auto wrongPassword = WRAPD(TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>("wrong"), 5));
    EXPECT_TRUE(sealedMsgIsNull(TWStoredKeySignEthereumMessage(f.key.get(), TWCoinTypeEthereum, f.address.get(), wrongPassword.get(), message.get())));
}

} // namespace TW::SealedMessageTests
