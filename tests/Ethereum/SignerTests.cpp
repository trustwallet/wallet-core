// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/Address.h"
#include "Ethereum/RLP.h"
#include "Ethereum/Signer.h"
#include "Ethereum/Transaction.h"
#include "HexCoding.h"

#include "HDWallet.h"
#include "Coin.h"
#include "Hash.h"
#include "Data.h"

#include <gtest/gtest.h>

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>
#include <boost/multiprecision/cpp_int.hpp>

namespace TW::Ethereum {

using boost::multiprecision::uint256_t;


byte calculateSigRecovery(byte v, byte chainId) {
    //std::cout << "calculateSigRecovery  chainId: " << (int)chainId << " v_hex: " << hex(store(v)) << " v_int: " << int(v) << "\n";
    if (chainId != 0 && v >= 35) {
        return v - (35 + chainId * 2);
    }
    if (v >= 27) {
        return v - 27;
    }
    return v;
}

Data uint256ToData(const uint256_t number) {
    Data d;
    export_bits(number, std::back_inserter(d), 8);
    if (d.size() < 32) {
        std::cout << "\nadjustLength32 SPECIAL CASE d.size != 32 " << d.size() << "\n\n";
        Data corr(32 - d.size());
        append(corr, d);
        d = corr;
    }
    return d;
}

// Signature struct to byte array, reverse of Signer::signatureDataToStruct()
Data signatureToData(const Signature& signature, byte chainId) {
    Data rr = uint256ToData(signature.r);
    Data ss = uint256ToData(signature.s);

    byte vv = byte(signature.v);
    if (vv >= 35) {
        vv = vv - (35 + chainId * 2);
    } else if (vv >= 27) {
        vv = vv - 27;
    }

    Data sig;
    append(sig, rr);
    append(sig, ss);
    append(sig, vv); // 1 byte
    //std::cout << "signatureToData sig " << sig.size() << " " << hex(sig) << "\n";
    return sig;
}

PublicKey ecdsaRecover(const Signature& signature, byte recoveryV, const Data& msgHash, byte chainId) {
    const Data sig = signatureToData(signature, chainId);
    //std::cout << "ecdsaRecover sig " << sig.size() << " " << hex(sig) << "\n";

    Data pubkey(65);
    int res = ecdsa_recover_pub_from_sig(&secp256k1, pubkey.data(), sig.data(), msgHash.data(), (int)recoveryV);
    if (res) {
        std::cout << "ecdsaRecover wrong res " << res << " recovery " << (int)recoveryV << "\n";

        curve_point cp;
        // read r
        bn_read_be(sig.data(), &cp.x);
        uncompress_coords(&secp256k1, recoveryV & 1, &cp.x, &cp.y);

        Data xd(32);
        bn_write_be(&cp.x, xd.data());
        std::cout << "x " << hex(xd) << "\n";
        Data yd(32);
        bn_write_be(&cp.y, yd.data());
        std::cout << "y " << hex(yd) << "\n";

        int validate = ecdsa_validate_pubkey(&secp256k1, &cp);
        std::cout << "validate " << validate << "\n";
    }
    //std::cout << "ecdsaRecover pubkey " << pubkey.size() << " " << hex(pubkey) << "\n";
    assert(res == 0);

    /*
    curve_point pub;
    int res2 = ecdsa_read_pubkey(&secp256k1, pubkey.data(), &pub);
    assert(res2 == 1);
    Data compr(33);
    compress_coords(&pub, compr.data());
    std::cout << "ecdsaRecover compr " << compr.size() << " " << hex(compr) << "\n";

    const PublicKey pubKey = PublicKey(compr, TWPublicKeyTypeSECP256k1);
    */
    const PublicKey pubKey = PublicKey(pubkey, TWPublicKeyTypeSECP256k1Extended);
    return pubKey;
}

PublicKey ecrecover(const Data& msgHash, const Signature& signature, byte chainId) {
    const byte recovery = calculateSigRecovery(byte(signature.v), chainId);
    if (recovery < 0 || recovery > 1) {
        std::cout << "wrong recovery value!  v: " << hex(store(signature.v)) << " recovery: " << (int)recovery << " chainId: " << (int)chainId << "\n";
    }
    assert(recovery >= 0 && recovery <= 1);
    const PublicKey publicKey = ecdsaRecover(signature, recovery, msgHash, chainId);
    return publicKey;
}

bool testRecover(const PrivateKey& privateKey, const Data& hashToSign, byte chainId) {
    bool res = true;
    std::cout << "privateKey: " << hex(privateKey.bytes) << "\n";
    //const PublicKey publicKeyCompr = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1); // compressed
    //std::cout << "publicKeyCompr: " << hex(publicKeyCompr.bytes) << "\n";
    const PublicKey publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended); // extended
    std::cout << "publicKey: " << hex(publicKey.bytes) << "\n";

