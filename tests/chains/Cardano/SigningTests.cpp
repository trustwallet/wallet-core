// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/AddressV3.h"
#include "Cardano/Signer.h"
#include "proto/Cardano.pb.h"
#include <TrustWalletCore/TWCardano.h>

#include "Cbor.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace std;

namespace TW::Cardano::SigningTests {

const auto privateKeyTest1 = "089b68e458861be0c44bf9f7967f05cc91e51ede86dc679448a3566990b7785bd48c330875b1e0d03caaed0e67cecc42075dce1c7a13b1c49240508848ac82f603391c68824881ae3fc23a56a1a75ada3b96382db502e37564e84a5413cfaf1290dbd508e5ec71afaea98da2df1533c22ef02a26bb87b31907d0b2738fb7785b38d53aa68fc01230784c9209b2b2a2faf28491b3b1f1d221e63e704bbd0403c4154425dfbb01a2c5c042da411703603f89af89e57faae2946e2a5c18b1c5ca0e";
const auto ownAddress1 = "addr1q8043m5heeaydnvtmmkyuhe6qv5havvhsf0d26q3jygsspxlyfpyk6yqkw0yhtyvtr0flekj84u64az82cufmqn65zdsylzk23";
const auto sundaeTokenPolicy = "9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77";

TEST(CardanoSigning, SelectInputs) {
    const auto inputs = std::vector<TxInput>({
        TxInput{{parse_hex("0001"), 0}, "ad01", 700, {}},
        TxInput{{parse_hex("0002"), 1}, "ad02", 900, {}},
        TxInput{{parse_hex("0003"), 2}, "ad03", 300, {}},
        TxInput{{parse_hex("0004"), 3}, "ad04", 600, {}},
    });

    { // 2
        const auto s1 = Signer::selectInputsWithTokens(inputs, 1500, {});
        ASSERT_EQ(s1.size(), 2ul);
        EXPECT_EQ(s1[0].amount, 900ul);
        EXPECT_EQ(s1[1].amount, 700ul);
    }
    { // all
        const auto s1 = Signer::selectInputsWithTokens(inputs, 10000, {});
        ASSERT_EQ(s1.size(), 4ul);
        EXPECT_EQ(s1[0].amount, 900ul);
        EXPECT_EQ(s1[1].amount, 700ul);
        EXPECT_EQ(s1[2].amount, 600ul);
        EXPECT_EQ(s1[3].amount, 300ul);
    }
    { // 3
        const auto s1 = Signer::selectInputsWithTokens(inputs, 2000, {});
        ASSERT_EQ(s1.size(), 3ul);
    }
    { // 1
        const auto s1 = Signer::selectInputsWithTokens(inputs, 500, {});
        ASSERT_EQ(s1.size(), 1ul);
    }
    { // at least 0 is returned
        const auto s1 = Signer::selectInputsWithTokens(inputs, 0, {});
        ASSERT_EQ(s1.size(), 1ul);
    }
}

Proto::SigningInput createSampleInput(uint64_t amount, int utxoCount = 10,
                                      const std::string& alternateToAddress = "", bool omitPrivateKey = false) {
    const std::string toAddress = (alternateToAddress.length() > 0) ? alternateToAddress : "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5";

    Proto::SigningInput input;
    if (utxoCount >= 1) {
        auto* utxo1 = input.add_utxos();
        const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
        utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
        utxo1->mutable_out_point()->set_output_index(1);
        utxo1->set_address(ownAddress1);
        utxo1->set_amount(1500000);
    }
    if (utxoCount >= 2) {
        auto* utxo2 = input.add_utxos();
        const auto txHash2 = parse_hex("554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0");
        utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
        utxo2->mutable_out_point()->set_output_index(0);
        utxo2->set_address(ownAddress1);
        utxo2->set_amount(6500000);
    }

    if (!omitPrivateKey) {
        const auto privateKeyData = parse_hex(privateKeyTest1);
        input.add_private_key(privateKeyData.data(), privateKeyData.size());
    }
    input.mutable_transfer_message()->set_to_address(toAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(amount);
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(53333333);
    return input;
}

/// Successfully broadcasted:
/// https://cardanoscan.io/transaction/87ca43a36b09c0b140f0ef2b71fbdcfcf1fdc88f7aa378b861e8eed3e8974628
TEST(CardanoSigning, SendNft) {
    const auto fromAddressPrivKey = "d09831a668db6b36ffb747600cb1cd3e3d34f36e1e6feefc11b5f988719b7557a7029ab80d3e6fe4180ad07a59ddf742ea9730f3c4145df6365fa4ae2ee49c3392e19444caf461567727b7fefec40a3763bdb6ce5e0e8c05f5e340355a8fef4528dfe7502cfbda49e38f5a0021962d52dc3dee82834a23abb6750981799b75577d1ed9af9853707f0ef74264274e71b2f12e86e3c91314b6efa75ef750d9711b84cedd742ab873ef2f9566ad20b3fc702232c6d2f5d83ff425019234037d1e58";
    const auto fromAddress = "addr1qy5eme9r6frr0m6q2qpncg282jtrhq5lg09uxy2j0545hj8rv7v2ntdxuv6p4s3eq4lqzg39lewgvt6fk5kmpa0zppesufzjud";
    const auto toAddress = "addr1qy9wjfn6nd8kak6dd8z53u7t5wt9f4lx0umll40px5hnq05avwcsq5r3ytdp36wttzv4558jaq8lvhgqhe3y8nuf5xrquju7z4";
    const auto nftPolicyId = "219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5e";
    const auto nftAssetName = "coolcatssociety4567";
    const auto nftTokenAmount = 1ul;
    // 1.20249 ADA. Amount locked by the NFT.
    const auto nftInputAmount = 1202490ul;
    const auto ttl = 89130965ul;

    Proto::SigningInput input;

    // Set the first utxo (NFT token and locked ADA).

    auto* utxo1 = input.add_utxos();
    // NFT unspent output.
    const auto txHash1 = parse_hex("aba499ec2f23529e70bb256ceaffcc6274a882cf02f29e5670c75ee980d7c2b8");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(fromAddress);
    utxo1->set_amount(nftInputAmount);

    auto* token1 = utxo1->add_token_amount();
    token1->set_policy_id(nftPolicyId);
    token1->set_asset_name(nftAssetName);
    const auto tokenAmount1 = store(uint256_t(nftTokenAmount));
    token1->set_amount(tokenAmount1.data(), tokenAmount1.size());

    // Set additional utxos to pay fee.

    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("ee414d635b3bc67831907354d274a31174664777c57c21ae923b9459e5644840");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address(fromAddress);
    utxo2->set_amount(1000000);

    auto* utxo3 = input.add_utxos();
    const auto txHash3 = parse_hex("6a7221dcc28353ed69b733391ffeb984a34c1e72293af111d59f9ddfa8639167");
    utxo3->mutable_out_point()->set_tx_hash(txHash3.data(), txHash3.size());
    utxo3->mutable_out_point()->set_output_index(0);
    utxo3->set_address(fromAddress);
    utxo3->set_amount(2000000);

    PrivateKey privKey(parse_hex(fromAddressPrivKey));
    input.add_private_key(privKey.bytes.data(), privKey.bytes.size());

    // Set an output info.

    input.mutable_transfer_message()->set_to_address(toAddress);
    input.mutable_transfer_message()->set_change_address(fromAddress);
    input.mutable_transfer_message()->set_amount(nftInputAmount);

    auto* toToken = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    toToken->set_policy_id(nftPolicyId);
    toToken->set_asset_name(nftAssetName);
    const auto toTokenAmount = store(uint256_t(nftTokenAmount));
    toToken->set_amount(toTokenAmount.data(), toTokenAmount.size());
    input.set_ttl(ttl);

    { // check min ADA amount
        // The byte cost at the moment when the transaction was constructed.
        // See `ProtocolParams::coinsPerUtxoByte`:
        // https://input-output-hk.github.io/cardano-graphql/
        const auto coinsPerUtxoByte = STRING("4310");

        const auto bundleProtoData = data(input.transfer_message().token_amount().SerializeAsString());
        const auto toAddressPtr = STRING(toAddress);

        const auto minAdaAmount = WRAPS(TWCardanoOutputMinAdaAmount(toAddressPtr.get(), &bundleProtoData, coinsPerUtxoByte.get()));
        assertStringsEqual(minAdaAmount, std::to_string(nftInputAmount).c_str());
        EXPECT_EQ(input.transfer_message().amount(), nftInputAmount);
    }

    // run plan and check result
    auto signer = Signer(input);
    const auto plan = signer.doPlan();
    const auto output = signer.sign();

    const auto txid = hex(data(output.tx_id()));
    EXPECT_EQ(txid, "87ca43a36b09c0b140f0ef2b71fbdcfcf1fdc88f7aa378b861e8eed3e8974628");

    EXPECT_EQ(plan.availableAmount, nftInputAmount + 1000000ul + 2000000ul);
    EXPECT_EQ(plan.amount, nftInputAmount);
    EXPECT_EQ(plan.fee, 176539ul);
    EXPECT_EQ(plan.change, 1000000ul + 2000000ul - 176539ul);
    EXPECT_EQ(plan.utxos.size(), 3ul);
    EXPECT_EQ(plan.availableTokens.size(), nftTokenAmount);
    EXPECT_EQ(plan.availableTokens.getAmount("219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5e_coolcatssociety4567"), nftTokenAmount);
    EXPECT_EQ(plan.outputTokens.size(), 1ul);
    EXPECT_EQ(plan.outputTokens.getAmount("219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5e_coolcatssociety4567"), nftTokenAmount);
    EXPECT_EQ(plan.changeTokens.size(), 0ul);

    const auto txHex = hex(data(output.encoded()));
    EXPECT_EQ(txHex, "83a400838258206a7221dcc28353ed69b733391ffeb984a34c1e72293af111d59f9ddfa863916700825820aba499ec2f23529e70bb256ceaffcc6274a882cf02f29e5670c75ee980d7c2b800825820ee414d635b3bc67831907354d274a31174664777c57c21ae923b9459e5644840000182825839010ae9267a9b4f6edb4d69c548f3cba39654d7e67f37ffd5e1352f303e9d63b100507122da18e9cb58995a50f2e80ff65d00be6243cf89a186821a0012593aa1581c219820e6cb04316f41a337fea356480f412e7acc147d28f175f21b5ea153636f6f6c63617473736f6369657479343536370182583901299de4a3d24637ef4050033c214754963b829f43cbc311527d2b4bc8e36798a9ada6e3341ac239057e012225fe5c862f49b52db0f5e208731a002b1525021a0002b19b031a055007d5a1008182582088bd26e8656fa7dead846c3373588f0192da5bfb90bf5d3fb877decfb3b3fd085840da8656aca0dacc57d4c2d957fc7dff03908f6dcf60c48f1e40b3006e2fd0cfacfa4c24fa02e35a310572526586d4ce0d30bf660ba274c8efd507848cbe177d09f6");
}

TEST(CardanoSigning, Plan) {
    auto input = createSampleInput(7000000);

    {
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 2ul);
        EXPECT_EQ(plan.availableAmount, 8000000ul);
        EXPECT_EQ(plan.amount, 7000000ul);
        EXPECT_EQ(plan.fee, 170196ul);
        EXPECT_EQ(plan.change, 829804ul);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
        EXPECT_EQ(plan.error, Common::Proto::OK);
    }
    { // very small target amount
        input.mutable_transfer_message()->set_amount(1);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 1ul);
        EXPECT_EQ(plan.availableAmount, 6500000ul);
        EXPECT_EQ(plan.amount, 1ul);
        EXPECT_EQ(plan.fee, 168435ul);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    { // small target amount
        input.mutable_transfer_message()->set_amount(2000000);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 1ul);
        EXPECT_EQ(plan.availableAmount, 6500000ul);
        EXPECT_EQ(plan.amount, 2000000ul);
        EXPECT_EQ(plan.fee, 168611ul);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    { // small target amount requested, but max amount
        input.mutable_transfer_message()->set_amount(2000000);
        input.mutable_transfer_message()->set_use_max_amount(true);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.utxos.size(), 2ul);
        EXPECT_EQ(plan.availableAmount, 8000000ul);
        EXPECT_EQ(plan.amount, 7832667ul);
        EXPECT_EQ(plan.fee, 167333ul);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
}

TEST(CardanoSigning, ExtraOutputPlan) {
    auto input = createSampleInput(2000000, 10, "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5", true);
    // two output
    Proto::TxOutput txOutput;
    txOutput.set_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    txOutput.set_amount(2000000);
    *input.add_extra_outputs() = txOutput;
    auto signer = Signer(input);
    const auto plan = signer.doPlan();
    EXPECT_EQ(plan.utxos.size(), 1ul);
    EXPECT_EQ(plan.availableAmount, 6500000ul);
    EXPECT_EQ(plan.amount, 2000000ul);
    EXPECT_EQ(plan.fee, 171474ul);
    uint64_t extraAmountSum = 0;
    for (auto& output: plan.extraOutputs) {
        extraAmountSum = extraAmountSum + output.amount;
    }
    EXPECT_EQ(plan.amount + plan.change + plan.fee + extraAmountSum, plan.availableAmount);

    {
        // also test proto fromProto / fromProto
        Proto::TxOutput txOutputProto;
        txOutputProto.set_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
        txOutputProto.set_amount(2000000);

        auto* token = txOutputProto.add_token_amount();
        token->set_policy_id(sundaeTokenPolicy);
        token->set_asset_name_hex("43554259");
        const auto tokenAmount = store(uint256_t(3000000));
        token->set_amount(tokenAmount.data(), tokenAmount.size());

        const auto txOutput1 = TxOutput::fromProto(txOutputProto);
        EXPECT_EQ(txOutput1.amount, 2000000ul);
        const auto toAddress = AddressV3(txOutput1.address);
        EXPECT_EQ(toAddress.string(), "addr1v9jxgu33wyunycmdddnh5a3edq6x2dt3xakkuun6wd6hsar8v9uhvee5w9erw7fnvauhswfhw44k673nv3n8sdmj89n82denweckuv34xvmnw6m9xeerq7rt8ymh5aesxaj8zu3e0y6k67tcd3nkzervxfenqer8ddjn27jkkrj");
        EXPECT_EQ(txOutput1.tokenBundle.getByPolicyId(sundaeTokenPolicy)[0].amount, 3000000);
        EXPECT_EQ(txOutput1.tokenBundle.getByPolicyId(sundaeTokenPolicy)[0].assetName, "CUBY");
        EXPECT_EQ(txOutput1.tokenBundle.getByPolicyId(sundaeTokenPolicy)[0].policyId, sundaeTokenPolicy);
    }
    {
        // also test proto toProto / toProto
        const auto toAddress = AddressV3("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
        std::vector<TokenAmount> tokenAmount;
        tokenAmount.emplace_back(sundaeTokenPolicy, "CUBY", 3000000);
        const Proto::TxOutput txOutputProto = TxOutput(toAddress.data(), 2000000, TokenBundle(tokenAmount)).toProto();
        EXPECT_EQ(txOutputProto.amount(), 2000000ul);
        EXPECT_EQ(txOutputProto.address(), "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
        const auto token = txOutputProto.token_amount(0);
        EXPECT_EQ(token.policy_id(), sundaeTokenPolicy);
        EXPECT_EQ(token.asset_name(), "CUBY");
        EXPECT_EQ(token.asset_name_hex(), "43554259");
        const auto amount = store(uint256_t(3000000));
        EXPECT_EQ(data(token.amount()), amount);
    }

    {
        // also test proto toProto / fromProto
        const Proto::TransactionPlan planProto = Signer::plan(input);
        const auto plan2 = TransactionPlan::fromProto(planProto);
        EXPECT_EQ(plan2.amount, 2000000ul);
        EXPECT_EQ(plan2.change, 2328526ul);
    }
}

TEST(CardanoSigning, ErrorDoPlan) {
    {
        // Common::Proto::Error_missing_input_utxos
        auto input = createSampleInput(2000000, 0, "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5", true);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.error, Common::Proto::Error_missing_input_utxos);
    }
    {
        // Common::Proto::Error_low_balance
        auto input = createSampleInput(9000000, 1, "addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5", true);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.error, Common::Proto::Error_low_balance);

    }
}

TEST(CardanoSigning, PlanForceFee) {
    auto requestedAmount = 6500000ul;
    auto availableAmount = 8000000ul;
    auto input = createSampleInput(requestedAmount);

    {
        auto fee = 170147ul;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, availableAmount - requestedAmount - fee);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
        EXPECT_EQ(plan.error, Common::Proto::OK);
    }
    { // tiny fee
        auto fee = 100ul;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, availableAmount - requestedAmount - fee);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    { // large fee
        auto fee = 1200000ul;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, availableAmount - requestedAmount - fee);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    { // very large fee, larger than possible, truncated
        auto fee = 3000000ul;
        input.mutable_transfer_message()->set_force_fee(fee);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, requestedAmount);
        EXPECT_EQ(plan.fee, 1500000ul);
        EXPECT_EQ(plan.change, 0ul);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
    { // force fee and max amount: fee is used, amount is max, change 0
        auto fee = 160000ul;
        input.mutable_transfer_message()->set_force_fee(fee);
        input.mutable_transfer_message()->set_use_max_amount(true);
        auto signer = Signer(input);
        const auto plan = signer.doPlan();
        EXPECT_EQ(plan.availableAmount, availableAmount);
        EXPECT_EQ(plan.amount, 7840000ul);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, 0ul);
        EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    }
}

