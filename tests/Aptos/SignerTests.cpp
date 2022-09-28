// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aptos/Signer.h"
#include "Aptos/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosSigner, Sign) {
    // equivalent of
    // #[test]
    // fn test_transfer() {
    //    let amount = 1000u64;
    //    let to = AccountAddress::from_str("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b").unwrap();
    //    let st = StructTag {
    //        address: AccountAddress::ONE,
    //        module: Identifier::from_utf8(("aptos_coin".as_bytes()).to_vec()).unwrap(),
    //        name: Identifier::from_utf8(("AptosCoin".as_bytes()).to_vec()).unwrap(),
    //        type_params: vec![],
    //    };
    //    let tag = TypeTag::Struct(st);
    //    let val = TransactionPayload::EntryFunction(EntryFunction::new(
    //        ModuleId::new(AccountAddress::ONE, Identifier::new("coin").unwrap()),
    //        Identifier::new("transfer").unwrap(),
    //        vec![tag],
    //        vec![
    //            bcs::to_bytes(&to).unwrap(),
    //            bcs::to_bytes(&amount).unwrap(),
    //        ],
    //    ));
    //    let tx = RawTransaction::new(AccountAddress::from_str("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b").unwrap(), 1, val,1,1,1, ChainId::new(1));
    //    let val = hex::encode(bcs::to_bytes(&tx).unwrap());
    //    assert_eq!(val, "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b010000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e80300000000000001000000000000000100000000000000010000000000000001");
    //}
    Proto::SigningInput input;
    input.set_sender("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b");
    input.set_sequence_number(1);
    auto& tf = *input.mutable_transfer();
    tf.set_to("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b");
    tf.set_amount(1000);
    input.set_max_gas_amount(1);
    input.set_gas_unit_price(1);
    input.set_expiration_timestamp_secs(1);
    input.set_chain_id(1);
    auto privateKey = PrivateKey(parse_hex("7f2634c0e2414a621e96e39c41d09021700cee12ee43328ed094c5580cd0bd6f"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    auto result = Signer::sign(input);
    ASSERT_EQ(hex(result.raw_txn()), "eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b010000000000000002000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e80300000000000001000000000000000100000000000000010000000000000001");
    // signature should be 9d3bd902bd358364c43fa65ece335dd4411527e72e1c6deb9148744eaa24e39b6bd74ff6b0195114243bdd2ee3a98511ff05883d9e79161b2b8f5029d883c309
    ASSERT_EQ(hex(result.authenticator().signature()), "b92844af7e100e36dccd739c299650747e0d86963d385bd3f543d271e56ffe228afeb2365640c1b9cdff54173ee133fa7bb79f7fe48f8164f89f5add4d2b3b00");
}

} // namespace TW::Aptos::tests
