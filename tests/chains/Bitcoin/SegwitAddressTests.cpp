// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32.h"
#include "Bitcoin/SegwitAddress.h"
#include "HDWallet.h"
#include "HexCoding.h"

#include <string>
#include <cstring>
#include <vector>
#include <gtest/gtest.h>

using namespace TW;
namespace TW::Bitcoin::tests {

static const std::string valid_checksum[] = {
    "A12UEL5L",
    "an83characterlonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1tt5tgs",
    "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw",
    "11qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqc8247j",
    "split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w",
};

static const std::string invalid_checksum[] = {
    " 1nwldj5",
    "\x7f""1axkwrx",
    "an124characterslonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1569pvx0123456789012345678901234567890123456789",
    "pzry9x0s0muk",
    "1pzry9x0s0muk",
    "x1b4n0q5v",
    "li1dgmt3",
    "de1lg7wt\xff",
};

struct valid_address_data {
    std::string address;
    std::string scriptPubKeyHex;
};

struct invalid_address_data {
    std::string hrp;
    int version;
    size_t program_length;
};

static const std::vector<struct valid_address_data> valid_address = {
    /// test vectors from BIP173
    {"BC1QW508D6QEJXTDG4Y5R3ZARVARY0C5XW7KV8F3T4", "0014751e76e8199196d454941c45d1b3a323f1433bd6"},
    {"tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sl5k7", "00201863143c14c5166804bd19203356da136c985678cd4d27a1b8c6329604903262"},
    //{"bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7k7grplx", "5128751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6"},
    //{"BC1SW50QA3JX3S", "6002751e"},
    //{"bc1zw508d6qejxtdg4y5r3zarvaryvg6kdaj", "5210751e76e8199196d454941c45d1b3a323"},
    {"tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy", "0020000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"},
    {"bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8", "00140cb9f5c6b62c03249367bc20a90dd2425e6926af"},
    {"bc1qm9jzmujvdqjj6y28hptk859zs3yyv78hpqqjfj", "0014d9642df24c68252d1147b85763d0a284484678f7"},
    {"bc1ptmsk7c2yut2xah4pgflpygh2s7fh0cpfkrza9cjj29awapv53mrslgd5cf", "51205ee16f6144e2d46edea1427e1222ea879377e029b0c5d2e252517aee85948ec7"}, // Taproot

    /// test vectors from BIP350
    {"bc1pw508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kt5nd6y", "5128751e76e8199196d454941c45d1b3a323f1433bd6751e76e8199196d454941c45d1b3a323f1433bd6"},
    {"BC1SW50QGDZ25J", "6002751e"},
    {"bc1zw508d6qejxtdg4y5r3zarvaryvaxxpcs", "5210751e76e8199196d454941c45d1b3a323"},
    {"tb1qqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesrxh6hy", "0020000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"},
    {"tb1pqqqqp399et2xygdj5xreqhjjvcmzhxw4aywxecjdzew6hylgvsesf3hn0c", "5120000000c4a5cad46221b2a187905e5266362b99d5e91c6ce24d165dab93e86433"},
    {"bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqzk5jj0", "512079be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798"},
};

static const std::vector<std::string> invalid_address = {
    /// test vectors from BIP73
    //"tc1qw508d6qejxtdg4y5r3zarvary0c5xw7kg3g4ty", // Invalid human-readable part
    "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t5", // Invalid checksum
    "BC13W508D6QEJXTDG4Y5R3ZARVARY0C5XW7KN40WF2", // Invalid witness version
    "bc1rw5uspcuh", // Invalid program length
    "bc10w508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kw5rljs90", // Invalid program length
    "BC1QR508D6QEJXTDG4Y5R3ZARVARYV98GJ9P", // Invalid program length for witness version 0 (per BIP141)
    "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sL5k7", // Mixed case
    "bc1zw508d6qejxtdg4y5r3zarvaryvqyzf3du", // zero padding of more than 4 bits
    "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3pjxtptv", // Non-zero padding in 8-to-5 conversion
    "bc1gmk9yu", // Empty data section

    /// test vectors from BIP350
    //"tc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vq5zuyut", // Invalid human-readable part
    "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqh2y7hd", // Invalid checksum (Bech32 instead of Bech32m)
    "tb1z0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vqglt7rf", // Invalid checksum (Bech32 instead of Bech32m)
    "BC1S0XLXVLHEMJA6C4DQV22UAPCTQUPFHLXM9H8Z3K2E72Q4K9HCZ7VQ54WELL", // Invalid checksum (Bech32 instead of Bech32m)
    "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kemeawh", // Invalid checksum (Bech32m instead of Bech32)
    "tb1q0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vq24jc47", // Invalid checksum (Bech32m instead of Bech32)
    "bc1p38j9r5y49hruaue7wxjce0updqjuyyx0kh56v8s25huc6995vvpql3jow4", // Invalid character in checksum
    "BC130XLXVLHEMJA6C4DQV22UAPCTQUPFHLXM9H8Z3K2E72Q4K9HCZ7VQ7ZWS8R", // Invalid witness version
    "bc1pw5dgrnzv", // Invalid program length (1 byte)
    "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7v8n0nx0muaewav253zgeav", // Invalid program length (41 bytes)
    "BC1QR508D6QEJXTDG4Y5R3ZARVARYV98GJ9P", // Invalid program length for witness version 0 (per BIP141)
    "tb1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vq47Zagq", // Mixed case
    "bc1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7v07qwwzcrf", // zero padding of more than 4 bits
    "tb1p0xlxvlhemja6c4dqv22uapctqupfhlxm9h8z3k2e72q4k9hcz7vpggkg4j", // Non-zero padding in 8-to-5 conversion
    "bc1gmk9yu", // Empty data section

    "an84characterslonghumanreadablepartthatcontainsthetheexcludedcharactersbioandnumber11d6pts4", // overall max length exceeded
    "bc1q9zpgru", // 1 byte data (only version byte)
    "BC1SW50QA3JX3S", // version = 16
    "bc1zw508d6qejxtdg4y5r3zarvaryvg6kdaj", // version = 2
};

static const invalid_address_data invalid_address_enc[] = {
    {"BC", 0, 20},
    {"bc", 0, 21},
    {"bc", 17, 32},
    {"bc", 1, 1},
    {"bc", 16, 41},
};

static Data segwit_scriptpubkey(int witver, const Data& witprog) {
    Data ret;
    ret.push_back(witver ? (0x50 + (witver & 0x1f)) : 0);
    ret.push_back(witprog.size());
    ret.insert(ret.end(), witprog.begin(), witprog.end());
    return ret;
}

bool case_insensitive_equal(const std::string& s1, const std::string& s2) {
    std::string s1l = s1;
    std::transform(s1l.begin(), s1l.end(), s1l.begin(), [](unsigned char c){ return std::tolower(c); });
    std::string s2l = s2;
    std::transform(s2l.begin(), s2l.end(), s2l.begin(), [](unsigned char c){ return std::tolower(c); });
    return s1l == s2l;
}

TEST(SegwitAddress, ValidChecksum) {
    for (auto i = 0ul; i < sizeof(valid_checksum) / sizeof(valid_checksum[0]); ++i) {
        auto dec = Bech32::decode(valid_checksum[i]);
        ASSERT_FALSE(std::get<0>(dec).empty());

        auto recode = Bech32::encode(std::get<0>(dec), std::get<1>(dec), Bech32::ChecksumVariant::Bech32);
        ASSERT_FALSE(recode.empty());

        ASSERT_TRUE(case_insensitive_equal(recode, valid_checksum[i]));
    }
}

TEST(SegwitAddress, InvalidChecksum) {
    for (auto i = 0ul; i < sizeof(invalid_checksum) / sizeof(invalid_checksum[0]); ++i) {
        auto dec = Bech32::decode(invalid_checksum[i]);
        EXPECT_TRUE(std::get<0>(dec).empty() && std::get<1>(dec).empty());
    }
}

TEST(SegwitAddress, ValidAddress) {
    for (auto& td: valid_address) {
        auto dec = SegwitAddress::decode(td.address);
        EXPECT_TRUE(std::get<2>(dec)) << "Valid address could not be decoded " << td.address;
        EXPECT_TRUE(std::get<0>(dec).witnessProgram.size() > 0)  << "Empty decoded address data for " << td.address;
        EXPECT_EQ(std::get<1>(dec).length(), 2ul); // hrp

        // recode
        std::string recode = std::get<0>(dec).string();
        EXPECT_FALSE(recode.empty()) << "Recode failed for " << td.address;
        EXPECT_TRUE(case_insensitive_equal(td.address, recode));

        Data spk = segwit_scriptpubkey(std::get<0>(dec).witnessVersion, std::get<0>(dec).witnessProgram);
        EXPECT_EQ(hex(spk), td.scriptPubKeyHex);
    }
}

TEST(SegwitAddress, InvalidAddress) {
    for (auto i = 0ul; i < invalid_address.size(); ++i) {
        auto dec = SegwitAddress::decode(invalid_address[i]);
        EXPECT_FALSE(std::get<2>(dec)) <<  "Invalid address reported as valid: " << invalid_address[i];
    }
}

TEST(SegwitAddress, InvalidAddressEncoding) {
    for (auto i = 0ul; i < sizeof(invalid_address_enc) / sizeof(invalid_address_enc[0]); ++i) {
        auto address = SegwitAddress(invalid_address_enc[i].hrp, invalid_address_enc[i].version, Data(invalid_address_enc[i].program_length, 0));
        std::string code = address.string();
        EXPECT_TRUE(code.empty());
    }
}

TEST(SegwitAddress, LegacyAddress) {
    auto result = SegwitAddress::decode("TLWEciM1CjP5fJqM2r9wymAidkkYtTU5k3");
    EXPECT_FALSE(std::get<2>(result));
}

TEST(SegwitAddress, fromRaw) {
    {
        auto addr = SegwitAddress::fromRaw("bc", parse_hex("000e140f070d1a001912060b0d081504140311021d030c1d03040f1814060e1e16"));
        EXPECT_TRUE(addr.second);
        EXPECT_EQ(addr.first.string(), "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4");
    }
    {
        // empty data
        auto addr = SegwitAddress::fromRaw("bc", Data());
        EXPECT_FALSE(addr.second);
    }
}

TEST(SegwitAddress, Equals) {
    const auto dec1 = SegwitAddress::decode("bc1qpjult34k9spjfym8hss2jrwjgf0xjf40ze0pp8");
    EXPECT_TRUE(std::get<2>(dec1));
    const auto addr1 = std::get<0>(dec1);
    const auto dec2 = SegwitAddress::decode("bc1qm9jzmujvdqjj6y28hptk859zs3yyv78hpqqjfj");
    EXPECT_TRUE(std::get<2>(dec2));
    const auto addr2 = std::get<0>(dec2);

    ASSERT_TRUE(addr1 == addr1);
    ASSERT_FALSE(addr1 == addr2);
}

TEST(SegwitAddress, TestnetAddress) {
    const auto mnemonic1 = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    const auto passphrase = "";
    const auto coin = TWCoinTypeBitcoin;
    HDWallet wallet = HDWallet(mnemonic1, passphrase);

    // default
    {
        const auto privKey = wallet.getKey(coin, TWDerivationDefault);
        const auto pubKey = privKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(pubKey.bytes), "02df9ef2a7a5552765178b181e1e1afdefc7849985c7dfe9647706dd4fa40df6ac");
        EXPECT_EQ(SegwitAddress(pubKey, "bc").string(), "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd");
    }