TEST(CardanoSigning, PlanMissingPrivateKey) {
    auto input = createSampleInput(7000000, 10, "", true);

    auto signer = Signer(input);
    const auto plan = signer.doPlan();

    EXPECT_EQ(plan.utxos.size(), 2ul);
    EXPECT_EQ(plan.availableAmount, 8000000ul);
    EXPECT_EQ(plan.amount, 7000000ul);
    EXPECT_EQ(plan.fee, 170196ul);
    EXPECT_EQ(plan.change, 829804ul);
    EXPECT_EQ(plan.amount + plan.change + plan.fee, plan.availableAmount);
    EXPECT_EQ(plan.error, Common::Proto::OK);
}

TEST(CardanoSigning, SignTransfer1) {
    const auto input = createSampleInput(7000000);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca96c021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058407cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200ef6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "9b5b15e133cd73ccaa85307d2986aebc846505118a2eb4e6111e6b4b67d1f389");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af0\", 0], [h\"f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767\", 1]], 1: [[h\"01558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5\", 7000000], [h\"01df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b\", 829804]], 2: 170196, 3: 53333333}, {0: [[h\"6d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df290\", h\"7cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200e\"]]}, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 3ul);
    }
}

TEST(CardanoSigning, PlanAndSignTransfer1) {
    uint amount = 6000000;
    auto input = createSampleInput(amount);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 8000000ul);
        EXPECT_EQ(plan.amount, amount);
        EXPECT_EQ(plan.fee, 170196ul);
        EXPECT_EQ(plan.change, 8000000 - amount - 170196);
        ASSERT_EQ(plan.utxos.size(), 2ul);
        EXPECT_EQ(plan.utxos[0].amount, 6500000ul);
        EXPECT_EQ(plan.utxos[1].amount, 1500000ul);

        // perform sign with default plan
        const auto output = signer.sign();

        EXPECT_EQ(output.error(), Common::Proto::OK);

        const auto encoded = data(output.encoded());
        EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a005b8d8082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a001bebac021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058404abc749ffaffcf2f87970e4f1983c5e44b352ee1515b60017fc65e581d42b3a6ed146d5eb35d04a770460b0541a25afd5aedfd027fdaded82686f43454196a0cf6");
        const auto txid = data(output.tx_id());
        EXPECT_EQ(hex(txid), "3852f809245d7000ad0c5ccb1357e5d333b0dd25158924581e4c7049ec68c564");
    }

    // set different plan, with one input only
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(6500000);
    input.mutable_plan()->set_fee(165489);
    input.mutable_plan()->set_change(17191988);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40081825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a005b8d8082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a01065434021a00028671031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058408311a058035d75545a47b844fea401aa9c23e99fe7bc8136b554396eef135d4cd93062c5df38e613185c21bb1c98b881d1e0fd1024d3539b163c8e14d1a6e40df6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "e319c0bfc99cdb79d64f00b7e8fb8bfbf29fa70554c84f101e92b7dfed172448");
}

