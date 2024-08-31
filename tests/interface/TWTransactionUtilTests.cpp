// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWTransactionUtil.h>

#include "Bitcoin/Script.h"
#include "Bitcoin/SegwitAddress.h"

#include "HexCoding.h"
#include "PublicKey.h"
#include "uint256.h"

#include "TestUtilities.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <thread>

using namespace TW;

TEST(TWTransactionUtil, CalcTxHashBitcoin) {
    constexpr auto coin = TWCoinTypeBitcoin;
    const auto encodedTxPtr = WRAPS(TWStringCreateWithUTF8Bytes("02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000"));
    const auto txHashResult = WRAPS(TWTransactionUtilCalcTxHash(coin, encodedTxPtr.get()));

    assertStringsEqual(txHashResult, "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1");
}

TEST(TWTransactionUtil, CalcTxHashEthereum) {
    constexpr auto coin = TWCoinTypeEthereum;
    const auto encodedTxPtr = WRAPS(TWStringCreateWithUTF8Bytes("f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931"));
    const auto txHashResult = WRAPS(TWTransactionUtilCalcTxHash(coin, encodedTxPtr.get()));

    assertStringsEqual(txHashResult, "0x199a7829fc5149e49b452c2cab76d8fa5a9682fee6e4891b8acb697ac142513e");
}

TEST(TWTransactionUtil, CalcTxHashSolana) {
    constexpr auto coin = TWCoinTypeSolana;
    const auto encodedTxPtr = WRAPS(TWStringCreateWithUTF8Bytes("AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG"));
    const auto txHashResult = WRAPS(TWTransactionUtilCalcTxHash(coin, encodedTxPtr.get()));

    assertStringsEqual(txHashResult, "3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej");
}

TEST(TWTransactionUtil, CalcTxHashCosmos) {
    constexpr auto coin = TWCoinTypeCosmos;
    const auto encodedTxPtr = WRAPS(TWStringCreateWithUTF8Bytes(
        "CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZl"
        "Yzl1cHNkcGhrdHh0MDNna3dseBItY29zbW9zMThzMGhkbnNsbGdjY2x3ZXU5YXltdzRuZ2t0cjJrMHJreWdkemRwGg"
        "8KBXVhdG9tEgY0MDAwMDASZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAuzvXOQ3owLG"
        "f5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJEgQKAggBEhMKDQoFdWF0b20SBDEwMDAQwJoMGkCvvVE6d29P30cO9/"
        "lnXyGunWMPxNY12NuqDcCnFkNM0H4CUQdl1Gc9+ogIJbro5nyzZzlv9rl2/GsZox/JXoCX"));
    const auto txHashResult = WRAPS(TWTransactionUtilCalcTxHash(coin, encodedTxPtr.get()));

    assertStringsEqual(txHashResult, "85392373F54577562067030BF0D61596C91188AA5E6CA8FFE731BD0349296411");
}
