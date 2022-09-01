// Copyright © 2022-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Lightning/LNInvoice.h"

#include "HexCoding.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

using namespace TW::Lightning;
using namespace TW;

// Test vectors from the standard, https://github.com/lightning/bolts/blob/master/11-payment-encoding.md
const auto specTest1 = "lnbc1pvjluezsp5zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zygspp5qqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqypqdpl2pkx2ctnv5sxxmmwwd5kgetjypeh2ursdae8g6twvus8g6rfwvs8qun0dfjkxaq9qrsgq357wnc5r2ueh7ck6q93dj32dlqnls087fxdwk8qakdyafkq3yap9us6v52vjjsrvywa6rt52cm9r9zqt8r2t7mlcwspyetp5h2tztugp9lfyql";
const auto specTest2 = "lnbc2500u1pvjluezsp5zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zygspp5qqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqypqdq5xysxxatsyp3k7enxv4jsxqzpu9qrsgquk0rl77nj30yxdy8j9vdx85fkpmdla2087ne0xh8nhedh8w27kyke0lp53ut353s06fv3qfegext0eh0ymjpf39tuven09sam30g4vgpfna3rh";
const auto specTestPrivkey = "e126f68f7eafcc8b74f54d269fe206be715000f94dac067d1c04a8ca3b2db734";
const auto specTestPubkey = "03e7156ae33b0a208d0744199163177e909e80176e55d97a2f221ede0f934dd9ad";
const auto dummyPrivkey = "1111111111111111111111111111111111111111111111111111111111111111";
const auto dummyPubkey = "034f355bdcb7cc0af728ef3cceb9615d90684bb5b2ca5f859ab0f0b704075871aa";

const std::string linv22 = "lntb1230n1p3072yfdqud3jxktt5w46x7unfv9kz6mn0v3jsnp4qg09s25g00v56ey2jfn3g04kqpzf4r2e5rdsv5m5pvfhspn6d5xwusp59553gh4vqdflsfznxjcywgu3kelxa4ryfpgn7f640h7q2y63vflspp5hpqe4wuc2smftu8k3qqcm9z489vdz6zh3f0nc8jqt27qukkkp5lq9qyysgqxqy7sjqcqpc5c5fgfkegvezl0ykf4j88wscpd6p5n3h658gmjln9zu9ks9xhajpp0mzkqxh8k0gcwkax3kpcn0wxzsvrx43pl7qmqqtg3ls0xesq7qqcz2y7r";
const std::string linv23WithHint = "lnbc10010n1p39j00rnp4qg09s25g00v56ey2jfn3g04kqpzf4r2e5rdsv5m5pvfhspn6d5xwudqqsp5uxw3y3mpf2xky62qrr4a543rlrwa6dvdds305mcswk0t3578uc9qpp58dlferm3cr3lwwwjjstnfh529sk94keghm9yyk9heuusn2338snsrzjqw8c7yfutqqy3kz8662fxutjvef7q2ujsxtt45csu0k688lkzu3ld28pfhz4wtgyryqqqqryqqqqthqqpy9qypqsqxqyz5vqcqzzgrdfcgf6ggahwyzml044tl9m2t5qvulwzl658sa8uafkd95xk0zrs9zznu6tqtmt8zdmjwr83ahhvpqwjv3729hg42t55nze65qzx65gquk7k79";
const auto privkey22 = "13fcaabaf9e71ffaf915e242ec58a743d55f102cf836968e5bd4881135e0c52c";
const auto pubkey22 = "021e582a887bd94d648a9267143eb600449a8d59a0db0653740b1378067a6d0cee";