TEST(CardanoSigning, PlanAndSignMaxAmount) {
    auto input = createSampleInput(7000000);
    input.mutable_transfer_message()->set_use_max_amount(true);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 8000000ul);
        EXPECT_EQ(plan.amount, 8000000 - 167333ul);
        EXPECT_EQ(plan.fee, 167333ul);
        EXPECT_EQ(plan.change, 0ul);
        ASSERT_EQ(plan.utxos.size(), 2ul);
        EXPECT_EQ(plan.utxos[0].amount, 1500000ul);
        EXPECT_EQ(plan.utxos[1].amount, 6500000ul);
    }

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000018182583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a0077845b021a00028da5031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058403e64473e08adc863953c0e9f820b658dda0b8a423d6172fdccff73fcd5559956c9df8ed93ff67405331d368a0c11fd18c69781046384946582e1555e9e8ec70bf6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "ca0f1e12f20c95011da7d686d206a1eb98df94accd74c4df4ef403c5ce836057");
}

TEST(CardanoSigning, SignNegative) {
    { // plan with error
        auto input = createSampleInput(7000000);
        const auto error = Common::Proto::Error_invalid_memo;
        input.mutable_plan()->set_error(error);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), error);
    }
    { // zero requested amount
        auto input = createSampleInput(0);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_zero_amount_requested);
    }
    { // no utxo
        auto input = createSampleInput(7000000, 0);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_missing_input_utxos);
    }
    { // low balance
        auto input = createSampleInput(7000000000);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_low_balance);
    }
    { // missing private key
        auto input = createSampleInput(7000000, 10, "", true);
        auto signer = Signer(input);
        const auto output = signer.sign();
        EXPECT_EQ(output.error(), Common::Proto::Error_missing_private_key);
    }
}

