// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

enum OpCode {
    // push value
   OP_0 = 0x00,
   OP_FALSE [[maybe_unused]] = OP_0,
   // Note: values 0x01 -- 0x4b (1--75) mean that the next N bytes are interpreted as data pushed into the stack
   OP_PUSHDATA1 = 0x4c,
   OP_PUSHDATA2 = 0x4d,
   OP_PUSHDATA4 = 0x4e,
   OP_1NEGATE [[maybe_unused]] = 0x4f,
   OP_RESERVED [[maybe_unused]] = 0x50,
   OP_1 = 0x51,
   OP_TRUE [[maybe_unused]] = OP_1,
   OP_2 [[maybe_unused]] = 0x52,
   OP_3 = 0x53,
   OP_4 [[maybe_unused]] = 0x54,
   OP_5 [[maybe_unused]] = 0x55,
   OP_6 [[maybe_unused]] = 0x56,
   OP_7 [[maybe_unused]] = 0x57,
   OP_8 [[maybe_unused]] = 0x58,
   OP_9 = 0x59,
   OP_10 [[maybe_unused]] = 0x5a,
   OP_11 [[maybe_unused]] = 0x5b,
   OP_12 [[maybe_unused]] = 0x5c,
   OP_13 [[maybe_unused]] = 0x5d,
   OP_14 [[maybe_unused]] = 0x5e,
   OP_15 [[maybe_unused]] = 0x5f,
   OP_16 = 0x60,

    // control
   OP_NOP [[maybe_unused]] = 0x61,
   OP_VER [[maybe_unused]] = 0x62,
   OP_IF [[maybe_unused]] = 0x63,
   OP_NOTIF [[maybe_unused]] = 0x64,
   OP_VERIF [[maybe_unused]] = 0x65,
   OP_VERNOTIF [[maybe_unused]] = 0x66,
   OP_ELSE [[maybe_unused]] = 0x67,
   OP_ENDIF [[maybe_unused]] = 0x68,
   OP_VERIFY [[maybe_unused]] = 0x69,
   OP_RETURN = 0x6a,

    // stack ops
   OP_TOALTSTACK [[maybe_unused]] = 0x6b,
   OP_FROMALTSTACK [[maybe_unused]] = 0x6c,
   OP_2DROP [[maybe_unused]] = 0x6d,
   OP_2DUP [[maybe_unused]] = 0x6e,
   OP_3DUP [[maybe_unused]] = 0x6f,
   OP_2OVER [[maybe_unused]] = 0x70,
   OP_2ROT [[maybe_unused]] = 0x71,
   OP_2SWAP [[maybe_unused]] = 0x72,
   OP_IFDUP [[maybe_unused]] = 0x73,
   OP_DEPTH [[maybe_unused]] = 0x74,
   OP_DROP [[maybe_unused]] = 0x75,
   OP_DUP = 0x76,
   OP_NIP [[maybe_unused]] = 0x77,
   OP_OVER [[maybe_unused]] = 0x78,
   OP_PICK [[maybe_unused]] = 0x79,
   OP_ROLL [[maybe_unused]] = 0x7a,
   OP_ROT [[maybe_unused]] = 0x7b,
   OP_SWAP [[maybe_unused]] = 0x7c,
   OP_TUCK [[maybe_unused]] = 0x7d,

    // splice ops
   OP_CAT [[maybe_unused]] = 0x7e,
   OP_SUBSTR [[maybe_unused]] = 0x7f,
   OP_LEFT [[maybe_unused]] = 0x80,
   OP_RIGHT [[maybe_unused]] = 0x81,
   OP_SIZE [[maybe_unused]] = 0x82,

    // bit logic
   OP_INVERT [[maybe_unused]] = 0x83,
   OP_AND [[maybe_unused]] = 0x84,
   OP_OR [[maybe_unused]] = 0x85,
   OP_XOR [[maybe_unused]] = 0x86,
   OP_EQUAL = 0x87,
   OP_EQUALVERIFY = 0x88,
   OP_RESERVED1 [[maybe_unused]] = 0x89,
   OP_RESERVED2 [[maybe_unused]] = 0x8a,

