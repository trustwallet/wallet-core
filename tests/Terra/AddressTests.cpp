#include <gtest/gtest.h>
#include "../../include/TrustWalletCore/TWPublicKeyType.h"
#include "../../src/HexCoding.h"
#include "../../src/PrivateKey.h"
#include "../../src/Terra/Address.h"

namespace TW::TERRA{
    TEST(Address, Valid) {
        ASSERT_TRUE(Address::isValid("terra1jyjg55hzsh0f4xymy0kuuan30pp4q75ru0h35c"));
    }

    TEST(Address, Invalid) {
        ASSERT_FALSE(Address::isValid("bnb1grpf0955h0ykzq3ar6nmum7y6gdfl6lxfn46h2"));
    }

    TEST(Address, FromKeyHash) {
        auto privateKey = PrivateKey(parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832"));
        auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        ASSERT_EQ(hex(publicKeyData.bytes), "026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502");

        auto publicKey = PublicKey(publicKeyData);
        auto address = Address("terra", publicKey);
        ASSERT_EQ(address.string(), "terra1grpf0955h0ykzq3ar5nmum7y6gdfl6lxdnsk5y");
    }

} // namespace TW::TERRA