TEST(CardanoSigning, SignTransfer_0db1ea) {
    const auto amount = 1100000ul;

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("81b935447bb994567f041d181b628a0afbcd747d0199c9ff4cd895686bbee8c6");
    utxo1->mutable_out_point()->set_tx_hash(std::string(txHash1.begin(), txHash1.end()));
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(1000000);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("3a9068a273cc2af59b45593b78973841d972d01802abe992c55dbeecdffc561b");
    utxo2->mutable_out_point()->set_tx_hash(std::string(txHash2.begin(), txHash2.end()));
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address(ownAddress1);
    utxo2->set_amount(1800000);

    const auto privateKeyData1 = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData1.data(), privateKeyData1.size());
    input.mutable_transfer_message()->set_to_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(amount);
    auto fee = 170147ul;
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.mutable_transfer_message()->set_force_fee(fee); // use force fee feature here
    input.set_ttl(54675589);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 2800000ul);
        EXPECT_EQ(plan.amount, amount);
        EXPECT_EQ(plan.fee, fee);
        EXPECT_EQ(plan.change, 2800000ul - amount - fee);
        EXPECT_EQ(plan.utxos.size(), 2ul);
    }

    // set plan with specific fee, to match the real transaction
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(2800000);
    input.mutable_plan()->set_fee(fee);
    input.mutable_plan()->set_change(2800000 - amount - fee);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(1);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    // https://cardanoscan.io/transaction/0db1ea8c5c5828bbd027fcef3da02a63b86899db670ad7bb0630cefbe35944fa
    // curl -d '{"txHash":"0db1ea..44fa","txBody":"83a400..06f6"}' -H "Content-Type: application/json" https://<cardano-node>/api/txs/submit
    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a4008282582081b935447bb994567f041d181b628a0afbcd747d0199c9ff4cd895686bbee8c6008258203a9068a273cc2af59b45593b78973841d972d01802abe992c55dbeecdffc561b000182825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a34681a0010c8e082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a001757fd021a000298a3031a03424885a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058406300b52aaff1e26067a3e0a48ae26f4f068765f46f934fabeab872c1d25535fc94893ec72feacd787f0174fbabd8933727d9a2b319b406e7a855843b0c051806f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "0db1ea8c5c5828bbd027fcef3da02a63b86899db670ad7bb0630cefbe35944fa");
}

