// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/Signer.h"
#include "Ethereum/Transaction.h"
#include "Ethereum/Address.h"
#include "Ethereum/RLP.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Binance {

using namespace TW::Ethereum;

class SignerExposed : public Signer {
public:
    SignerExposed(boost::multiprecision::uint256_t chainID) : Signer(chainID) {}
    using Signer::hash;
};

TEST(BinanceSmartSigner, TokenTransfer) {
    auto address = parse_hex("0x31BE00EB1fc8e14A696DBC72f746ec3e95f49683");
    auto transaction = Transaction(
        /* nonce: */ 1,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 10000000000000000, // 0.01
        /* payload: */ {}
    );

    // addr: 0xB9F5771C27664bF2282D98E09D7F50cEc7cB01a7  mnemonic: isolate dismiss ... cruel note
    auto key = PrivateKey(parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904"));
    auto signer = SignerExposed(96); // BSC TESTNET
    signer.sign(key, transaction);

    auto encoded = RLP::encode(transaction);
    ASSERT_EQ(hex(encoded), "f86c018504a817c8008252089431be00eb1fc8e14a696dbc72f746ec3e95f49683872386f26fc100008081e3a0fa0996244e33efd24cf4e9d53c0e9389f9922cdaf8e5f96d83f4e85c09b31e44a0544b3fd834cd354809e96cd8181d454cb9f8ffa927d95cb522ae5c81f57a0ce2");
    // 0xe1bc72a6a0e6183b889b23080137272e2a1c84affd187324544b2d943237c10c
    // https://explorer.binance.org/smart-testnet/tx/0xe1bc72a6a0e6183b889b23080137272e2a1c84affd187324544b2d943237c10c
}

} // namespace TW::Binance
