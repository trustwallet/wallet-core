// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include "Ethereum/Signer.h"
#include "Ethereum/Transaction.h"
#include "Ethereum/Address.h"
#include "Ethereum/RLP.h"
#include "Ethereum/ABI.h"
#include "proto/Ethereum.pb.h"
#include "HexCoding.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"

#include <gtest/gtest.h>

namespace TW{

using namespace TW::Ethereum;
using namespace TW::Ethereum::ABI;


TEST(SignerEnergyWebToken, SignNativeTransfer) {

    auto toAddress = parse_hex("0x4786B95B0cB88C986ab68be15E87806e42cf153A");
    auto transaction = TransactionNonTyped::buildNativeTransfer(
        /* nonce: */ 1,
        /* gasPrice: */ 1000,
        /* gasLimit: */ 999999,
        /* to: */ toAddress,
        /* amount: */ 1000000000000000 // 0.001
    );

    // addr: 0xB9F5771C27664bF2282D98E09D7F50cEc7cB01a7 
    auto privateKey = PrivateKey(parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904"));
    uint256_t chainID = 246;
    auto signature = Signer::sign(privateKey, chainID, transaction);

    auto encoded = transaction->encoded(signature, chainID);
    
    
    ASSERT_EQ(hex(encoded), "f86b018203e8830f423f944786b95b0cb88c986ab68be15e87806e42cf153a87038d7ea4c6800080820210a00856695fa9cc373220ce337c5d47ee68186752473bce669b8a14cc4e00450e3da0419bda5d030dfe4059e104135a078ac8250ef6187e96b0146bf6290d089fcbe7");
}

}
