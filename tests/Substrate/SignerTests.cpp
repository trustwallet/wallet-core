// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Substrate/Signer.h"
#include "Data.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "proto/Substrate.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;

namespace TW::Substrate::tests {

Proto::SigningInput buildSigningInput() {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9a4283cc38f7e769c53ad2d1c5cf292fc85a740ec1c1aa80c180847e51928650

    Substrate::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);
    auto blockHash = parse_hex("898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");
    auto genesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(genesisHash.begin(), genesisHash.end()));
    input.set_nonce(1ul);
    input.set_spec_version(3010u);
    input.set_transaction_version(2u);

    auto* era = input.mutable_era();
    era->set_block_number(4298130ul);
    era->set_period(64ul);

    auto* transfer = input.mutable_balance_call()->mutable_transfer();
    transfer->set_module_index(0x05);
    transfer->set_method_index(0x01);
    transfer->set_to_address("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");

    auto value = store(1000000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_memo("MEMO PADDED WITH SPACES");
    return input;
}

TEST(SubstrateSigner, signaturePreImage) {
    auto input = buildSigningInput();
    auto preImage = Signer::signaturePreImage(input);
    ASSERT_EQ(hex(preImage), "050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f2050414444454420574954482053504143455320202020202020202025010400c20b0000020000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");
}

TEST(SubstrateSigner, encodeTransaction) {
    auto input = buildSigningInput();
    auto publicKey = parse_hex("4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee");
    auto signature = parse_hex("0791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "bd0284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee000791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f25010400050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f20504144444544205749544820535041434553202020202020202020");
}

} // namespace TW::Substrate::tests