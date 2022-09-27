// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/MoveTypes.h>
#include <HexCoding.h>
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
    ASSERT_EQ(module.string(), "0x0000000000000000000000000000000000000000000000000000000000000001::coin");
    ASSERT_EQ(module.shortString(), "0x1::coin");
}

TEST(AptosMoveTypes, StructTag) {
    StructTag st(gAddressOne, "abc", "abc", std::vector{{TypeTag{.tags = U8{}}}});
    ASSERT_EQ(st.moduleID().name(), "abc");
    ASSERT_EQ(st.moduleID().address(), gAddressOne);
    // 0100000000000000000000000000000000000000000000000000000000000000010361626303616263
    ASSERT_EQ(hex(st.serialize()), "01000000000000000000000000000000000000000000000000000000000000000103616263036162630101");
}

} // namespace TW::Aptos::tests
