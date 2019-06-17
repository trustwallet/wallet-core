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

TEST(PivxTransaction, SignTransaction) {

    const int64_t utxo_amount = int64_t(2000) * 100000000;
    const int64_t amount = int64_t(1000) * 100000000;
    const int64_t feeRate = 1;

    auto input = Bitcoin::Proto::SigningInput();
    input.set_hash_type(TWSignatureHashTypeAll);
    input.set_amount(amount);
    input.set_byte_fee(feeRate);
    input.set_to_address("DRScf288kQMb8wJYj4KpNEMK6hwzUSTQ8j");
    input.set_change_address("DNzepvRm2MLG5GKdXaJDfX2T4BtWigMdAJ");

    auto hash0 = DATA("6dbcafe704e3f3997e6011a967466239c116aeda0cb5e34ef93ea750e6d8fdee");
    auto utxo0 = input.add_utxo();
    utxo0->mutable_out_point()->set_hash(TWDataBytes(hash0.get()), TWDataSize(hash0.get()));
    utxo0->mutable_out_point()->set_index(1);
    utxo0->mutable_out_point()->set_sequence(UINT32_MAX);
    utxo0->set_amount(utxo_amount);
    auto script0 = parse_hex("21030bdba3be2cc6028244568570d225ea254e43e3c03ee3a670c05c04a81b4817a8ac");
    utxo0->set_script(script0.data(), script0.size());

    auto utxoKey0 = DATA("e2f7e8a1b0b0fa6934f0171a632ac3fc2c96901255ad27c2e97281e1359c76ef");
    input.add_private_key(TWDataBytes(utxoKey0.get()), TWDataSize(utxoKey0.get()));


    auto signer = TW::Bitcoin::TransactionSigner<TW::Bitcoin::Transaction>(std::move(input));
    auto result = signer.sign();
    auto signedTx = result.payload();

    ASSERT_TRUE(result);
    ASSERT_EQ(signer.plan.fee, 226);

    Data serialized;
    signedTx.encode(false, serialized);
    ASSERT_EQ(
        hex(serialized),
        "01000000016dbcafe704e3f3997e6011a967466239c116aeda0cb5e34ef93ea750e6d8fdee0100000049483045022100ac75bc001ab9ea6a7a8c453202717cea50b691ba7f97b8b5c0396a7eba5de71e022056013b17350fc9c596d24491d5f1365ba1829eb300ebd41756d7d322d0bf3f0901ffffffff0200e87648170000001976a914deb2ba2211693b3082b4e308c25d9e409de37be188ac1ee77648170000001976a914c3d978f805921148b7e59d2bebe23ccca85237f088ac00000000"
    ); 
}

TEST(PivxTransaction, LockScripts) {
 
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("DMkaY8mK98YgyDTYBZFLP3Rx1cMotAVA5Z").get()));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "76a914b637f1512b4517737009cfd3f7af3c9c692e13f088ac");
 
    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptBuildForAddress(STRING("D8dcUg3pjJy15tkC2MYKtnUsYJj83YyEjS").get()));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "76a914264d03f3656b27b517a2d12908faa6a2a888d63588ac");
}