/// Successfully broadcasted:
/// https://cardanoscan.io/transaction/0203ce2c91f59f169a26e9ef91254639d2b7911afac9c7c0ae64539f88ba46a5
TEST(CardanoSigning, SignTransferFromLegacy) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("8316e5007d61fb90652cabb41141972a38b5bc60954d602cf843476aa3f67f63");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address("Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8");
    utxo1->set_amount(2500000);
    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("e29392c59c903fefb905730587d22cae8bda30bd8d9aeec3eca082ae77675946");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address("Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8");
    utxo2->set_amount(1700000);

    const auto privateKeyData = parse_hex("98f266d1aac660179bc2f456033941238ee6b2beb8ed0f9f34c9902816781f5a9903d1d395d6ab887b65ea5e344ef09b449507c21a75f0ce8c59d0ed1c6764eba7f484aa383806735c46fd769c679ee41f8952952036a6e2338ada940b8a91f4e890ca4eb6bec44bf751b5a843174534af64d6ad1f44e0613db78a7018781f5aa151d2997f52059466b715d8eefab30a78b874ae6ef4931fa58bb21ef8ce2423d46f19d0fbf75afb0b9a24e31d533f4fd74cee3b56e162568e8defe37123afc4");
    {
        const auto privKey = PrivateKey(privateKeyData);
        const auto pubKey = privKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        const auto addr = AddressV2(pubKey);
        EXPECT_EQ(addr.string(), "Ae2tdPwUPEZ6vkqxSjJxaQYmDxHf5DTnxtZ67pFLJGTb9LTnCGkDP6ca3f8");
    }
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q90uh2eawrdc9vaemftgd50l28yrh9lqxtjjh4z6dnn0u7ggasexxdyyk9f05atygnjlccsjsggtc87hhqjna32fpv5qeq96ls");
    input.mutable_transfer_message()->set_change_address("addr1qx55ymlqemndq8gluv40v58pu76a2tp4mzjnyx8n6zrp2vtzrs43a0057y0edkn8lh9su8vh5lnhs4npv6l9tuvncv8swc7t08");
    input.mutable_transfer_message()->set_amount(3000000);
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(190000000);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);
    EXPECT_EQ(hex(output.encoded()), "83a400828258208316e5007d61fb90652cabb41141972a38b5bc60954d602cf843476aa3f67f6300825820e29392c59c903fefb905730587d22cae8bda30bd8d9aeec3eca082ae77675946000182825839015fcbab3d70db82b3b9da5686d1ff51c83b97e032e52bd45a6ce6fe7908ec32633484b152fa756444e5fc62128210bc1fd7b8253ec5490b281a002dc6c082583901a9426fe0cee6d01d1fe32af650e1e7b5d52c35d8a53218f3d0861531621c2b1ebdf4f11f96da67fdcb0e1d97a7e778566166be55f193c30f1a000f9ec1021a0002b0bf031a0b532b80a20081825820d163c8c4f0be7c22cd3a1152abb013c855ea614b92201497a568c5d93ceeb41e58406a23ab9267867fbf021c1cb2232bc83d2cdd663d651d22d59b6cddbca5cb106d4db99da50672f69a2309ca8a329a3f9576438afe4538b013de4591a6dfcd4d090281845820d163c8c4f0be7c22cd3a1152abb013c855ea614b92201497a568c5d93ceeb41e58406a23ab9267867fbf021c1cb2232bc83d2cdd663d651d22d59b6cddbca5cb106d4db99da50672f69a2309ca8a329a3f9576438afe4538b013de4591a6dfcd4d095820a7f484aa383806735c46fd769c679ee41f8952952036a6e2338ada940b8a91f441a0f6");
    EXPECT_EQ(hex(data(output.tx_id())), "0203ce2c91f59f169a26e9ef91254639d2b7911afac9c7c0ae64539f88ba46a5");
}