    // numeric
   OP_1ADD [[maybe_unused]] = 0x8b,
   OP_1SUB [[maybe_unused]] = 0x8c,
   OP_2MUL [[maybe_unused]] = 0x8d,
   OP_2DIV [[maybe_unused]] = 0x8e,
   OP_NEGATE [[maybe_unused]] = 0x8f,
   OP_ABS [[maybe_unused]] = 0x90,
   OP_NOT [[maybe_unused]] = 0x91,
   OP_0NOTEQUAL [[maybe_unused]] = 0x92,

   OP_ADD [[maybe_unused]] = 0x93,
   OP_SUB [[maybe_unused]] = 0x94,
   OP_MUL [[maybe_unused]] = 0x95,
   OP_DIV [[maybe_unused]] = 0x96,
   OP_MOD [[maybe_unused]] = 0x97,
   OP_LSHIFT [[maybe_unused]] = 0x98,
   OP_RSHIFT [[maybe_unused]] = 0x99,

   OP_BOOLAND [[maybe_unused]] = 0x9a,
   OP_BOOLOR [[maybe_unused]] = 0x9b,
   OP_NUMEQUAL [[maybe_unused]] = 0x9c,
   OP_NUMEQUALVERIFY [[maybe_unused]] = 0x9d,
   OP_NUMNOTEQUAL [[maybe_unused]] = 0x9e,
   OP_LESSTHAN [[maybe_unused]] = 0x9f,
   OP_GREATERTHAN [[maybe_unused]] = 0xa0,
   OP_LESSTHANOREQUAL [[maybe_unused]] = 0xa1,
   OP_GREATERTHANOREQUAL [[maybe_unused]] = 0xa2,
   OP_MIN [[maybe_unused]] = 0xa3,
   OP_MAX [[maybe_unused]] = 0xa4,

   OP_WITHIN [[maybe_unused]] = 0xa5,

    // crypto
   OP_RIPEMD160 [[maybe_unused]] = 0xa6,
   OP_SHA1 [[maybe_unused]] = 0xa7,
   OP_SHA256 [[maybe_unused]] = 0xa8,
   OP_HASH160 = 0xa9,
   OP_HASH256 [[maybe_unused]] = 0xaa,
   OP_CODESEPARATOR [[maybe_unused]] = 0xab,
   OP_CHECKSIG = 0xac,
   OP_CHECKSIGVERIFY [[maybe_unused]] = 0xad,
   OP_CHECKMULTISIG = 0xae,
   OP_CHECKMULTISIGVERIFY [[maybe_unused]] = 0xaf,

    // expansion
   OP_NOP1 [[maybe_unused]] = 0xb0,
   OP_CHECKLOCKTIMEVERIFY = 0xb1,
   OP_NOP2 [[maybe_unused]] = OP_CHECKLOCKTIMEVERIFY,
   OP_CHECKSEQUENCEVERIFY = 0xb2,
   OP_NOP3 [[maybe_unused]] = OP_CHECKSEQUENCEVERIFY,
   OP_NOP4 [[maybe_unused]] = 0xb3,
   OP_NOP5 [[maybe_unused]] = 0xb4,
   OP_CHECKBLOCKATHEIGHT = OP_NOP5,
   OP_NOP6 [[maybe_unused]] = 0xb5,
   OP_NOP7 [[maybe_unused]] = 0xb6,
   OP_NOP8 [[maybe_unused]] = 0xb7,
   OP_NOP9 [[maybe_unused]] = 0xb8,
   OP_NOP10 [[maybe_unused]] = 0xb9,

    // firo, see: https://github.com/firoorg/firo/blob/8bd4abdea223e22f15c36e7d2d42618dc843e2ef/src/script/script.h#L212
   OP_EXCHANGEADDR = 0xe0,

   OP_INVALIDOPCODE [[maybe_unused]] = 0xff,
};

static inline bool TWOpCodeIsSmallInteger(uint8_t opcode) {
    return opcode >= OP_1 && opcode <= OP_16;
}