    const Signature signature = Signer::sign(privateKey, hashToSign, true, chainId);
    const Data sigData = signatureToData(signature, chainId);
    std::cout << "sigData: " << sigData.size() << " " << hex(sigData) << "\n";

    // Check recover, PublicKey implementation
    {
        const PublicKey recoveredKeyPK = PublicKey::recover(sigData, hashToSign);
        std::cout << "recoveredKeyPK: " << recoveredKeyPK.bytes.size() << " " << hex(recoveredKeyPK.bytes) << "\n";
        if (hex(recoveredKeyPK.bytes) != hex(publicKey.bytes)) {
            std::cout << "MISMATCH recoveredKeyPK! \n";
            res = false;
        }
    }

    // Check ecrecover, local implementation
    {
        Data sig2 = sigData;
        sig2[64] = sig2[64] - (35 + chainId * 2); // need to adjust v, recover() does not support chainId
        const PublicKey recoveredKeyLocal = ecrecover(hashToSign, signature, chainId);
        std::cout << "recoveredKeyLocal: " << recoveredKeyLocal.bytes.size() << " " << hex(recoveredKeyLocal.bytes) << "\n";
        if (hex(recoveredKeyLocal.bytes) != hex(publicKey.bytes)) {
            std::cout << "MISMATCH recoveredKeyLocal! \n";
            res = false;
        }
    }

    std::cout << "\n";
    return res;
}

TEST(EthereumSigning, verifySignature) {
    const auto messageContent = "Some data";
    const Data ethPrefix = data("\x019""Ethereum Signed Message:\n");
    EXPECT_EQ(hex(ethPrefix), "19457468657265756d205369676e6564204d6573736167653a0a");
    Data message = ethPrefix;
    TW::append(message, '0' + strlen(messageContent));
    TW::append(message, data(messageContent));
    EXPECT_EQ(hex(message), "19457468657265756d205369676e6564204d6573736167653a0a39536f6d652064617461");
    const Data hashToSign1 = Hash::keccak256(message);
    EXPECT_EQ(hex(hashToSign1), "1da44b586eb0729ff70a73c326926f6ed5a25f5b056e7f47fbc6e58d86871655");
    const byte chainId = 1;

    {   // a case with leading 0
        const PrivateKey privKey2 = PrivateKey(parse_hex("54f485342a8ebccd04351e0a0c1a458fefc2571f7aeeaaf88a4196c1e3083b3e"));

        const Signature signature = Signer::sign(privKey2, hashToSign1, true, chainId);
        const Data sigData = signatureToData(signature, chainId);
        EXPECT_EQ(sigData[0], 0);

        bool res = testRecover(privKey2, hashToSign1, chainId);
        ASSERT_TRUE(res);
    }

    {   // a case with leading 0 in second coordinate
        const PrivateKey privKey2 = PrivateKey(parse_hex("0fcb27122f5e3274beefc54ce86272b11eff9ed6a284cc8fa09a19c890593f9d"));

        const Signature signature = Signer::sign(privKey2, hashToSign1, true, chainId);
        const Data sigData = signatureToData(signature, chainId);
        EXPECT_EQ(sigData[32], 0);

        bool res = testRecover(privKey2, hashToSign1, chainId);
        ASSERT_TRUE(res);
    }

    {
        const auto derivationPath = TW::derivationPath(TWCoinTypeEthereum);
        for(auto i = 0; i < 2000; ++i) {
            std::cout << ">>> i: " << i << ":   ";

            const HDWallet wallet = HDWallet(128, "");
            const PrivateKey privateKey = wallet.getKey(TWCoinTypeEthereum, derivationPath);

            bool res = testRecover(privateKey, hashToSign1, chainId);
            ASSERT_TRUE(res); // stop on error
        }
    }
}

