// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Icon/Signer.h"
#include "PrivateKeySecp256k1.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW::secp256k1;

namespace TW {
namespace Icon {

TEST(IconSigner, Sign) {
    auto input = Proto::SigningInput();
    input.set_from_address("hxbe258ceb872e08851f1f59694dac2558708ece11");
    input.set_to_address("hx5bfdb090f43a808005ffc27c25b213145e80b7cd");

    auto value = uint256_t("1000000000000000000");
    auto valueData = store(value);
    input.set_value(valueData.data(), valueData.size());

    auto stepLimitData = store(uint256_t("74565"));
    std::cout << hex(stepLimitData) << std::endl;
    input.set_step_limit(stepLimitData.data(), stepLimitData.size());

    input.set_timestamp(1516942975500598);

    auto nonceData = store(uint256_t(1));
    input.set_nonce(nonceData.data(), nonceData.size());

    auto nidData = store(uint256_t(1));
    input.set_network_id(nidData.data(), nidData.size());

    const auto privateKey = PrivateKey(parse_hex("2d42994b2f7735bbc93a3e64381864d06747e574aa94655c516f9ad0a74eed79"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto signer = Signer(input);

    const auto preImage = signer.preImage();
    ASSERT_EQ(preImage, "icx_sendTransaction.from.hxbe258ceb872e08851f1f59694dac2558708ece11.nid.0x1.nonce.0x1.stepLimit.0x12345.timestamp.0x563a6cf330136.to.hx5bfdb090f43a808005ffc27c25b213145e80b7cd.value.0xde0b6b3a7640000.version.0x3");

    const auto output = signer.sign();
    const auto signature = Data(output.signature().begin(), output.signature().end());
    ASSERT_EQ(hex(signature), "c51eb02acf8803eec4f756d3f3deba8c52a52b999acaeb570af6b2b92302af1f4a07bebbd02b166b407b2d0ce0b3153418b5daa2f9404f630bb355ee0e249a6501");
    ASSERT_EQ(output.encoded(), "{\"from\":\"hxbe258ceb872e08851f1f59694dac2558708ece11\",\"nid\":\"0x1\",\"nonce\":\"0x1\",\"signature\":\"xR6wKs+IA+7E91bT8966jFKlK5mayutXCvayuSMCrx9KB7670CsWa0B7LQzgsxU0GLXaovlAT2MLs1XuDiSaZQE=\",\"stepLimit\":\"0x12345\",\"timestamp\":\"0x563a6cf330136\",\"to\":\"hx5bfdb090f43a808005ffc27c25b213145e80b7cd\",\"value\":\"0xde0b6b3a7640000\",\"version\":\"0x3\"}");
}

}} // namespace
