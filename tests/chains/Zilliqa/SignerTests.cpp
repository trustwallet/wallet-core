// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Zilliqa/Address.h"
#include "Zilliqa/Signer.h"
#include "proto/Zilliqa.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

namespace TW::Zilliqa::tests {

TEST(ZilliqaSigner, PreImage) {
    auto privateKey = PrivateKey(parse_hex("0E891B9DFF485000C7D1DC22ECF3A583CC50328684321D61947A86E57CF6C638"));
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    ASSERT_EQ(hex(pubKey.bytes), "034ae47910d58b9bde819c3cffa8de4441955508db00aa2540db8e6bf6e99abc1b");

    auto amount = uint256_t(15000000000000);
    auto gasPrice = uint256_t(1000000000);
    auto amountData = store(amount);
    auto gasData = store(gasPrice);
    auto toAddress = Address(parse_hex("0x9Ca91EB535Fb92Fda5094110FDaEB752eDb9B039"));

    auto input = Proto::SigningInput();
    auto& tx = *input.mutable_transaction();
    auto& transfer = *tx.mutable_transfer();
    transfer.set_amount(amountData.data(), amountData.size());

    input.set_version(65537);
    input.set_nonce(4);
    input.set_to(toAddress.string());
    input.set_gas_price(gasData.data(), gasData.size());
    input.set_gas_limit(uint64_t(1));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    Address address;
    auto preImage = Signer::getPreImage(input, address);
    auto signature = Signer::sign(input).signature();

    ASSERT_EQ(hex(preImage), "0881800410041a149ca91eb535fb92fda5094110fdaeb752edb9b03922230a21034ae47910d58b9bde819c3cffa8de4441955508db00aa2540db8e6bf6e99abc1b2a120a10000000000000000000000da475abf00032120a100000000000000000000000003b9aca003801");

    ASSERT_TRUE(pubKey.verifyZilliqa(Data(signature.begin(), signature.end()), preImage));
}

TEST(ZilliqaSigner, Signing) {
    auto privateKey = PrivateKey(parse_hex("0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748"));
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    // 1 ZIL
    auto amount = uint256_t(1000000000000);
    auto gasPrice = uint256_t(1000000000);
    auto amountData = store(amount);
    auto gasData = store(gasPrice);
    auto toAddress = Address(parse_hex("0x7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C"));

    auto input = Proto::SigningInput();
    auto& tx = *input.mutable_transaction();
    auto& transfer = *tx.mutable_transfer();
    transfer.set_amount(amountData.data(), amountData.size());

    input.set_version(65537);
    input.set_nonce(2);
    input.set_to(toAddress.string());
    input.set_gas_price(gasData.data(), gasData.size());
    input.set_gas_limit(uint64_t(1));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.signature()), "001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268");
    ASSERT_EQ(output.json(), R"({"amount":"1000000000000","code":"","data":"","gasLimit":"1","gasPrice":"1000000000","nonce":2,"pubKey":"03fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5c","signature":"001fa4df08c11a4a79e96e69399ee48eeecc78231a78b0355a8ca783c77c139436e37934fecc2252ed8dac00e235e22d18410461fb896685c4270642738ed268","toAddr":"7FCcaCf066a5F26Ee3AFfc2ED1FA9810Deaa632C","version":65537})");
}

TEST(ZilliqaSigner, SigningData) {
    // https://viewblock.io/zilliqa/tx/0x6228b3d7e69fc3481b84fd00e892cec359a41654f58948ff7b1b932396b00ad9
    auto privateKey = PrivateKey(parse_hex("0x68ffa8ec149ce50da647166036555f73d57f662eb420e154621e5f24f6cf9748"));
    auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    // 10 ZIL
    auto amount = uint256_t(10000000000000);
    auto gasPrice = uint256_t(2000000000);
    auto amountData = store(amount);
    auto gasData = store(gasPrice);

    std::string json = "{\"_tag\":\"DelegateStake\",\"params\":[{\"type\":\"ByStr20\",\"value\":\"0x122219cCeAb410901e96c3A0e55E46231480341b\",\"vname\":\"ssnaddr\"}]}";
    auto jsonData = Data(json.begin(), json.end());

    auto input = Proto::SigningInput();
    auto& tx = *input.mutable_transaction();
    auto& raw = *tx.mutable_raw_transaction();
    raw.set_amount(amountData.data(), amountData.size());
    raw.set_data(jsonData.data(), jsonData.size());

    input.set_version(65537);
    input.set_nonce(56);
    input.set_to("zil1g029nmzsf36r99vupp4s43lhs40fsscx3jjpuy");
    input.set_gas_price(gasData.data(), gasData.size());
    input.set_gas_limit(uint64_t(5000));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto output = Signer::sign(input);
    ASSERT_EQ(output.json(), R"({"amount":"10000000000000","code":"","data":"{\"_tag\":\"DelegateStake\",\"params\":[{\"type\":\"ByStr20\",\"value\":\"0x122219cCeAb410901e96c3A0e55E46231480341b\",\"vname\":\"ssnaddr\"}]}","gasLimit":"5000","gasPrice":"2000000000","nonce":56,"pubKey":"03fb30b196ce3e976593ecc2da220dca9cdea8c84d2373770042a930b892ac0f5c","signature":"437fb5c3ce2c6b01f9d490f670539fae4533c82a21fa7edfe6b23df70d732937e8c578c8d6ed24be9150f5126f7b7c977a467af8947ef92a720908a761a6eb0d","toAddr":"43D459eC504C7432959c086B0ac7F7855E984306","version":65537})");
    ASSERT_EQ(hex(output.signature()), "437fb5c3ce2c6b01f9d490f670539fae4533c82a21fa7edfe6b23df70d732937e8c578c8d6ed24be9150f5126f7b7c977a467af8947ef92a720908a761a6eb0d");
}

} // namespace TW::Zilliqa::tests
