#include "Base58.h"
#include "Bitcoin/Taproot.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Taproot::tests {

TEST(Taproot, PayToPublicKeyHash) {
    auto alice_wif = "cQUNzeMnF9xPPLqZhH7hMVYGwSuu3b78zznuc5UrxgXnYQBq6Bx1";
    auto alice_decoded = Base58::decodeCheck(alice_wif);
    auto alice_seckey = PrivateKey(Data(alice_decoded.begin() + 1, alice_decoded.begin() + 33));
    auto alice_pubkey = alice_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

    auto bob_wif = "cTk5wSci88FPka7JwHpNEA82dUMjAysdDbCiuYB2fegfgGESAZVn";
    auto bob_decoded = Base58::decodeCheck(bob_wif);
    auto bob_seckey = PrivateKey(Data(bob_decoded.begin() + 1, bob_decoded.begin() + 33));
    auto bob_pubkey = alice_seckey.getPublicKey(TWPublicKeyTypeSECP256k1Extended);

	const char* TXID = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";
	const uint64_t ONE_BTC = 100000000;
	const uint64_t SEND_TO_ALICE = 50 * ONE_BTC;
	const uint64_t MINER_FEE = ONE_BTC / 100;

	TaprootBuilder builder = TaprootBuilder();
	assert(builder.AddP2PKHInput(parse_hex(TXID).data(), 0, alice_pubkey.bytes.data(), alice_pubkey.bytes.size(), SEND_TO_ALICE).isSuccess());
	assert(builder.AddP2PKHOutput(bob_pubkey.bytes.data(), bob_pubkey.bytes.size(), SEND_TO_ALICE - MINER_FEE).isSuccess());

	auto signature = builder.Sign(alice_seckey.bytes.data(), alice_seckey.bytes.size());
	assert(hex(signature) == "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}

} // namespace TW::Taproot::tests