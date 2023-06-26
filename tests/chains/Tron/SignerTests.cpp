// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Address.h"
#include "Tron/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "uint256.h"
#include "proto/Tron.pb.h"
#include "Tron/Signer.h"

#include <gtest/gtest.h>

namespace TW::Tron {

TEST(TronSigner, SignDirectTransferAsset) {
    auto input = Proto::SigningInput();
    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_txid("546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb");
    const auto output = Signer::sign(input);
    ASSERT_EQ(hex(output.id()), "546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb");
    ASSERT_EQ(hex(output.signature()), "77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00");
}

TEST(TronSigner, SignTransferAsset) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& transfer = *transaction.mutable_transfer_asset();
    transfer.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    transfer.set_to_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    transfer.set_amount(4);
    transfer.set_asset_name("1000959");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1541890116000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1541890116000);
    const auto txTrieRoot = parse_hex("845ab51bf63c2c21ee71a4dc0ac3781619f07a7cd05e1e0bd8ba828979332ffa");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000003cb800a7e69e9144e3d16f0cf33f33a95c7ce274097822c67243c1");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3979265);
    const auto witnessAddress = parse_hex("41b487cdc02de90f15ac89a68c82f44cbfe3d915ea");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb");
    ASSERT_EQ(hex(output.signature()), "77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00");
}

TEST(TronSigner, SignTransfer) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& transfer = *transaction.mutable_transfer();
    transfer.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    transfer.set_to_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    transfer.set_amount(2000000);

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "dc6f6d9325ee44ab3c00528472be16e1572ab076aa161ccd12515029869d0451");
    ASSERT_EQ(hex(output.signature()), "ede769f6df28aefe6a846be169958c155e23e7e5c9621d2e8dce1719b4d952b63e8a8bf9f00e41204ac1bf69b1a663dacdf764367e48e4a5afcd6b055a747fb200");
}

