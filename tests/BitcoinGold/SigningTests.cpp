// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/TransactionPlan.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWCoinType.h>

#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;

// L41CbYm3UNgMh4F4ZxNYmmNdfWKcMzeWADmpxnsJ3FdpdjsAJfNN
const char* ADDRESS = "GSGUyooxtCUVBonYV8AANp7FvKy3WTvpMR";
const char* PRIVATE_KEY = "CA6D1402199530A5D610A01A53505B6A344CF61B0CCB2902D5AEFBEA63C274BB";
const char* P2PKH_SCRIPT = "76a9145c662c05ac26c503f501f144c11336d4acf2e13788ac";
const char* SIGNED_GOLDEN = "01000000010000000000000000000000000000000000000000000000000000000000000000000000006a473044022052a2ad383cff0919a74ed535e62f1969aa36ae913c365dc2823e0a4ca052346a02201d7ac800d1f02fd213194805b1234e7e965dcdd3f4dd07a7cb2ed05689042d3141210201c0925e857278fe33ccdd1d184af138f19f411a69f654d42d3c1288ab91c3d00000000002a0860100000000001976a9145c662c05ac26c503f501f144c11336d4acf2e13788ac64e31c00000000001976a9145c662c05ac26c503f501f144c11336d4acf2e13788ac00000000";
auto const txOutPoint = OutPoint(std::vector<uint8_t>(32), 0);

inline auto sum(const std::vector<Proto::UnspentTransaction>& utxos) {
    int64_t s = 0u;
    for (auto& utxo : utxos) {
        s += utxo.amount();
    }
    return s;
}

static Proto::UnspentTransaction buildUTXO(const OutPoint& outPoint, Amount amount) {
    Proto::UnspentTransaction utxo;
    utxo.set_amount(amount);
    utxo.mutable_out_point()->set_hash(outPoint.hash.data(), outPoint.hash.size());
    utxo.mutable_out_point()->set_index(outPoint.index);
    // Set input script
    auto data = parse_hex(P2PKH_SCRIPT);
    std::string binaryData(data.begin(), data.end());
    utxo.set_script(binaryData);
    return utxo;
}

static Proto::SigningInput buildSigningInput(Amount amount, int byteFee, const std::vector<Proto::UnspentTransaction>& utxos, bool useMaxAmount, enum TWCoinType coin) {
    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll | TWBitcoinSigHashTypeFork);
    input.set_amount(amount);
    input.set_byte_fee(byteFee);
    input.set_use_max_amount(useMaxAmount);
    input.set_coin_type(coin);
    // Set binary private key
    auto data = parse_hex(PRIVATE_KEY);
    std::string binPrivkey(data.begin(), data.end());
    input.add_private_key(binPrivkey);
    // Set target addresses
    input.set_to_address(ADDRESS);
    input.set_change_address(ADDRESS);

    *input.mutable_utxo() = { utxos.begin(), utxos.end() };
    return input;
}

TEST(BitcoinGold, Address) {
    const auto privateKey = PrivateKey(parse_hex(PRIVATE_KEY));
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const Address address(publicKey, 38);
    ASSERT_EQ(address.string(), ADDRESS);
}

TEST(BitcoinGold, SignTransaction) {
    auto utxos = std::vector<Proto::UnspentTransaction>();
    utxos.push_back(buildUTXO(txOutPoint, 2000000));
    ASSERT_EQ(sum(utxos), 2000000);

    auto signingInput = buildSigningInput(100000, 30, utxos, true, TWCoinTypeBitcoinGold);
    ASSERT_EQ(signingInput.coin_type(), TWCoinTypeBitcoinGold);
    ASSERT_EQ(signingInput.to_address(), ADDRESS);
    ASSERT_EQ(signingInput.change_address(), ADDRESS);

    auto txPlan = TransactionBuilder::plan(signingInput);
    TransactionSigner<Transaction> signer(std::move(signingInput), txPlan);

    const auto& tx = signer.transaction;
    ASSERT_EQ(tx.coinType, TWCoinTypeBitcoinGold);

    const auto result = signer.sign();
    ASSERT_TRUE(result.isSuccess());

    const auto signedTx = result.payload();

    TW::Data buffer;
    signedTx.encode(false, buffer);
    ASSERT_EQ(hex(buffer), SIGNED_GOLDEN);
}
