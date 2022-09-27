// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/MoveTypes.h>
#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosMoveTypes, ModuleId) {
    ModuleId module(gAddressOne, "coin");
    ASSERT_EQ(module.address(), gAddressOne);
    ASSERT_EQ(module.name(), "coin");
    // same as
    // #[test]
    // fn test_module_id() {
    //     let a = ModuleId::new("0x1".parse().unwrap(), "coin".parse().unwrap());
    //     assert_eq!(hex::encode(a.access_vector()).as_str(), "00000000000000000000000000000000000000000000000000000000000000000104636f696e");
    // }
    ASSERT_EQ(hex(module.serialize()), "00000000000000000000000000000000000000000000000000000000000000000104636f696e");
}

} // namespace TW::Aptos::tests