TEST(TronSigner, SignFreezeBalanceV2) {
    // Successfully broadcasted https://nile.tronscan.org/#/transaction/3a46321487ce1fd115da38b3431006ea529f65ef2507f19233f5a23c05abd01d
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& freeze = *transaction.mutable_freeze_balance_v2();
    freeze.set_owner_address("TWWb9EjUWai17YEVB7FR8hreupYJKG9sMR");
    freeze.set_frozen_balance(10000000);
    freeze.set_resource("ENERGY");

    transaction.set_timestamp(1676983541337);
    transaction.set_expiration(1676983599000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1676983485000);
    const auto txTrieRoot = parse_hex("9b54db7f84bd19bbad9ff1fccef894c1aade6879450e9e9e2accec751eaa1f52");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000020cd4c13a67497a3a433a3105bc5a73a041ee3da98407d5a2a2bf1b");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(34395330);
    const auto witnessAddress = parse_hex("4150d3765e4e670727ebac9d5b598f74b75a3d54a7");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(26);

    const auto privateKey = PrivateKey(parse_hex("75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "3a46321487ce1fd115da38b3431006ea529f65ef2507f19233f5a23c05abd01d");
    ASSERT_EQ(hex(output.signature()), "d4b539a389f6721b4e9d0eb9f39b62a539069060e1af2a118f06b81737ad9cdb49d5b4fda85f10603012f8de3996da2a1234c21d74ac6ea5e60217d3c10b630900");
}

TEST(TronSigner, WithdrawExpireUnfreezeContract) {
    // Successfully broadcasted https://nile.tronscan.org/#/transaction/65ff34192eebda9ba7013771ff2da1010615e348b70c046647f41afe865f00eb
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& freeze = *transaction.mutable_withdraw_expire_unfreeze();
    freeze.set_owner_address("TWWb9EjUWai17YEVB7FR8hreupYJKG9sMR");

    transaction.set_timestamp(1677574466457);
    transaction.set_expiration(1677574524000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1677574410000);
    const auto txTrieRoot = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000020fce45738ef00be07c350c03d027851308bc19d61c32312c673d3d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(34590278);
    const auto witnessAddress = parse_hex("41e7860196ad5b5718c1d6326babab039b70b8c1cd");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(27);

    const auto privateKey = PrivateKey(parse_hex("75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "65ff34192eebda9ba7013771ff2da1010615e348b70c046647f41afe865f00eb");
    ASSERT_EQ(hex(output.signature()), "ef0361248c118b8afae9c4c8e6dfad1e63eec4fb6c182ae369fa3bbecc2ac29a292838949ad74300b2b7322a110ffd4458224e283181cf6d64df0324b068bb0001");
}

TEST(TronSigner, SignUnFreezeBalanceV2) {
    // Successfully broadcasted https://nile.tronscan.org/#/transaction/3070adc1743e6fdd20e04a749cc2af691ca26d2ce70e40cc0886be03595f9eeb
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& freeze = *transaction.mutable_unfreeze_balance_v2();
    freeze.set_owner_address("TWWb9EjUWai17YEVB7FR8hreupYJKG9sMR");
    freeze.set_unfreeze_balance(510000000);
    freeze.set_resource("ENERGY");

    transaction.set_timestamp(1676992267490);
    transaction.set_expiration(1676992326000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1676992212000);
    const auto txTrieRoot = parse_hex("4b1edc58d14a5c60c083365d8b77771ba626394b445c7a7b8b5d67330bb6c92d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000020ce000354fbb346d676de268b3f83124381f8496835afe88da4a01");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(34398209);
    const auto witnessAddress = parse_hex("4194a21bec5d0e1dde2151475f72ed158a87eb4817");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(26);

    const auto privateKey = PrivateKey(parse_hex("75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "3070adc1743e6fdd20e04a749cc2af691ca26d2ce70e40cc0886be03595f9eeb");
    ASSERT_EQ(hex(output.signature()), "10bc05c47102f1db1a3a4c0b4a6aba028d5a35dda4e505563c3f0ccf95a562cf18b53f7f7053c485299cfc599a432d1f0ee5554a56cd5981ccfff31d79b9868b00");
}

TEST(TronSigner, DelegateResourceContract) {
    // Successfully broadcasted https://nile.tronscan.org/#/transaction/ceabcd0f105854c13aae12ba35c0766945713c29cee540be1239bb0f1f0cde2c
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& freeze = *transaction.mutable_delegate_resource();
    freeze.set_owner_address("TWWb9EjUWai17YEVB7FR8hreupYJKG9sMR");
    freeze.set_receiver_address("TPFfHr1CWfTcS9eugQXQmvqHNGufnjxjXP");
    freeze.set_balance(68000000);
    freeze.set_resource("ENERGY");

    transaction.set_timestamp(1676991607274);
    transaction.set_expiration(1676991660000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1676991546000);
    const auto txTrieRoot = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000020cdf260ff2357d814141106c375c101913c933c2b5c31a390db7fc");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(34397991);
    const auto witnessAddress = parse_hex("417d3601dbd9d033b034c154868acc2904d9c45565");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(26);

    const auto privateKey = PrivateKey(parse_hex("75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "ceabcd0f105854c13aae12ba35c0766945713c29cee540be1239bb0f1f0cde2c");
    ASSERT_EQ(hex(output.signature()), "664500a76466497a442cecc0e9282a9234483f047c12a997b6206d7f6a9030c70b700c879d7948c4cbdfe339c2c81a29dea18e00e9916504196c1b20cf045ca300");
}

TEST(TronSigner, UnDelegateResourceContract) {
    // Successfully broadcasted https://nile.tronscan.org/#/transaction/3609519cc700cf2446b5e048864abc4b45e2ba6b7f9f8890d471ba2876599d3b
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& freeze = *transaction.mutable_undelegate_resource();
    freeze.set_owner_address("TWWb9EjUWai17YEVB7FR8hreupYJKG9sMR");
    freeze.set_receiver_address("TPFfHr1CWfTcS9eugQXQmvqHNGufnjxjXP");
    freeze.set_balance(68000000);
    freeze.set_resource("ENERGY");

    transaction.set_timestamp(1676992063012);
    transaction.set_expiration(1676992122000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1676992008000);
    const auto txTrieRoot = parse_hex("85a47017a4380e92d09bac0f8991031e8de13b8b65767a6f5372d3f0992eabcd");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000020cdfbe4d7f36fcbb3d96dd634987b897eaf885001dd62fd92eb263");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(34398143);
    const auto witnessAddress = parse_hex("4196409f85790883057edf03286d08e4aa608c0d0a");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(26);

    const auto privateKey = PrivateKey(parse_hex("75065f100e38d3f3b4c5c4235834ba8216de62272a4f03532c44b31a5734360a"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "3609519cc700cf2446b5e048864abc4b45e2ba6b7f9f8890d471ba2876599d3b");
    ASSERT_EQ(hex(output.signature()), "b08e32a704d5a366df499d283d407c428dd50e60665f54ecf967226b75bec37157e6bc23312af07fad9dd3551cd668ce027cc280932fd4772af89d6f0fecf11900");
}

TEST(TronSigner, SignFreezeBalance) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& freeze = *transaction.mutable_freeze_balance();
    freeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    freeze.set_receiver_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    freeze.set_frozen_duration(1000000);
    freeze.set_frozen_duration(100);
    freeze.set_resource("ENERGY");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "d314967bc1d153d649d9f54a1cc78033f0d696a58ff6922f490ddaec82558c83");
    ASSERT_EQ(hex(output.signature()), "aa7cf79fb1692ff432a1a3e520be3355c3e8168c5fa22f6e3b96c2a9f2e2827b49d67d5e6eea5c7e7cf872047d422ce5d4d149c4df752b176d13f8f48920271201");
}

TEST(TronSigner, SignUnFreezeBalance) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& unfreeze = *transaction.mutable_unfreeze_balance();
    unfreeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    unfreeze.set_receiver_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    unfreeze.set_resource("ENERGY");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "c5bd624bb53fed8ce4a7361475263b3a91ae71ef389630e0b3b8693c8c56d7a1");
    ASSERT_EQ(hex(output.signature()), "4b4b12b5fd091d5343335f14ac90bf23ea9a8167d648dd9d10d00c9c9b24731c484937bf133e5010f0338fb70a679a9a2eca8b945574005bc4015b419a68897300");
}

TEST(TronSigner, SignUnFreezeAsset) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& unfreeze = *transaction.mutable_unfreeze_asset();
    unfreeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "432bd5cf77ff134787712724709a672fc6e51763de00292438db02d23931e13d");
    ASSERT_EQ(hex(output.signature()), "f493d8f275538a50bb8a832d759df9cad535bb2c5cc73296b04983f551d8398b6d7a30fc0fdfd73e8a9cac77a1a6a9435dc6309bb98fbb219035e88809a0b65901");
}

TEST(TronSigner, SignWithdrawBalance) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& unfreeze = *transaction.mutable_withdraw_balance();
    unfreeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "69aaa954dcd61f28a6a73e979addece6e36541522e5b3374b18b4ef9bc3de4cb");
    ASSERT_EQ(hex(output.signature()), "cb7d23a5eb23284a25ba6deaa231de0f18d8d103592e3312bff101a4219a3e02167eca24b3f4ce78b34f0c1842b6f7fb8d813f530c4c54342cdedef9f8e1f85100");
}

TEST(TronSigner, SignVoteAsset) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& vote = *transaction.mutable_vote_asset();
    vote.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    vote.add_vote_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    vote.set_support(true);
    vote.set_count(1);

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "59b5736fb9756124f9470e4fadbcdafdc8c970da7157fa0ad34a41559418bf0a");
    ASSERT_EQ(hex(output.signature()), "501e04b08f359116a26d9ec784abc50830f92a9dc05d2c1aceefe0eba79466d2730b63b6739edf0f1f1972181618b201ce0b4167d14a66abf40eba4097c39ec400");
}