TEST(EthereumTransaction, encodeTransactionNonTyped) {
    const auto transaction = TransactionNonTyped::buildERC20Transfer(
        /* nonce: */ 0,
        /* gasPrice: */ 42000000000, // 0x09c7652400
        /* gasLimit: */ 78009, // 130B9
        /* tokenContract: */ parse_hex("0x6b175474e89094c44da98b954eedeac495271d0f"), // DAI
        /* toAddress: */ parse_hex("0x5322b34c88ed0691971bf52a7047448f0f4efc84"),
        /* amount: */ 2000000000000000000
    );
    const uint256_t dummyChain = 0x34;
    const auto dummySignature = Signature{0, 0, 0};

    const auto preHash = transaction->preHash(dummyChain);
    EXPECT_EQ(hex(preHash), "b3525019dc367d3ecac48905f9a95ff3550c25a24823db765f92cae2dec7ebfd");
    
    const auto encoded = transaction->encoded(dummySignature, dummyChain);
    EXPECT_EQ(hex(encoded), "f86a808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000808080");
}

TEST(EthereumSigner, PreHash) {
    const auto address = parse_hex("0x3535353535353535353535353535353535353535");
    const auto transaction = TransactionNonTyped::buildNativeTransfer(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000
    );
    const auto preHash = transaction->preHash(1);
    
    ASSERT_EQ(hex(preHash), "daf5a779ae972f972197303d7b574746c7ef83eadac0f2791ad23db92e4c8e53");
}

TEST(EthereumSigner, Sign) {
    const auto address = parse_hex("0x3535353535353535353535353535353535353535");
    const auto transaction = TransactionNonTyped::buildNativeTransfer(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000
    );

    const auto key = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    const auto signature = Signer::sign(key, 1, transaction);

    ASSERT_EQ(signature.v, 37);
    ASSERT_EQ(signature.r, uint256_t("18515461264373351373200002665853028612451056578545711640558177340181847433846"));
    ASSERT_EQ(signature.s, uint256_t("46948507304638947509940763649030358759909902576025900602547168820602576006531"));
}

TEST(EthereumSigner, SignERC20Transfer) {
    const auto transaction = TransactionNonTyped::buildERC20Transfer(
        /* nonce: */ 0,
        /* gasPrice: */ 42000000000, // 0x09c7652400
        /* gasLimit: */ 78009, // 130B9
        /* tokenContract: */ parse_hex("0x6b175474e89094c44da98b954eedeac495271d0f"), // DAI
        /* toAddress: */ parse_hex("0x5322b34c88ed0691971bf52a7047448f0f4efc84"),
        /* amount: */ 2000000000000000000
    );

    const auto key = PrivateKey(parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151"));
    const auto signature = Signer::sign(key, 1, transaction);

    ASSERT_EQ(signature.v, 37);
    ASSERT_EQ(hex(store(signature.r)), "724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98ce");
    ASSERT_EQ(hex(store(signature.s)), "032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931");
}

TEST(EthereumSigner, EIP1559_1442) {
    const auto transaction = TransactionEip1559::buildNativeTransfer(
        6, // nonce
        2000000000, // maxInclusionFeePerGas
        3000000000, // maxFeePerGas
        21100, // gasLimit
        parse_hex("B9F5771C27664bF2282D98E09D7F50cEc7cB01a7"),
        543210987654321, // amount
        Data() // data
    );

    const uint256_t chainID = 3;
    const auto key = PrivateKey(parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904"));
    const auto signature = Signer::sign(key, chainID, transaction);
    EXPECT_EQ(signature.v, 0);
    EXPECT_EQ(hex(store(signature.r)), "92c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64");
    EXPECT_EQ(hex(store(signature.s)), "6487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58");

    const auto encoded = transaction->encoded(signature, chainID);
    // https://ropsten.etherscan.io/tx/0x14429509307efebfdaa05227d84c147450d168c68539351fbc01ed87c916ab2e
    EXPECT_EQ(hex(encoded), "02f8710306847735940084b2d05e0082526c94b9f5771c27664bf2282d98e09d7f50cec7cb01a78701ee0c29f50cb180c080a092c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64a06487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58");
}

} // namespace TW::Ethereum