    // testnet: different derivation path and hrp
    {
        const auto privKey = wallet.getKey(coin, TW::DerivationPath("m/84'/1'/0'/0/0"));
        const auto pubKey = privKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        EXPECT_EQ(hex(pubKey.bytes), "03eb1a535b59f03894b99319f850c784cf4164f4de07620695c5cf0dc5c1ab2a54");
        EXPECT_EQ(SegwitAddress::createTestnetFromPublicKey(pubKey).string(), "tb1qq8p994ak933c39d2jaj8n4sg598tnkhnyk5sg5");
        // alternative with custom hrp
        EXPECT_EQ(SegwitAddress(pubKey, "tb").string(), "tb1qq8p994ak933c39d2jaj8n4sg598tnkhnyk5sg5");
    }

    EXPECT_TRUE(SegwitAddress::isValid("tb1qq8p994ak933c39d2jaj8n4sg598tnkhnyk5sg5"));
}

TEST(SegwitAddress, SegwitDerivationHDWallet) {
    const auto mnemonic1 = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    const auto passphrase = "";
    const auto coin = TWCoinTypeBitcoin;
    HDWallet wallet = HDWallet(mnemonic1, passphrase);

    // addresses with different derivations
    EXPECT_EQ(wallet.deriveAddress(coin), "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd");
    EXPECT_EQ(wallet.deriveAddress(coin, TWDerivationDefault), "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd");
    EXPECT_EQ(wallet.deriveAddress(coin, TWDerivationBitcoinSegwit), "bc1qpsp72plnsqe6e2dvtsetxtww2cz36ztmfxghpd");
    EXPECT_EQ(wallet.deriveAddress(coin, TWDerivationBitcoinLegacy), "1GVb4mfQrvymPLz7zeZ3LnQ8sFv3NedZXe");
    EXPECT_EQ(wallet.deriveAddress(coin, TWDerivationBitcoinTestnet), "tb1qq8p994ak933c39d2jaj8n4sg598tnkhnyk5sg5");
}

} // namespace TW::Bitcoin::tests
