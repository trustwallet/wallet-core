#include "Base58.h"
#include "Bitcoin/Taproot.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Taproot::tests {

TEST(Taproot, PayToPublicKeyHash) {
    // Using the keys from a _regtest_ environment, copied from `rust/tw_bitcoin`.
    auto alice_wif = "cQUNzeMnF9xPPLqZhH7hMVYGwSuu3b78zznuc5UrxgXnYQBq6Bx1";
    auto alice_decoded = Base58::decodeCheck(alice_wif);
    auto alice_seckey = PrivateKey(Data(alice_decoded.begin() + 1, alice_decoded.begin() + 33));
    auto alice_pubkey = alice_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    auto bob_wif = "cTk5wSci88FPka7JwHpNEA82dUMjAysdDbCiuYB2fegfgGESAZVn";
    auto bob_decoded = Base58::decodeCheck(bob_wif);
    auto bob_seckey = PrivateKey(Data(bob_decoded.begin() + 1, bob_decoded.begin() + 33));
    auto bob_pubkey = bob_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    const char* TXID = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";
    const uint64_t ONE_BTC = 100000000;
    const uint64_t MINER_FEE = ONE_BTC / 100;
    const uint64_t SEND_TO_ALICE = 50 * ONE_BTC;
    const uint64_t SEND_TO_BOB = SEND_TO_ALICE - MINER_FEE;

    TaprootBuilder builder = TaprootBuilder();

    // Alice claims P2PKH input.
    assert(builder.AddP2PKHInput(parse_hex(TXID).data(), 0, alice_pubkey, SEND_TO_ALICE).isSuccess());
    // Alice constructs P2PKH output.
    assert(builder.AddP2PKHOutput(bob_pubkey, SEND_TO_BOB).isSuccess());
    // Alice signs transaction.
    auto signature = builder.Sign(alice_seckey);

    assert(hex(signature.payload()) == "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}

TEST(Taproot, PayToWitnessPublicKeyHash) {
    // Using the keys from a _regtest_ environment, copied from `rust/tw_bitcoin`.
    auto alice_wif = "cQX5ePcXjTx7C5p6xV8zkp2NN9unhZx4a8RQVPiHd52WxoApV6yK";
    auto alice_decoded = Base58::decodeCheck(alice_wif);
    auto alice_seckey = PrivateKey(Data(alice_decoded.begin() + 1, alice_decoded.begin() + 33));
    auto alice_pubkey = alice_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    auto bob_wif = "cMn7SSCtE5yt2PS97P4NCMvxpCVvT4cBuHiCzKFW5XMvio4fQbD1";
    auto bob_decoded = Base58::decodeCheck(bob_wif);
    auto bob_seckey = PrivateKey(Data(bob_decoded.begin() + 1, bob_decoded.begin() + 33));
    auto bob_pubkey = bob_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    const uint64_t ONE_BTC = 100000000;
    const uint64_t MINER_FEE = ONE_BTC / 100;
    const uint64_t SEND_TO_ALICE = 50 * ONE_BTC;
    const uint64_t SEND_TO_BOB = SEND_TO_ALICE - MINER_FEE;
    const uint64_t SEND_BACK = SEND_TO_BOB - MINER_FEE;

    {
        const char* TXID = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";

        TaprootBuilder builder = TaprootBuilder();
        // Alice claims P2PKH input.
        assert(builder.AddP2PKHInput(parse_hex(TXID).data(), 0, alice_pubkey, SEND_TO_ALICE).isSuccess());
        // Alice constructs P2WPKH output.
        assert(builder.AddP2WPKHOutput(bob_pubkey, SEND_TO_BOB).isSuccess());
        // Alice sign transaction.
        auto sig = builder.Sign(alice_seckey);
        assert(hex(sig.payload()) == "020000000111b9f62923af73e297abb69f749e7a1aa2735fbdfd32ac5f6aa89e5c96841c18000000006b483045022100df9ed0b662b759e68b89a42e7144cddf787782a7129d4df05642dd825930e6e6022051a08f577f11cc7390684bbad2951a6374072253ffcf2468d14035ed0d8cd6490121028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28fffffffff01c0aff629010000001600140d0e1cec6c2babe8badde5e9b3dea667da90036d00000000");
    }

    {
        const char* TXID_2 = "858e450a1da44397bde05ca2f8a78510d74c623cc2f69736a8b3fbfadc161f6e";

        TaprootBuilder builder = TaprootBuilder();
        // Bob claims P2WPKH input.
        assert(builder.AddP2WPKHInput(parse_hex(TXID_2).data(), 0, bob_pubkey, SEND_TO_BOB).isSuccess());
        // Bob constructs P2WPKH output for Alice.
        assert(builder.AddP2WPKHOutput(alice_pubkey, SEND_BACK).isSuccess());
        // Bob signs transaction.
        auto sig = builder.Sign(bob_seckey);
        assert(hex(sig.payload()) == "020000000001016e1f16dcfafbb3a83697f6c23c624cd71085a7f8a25ce0bd9743a41d0a458e850000000000ffffffff01806de7290100000016001460cda7b50f14c152d7401c28ae773c698db9237302483045022100a9b517de5a5e036d7133df499b5b751db6f9a01576a6c5dc38229ec08b6c45cd02200e42c9f8c707c9bf0ceab4f739ec8d683dc1f1f29e195a8da9bc183584d624a60121025a0af1510f0f24d40dd00d7c0e51605ca504bbc177c3e19b065f373a1efdd22f00000000");
    }
}

TEST(Taproot, PayToTaprootKeyPath) {
    // Using the keys from a _regtest_ environment, copied from `rust/tw_bitcoin`.
    auto alice_wif = "cNDFvH3TXCjxgWeVc7vbu4Jw5m2Lu8FkQ69Z2XvFUD9D9rGjofN1";
    auto alice_decoded = Base58::decodeCheck(alice_wif);
    auto alice_seckey = PrivateKey(Data(alice_decoded.begin() + 1, alice_decoded.begin() + 33));
    auto alice_pubkey = alice_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    auto bob_wif = "cNt3XNHiJdJpoX5zt3CXY8ncgrCted8bxmFBzcGeTZbBw6jkByWB";
    auto bob_decoded = Base58::decodeCheck(bob_wif);
    auto bob_seckey = PrivateKey(Data(bob_decoded.begin() + 1, bob_decoded.begin() + 33));
    auto bob_pubkey = bob_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    const uint64_t ONE_BTC = 100000000;
    const uint64_t MINER_FEE = ONE_BTC / 100;
    const uint64_t SEND_TO_ALICE = 50 * ONE_BTC;
    const uint64_t SEND_TO_BOB = SEND_TO_ALICE - MINER_FEE;
    const uint64_t SEND_BACK = SEND_TO_BOB - MINER_FEE;

    {
        const char* TXID = "c50563913e5a838f937c94232f5a8fc74e58b629fae41dfdffcc9a70f833b53a";

        TaprootBuilder builder = TaprootBuilder();
        // Alice claims P2PKH input.
        assert(builder.AddP2PKHInput(parse_hex(TXID).data(), 0, alice_pubkey, SEND_TO_ALICE).isSuccess());
        // Alice constructs P2TR key-path output.
        assert(builder.AddP2TRKeyPathOutput(bob_pubkey, SEND_TO_BOB).isSuccess());
        // Alice sign transaction.
        auto sig = builder.Sign(alice_seckey);
        assert(hex(sig.payload()) == "02000000013ab533f8709accfffd1de4fa29b6584ec78f5a2f23947c938f835a3e916305c5000000006b48304502210086ab2c2192e2738529d6cd9604d8ee75c5b09b0c2f4066a5c5fa3f87a26c0af602202afc7096aaa992235c43e712146057b5ed6a776d82b9129620bc5a21991c0a5301210351e003fdc48e7f31c9bc94996c91f6c3273b7ef4208a1686021bedf7673bb058ffffffff01c0aff62901000000225120e01cfdd05da8fa1d71f987373f3790d45dea9861acb0525c86656fe50f4397a600000000");
    }

    {
        const char* TXID_2 = "9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac";

        TaprootBuilder builder = TaprootBuilder();
        // Bob claims P2TR key-path input.
        assert(builder.AddP2WPKHInput(parse_hex(TXID_2).data(), 0, bob_pubkey, SEND_TO_BOB).isSuccess());
        // Bob constructs P2TR key-path output for Alice.
        assert(builder.AddP2WPKHOutput(alice_pubkey, SEND_BACK).isSuccess());
        // Bob signs transaction.
        auto sig = builder.Sign(bob_seckey);
        assert(hex(sig.payload()) == "02000000000101ac6058397e18c277e98defda1bc38bdf3ab304563d7df7afed0ca5f63220589a0000000000ffffffff01806de72901000000225120a5c027857e359d19f625e52a106b8ac6ca2d6a8728f6cf2107cd7958ee0787c20140ec2d3910d41506b60aaa20520bb72f15e2d2cbd97e3a8e26ee7bad5f4c56b0f2fb0ceaddac33cb2813a33ba017ba6b1d011bab74a0426f12a2bcf47b4ed5bc8600000000");
    }
}

} // namespace TW::Taproot::tests