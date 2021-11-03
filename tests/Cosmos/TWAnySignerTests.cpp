// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cosmos/Address.h"
#include "HexCoding.h"
#include "proto/Cosmos.pb.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cosmos;

TEST(TWAnySignerCosmos, SignTx) {
    auto privateKey = parse_hex("8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af");
    Proto::SigningInput input;
    input.set_signing_mode(Proto::Protobuf);
    input.set_account_number(546179);
    input.set_chain_id("cosmoshub-4");
    input.set_memo("");
    input.set_sequence(0);
    input.set_private_key(privateKey.data(), privateKey.size());

    Address fromAddress;
    EXPECT_TRUE(Address::decode("cosmos1mky69cn8ektwy0845vec9upsdphktxt03gkwlx", fromAddress));
    Address toAddress;
    EXPECT_TRUE(Address::decode("cosmos18s0hdnsllgcclweu9aymw4ngktr2k0rkygdzdp", toAddress));

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress.string());
    message.set_to_address(toAddress.string());
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("uatom");
    amountOfTx->set_amount(400000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("uatom");
    amountOfFee->set_amount(1000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeCosmos);

    // https://www.mintscan.io/cosmos/txs/85392373F54577562067030BF0D61596C91188AA5E6CA8FFE731BD0349296411
    // curl -H 'Content-Type: application/json' --data-binary '{"tx_bytes": "CpIBC...JXoCX", "mode": "BROADCAST_MODE_BLOCK"}' https://api.cosmos.network/cosmos/tx/v1beta1/txs
    EXPECT_EQ(hex(output.serialized()), "0a92010a8f010a1c2f636f736d6f732e62616e6b2e763162657461312e4d736753656e64126f0a2d636f736d6f73316d6b793639636e38656b74777930383435766563397570736470686b7478743033676b776c78122d636f736d6f733138733068646e736c6c6763636c7765753961796d77346e676b7472326b30726b7967647a64701a0f0a057561746f6d120634303030303012650a4e0a460a1f2f636f736d6f732e63727970746f2e736563703235366b312e5075624b657912230a2102ecef5ce437a302c67f95468de4b31f36e911f467d7e6a52b41c1e13e1d56364912040a02080112130a0d0a057561746f6d12043130303010c09a0c1a40afbd513a776f4fdf470ef7f9675f21ae9d630fc4d635d8dbaa0dc0a716434cd07e02510765d4673dfa880825bae8e67cb367396ff6b976fc6b19a31fc95e8097");
    EXPECT_EQ(output.serialized_base64(), "CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseBItY29zbW9zMThzMGhkbnNsbGdjY2x3ZXU5YXltdzRuZ2t0cjJrMHJreWdkemRwGg8KBXVhdG9tEgY0MDAwMDASZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJEgQKAggBEhMKDQoFdWF0b20SBDEwMDAQwJoMGkCvvVE6d29P30cO9/lnXyGunWMPxNY12NuqDcCnFkNM0H4CUQdl1Gc9+ogIJbro5nyzZzlv9rl2/GsZox/JXoCX");
    EXPECT_EQ(hex(output.signature()), "afbd513a776f4fdf470ef7f9675f21ae9d630fc4d635d8dbaa0dc0a716434cd07e02510765d4673dfa880825bae8e67cb367396ff6b976fc6b19a31fc95e8097");
    EXPECT_EQ(output.json(), "");
    EXPECT_EQ(output.error(), "");
}

TEST(TWAnySignerCosmos, SignJSON) {
    auto json = STRING(R"({"accountNumber":"8733","chainId":"cosmoshub-2","fee":{"amounts":[{"denom":"uatom","amount":"5000"}],"gas":"200000"}, "memo":"Testing", "messages":[{"sendCoinsMessage":{"fromAddress":"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575","toAddress":"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0","amounts":[{"denom":"uatom","amount":"995000"}]}}]})");
    auto key = DATA("c9b0a273831931aa4a5f8d1a570d5021dda91d3319bd3819becdaabfb7b44e3b");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeCosmos));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeCosmos));
    assertStringsEqual(result, R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"5000","denom":"uatom"}],"gas":"200000"},"memo":"Testing","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"995000","denom":"uatom"}],"from_address":"cosmos1ufwv9ymhqaal6xz47n0jhzm2wf4empfqvjy575","to_address":"cosmos135qla4294zxarqhhgxsx0sw56yssa3z0f78pm0"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6EsukEXB53GhohQVeDpxtkeH8KQIayd/Co/ApYRYkTm"},"signature":"ULEpUqNzoAnYEx2x22F3ANAiPXquAU9+mqLWoAA/ZOUGTMsdb6vryzsW6AKX2Kqj1pGNdrTcQ58Z09JPyjpgEA=="}]}})");
}
