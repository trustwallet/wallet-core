
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "PublicKey.h"

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWHDWallet.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace Bitcoin;

/*
 https://iancoleman.io/bip39/ and https://www.mobilefish.com/download/ethereum/hd_wallet.html
 Mnemonic - shoot island position soft burden budget tooth cruel issue economy destroy above
 m/44'/20'/0'/0/0 Private key in Base58 encoding - L37s29UXmQowW7y5BeDhe8AwM7quvRS2FmkWxSuctw9PbFprhhqA
 m/44'/20'/0'/0/0 Private key in bytes - b00420bab8b663f0870ee8e46435743ba9588eb88d8d31410ed54afa67602e8d
 
 https://blockbook.monetaryunit.org/tx/c0090762084b77656cef7a4730f4dee6dcc66d9b4fbe48fd2ef1f4f4a8031f22
 
 */
TEST(MonetaryUnitTransaction, SignTransaction) {
    const int64_t utxo_amount = 336855835;
    const int64_t amount = 236853579;
    
    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWSignatureHashTypeFork | TWSignatureHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("7ZemuyZukEnCirZaFZsiQ36KvcdmQ7qEdS");
    input.set_change_address("7eJR8UQnd5srGL6CQFZfTBbBRdmmoDsapW");
    
    auto hash0 = DATA("ecb6856eda9edec8a340709dce9a36b010e256e1331384f7598c09d6b748f118");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("76a91447825943ca6a936b177fdc7c9dc05251640169c288ac");
    utxo0->set_script(script0.data(), script0.size());
    
    auto utxoKey0 = DATA("b00420bab8b663f0870ee8e46435743ba9588eb88d8d31410ed54afa67602e8d");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));
    
    // Sign
    auto signer = TW::Bitcoin::TransactionSigner<TW::Bitcoin::Transaction>(std::move(input));
    auto result = signer.sign();
    auto signedTx = result.payload();
    
    ASSERT_TRUE(result);
    //ASSERT_EQ(fee, signer.plan.fee);
    
    Data serialized;
    signedTx.encode(false, serialized);
    ASSERT_EQ(
              hex(serialized),
              "01000000"
              "01"
              "ecb6856eda9edec8a340709dce9a36b010e256e1331384f7598c09d6b748f118"
              "01000000"
              "69"
              "46304302201387c7fe7a9ba470144ec5c0fdd96804e5e87bb5a48db367b42abac99a659299021f38dc7f25bb68c03d0a8154ddb692fb0d41bf9ac1a2562fe500a3f49b0686bc012102825357682ee04ac2bddc0df89b885e0eba5efd65dc7ce545807741af5e6a5e14"
              "ffffffff"
              "02"
              "00e1f505000000001976a9148273a3ccd46cc77677bba8fa02d47ed1b8873e4688ac4b191e0e000000001976a9144f74ae2269b2278168f7c4480cbe4334ea4fddbc88ac"
              "00000000"
              );
}
