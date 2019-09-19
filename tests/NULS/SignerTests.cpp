// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>

#include "NULS/Address.h"
#include "NULS/Signer.h"
#include "NULS/TransactionBuilder.h"

using namespace TW;
using namespace TW::NULS;

inline auto makeInput(std::string priKey, std::string addressFrom, std::string addressTo,
                      uint256_t amount, uint32_t chainId, uint32_t assetId, std::string nonce, std::string remark,
                      uint256_t balance) {
    Proto::SigningInput input;
    input.set_private_key(priKey);

    input.set_from_address(addressFrom);
    input.set_to_address(addressTo);

    Data amountData = store(amount);
    std::string amountStr;
    amountStr.insert(amountStr.begin(), amountData.begin(), amountData.end());
    input.set_amount(amountStr);

    input.set_chain_id(chainId);
    input.set_nonce(nonce);
    input.set_remark(remark);

    Data balanceData = store(balance);
    std::string balanceStr;
    balanceStr.insert(balanceStr.begin(), balanceData.begin(), balanceData.end());
    input.set_balance(balanceStr);

    input.set_timestamp(0x12345678);
    return input;
}

TEST(NULSSigner, Sign) {
    Data nonceData = parse_hex("46a7e127e1fb5302");
    std::string nonce;
    nonce.insert(nonce.begin(), nonceData.begin(), nonceData.end());
    auto input = makeInput("a1269039e4ffdf43687852d7247a295f0b5bc55e6dda031cffaa3295ca0a9d7a",
                           "NULSd6HghWa4CN5qdxqMwYVikQxRZyj57Jn4L", "NULSd6HgUiMKPNi221bPfqvvho8QpuYBvn1x3",
                           10000000L, 1, 1, nonce, "这是个签名用例", 10000000000L);
    auto signer = NULS::Signer(input);
    std::vector<uint8_t> signature = signer.sign();
    std::string signedTransaction = hex(signature);

    ASSERT_EQ(
            signedTransaction,
    "020093e1628b69010be8bdace8b4a6316e756c73ffffffff012198acb2d467b5ea70404021789755d6b11fc581"
    "5a0dcf2c05dcbb5b8fde68fd490080f0fa020000000000000000000002170100010dbd9a8320dc929ff589a3c4"
    "06396352498c1e46809698000000000000000000000017010001ddd945f023c3bf91cabb60f5cafe31ee9d82ea"
    "9860d36002000000000000000000006b21030dbcfb41892cf129fb6c12fd5c06d79fe5d342eaf826714d3dc7e9"
    "3602ccde9a00473045022100cff47a4b7b7577427fc74bb98c23180c7c6b25374dfae5eedf81bb1ee90c93df02"
    "2079c16b86111a04b3f7fe8996af602de3d41a3beb95f24de3436ab3c786d8cf27");
}