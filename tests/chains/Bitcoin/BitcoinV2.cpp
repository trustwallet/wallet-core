// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TrustWalletCore/TWBitcoinV2.h"
#include "proto/BitcoinV2.pb.h"
#include "proto/Utxo.pb.h"
#include "HexCoding.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

using namespace TW;

TEST(BitcoinV2, Sign) {
	auto alice_private_key = parse_hex("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657");
	auto alice_pubkey = parse_hex("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536");
	auto bob_pubkey = parse_hex("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf");

	auto txid = parse_hex("7be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e");

	BitcoinV2::Proto::SigningInput signing;
	signing.set_version(2),
	signing.set_private_key(alice_private_key.data(), alice_private_key.size());
	signing.set_input_selector(Utxo::Proto::InputSelector::UseAll);
	signing.set_disable_change_output(true);

	auto *input = signing.add_inputs();
	input->set_txid(txid.data(), txid.size());
	input->set_vout(0);
	input->set_value(5000000000);
	input->set_sequence(4294967295);
	input->set_sighash_type(Utxo::Proto::SighashType::All);
	input->mutable_builder()->set_p2pkh(alice_pubkey.data(), alice_pubkey.size());

	auto *output = signing.add_outputs();
	output->set_value(5000000000 - 1000000);
	output->mutable_builder()->mutable_p2pkh()->set_pubkey(bob_pubkey.data(), bob_pubkey.size());

	auto signingData = signing.SerializeAsString();
	auto signingTWData = WRAPD(TWDataCreateWithBytes((const uint8_t *)signingData.data(), signingData.size()));
	auto signedOutputTWData = WRAPD(TWBitcoinV2Sign(TWCoinTypeBitcoin, signingTWData.get()));

	BitcoinV2::Proto::SigningOutput signedOutput;
    signedOutput.ParseFromArray(TWDataBytes(signedOutputTWData.get()), static_cast<int>(TWDataSize(signedOutputTWData.get())));

	EXPECT_EQ(signedOutput.error(), BitcoinV2::Proto::Error::OK);
	EXPECT_EQ(hex(signedOutput.encoded()), "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000");
}