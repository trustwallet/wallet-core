// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <gtest/gtest.h>

#include "Bitcoin/OutPoint.h"
#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"
#include "Bitcoin/SigHashType.h"
#include "Bitcoin/Transaction.h"
#include "Bitcoin/TransactionBuilder.h"
#include "Bitcoin/TransactionSigner.h"
#include "HexCoding.h"
#include "proto/Bitcoin.pb.h"
#include "TestUtilities.h"

using namespace TW;

namespace TW::Bitcoin::tests {

// clang-format off
TEST(TWBitcoinGoldScript, LockScriptTest) {
    auto script = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("btg1q6572ulr0kmywle8a30lvagm9xsg9k9n5cmzfdj").get(), TWCoinTypeBitcoinGold));
    auto scriptData = WRAPD(TWBitcoinScriptData(script.get()));
    assertHexEqual(scriptData, "0014d53cae7c6fb6c8efe4fd8bfecea36534105b1674");

    auto script2 = WRAP(TWBitcoinScript, TWBitcoinScriptLockScriptForAddress(STRING("btg1qawhpp9gv3g662phqufjmj2ps2ge7sq4thy5g07").get(), TWCoinTypeBitcoinGold));
    auto scriptData2 = WRAPD(TWBitcoinScriptData(script2.get()));
    assertHexEqual(scriptData2, "0014ebae10950c8a35a506e0e265b928305233e802ab");
}

TEST(BitcoinGoldKey, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("shoot island position soft burden budget tooth cruel issue economy destroy above").get(),
        STRING("TREZOR").get()
    ));

    // .bip84
    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoinGold, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoinGold, TWHDVersionZPUB));

    assertStringsEqual(zprv, "zprvAdB7dYnT955ubXEkdBWhDqFSyeDfVpKQmVJPbRXGiAg4mnGT7dCBsZZFeik1mNt6bS4zkdZSNtZm8dqu1Lrp1brQ16NgYgeEoiz6ftUfVAW");
    assertStringsEqual(zpub, "zpub6rAU34KLySeCp1KDjD3hayCBXg49uH3G8iDzPovtGWD3eabbfAWSRMsjVyfuRfCCquiKTD6YV42nHUBtwh2TbVPvWqxrGuyEvHN17c3XUXw");
}

TEST(BitcoinGoldKey, DeriveFromZPub) {
    auto zpub = STRING("zpub6rAU34KLySeCp1KDjD3hayCBXg49uH3G8iDzPovtGWD3eabbfAWSRMsjVyfuRfCCquiKTD6YV42nHUBtwh2TbVPvWqxrGuyEvHN17c3XUXw");
    auto pubKey2 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeBitcoinGold, STRING("m/84'/156'/0'/0/2").get()));
    auto pubKey9 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeBitcoinGold, STRING("m/84'/156'/0'/0/9").get()));

    auto address2 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey2.get(), TWCoinTypeBitcoinGold));
    auto address2String = WRAPS(TWAnyAddressDescription(address2.get()));

    auto address9 = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey9.get(), TWCoinTypeBitcoinGold));
    auto address9String = WRAPS(TWAnyAddressDescription(address9.get()));

    assertStringsEqual(address2String, "btg1qkdgxykht6nww9l9rn0xhslf78nl605gwka9zak");
    assertStringsEqual(address9String, "btg1qzw2ptuyaw023gm7te2r5e3xkufn9wrm3kzrg8t");
}

TEST(TWBitcoinGoldTxGeneration, TxGeneration) {
    const int64_t amount = 5000;

    // Setup input
    Proto::SigningInput input;
    input.set_coin_type(TWCoinTypeBitcoinGold);
    input.set_hash_type(hashTypeForCoin(TWCoinTypeBitcoinGold));
    input.set_amount(amount);
    input.set_byte_fee(1);
    input.set_to_address("btg1qmd6x5awe4t5fjhgntv0pngzdwajjg250wxdcs0");
    input.set_change_address("btg1qawhpp9gv3g662phqufjmj2ps2ge7sq4thy5g07");

    auto utxoKey0 = parse_hex("cbe13a79b82ec7f8871b336a64fd8d531f598e7c9022e29c67e824cfd54af57f");
    input.add_private_key(utxoKey0.data(), utxoKey0.size());
    input.set_lock_time(0x00098971);


    auto scriptPub1 = Script(parse_hex("0014db746a75d9aae8995d135b1e19a04d7765242a8f"));
    auto scriptHash = std::vector<uint8_t>();
    scriptPub1.matchPayToWitnessPublicKeyHash(scriptHash);
    auto scriptHashHex = hex(scriptHash);

    auto redeemScript = Script::buildPayToPublicKeyHash(scriptHash);
    auto scriptString = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());
    (*input.mutable_scripts())[scriptHashHex] = scriptString;

    auto utxo0 = input.add_utxo();
    auto utxo0Script = parse_hex("0014d53cae7c6fb6c8efe4fd8bfecea36534105b1674");
    utxo0->set_script(utxo0Script.data(), utxo0Script.size());
    utxo0->set_amount(10000);

    auto hash0 = parse_hex("5727794fa2b94aa22a226e206130524201ede9b50e032526e713c848493a890f");
    utxo0->mutable_out_point()->set_hash(hash0.data(), hash0.size());
    utxo0->mutable_out_point()->set_index(0);
    utxo0->mutable_out_point()->set_sequence(0xfffffffd);

    // Sign
    auto signingInput = SigningInput(input);
    auto result = TransactionSigner<Transaction, TransactionBuilder>::sign(signingInput);

    ASSERT_TRUE(result) << std::to_string(result.error());
    auto signedTx = result.payload();

    Data serialized;
    signedTx.encode(serialized);
    ASSERT_EQ(hex(serialized), // printed using prettyPrintTransaction
        "01000000" // version
        "0001" // marker & flag
        "01" // inputs
            "5727794fa2b94aa22a226e206130524201ede9b50e032526e713c848493a890f"  "00000000"  "00"  ""  "fdffffff"
        "02" // outputs
            "8813000000000000"  "16"  "0014db746a75d9aae8995d135b1e19a04d7765242a8f"
            "fb12000000000000"  "16"  "0014ebae10950c8a35a506e0e265b928305233e802ab"
        // witness
            "02"  "47"  "304402202b371b7cae885463c06357d1fc6ca95ab155613f212711bc7fb115500654946d0220430af77cbbb30afe7d7dcaccb72a55da802ee0a2bfea790dfe7c4e1a4c53fd7d41"  "21"  "03e00b5dec8078d526fba090247bd92db6b67a4dd1953b788cea9b52de9471b8cf"
        "71890900" // nLockTime
    );
}
// clang-format on

} // namespace TW::Bitcoin::tests
