// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "proto/Pactus.pb.h"
#include "proto/TransactionCompiler.pb.h"

#include <string>

namespace TransferTransaction1 {

static TW::Pactus::Proto::SigningInput createSigningInput() {
    TW::Pactus::Proto::SigningInput input;

    // Set up the transaction message
    TW::Pactus::Proto::TransactionMessage* trx = input.mutable_transaction();
    trx->set_lock_time(0x00030201);
    trx->set_fee(1000);
    trx->set_memo("test");

    // Set up the transfer payload
    TW::Pactus::Proto::TransferPayload* pld = trx->mutable_transfer();
    pld->set_sender("pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr");
    pld->set_receiver("pc1r0g22ufzn8qtw0742dmfglnw73e260hep0k3yra");
    pld->set_amount(20000);

    return input;
}

const std::string PRIVATE_KEY_HEX = "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6";
const std::string PRE_HASH =
    "b5e97db07fa0bd0e5598aa3643a9bc6f6693bddc1a9fec9e674a461eaa00b19307968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3063000000000000000200000000000000000000000000000000000000000000000000000000000000010d6170746f735f6163636f756e74087472616e7366657200022007968dab936c1bad187c60ce4082f307d030d780e91e694ae03aef16aba73f3008e803000000000000fe4d3200000000006400000000000000c2276ada0000000021";
const std::string SIGNATURE =
    "50ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd09";
const std::string DATA_TO_SIGN =
    "0101020300e807047465737401037098338e0b6808119dfd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df21a09c01";
const std::string SIGNED_TRANSACTION_DATA =
    "000101020300e807047465737401037098338e0b6808119dfd4457ab806b9c2059b89b037a14ae24533816e7faaa6ed28fcdde8e55a7df21a09c0150ac25c7125271489b0cd230549257c93fb8c6265f2914a988ba7b81c1bc47fff027412dd59447867911035ff69742d171060a1f132ac38b95acc6e39ec0bd0995794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa";
const std::string TRANSACTION_ID = "34cd4656a98f7eb996e83efdc384cefbe3a9c52dca79a99245b4eacc0b0b4311";
} // namespace TransferTransaction1
