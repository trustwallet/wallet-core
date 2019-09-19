// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Waves/Address.h"
#include "proto/Waves.pb.h"
#include "Waves/Transaction.h"

#include <Waves/Signer.h>
#include <gtest/gtest.h>

using json = nlohmann::json;

using namespace std;
using namespace TW;
using namespace TW::Waves;

TEST(WavesLease, serialize) {
    const auto privateKey =
    PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1526646497465));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    
    auto &message = *input.mutable_lease_message();
    message.set_amount(int64_t(100000000));
    message.set_fee(int64_t(100000));
    message.set_to("3P9DEDP5VbyXQyKtXDUt2crRPn5B7gs6ujc");
    auto tx1 = Transaction(
                           input,
                           /* pub_key */
                           parse_hex("425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d43461"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "080200425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d4346101574"
                                "fdfcd1bfb19114bd2ac369e32013c70c6d03a4627879cbf0000000005f5e100000000000001"
                                "86a0000001637338e0b9");
}

TEST(WavesLease, CancelSerialize) {
    const auto privateKey =
    PrivateKey(parse_hex("9864a747e1b97f131fabb6b447296c9b6f0201e79fb3c5356e6c77e89b6a806a"));
    auto input = Proto::SigningInput();
    input.set_timestamp(int64_t(1568831000826));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    
    auto &message = *input.mutable_cancel_lease_message();
    message.set_fee(int64_t(100000));
    message.set_lease_id("44re3UEDw1QwPFP8dKzfuGHVMNBejUW9NbhxG6b4KJ1T");
    auto tx1 = Transaction(
                           input,
                           /* pub_key */
                           parse_hex("425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d43461"));
    auto serialized1 = tx1.serializeToSign();
    ASSERT_EQ(hex(serialized1), "090257425f57a8cb5439e4e912e66376f7041565d029ae4437dae1a3ebe15649d"
                                "4346100000000000186a00000016d459d50fa2d8fee08efc97f79bcd97a4d977c"
                                "76183580d723909af2b50e72b02f1e36707e");
}


