// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>

#include "NULS/Signer.h"

using namespace TW;
using namespace TW::NULS;

inline auto makeInput(std::string priKey, std::string addressFrom, std::string addressTo,
                      uint256_t amount, uint32_t chainId, uint32_t assetId, std::string nonce, std::string remark,
                      uint256_t balance) {
    Proto::SigningInput input;
    Data key = parse_hex(priKey); 
    std::string keyStr;
    keyStr.insert(keyStr.begin(), key.begin(), key.end());
    input.set_private_key(keyStr);

    input.set_from(addressFrom);
    input.set_to(addressTo);

    Data amountData = store(amount);
    std::string amountStr;
    amountStr.insert(amountStr.begin(), amountData.begin(), amountData.end());
    input.set_amount(amountStr);

    input.set_chain_id(chainId);
    input.set_idassets_id(assetId);
    input.set_nonce(nonce);
    input.set_remark(remark);

    Data balanceData = store(balance);
    std::string balanceStr;
    balanceStr.insert(balanceStr.begin(), balanceData.begin(), balanceData.end());
    input.set_balance(balanceStr);

    input.set_timestamp(0x5d8885f8);
    return input;
}

TEST(NULSSigner, Sign) {
    auto input = makeInput("9ce21dad67e0f0af2599b41b515a7f7018059418bab892a7b68f283d489abc4b",
                           "NULSd6Hgj7ZoVgsPN9ybB4C1N2TbvkgLc8Z9H", "NULSd6Hgied7ym6qMEfVzZanMaa9qeqA6TZSe",
                           10000000, 1, 1, "0000000000000000", "", 100000000);
    auto signer = Signer(input);
    Data signature = signer.sign();
    std::string signedTransaction = hex(signature);

    ASSERT_EQ(
            signedTransaction,
            "0200f885885d00008c0117010001f7ec6473df12e751d64cf20a8baa7edd50810f8101000100201d9a"
            "0000000000000000000000000000000000000000000000000000000000080000000000000000000117"
            "010001f05e7878971f3374515eabb6f16d75219d887312010001008096980000000000000000000000"
            "0000000000000000000000000000000000000000000000000000692103958b790c331954ed367d37ba"
            "c901de5c2f06ac8368b37d7bd6cd5ae143c1d7e3463044022028019c0099e2233c7adb84bb03a9a566"
            "6ece4a5b65a026a090fa460f3679654702204df0fcb8762b5944b3aba033fa1a287ccb098150035dd8"
            "b66f52dc58d3d0843a"
            );
}