TEST(TronSigner, SignVoteWitness) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& vote_witness = *transaction.mutable_vote_witness();
    vote_witness.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    vote_witness.set_support(true);

    auto& vote = *vote_witness.add_votes();
    vote.set_vote_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    vote.set_vote_count(3);

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "3f923e9dd9571a66624fafeda27baa3e00aba1709d3fdc5c97c77b81fda18c1f");
    ASSERT_EQ(hex(output.signature()), "79ec1073ae1319ef9303a2f5a515876cfd67f8f0e155bdbde1115d391c05358a3c32f148bfafacf07e1619aaed728d9ffbc2c7e4a5046003c7b74feb86fc68e400");
}

TEST(TronSigner, SignTriggerSmartContract) {
    auto input = Proto::SigningInput();
    auto data = parse_hex("736f6d652064617461");
    auto& transaction = *input.mutable_transaction();
    auto& trigger_contract = *transaction.mutable_trigger_smart_contract();
    trigger_contract.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    trigger_contract.set_contract_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    trigger_contract.set_call_value(0);
    trigger_contract.set_call_token_value(10000);
    trigger_contract.set_token_id(1);
    trigger_contract.set_data(data.data(), data.size());

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "9927d3daae10ad001b25ef3c1bb03073c928cc0e0823f6f3ce404c2b03ce3570");
    ASSERT_EQ(hex(output.signature()), "21a99aafeabdddfdfae86538df048d120a83eb36bbcf5656595919ba6afddacd0a07d0ba051ae80337613174b109f36cb583b6e46ee5aecf6ffe3392fdbb8a2a01");
}

TEST(TronSigner, SignTransferTrc20Contract) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();
    auto& transfer_contract = *transaction.mutable_transfer_trc20_contract();
    transfer_contract.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    transfer_contract.set_contract_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    transfer_contract.set_to_address("TW1dU4L3eNm7Lw8WvieLKEHpXWAussRG9Z");
    Data amount = store(uint256_t(1000));
    transfer_contract.set_amount(std::string(amount.begin(), amount.end()));

    transaction.set_timestamp(1539295479000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "0d644290e3cf554f6219c7747f5287589b6e7e30e1b02793b48ba362da6a5058");
    ASSERT_EQ(hex(output.signature()), "bec790877b3a008640781e3948b070740b1f6023c29ecb3f7b5835433c13fc5835e5cad3bd44360ff2ddad5ed7dc9d7dee6878f90e86a40355b7697f5954b88c01");
}
} // namespace TW::Tron
