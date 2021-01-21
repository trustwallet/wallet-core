// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Flow/Signer.h"
#include "Flow/Address.h"
#include "proto/Flow.pb.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Flow;

// TODO: Add tests

TEST(FlowSigner, SignTransfer1) {
    auto input = Proto::SigningInput();
    auto key1 = input.add_keys();
    key1->set_key("rV/OVMsGBQUf6+zmmJkI+/cVpWlNuO3NBkfqaE8R1/1T4ERzy2OKOpM//qWrh6wkSOUCbKNvrEKxVonPKdh0pg==");
    key1->set_sign_algo(Proto::SIGNALGO_ECDSA_SECP256K1);
    key1->set_hash_algo(Proto::HASHALGO_SHA2_256);
    key1->set_weight(1000);
    input.mutable_transfer()->set_amount(10000000);
    input.mutable_transfer()->set_destination("0x3bf5d7407caaeb32");

    auto output = Signer::sign(input);

    //ASSERT_EQ(hex(output.encoded()), "encoded");
}