TEST(CardanoSigning, SignTransferToLegacy) {
    const auto toAddressLegacy = "DdzFFzCqrhssmYoG5Eca1bKZFdGS8d6iag1mU4wbLeYcSPVvBNF2wRG8yhjzQqErbg63N6KJA4DHqha113tjKDpGEwS5x1dT2KfLSbSJ";
    EXPECT_FALSE(AddressV3::isValid(toAddressLegacy)); // not V3
    EXPECT_TRUE(AddressV3::isValidLegacy(toAddressLegacy));

    const auto input = createSampleInput(7000000, 10, toAddressLegacy);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);
    EXPECT_EQ(hex(output.encoded()), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282584c82d818584283581c6aebd89cf88271c3ee76339930d8956b03f018b2f4871522f88eb8f9a101581e581c692a37dae3bc63dfc3e1463f12011f26655ab1d1e0f4ed4b8fc63708001ad8a9555b1a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca627021a00029c19031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840db9becdc733f4c08c0e7abc29b5cc6469f9339d32f565df8bf77455439ae1f949facc9b831754e74d3fbb42e99647eedd6c28de1461d18c315485f5d24b5b90af6");
    EXPECT_EQ(hex(data(output.tx_id())), "f9b713e9987ec1377ac223f50d63c7a5e155915302de43f40d7b2627accabf69");
}

TEST(CardanoSigning, SignTransferToInvalid) {
    const auto input = createSampleInput(7000000, 10, "__INVALID_ADDRESS__");

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_address);
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(CardanoSigning, SignTransferToken) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(8051373);
    // some token, to be preserved
    auto* token3 = utxo1->add_token_amount();
    token3->set_policy_id(sundaeTokenPolicy);
    token3->set_asset_name("CUBY");
    const auto tokenAmount3 = store(uint256_t(3000000));
    token3->set_amount(tokenAmount3.data(), tokenAmount3.size());

    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e767");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(2);
    utxo2->set_address(ownAddress1);
    utxo2->set_amount(2000000);
    // some SUNDAE token, to be transferred
    auto* token1 = utxo2->add_token_amount();
    token1->set_policy_id(sundaeTokenPolicy);
    token1->set_asset_name_hex("53554e444145");
    const auto tokenAmount1 = store(uint256_t(80996569));
    token1->set_amount(tokenAmount1.data(), tokenAmount1.size());
    // some other token, to be preserved
    auto* token2 = utxo2->add_token_amount();
    token2->set_policy_id(sundaeTokenPolicy);
    token2->set_asset_name("CUBY");
    // This should be ignored!
    token2->set_asset_name_hex("00");
    const auto tokenAmount2 = store(uint256_t(2000000));
    token2->set_amount(tokenAmount2.data(), tokenAmount2.size());

    const auto privateKeyData = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    input.mutable_transfer_message()->set_change_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq");
    input.mutable_transfer_message()->set_amount(1500000);
    auto* toToken = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    toToken->set_policy_id(sundaeTokenPolicy);
    toToken->set_asset_name_hex("53554e444145");
    const auto toTokenAmount = store(uint256_t(20000000));
    toToken->set_amount(toTokenAmount.data(), toTokenAmount.size());
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(53333333);

    { // check min ADA amount, set it
        const auto bundleProtoData = data(input.transfer_message().token_amount().SerializeAsString());
        const auto minAdaAmount = TWCardanoMinAdaAmount(&bundleProtoData);
        EXPECT_EQ(minAdaAmount, 1444443ul);
        input.mutable_transfer_message()->set_amount(minAdaAmount);
    }

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 10051373ul);
        EXPECT_EQ(plan.amount, 1444443ul);
        EXPECT_EQ(plan.fee, 174601ul);
        EXPECT_EQ(plan.change, 8432329ul);
        EXPECT_EQ(plan.utxos.size(), 2ul);
        EXPECT_EQ(plan.availableTokens.size(), 2ul);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_CUBY"), 5000000);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 80996569);
        EXPECT_EQ(plan.outputTokens.size(), 1ul);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_CUBY"), 0);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.changeTokens.size(), 2ul);
        EXPECT_EQ(plan.changeTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_CUBY"), 5000000);
        EXPECT_EQ(plan.changeTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 60996569);
    }

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76702018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5821a00160a5ba1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a01312d00825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468821a0080aac9a1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a244435542591a004c4b404653554e4441451a03a2bbd9021a0002aa09031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840d90dcfbd190cbe59c42094e59eeb49b3de9d80a85b786cc311f932c5c9302d1c8c6c577b22aa70ff7955c139c700ea918f8cb425c3ba43a27980e1d238e4e908f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "201c537693b005b64a0f0528e366ec67a84be0119ed4363b547f141f2a7770c2");

    {
        // also test proto toProto / fromProto
        const Proto::TransactionPlan planProto = Signer::plan(input);
        const auto plan2 = TransactionPlan::fromProto(planProto);
        EXPECT_EQ(plan2.amount, 1444443ul);
        EXPECT_EQ(plan2.change, 8432329ul);
    }
}

