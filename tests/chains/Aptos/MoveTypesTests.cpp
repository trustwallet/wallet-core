// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Aptos/MoveTypes.h>
#include <HexCoding.h>
#include <gtest/gtest.h>

namespace TW::Aptos::tests {

TEST(AptosMoveTypes, ModuleId) {
    ModuleId module(Address::one(), "coin");
    ASSERT_EQ(module.address(), Address::one());
    ASSERT_EQ(module.name(), "coin");
    ASSERT_EQ(hex(module.accessVector()), "00000000000000000000000000000000000000000000000000000000000000000104636f696e");
    ASSERT_EQ(module.string(), "0x0000000000000000000000000000000000000000000000000000000000000001::coin");
    ASSERT_EQ(module.shortString(), "0x1::coin");
}

TEST(AptosMoveTypes, StructTag) {
    auto functorTest = []<typename T>(T value, const std::string expectedHex) {
        TypeTag t{.tags = value};
        StructTag st(Address::one(), "abc", "abc", std::vector<TypeTag>{{t}});
        ASSERT_EQ(st.moduleID().name(), "abc");
        ASSERT_EQ(st.moduleID().address(), Address::one());
        ASSERT_EQ(hex(st.serialize()), expectedHex);
    };
    functorTest(Bool{}, "01000000000000000000000000000000000000000000000000000000000000000103616263036162630100");
    functorTest(U8{}, "01000000000000000000000000000000000000000000000000000000000000000103616263036162630101");
    functorTest(U64{}, "01000000000000000000000000000000000000000000000000000000000000000103616263036162630102");
    functorTest(U128{}, "01000000000000000000000000000000000000000000000000000000000000000103616263036162630103");
    functorTest(TAddress{}, "01000000000000000000000000000000000000000000000000000000000000000103616263036162630104");
    functorTest(TSigner{}, "01000000000000000000000000000000000000000000000000000000000000000103616263036162630105");
    functorTest(Vector{.tags = std::vector<TypeTag>{{TypeTag{.tags = U8{}}}}}, "0100000000000000000000000000000000000000000000000000000000000000010361626303616263010601");
    StructTag stInner(Address::one(), "foo", "bar", std::vector<TypeTag>{{U8{}}});
    functorTest(TStructTag{stInner}, "01000000000000000000000000000000000000000000000000000000000000000103616263036162630107000000000000000000000000000000000000000000000000000000000000000103666f6f036261720101");
}

TEST(AptosMoveTypes, TypeTagDisplay) {
    auto functorTest = [](const TypeTag &value, const std::string& expected) {
        ASSERT_EQ(TypeTagToString(value), expected);
    };
    functorTest(TypeTag{.tags = Bool{}}, "bool");
    functorTest(TypeTag{.tags = U8{}}, "u8");
    functorTest(TypeTag{.tags = U64{}}, "u64");
    functorTest(TypeTag{.tags = U128{}}, "u128");
    functorTest(TypeTag{.tags = TAddress{}}, "address");
    functorTest(TypeTag{.tags = TSigner{}}, "signer");
    TypeTag t{.tags = TypeTag::TypeTagVariant(Vector{.tags = {{U8{}}}})};
    functorTest(t, "vector<u8>");
    StructTag st(Address::one(), "foo", "bar", std::vector<TypeTag>{{U8{}}});
    TypeTag anotherT{.tags = TypeTag::TypeTagVariant(st)};
    functorTest(anotherT, "0x1::foo::bar<u8>");
    functorTest(gTransferTag, "0x1::aptos_coin::AptosCoin");
}

} // namespace TW::Aptos::tests
