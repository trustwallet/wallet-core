// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "TestUtilities.h"
#include <TrustWalletCore/Generated/TWEthereum.h>
#include <TrustWalletCore/Generated/TWBiz.h>
#include "proto/Barz.pb.h"
#include "proto/Biz.pb.h"
#include "uint256.h"

#include <PrivateKey.h>

namespace TW::Biz::tests {

TEST(Biz, SignExecuteWithSignatureCall) {
    Proto::ExecuteWithSignatureInput input;

    // Set up executions - IERC20(TWT).approve()
    auto* execution = input.add_executions();
    execution->set_address("0x4B0F1812e5Df2A09796481Ff14017e6005508003");
    execution->set_amount(parse_hex("0x00").data(), 0); // 0 amount
    // ERC20 approve(spender, allowance) - spender: 0xBC472b43BC237f733c78a581078F58A6a89c46Ec, allowance: 1000
    auto approvePayload = parse_hex("0x095ea7b3000000000000000000000000bc472b43bc237f733c78a581078f58a6a89c46ec00000000000000000000000000000000000000000000000000000000000003e8");
    execution->set_payload(approvePayload.data(), approvePayload.size());

    // Set private key
    auto privateKey = parse_hex("0xefec50f00ef0c09d967f3e363ee96502ce18a1881f6ac22321aa58071d43c66f");
    input.set_private_key(privateKey.data(), privateKey.size());

    // Set nonce (0)
    input.set_nonce(parse_hex("0x00").data(), 0);

    // Set encoding hash params
    auto* encodingHashParams = input.mutable_encoding_hash_params();
    auto chainId = parse_hex("0x38"); // 56
    encodingHashParams->set_chain_id(chainId.data(), chainId.size());
    encodingHashParams->set_code_address("0xba083F0EeAF806603d31582D4e7667fB5A4A1B30");
    encodingHashParams->set_code_name("Biz");
    encodingHashParams->set_code_version("v1.0.0");
    encodingHashParams->set_type_hash("0xec429430bbd6d0e373848272230d6fe2bac6319d903762e089c5cae97af53df0");
    encodingHashParams->set_domain_separator_hash("0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472");

    const auto& serialized = input.SerializeAsString();
    const auto& encodedCall = TWBizSignExecuteWithSignatureCall(WRAPD(TWDataCreateWithBytes((const uint8_t *)serialized.data(), serialized.size())).get());
    const auto& encodedCallData = hexEncoded(*reinterpret_cast<const Data*>(WRAPD(encodedCall).get()));

    // Successfully broadcasted tx:
    // https://bscscan.com/tx/0xaeef330e78c73fe1725323e1be66bf59e731a351788de88e238fe137ba5c38d1
    ASSERT_EQ(encodedCallData, "0x1d92e4b600000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000160000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000200000000000000000000000004b0f1812e5df2a09796481ff14017e6005508003000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000044095ea7b3000000000000000000000000bc472b43bc237f733c78a581078f58a6a89c46ec00000000000000000000000000000000000000000000000000000000000003e8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000041db18e3a0150ddef964810e480b25592942a22d0b583f7d5cbb33ef6fb4baa66e753af78e967ee374070e16cf963a6cd7a3adb713e50d553aefbc361c48366a101b00000000000000000000000000000000000000000000000000000000000000");
}

} // namespace TW::Biz::tests