TEST(CardanoSigning, SignTransferToken_1dd248) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("f2d2b11c8c07c5c646f5b5af20fddf2f0a174743c6a1b13cca27e28a6ca34710");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(1500000);
    // some token
    auto* token3 = utxo1->add_token_amount();
    token3->set_policy_id(sundaeTokenPolicy);
    token3->set_asset_name_hex("53554e444145");
    const auto tokenAmount3 = store(uint256_t(20000000));
    token3->set_amount(tokenAmount3.data(), tokenAmount3.size());

    auto* utxo2 = input.add_utxos();
    const auto txHash2 = parse_hex("6975fcf7bbca745c85f50777f956219868fd9cad14ba496fed1371252e8df60f");
    utxo2->mutable_out_point()->set_tx_hash(txHash2.data(), txHash2.size());
    utxo2->mutable_out_point()->set_output_index(0);
    utxo2->set_address(ownAddress1);
    utxo2->set_amount(10258890);

    const auto privateKeyData = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1qxxe304qg9py8hyyqu8evfj4wln7dnms943wsugpdzzsxnkvvjljtzuwxvx0pnwelkcruy95ujkq3aw6rl0vvg32x35qc92xkq"); // Test
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(1600000);
    auto* toToken = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    toToken->set_policy_id(sundaeTokenPolicy);
    toToken->set_asset_name_hex("53554e444145");
    const auto toTokenAmount = store(uint256_t(11000000));
    toToken->set_amount(toTokenAmount.data(), toTokenAmount.size());
    input.mutable_transfer_message()->set_use_max_amount(false);
    input.set_ttl(61232158);

    { // check min ADA amount
        const auto bundleProtoData = data(input.transfer_message().token_amount().SerializeAsString());
        EXPECT_EQ(TWCardanoMinAdaAmount(&bundleProtoData), 1444443ul);
        EXPECT_GT(input.transfer_message().amount(), TWCardanoMinAdaAmount(&bundleProtoData));
    }

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 11758890ul);
        EXPECT_EQ(plan.amount, 11758890 - 9984729 - 174161ul);
        EXPECT_EQ(plan.fee, 174161ul);
        EXPECT_EQ(plan.change, 9984729ul);
        EXPECT_EQ(plan.utxos.size(), 2ul);
        EXPECT_EQ(plan.availableTokens.size(), 1ul);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.outputTokens.size(), 1ul);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 11000000);
        EXPECT_EQ(plan.changeTokens.size(), 1ul);
        EXPECT_EQ(plan.changeTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 9000000);
    }

    // set plan with specific fee, to match the real transaction
    input.mutable_plan()->set_available_amount(11758890);
    input.mutable_plan()->set_amount(1600000);
    input.mutable_plan()->set_fee(174102);
    input.mutable_plan()->set_change(9984788);
    *(input.mutable_plan()->add_available_tokens()) = input.utxos(0).token_amount(0);
    *(input.mutable_plan()->add_output_tokens()) = input.utxos(0).token_amount(0);
    input.mutable_plan()->mutable_output_tokens(0)->set_amount(toTokenAmount.data(), toTokenAmount.size());
    *(input.mutable_plan()->add_change_tokens()) = input.utxos(0).token_amount(0);
    const auto changeTokenAmount = store(uint256_t(9000000));
    input.mutable_plan()->mutable_change_tokens(0)->set_amount(changeTokenAmount.data(), changeTokenAmount.size());
    *(input.mutable_plan()->add_utxos()) = input.utxos(1);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    // https://cardanoscan.io/transaction/1dd24872d93d3b5091b98e19b9f920cd0c4369e4c5ca178e898152c52f00c162
    // curl -d '{"txHash":"1dd248..c162","txBody":"83a400..08f6"}' -H "Content-Type: application/json" https://<cardano-node>/api/txs/submit
    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a400828258206975fcf7bbca745c85f50777f956219868fd9cad14ba496fed1371252e8df60f00825820f2d2b11c8c07c5c646f5b5af20fddf2f0a174743c6a1b13cca27e28a6ca34710000182825839018d98bea0414243dc84070f96265577e7e6cf702d62e871016885034ecc64bf258b8e330cf0cdd9fdb03e10b4e4ac08f5da1fdec6222a3468821a00186a00a1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a00a7d8c082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b821a00985b14a1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a00895440021a0002a816031a03a6541ea100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840c8cdee32bfd584f55cf334b4ec6f734635144736d48f882e647a7a6283f230bc5a67d4dd66a9e523e0c29c812ed1e3589febbcf96547a1fc6d061a7ccfb81308f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "1dd24872d93d3b5091b98e19b9f920cd0c4369e4c5ca178e898152c52f00c162");
}

TEST(CardanoSigning, SignTransferTokenMaxAmount_620b71) {
    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("46964521ad00d9b3f3d41f77c07e1b3093848048dbdf2d95cf900e15cdac0d7f");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress1);
    utxo1->set_amount(2170871);
    // some token
    auto* token1 = utxo1->add_token_amount();
    token1->set_policy_id(sundaeTokenPolicy);
    token1->set_asset_name_hex("53554e444145");
    const auto tokenAmount1 = store(uint256_t(20000000));
    token1->set_amount(tokenAmount1.data(), tokenAmount1.size());

    const auto privateKeyData = parse_hex(privateKeyTest1);
    input.add_private_key(privateKeyData.data(), privateKeyData.size());
    input.mutable_transfer_message()->set_to_address("addr1q92cmkgzv9h4e5q7mnrzsuxtgayvg4qr7y3gyx97ukmz3dfx7r9fu73vqn25377ke6r0xk97zw07dqr9y5myxlgadl2s0dgke5");
    input.mutable_transfer_message()->set_change_address(ownAddress1);
    input.mutable_transfer_message()->set_amount(666); // doesn't matter, max is used
    auto* toToken = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    toToken->set_policy_id(sundaeTokenPolicy);
    toToken->set_asset_name_hex("53554e444145");
    const auto toTokenAmount = store(uint256_t(666)); // doesn't matter, max is used
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(61085916);

    {
        // run plan and check result
        auto signer = Signer(input);
        const auto plan = signer.doPlan();

        EXPECT_EQ(plan.availableAmount, 2170871ul);
        EXPECT_EQ(plan.amount, 2170871 - 167730ul);
        EXPECT_EQ(plan.fee, 167730ul);
        EXPECT_EQ(plan.change, 0ul);
        EXPECT_EQ(plan.utxos.size(), 1ul);
        EXPECT_EQ(plan.availableTokens.size(), 1ul);
        EXPECT_EQ(plan.availableTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.outputTokens.size(), 1ul);
        EXPECT_EQ(plan.outputTokens.getAmount("9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77_SUNDAE"), 20000000);
        EXPECT_EQ(plan.changeTokens.size(), 0ul);
    }

    // set plan with specific fee, to match the real transaction
    input.mutable_plan()->set_available_amount(2170871);
    input.mutable_plan()->set_amount(1998526);
    input.mutable_plan()->set_fee(172345);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_available_tokens()) = input.utxos(0).token_amount(0);
    *(input.mutable_plan()->add_output_tokens()) = input.utxos(0).token_amount(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    // https://cardanoscan.io/transaction/620b719338efb419b0e1417bfbe01fc94a62d5669a4b8cbbf4e32ecc1ca3b872
    // curl -d '{"txHash":"620b71..b872","txBody":"83a400..08f6"}' -H "Content-Type: application/json" https://<cardano-node>/api/txs/submit
    EXPECT_EQ(output.error(), Common::Proto::OK);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a4008182582046964521ad00d9b3f3d41f77c07e1b3093848048dbdf2d95cf900e15cdac0d7f00018182583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd5821a001e7ebea1581c9a9693a9a37912a5097918f97918d15240c92ab729a0b7c4aa144d77a14653554e4441451a01312d00021a0002a139031a03a418dca100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df2905840e1d1565cd747b20b0f10a92f068f3d5faebdee92b4b4a4b96ce14736d975e17d1446f7f51e64997a0bb38e0151dc738468161d574d6cfcd8040e4455ff46bc08f6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "620b719338efb419b0e1417bfbe01fc94a62d5669a4b8cbbf4e32ecc1ca3b872");
}

