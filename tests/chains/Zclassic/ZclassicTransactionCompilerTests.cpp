#include "TransactionCompiler.h"
#include "proto/Bitcoin.pb.h"
#include "HexCoding.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <gtest/gtest.h>

namespace TW::Zclassic {

TEST(ZclassicCompiler, CompileWithSignatures) {
    const auto coin = TWCoinTypeZclassic;
    const int64_t amount = 17615;
    const int64_t fee = 10000;
    const std::string toAddress = "t1biXYN8wJahR76SqZTe1LBzTLf3JAsmT93";

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address(toAddress);
    input.set_coin_type(coin);

    auto txHash = parse_hex("2381825cd9069a200944996257e25b9403ba3e296bbc1dd98b01019cc7028cde");
    std::reverse(txHash.begin(), txHash.end());
    auto utxo = input.add_utxo();
    utxo->mutable_out_point()->set_hash(txHash.data(), txHash.size());
    utxo->mutable_out_point()->set_index(0);
    utxo->set_amount(27615);

    auto key = PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"), TWCurveSECP256k1);
    input.add_private_key(key.bytes.data(), key.bytes.size());

    auto txInputData = data(input.SerializeAsString());
    auto preImageHashes = TransactionCompiler::preImageHashes(coin, txInputData);
    EXPECT_GT(preImageHashes.size(), 0ul);
}

} // namespace TW::Zclassic