TEST(LNInvoice, Decode1) {
    const auto inv = InvoiceDecoder::decodeInvoice(specTest1);
    EXPECT_EQ(inv.network, Mainnet);
    EXPECT_FALSE(inv.amountPresent);
    EXPECT_EQ(inv.unparsedAmount, "");
    EXPECT_FALSE(inv.mismatchNodeidSignature);
    EXPECT_EQ(hex(inv.signature), "8d3ce9e28357337f62da0162d9454df827f83cfe499aeb1c1db349d4d81127425e434ca29929406c23bba1ae8ac6ca32880b38d4bf6ff874024cac34ba9625f101");
    EXPECT_EQ(inv.timestamp, 1496314658ull);
    EXPECT_EQ(hex(inv.nodeId), "");
    EXPECT_EQ(hex(inv.recoveredPubkey), "");
    EXPECT_EQ(hex(inv.paymentHash), "0001020304050607080900010203040506070809000102030405060708090102");
    EXPECT_EQ(hex(inv.secret), "1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_EQ(inv.description, "Please consider supporting this project");
    EXPECT_EQ(hex(inv.unparsedFeatures), "82");
    EXPECT_EQ(hex(inv.unparsedExpiry), "");
    EXPECT_EQ(inv.minFinalCltvExpiry, 0);

    EXPECT_TRUE(InvoiceDecoder::verifySignature(specTest1, parse_hex(specTestPubkey)));
}

TEST(LNInvoice, Decode22) {
    const auto inv = InvoiceDecoder::decodeInvoice(linv22);
    EXPECT_EQ(inv.network, Testnet);
    EXPECT_TRUE(inv.amountPresent);
    EXPECT_EQ(inv.unparsedAmount, "1230n");
    EXPECT_FALSE(inv.mismatchNodeidSignature);
    EXPECT_EQ(hex(inv.signature), "a6289426d943322fbc964d6473ba180b741a4e37d50e8dcbf328b85b40a6bf6410bf62b00d73d9e8c3add346c1c4dee30a0c19ab10ffc0d800b447f079b3007800");
    EXPECT_EQ(inv.timestamp, 1660889225ull);
    EXPECT_EQ(hex(inv.nodeId), pubkey22);
    EXPECT_EQ(hex(inv.recoveredPubkey), pubkey22);
    EXPECT_EQ(hex(inv.paymentHash), "b8419abb98543695f0f688018d94553958d168578a5f3c1e405abc0e5ad60d3e");
    EXPECT_EQ(hex(inv.secret), "2d29145eac0353f8245334b0472391b67e6ed46448513f27557dfc051351627f");
    EXPECT_EQ(inv.description, "ldk-tutorial-node");
    EXPECT_EQ(hex(inv.unparsedFeatures), "2410");
    EXPECT_EQ(hex(inv.unparsedExpiry), "f424");
    EXPECT_EQ(inv.minFinalCltvExpiry, 24);

    EXPECT_TRUE(InvoiceDecoder::verifySignature(linv22));
}

TEST(LNInvoice, Decode23WithHint) {
    const auto inv = InvoiceDecoder::decodeInvoice(linv23WithHint);
    EXPECT_EQ(inv.network, Mainnet);
    EXPECT_TRUE(inv.amountPresent);
    EXPECT_EQ(inv.unparsedAmount, "10010n");
    EXPECT_FALSE(inv.mismatchNodeidSignature);
    EXPECT_EQ(hex(inv.signature), "1b53842748476ee20b7f7d6abf976a5d00ce7dc2fea87874fcea6cd2d0d67887028853e69605ed671377270cf1edeec081d2647ca2dd1552e9498b3aa0046d5100");
    EXPECT_EQ(inv.timestamp, 1650015715ull);
    EXPECT_EQ(hex(inv.nodeId), pubkey22);
    EXPECT_EQ(hex(inv.recoveredPubkey), pubkey22);
    EXPECT_EQ(hex(inv.paymentHash), "3b7e9c8f71c0e3f739d2941734de8a2c2c5adb28beca4258b7cf3909aa313c27");
    EXPECT_EQ(hex(inv.secret), "e19d1247614a8d62694018ebda5623f8dddd358d6c22fa6f10759eb8d3c7e60a");
    ASSERT_EQ(inv.routing.size(), 1ul); // 1 routing hint
    EXPECT_EQ(hex(inv.routing[0].pubkey), "038f8f113c580048d847d6949371726653e02b928196bad310e3eda39ff61723f6");
    EXPECT_EQ(hex(inv.routing[0].shortChannelId), "a8e14dc5572d0419");
    EXPECT_EQ(hex(inv.routing[0].unparsedFeeBaseMsat), "00000064");
    EXPECT_EQ(hex(inv.routing[0].unparsedFeePPM), "000005dc");
    EXPECT_EQ(hex(inv.routing[0].unparsedCltvExpiryDelta), "0009");
    EXPECT_EQ(hex(inv.unparsedFeatures), "0820");
    EXPECT_EQ(hex(inv.unparsedExpiry), "1518");
    EXPECT_EQ(inv.minFinalCltvExpiry, 72);
}

TEST(LNInvoice, Encode1) {
    // Note that this invoice contains no nodeId
    const auto inv1 = InvoiceDecoder::decodeInvoice(specTest1);
    EXPECT_EQ(hex(inv1.signature), "8d3ce9e28357337f62da0162d9454df827f83cfe499aeb1c1db349d4d81127425e434ca29929406c23bba1ae8ac6ca32880b38d4bf6ff874024cac34ba9625f101");

    { // Re-encode, no re-sign, signature may get invalid because of reordering
        const auto enc1 = InvoiceDecoder::encodeInvoice(inv1);
        EXPECT_EQ(enc1, "lnbc1pvjluezpp5qqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqypq9qzsgdpl2pkx2ctnv5sxxmmwwd5kgetjypeh2ursdae8g6twvus8g6rfwvs8qun0dfjkxaqsp5zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zygs357wnc5r2ueh7ck6q93dj32dlqnls087fxdwk8qakdyafkq3yap9us6v52vjjsrvywa6rt52cm9r9zqt8r2t7mlcwspyetp5h2tztugpu52vvs");
        const auto inv2 = InvoiceDecoder::decodeInvoice(enc1);
        EXPECT_EQ(hex(inv2.signature), "8d3ce9e28357337f62da0162d9454df827f83cfe499aeb1c1db349d4d81127425e434ca29929406c23bba1ae8ac6ca32880b38d4bf6ff874024cac34ba9625f101");
    }

    { // Re-encode with re-sign, signature must be valid
        const auto enc3 = InvoiceDecoder::encodeInvoice(inv1, parse_hex(specTestPrivkey));
        EXPECT_EQ(enc3, "lnbc1pvjluezpp5qqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqypq9qzsgdpl2pkx2ctnv5sxxmmwwd5kgetjypeh2ursdae8g6twvus8g6rfwvs8qun0dfjkxaqsp5zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zygsu95vptkrw797t2ayyqads8xmcnem65c2l7248khxyvl48akt2393fde7aavfns3qmthhevx8lt8v4nmfutxnsm9f5lcmxcup66fsyaspduzyr8");
        EXPECT_TRUE(InvoiceDecoder::verifySignature(enc3, parse_hex(specTestPubkey)));
        const auto inv3 = InvoiceDecoder::decodeInvoice(enc3);
        EXPECT_FALSE(inv3.mismatchNodeidSignature);
        EXPECT_EQ(hex(inv3.signature), "e168c0aec3778be5aba4203ad81cdbc4f3bd530aff9553dae6233f53f6cb544b14b73eef5899c220daef7cb0c7facecacf69e2cd386ca9a7f1b36381d693027601");
    }

    { // Re-encode with re-sign with another key, signature must be valid
        const auto enc4 = InvoiceDecoder::encodeInvoice(inv1, parse_hex(dummyPrivkey));
        EXPECT_EQ(enc4, "lnbc1pvjluezpp5qqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqypq9qzsgdpl2pkx2ctnv5sxxmmwwd5kgetjypeh2ursdae8g6twvus8g6rfwvs8qun0dfjkxaqsp5zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zygswtnx62a4atq6g6kzd77st4ftwve567typauazhtpq5nnjt7lykty6wv9r5k6puxjtx8ky50fuklrqagaakk5w6gtl3k8ptm30vdgnsqqsryzl8");
        EXPECT_TRUE(InvoiceDecoder::verifySignature(enc4, parse_hex(dummyPubkey)));
        const auto inv4 = InvoiceDecoder::decodeInvoice(enc4);
        EXPECT_FALSE(inv4.mismatchNodeidSignature);
        EXPECT_EQ(hex(inv4.signature), "72e66d2bb5eac1a46ac26fbd05d52b73334d79640f79d15d610527392fdf25964d39851d2da0f0d2598f6251e9e5be30751dedad47690bfc6c70af717b1a89c000");
    }
}

TEST(LNInvoice, Encode22) {
    EXPECT_EQ(linv22.length(), 342ul);
    const auto inv1 = InvoiceDecoder::decodeInvoice(linv22);

    const auto enc1 = InvoiceDecoder::encodeInvoice(inv1, parse_hex(privkey22));

    // lenght should be the same, ordering may differ
    EXPECT_EQ(enc1.length(), linv22.length());
    EXPECT_EQ(enc1, "lntb1230n1p3072yfpp5hpqe4wuc2smftu8k3qqcm9z489vdz6zh3f0nc8jqt27qukkkp5lq9qyysgqxqy7sjqdqud3jxktt5w46x7unfv9kz6mn0v3jssp59553gh4vqdflsfznxjcywgu3kelxa4ryfpgn7f640h7q2y63vflsnp4qg09s25g00v56ey2jfn3g04kqpzf4r2e5rdsv5m5pvfhspn6d5xwucqpcwfq6gctg5dwj86ns7uplj6vchfh66wzxvrytzq5n68gept33s8h8yzqvfdse48v3u902kq87wv2edqqyquhtam5y384fvqpeceynk8cq2x44q0");

    const auto inv = InvoiceDecoder::decodeInvoice(enc1);
    EXPECT_EQ(inv.network, Testnet);
    EXPECT_TRUE(inv.amountPresent);
    EXPECT_EQ(inv.unparsedAmount, "1230n");
    EXPECT_EQ(hex(inv.signature), "7241a46168a35d23ea70f703f96998ba6fad384660c8b10293d1d190ae3181ee72080c4b619a9d91e15eab00fe7315968004072ebeee8489ea960039c6493b1f00");
    EXPECT_FALSE(inv.mismatchNodeidSignature);
    EXPECT_EQ(inv.timestamp, 1660889225ull);
    EXPECT_EQ(hex(inv.nodeId), pubkey22);
    EXPECT_EQ(hex(inv.recoveredPubkey), pubkey22);
    EXPECT_EQ(hex(inv.paymentHash), "b8419abb98543695f0f688018d94553958d168578a5f3c1e405abc0e5ad60d3e");
    EXPECT_EQ(hex(inv.secret), "2d29145eac0353f8245334b0472391b67e6ed46448513f27557dfc051351627f");
    EXPECT_EQ(inv.description, "ldk-tutorial-node");
    EXPECT_EQ(hex(inv.unparsedFeatures), "2410");
    EXPECT_EQ(hex(inv.unparsedExpiry), "f424");
    EXPECT_EQ(inv.minFinalCltvExpiry, 24);

    // 2nd round, should give same result as first, even without resign
    const auto inv2 = InvoiceDecoder::decodeInvoice(enc1);
    const auto enc2 = InvoiceDecoder::encodeInvoice(inv2);
    EXPECT_EQ(enc2.length(), linv22.length());
    EXPECT_EQ(enc2, enc1);
}

TEST(LNInvoice, Encode22WrongSig) {
    auto inv1 = InvoiceDecoder::decodeInvoice(linv22);
    EXPECT_EQ(hex(inv1.nodeId), pubkey22);
    EXPECT_EQ(hex(inv1.recoveredPubkey), pubkey22);
    EXPECT_FALSE(inv1.mismatchNodeidSignature);
    EXPECT_EQ(hex(inv1.signature), "a6289426d943322fbc964d6473ba180b741a4e37d50e8dcbf328b85b40a6bf6410bf62b00d73d9e8c3add346c1c4dee30a0c19ab10ffc0d800b447f079b3007800");
    EXPECT_TRUE(InvoiceDecoder::verifySignature(linv22, parse_hex(pubkey22)));

    // Change nodeId, re-encode, but no re-sign: signature will be invalid
    inv1.nodeId = parse_hex(dummyPubkey);
    const auto invstr2 = InvoiceDecoder::encodeInvoice(inv1);
    EXPECT_EQ(invstr2, "lntb1230n1p3072yfpp5hpqe4wuc2smftu8k3qqcm9z489vdz6zh3f0nc8jqt27qukkkp5lq9qyysgqxqy7sjqdqud3jxktt5w46x7unfv9kz6mn0v3jssp59553gh4vqdflsfznxjcywgu3kelxa4ryfpgn7f640h7q2y63vflsnp4qd8n2k7uklxq4aegau7vawtptkgxsja4kt99lpv6krctwpq8tpc65cqpc5c5fgfkegvezl0ykf4j88wscpd6p5n3h658gmjln9zu9ks9xhajpp0mzkqxh8k0gcwkax3kpcn0wxzsvrx43pl7qmqqtg3ls0xesq7qqm6u23w");

    // decode
    const auto inv2 = InvoiceDecoder::decodeInvoice(invstr2);
    EXPECT_EQ(hex(inv2.nodeId), dummyPubkey);
    EXPECT_EQ(hex(inv2.recoveredPubkey), "");
    EXPECT_TRUE(inv2.mismatchNodeidSignature);
    EXPECT_EQ(hex(inv2.signature), "a6289426d943322fbc964d6473ba180b741a4e37d50e8dcbf328b85b40a6bf6410bf62b00d73d9e8c3add346c1c4dee30a0c19ab10ffc0d800b447f079b3007800");
    EXPECT_FALSE(InvoiceDecoder::verifySignature(invstr2, parse_hex(pubkey22)));
}

TEST(LNInvoice, Encode23) {
    EXPECT_EQ(linv23WithHint.length(), 401ul);
    const auto inv1 = InvoiceDecoder::decodeInvoice(linv23WithHint);

    const auto enc1 = InvoiceDecoder::encodeInvoice(inv1, parse_hex(privkey22));

    // lenght should be the same, ordering may differ
    EXPECT_EQ(enc1.length(), linv23WithHint.length());
    EXPECT_EQ(enc1, "lnbc10010n1p39j00rpp58dlferm3cr3lwwwjjstnfh529sk94keghm9yyk9heuusn2338snsrzjqw8c7yfutqqy3kz8662fxutjvef7q2ujsxtt45csu0k688lkzu3ld28pfhz4wtgyryqqqqryqqqqthqqpy9qypqsqxqyz5vqdqqsp5uxw3y3mpf2xky62qrr4a543rlrwa6dvdds305mcswk0t3578uc9qnp4qg09s25g00v56ey2jfn3g04kqpzf4r2e5rdsv5m5pvfhspn6d5xwucqzzg7fwsewumf9vxuklhf0mzqvq2ttgq873u60na4s3teeqdye8cemfqkfcnc6s0p0w9srvu4xvkwggd2u37h3crcv55yy3hss0mpeeyatqpm9kr4k");

    const auto inv = InvoiceDecoder::decodeInvoice(enc1);
    EXPECT_EQ(inv.network, Mainnet);
    EXPECT_TRUE(inv.amountPresent);
    EXPECT_EQ(inv.unparsedAmount, "10010n");
    EXPECT_EQ(hex(inv.signature), "f25d0cbb9b49586e5bf74bf620300a5ad003fa3cd3e7dac22bce40d264f8ced20b2713c6a0f0bdc580d9ca99967210d5723ebc703c329421237841fb0e724eac01");
    EXPECT_FALSE(inv.mismatchNodeidSignature);
    EXPECT_EQ(inv.timestamp, 1650015715ull);
    EXPECT_EQ(hex(inv.nodeId), pubkey22);
    EXPECT_EQ(hex(inv.recoveredPubkey), pubkey22);
    EXPECT_EQ(hex(inv.paymentHash), "3b7e9c8f71c0e3f739d2941734de8a2c2c5adb28beca4258b7cf3909aa313c27");
    EXPECT_EQ(hex(inv.secret), "e19d1247614a8d62694018ebda5623f8dddd358d6c22fa6f10759eb8d3c7e60a");
    ASSERT_EQ(inv.routing.size(), 1ul); // 1 routing hint
    EXPECT_EQ(hex(inv.routing[0].pubkey), "038f8f113c580048d847d6949371726653e02b928196bad310e3eda39ff61723f6");
    EXPECT_EQ(hex(inv.routing[0].shortChannelId), "a8e14dc5572d0419");
    EXPECT_EQ(hex(inv.routing[0].unparsedFeeBaseMsat), "00000064");
    EXPECT_EQ(hex(inv.routing[0].unparsedFeePPM), "000005dc");
    EXPECT_EQ(hex(inv.routing[0].unparsedCltvExpiryDelta), "0009");
    EXPECT_EQ(hex(inv.unparsedFeatures), "0820");
    EXPECT_EQ(hex(inv.unparsedExpiry), "1518");
    EXPECT_EQ(inv.minFinalCltvExpiry, 72);

    // 2nd round, should give same result as first
    const auto inv2 = InvoiceDecoder::decodeInvoice(enc1);
    const auto enc2 = InvoiceDecoder::encodeInvoice(inv2, parse_hex(privkey22));
    EXPECT_EQ(enc2.length(), linv23WithHint.length());
    EXPECT_EQ(enc2, enc1);
}

TEST(LNInvoice, AddRoutingHint) {
    EXPECT_EQ(linv22.length(), 342ul);

    auto inv = InvoiceDecoder::decodeInvoice(linv22);
    EXPECT_EQ(hex(inv.nodeId), pubkey22);
    EXPECT_EQ(hex(inv.recoveredPubkey), pubkey22);
    EXPECT_EQ(hex(inv.signature), "a6289426d943322fbc964d6473ba180b741a4e37d50e8dcbf328b85b40a6bf6410bf62b00d73d9e8c3add346c1c4dee30a0c19ab10ffc0d800b447f079b3007800"); 

    // add routing
    RoutingInfo ri;
    ri.pubkey = parse_hex("038f8f113c580048d847d6949371726653e02b928196bad310e3eda39ff61723f6");
    ri.shortChannelId = parse_hex("a8e14dc5572d0419");
    ri.unparsedFeeBaseMsat = parse_hex("00000064");
    ri.unparsedFeePPM = parse_hex("000005dc");
    ri.unparsedCltvExpiryDelta = parse_hex("0009");
    inv.routing.push_back(ri);

    // encode
    const auto lnRouting = InvoiceDecoder::encodeInvoice(inv, parse_hex(privkey22));
    EXPECT_EQ(lnRouting, "lntb1230n1p3072yfpp5hpqe4wuc2smftu8k3qqcm9z489vdz6zh3f0nc8jqt27qukkkp5lqrzjqw8c7yfutqqy3kz8662fxutjvef7q2ujsxtt45csu0k688lkzu3ld28pfhz4wtgyryqqqqryqqqqthqqpy9qyysgqxqy7sjqdqud3jxktt5w46x7unfv9kz6mn0v3jssp59553gh4vqdflsfznxjcywgu3kelxa4ryfpgn7f640h7q2y63vflsnp4qg09s25g00v56ey2jfn3g04kqpzf4r2e5rdsv5m5pvfhspn6d5xwucqpcdvshmcwe48lm4g24e8nrqxeufmg09eqqthpl0ewl63v652pccm6n65qt9my23j6nx3245udqmw2g09w6uj9g7vmfqvzwfnuhucg05fqpygffvs");
    EXPECT_EQ(lnRouting.length(), 427ul);

    {
        const auto inv2 = InvoiceDecoder::decodeInvoice(lnRouting);
        EXPECT_EQ(inv2.network, inv.network);
        EXPECT_EQ(inv2.unparsedAmount, inv.unparsedAmount);
        EXPECT_NE(hex(inv2.signature), hex(inv.signature)); // sig has changed
        EXPECT_EQ(hex(inv2.signature), "6b217de1d9a9ffbaa155c9e6301b3c4ed0f2e4005dc3f7e5dfd459aa2838c6f53d500b2ec8a8cb5334555a71a0db948795dae48a8f33690304e4cf97e610fa2401");
        EXPECT_FALSE(inv2.mismatchNodeidSignature);
        EXPECT_EQ(inv2.timestamp, inv.timestamp);
        EXPECT_EQ(hex(inv2.nodeId), pubkey22);
        EXPECT_EQ(hex(inv2.recoveredPubkey), pubkey22);
        EXPECT_EQ(hex(inv2.paymentHash), hex(inv.paymentHash));
        EXPECT_EQ(hex(inv2.secret), hex(inv.secret));
        ASSERT_EQ(inv2.routing.size(), 1ul); // 1 routing hint
        EXPECT_EQ(hex(inv2.routing[0].pubkey), "038f8f113c580048d847d6949371726653e02b928196bad310e3eda39ff61723f6");
        EXPECT_EQ(hex(inv2.routing[0].shortChannelId), "a8e14dc5572d0419");
        EXPECT_EQ(hex(inv2.routing[0].unparsedFeeBaseMsat), "00000064");
        EXPECT_EQ(hex(inv2.routing[0].unparsedFeePPM), "000005dc");
        EXPECT_EQ(hex(inv2.routing[0].unparsedCltvExpiryDelta), "0009");
        EXPECT_EQ(hex(inv2.unparsedFeatures), hex(inv.unparsedFeatures));
        EXPECT_EQ(hex(inv2.unparsedExpiry), hex(inv.unparsedExpiry));
        EXPECT_EQ(inv2.minFinalCltvExpiry, inv.minFinalCltvExpiry);
    }
}

TEST(LNInvoice, VerifySignature) {
    EXPECT_TRUE(InvoiceDecoder::verifySignature(specTest1, parse_hex(specTestPubkey)));
    EXPECT_TRUE(InvoiceDecoder::verifySignature(specTest2, parse_hex(specTestPubkey)));

    EXPECT_TRUE(InvoiceDecoder::verifySignature(linv22));
    EXPECT_TRUE(InvoiceDecoder::verifySignature(linv22, parse_hex(pubkey22)));
}

TEST(LNInvoice, BuildSignature) {
    EXPECT_EQ(hex(InvoiceDecoder::buildSignature(specTest1, parse_hex(specTestPrivkey))),
        "8d3ce9e28357337f62da0162d9454df827f83cfe499aeb1c1db349d4d81127425e434ca29929406c23bba1ae8ac6ca32880b38d4bf6ff874024cac34ba9625f101");
    EXPECT_EQ(hex(InvoiceDecoder::buildSignature(specTest1, parse_hex(dummyPrivkey))),
        "e9631e8e6dac31dea75a2fc2c6023aadba0c5083457f470f5eb55118e60eef7f799cc87a36f3bbd3c315e3ead931a628a414af50a48f2ee0fc5cbdb5c263d9c701");
}

const auto specTest1WithNode = "lnbc1pvjluezpp5qqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqqqsyqcyq5rqwzqfqypq9qzsgdpl2pkx2ctnv5sxxmmwwd5kgetjypeh2ursdae8g6twvus8g6rfwvs8qun0dfjkxaqsp5zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zyg3zygsnp4q0n326hr8v9zprg8gsvezcch06gfaqqhde2aj730yg0durunfhv66fld5lkvam09e7n8wllj3jrhn7qj6zm8gnm5fvscaz990htcul4y8299pfc00ua009dryzp47l2q4nkxwac8gmfexj4mnrqp840uk2zcpacwdru";

TEST(LNInvoice, EncodeSignWrongKey) {
    const auto inv1 = InvoiceDecoder::decodeInvoice(specTest1WithNode);
    EXPECT_EQ(hex(inv1.nodeId), specTestPubkey);
    EXPECT_EQ(hex(inv1.recoveredPubkey), specTestPubkey);
    EXPECT_FALSE(inv1.mismatchNodeidSignature);
    EXPECT_EQ(hex(inv1.signature), "4fdb4fd99ddbcb9f4ceeffe5190ef3f025a16ce89ee896431d114afbaf1cfd487514a14e1efe75ef2b464106befa8159d8ceee0e8da7269577318027abf9650b01");

    const auto enc1 = InvoiceDecoder::encodeInvoice(inv1, parse_hex(specTestPrivkey));
    EXPECT_EQ(enc1, specTest1WithNode);

    EXPECT_EXCEPTION(
        InvoiceDecoder::encodeInvoice(inv1, parse_hex(dummyPrivkey)),
        "Wrong private key provided, public keys do not match 03e7156ae33b0a208d0744199163177e909e80176e55d97a2f221ede0f934dd9ad 034f355bdcb7cc0af728ef3cceb9615d90684bb5b2ca5f859ab0f0b704075871aa");
}