TEST(CardanoSigning, SignTransferTwoTokens) {
    auto input = createSampleInput(7000000);
    input.mutable_transfer_message()->set_amount(1500000);
    auto* token1 = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    token1->set_policy_id(sundaeTokenPolicy);
    token1->set_asset_name_hex("53554e444145");
    const auto tokenAmount1 = store(uint256_t(40000000));
    token1->set_amount(tokenAmount1.data(), tokenAmount1.size());
    auto* token2 = input.mutable_transfer_message()->mutable_token_amount()->add_token();
    token2->set_policy_id(sundaeTokenPolicy);
    token2->set_asset_name_hex("43554259");
    const auto tokenAmount2 = store(uint256_t(2000000));
    token2->set_amount(tokenAmount2.data(), tokenAmount2.size());

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::Error_invalid_requested_token_amount);
    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(output.encoded()), "");
}

TEST(CardanoSigning, SignMessageWithKey) {
    // test case from cardano-crypto.js

    const auto privateKey = PrivateKey(parse_hex(
        "d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"
        "d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"
        "69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "1111111111111111111111111111111111111111111111111111111111111111"
        "1111111111111111111111111111111111111111111111111111111111111111"));

    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
    EXPECT_EQ(hex(publicKey.bytes),
              "e6f04522f875c1563682ca876ddb04c2e2e3ae718e3ff9f11c03dd9f9dccf698"
              "69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000"
              "857eed804ff087b97f87848f6493e87257a8c5203cb9f422f6e7a7d8a4d299f3"
              "1111111111111111111111111111111111111111111111111111111111111111");

    const auto sampleMessageStr = "Hello world";
    const auto sampleMessage = data(sampleMessageStr);

    const auto signature = privateKey.sign(sampleMessage, TWCurveED25519ExtendedCardano);

    const auto sampleRightSignature = "1096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0e";
    EXPECT_EQ(hex(signature), sampleRightSignature);
}

TEST(CardanoSigning, AnySignTransfer1) {
    const auto input = createSampleInput(7000000);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeCardano);

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto encoded = data(output.encoded());
    EXPECT_EQ(hex(encoded), "83a40082825820554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af000825820f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76701018282583901558dd902616f5cd01edcc62870cb4748c45403f1228218bee5b628b526f0ca9e7a2c04d548fbd6ce86f358be139fe680652536437d1d6fd51a006acfc082583901df58ee97ce7a46cd8bdeec4e5f3a03297eb197825ed5681191110804df22424b6880b39e4bac8c58de9fe6d23d79aaf44756389d827aa09b1a000ca96c021a000298d4031a032dcd55a100818258206d8a0b425bd2ec9692af39b1c0cf0e51caa07a603550e22f54091e872c7df29058407cf591599852b5f5e007fdc241062405c47e519266c0d884b0767c1d4f5eacce00db035998e53ed10ca4ba5ce4aac8693798089717ce6cf4415f345cc764200ef6");
    const auto txid = data(output.tx_id());
    EXPECT_EQ(hex(txid), "9b5b15e133cd73ccaa85307d2986aebc846505118a2eb4e6111e6b4b67d1f389");
}

TEST(CardanoSigning, AnyPlan1) {
    const auto input = createSampleInput(7000000);

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeCardano);

    EXPECT_EQ(plan.error(), Common::Proto::OK);
    EXPECT_EQ(plan.amount(), 7000000ul);
    EXPECT_EQ(plan.available_amount(), 8000000ul);
    EXPECT_EQ(plan.fee(), 170196ul);
    EXPECT_EQ(plan.change(), 829804ul);
    ASSERT_EQ(plan.utxos_size(), 2);
    EXPECT_EQ(plan.utxos(0).amount(), 6500000ul);
    EXPECT_EQ(plan.utxos(1).amount(), 1500000ul);

    EXPECT_EQ(hex(plan.SerializeAsString()), "0880a4e80310c09fab0318d4b10a20ecd2324292010a220a20554f2fd942a23d06835d26bbd78f0106fa94c8a551114a0bef81927f66467af01267616464723171383034336d356865656179646e76746d6d6b7975686536717635686176766873663064323671336a7967737370786c796670796b3679716b77307968747976747230666c656b6a3834753634617a38326375666d716e36357a6473796c7a6b323318a0dd8c034293010a240a20f074134aabbfb13b8aec7cf5465b1e5a862bde5cb88532cc7e64619179b3e76710011267616464723171383034336d356865656179646e76746d6d6b7975686536717635686176766873663064323671336a7967737370786c796670796b3679716b77307968747976747230666c656b6a3834753634617a38326375666d716e36357a6473796c7a6b323318e0c65b");

    {
        // also test fromProto
        const auto plan2 = TransactionPlan::fromProto(plan);
        EXPECT_EQ(plan2.amount, plan.amount());
        EXPECT_EQ(plan2.change, plan.change());
    }
}

} // namespace TW::Cardano::tests
