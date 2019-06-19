// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>

/// https://github.com/neo-project/neo-vm/blob/master/src/neo-vm/OpCode.cs
namespace TW::NEO {

/// An empty array of bytes is pushed onto the stack.
static const uint8_t PUSH0{0x00};
static const uint8_t PUSHF{PUSH0};
static const uint8_t PUSH_BYTE_33{0x21};
/// The next byte contains the number of bytes to be pushed onto the stack.
static const uint8_t PUSH_DATA1{0x4C};
/// The next two bytes contain the number of bytes to be pushed onto the stack.
static const uint8_t PUSH_DATA2{0x4D};
/// The next four bytes contain the number of bytes to be pushed onto the stack.
static const uint8_t PUSH_DATA4{0x4E};
/// The publickey and signature are taken from main stack. Verifies if transaction was signed by given publickey and a boolean output is put on top of the main stack.
static const uint8_t CHECK_SIG{0xAC};
/// A set of n public keys (an array or value n followed by n pubkeys) is validated against a set of m signatures (an array or value m followed by m signatures). Verify transaction as multisig and a boolean output is put on top of the main stack.
static const uint8_t CHECK_MULTI_SIG{0xAE};
/// A value n is taken from top of main stack. The next n items on main stack are removed, put inside n-sized array and this array is put on top of the main stack.
static const uint8_t PACK{0xC1};
/// A value n is taken from top of main stack. A zero-filled struct type with size n is put on top of the main stack.
static const uint8_t NEW_STRUCT{0xC6};
/// Reads a string and executes the corresponding operation.
static const uint8_t SYS_CALL{0x68};
/// Duplicates the item on top of alt stack and put it on top of main stack.
static const uint8_t DUP_FROM_ALT_STACK{0x6A};
/// Puts the input onto the top of the alt stack. Removes it from the main stack.
static const uint8_t TO_ALT_STACK{0x6B};
/// Puts the input onto the top of the main stack. Removes it from the alt stack.
static const uint8_t FROM_ALT_STACK{0x6C};
/// The top two items on the stack are swapped.
static const uint8_t SWAP{0x7C};
/// The item on top of main stack is removed and appended to the second item on top of the main stack.
static const uint8_t APPEND{0xC8};
/// An input index n (or key) and an array (or map) are removed from the top of the main stack. Puts True on top of main stack if array[n] (or map[n]) exist, and False otherwise.
static const uint8_t HAS_KEY{0xCB};

/// 0x01-0x4B The next opcode bytes is data to be pushed onto the stack
static const uint8_t PUSHBYTES1{0x01};
static const uint8_t PUSHBYTES75{0x4B};
/// The number -1 is pushed onto the stack.
static const uint8_t PUSHM1{0x4F};
/// The number 1 is pushed onto the stack.
static const uint8_t PUSH1{0x51};
static const uint8_t PUSHT{PUSH1};
/// The number 2 is pushed onto the stack.
static const uint8_t PUSH2{0x52};
/// The number 3 is pushed onto the stack.
static const uint8_t PUSH3{0x53};
/// The number 4 is pushed onto the stack.
static const uint8_t PUSH4{0x54};
/// The number 5 is pushed onto the stack.
static const uint8_t PUSH5{0x55};
/// The number 6 is pushed onto the stack.
static const uint8_t PUSH6{0x56};
/// The number 7 is pushed onto the stack.
static const uint8_t PUSH7{0x57};
/// The number 8 is pushed onto the stack.
static const uint8_t PUSH8{0x58};
/// The number 9 is pushed onto the stack.
static const uint8_t PUSH9{0x59};
/// The number 10 is pushed onto the stack.
static const uint8_t PUSH10{0x5A};
/// The number 11 is pushed onto the stack.
static const uint8_t PUSH11{0x5B};
/// The number 12 is pushed onto the stack.
static const uint8_t PUSH12{0x5C};
/// The number 13 is pushed onto the stack.
static const uint8_t PUSH13{0x5D};
/// The number 14 is pushed onto the stack.
static const uint8_t PUSH14{0x5E};
/// The number 15 is pushed onto the stack.
static const uint8_t PUSH15{0x5F};
/// The number 16 is pushed onto the stack.
static const uint8_t PUSH16{0x60};

// Flow control
///  Does nothing.
static const uint8_t NOP{0x61};
/// Reads a 2-byte value n and a jump is performed to relative position n-3.
static const uint8_t JMP{0x62};
/// A boolean value b is taken from main stack and reads a 2-byte value n, if b is True then a jump is performed to relative position n-3.
static const uint8_t JMPIF{0x63};
/// A boolean value b is taken from main stack and reads a 2-byte value n, if b is False then a jump is performed to relative position n-3.
static const uint8_t JMPIFNOT{0x64};
/// Current context is copied to the invocation stack. Reads a 2-byte value n and a jump is performed to relative position n-3.
static const uint8_t CALL{0x65};
/// Stops the execution if invocation stack is empty.
static const uint8_t RET{0x66};
/// Reads a scripthash and executes the corresponding contract. If scripthash is zero, performs dynamic invoke by taking scripthash from main stack.
static const uint8_t APPCALL{0x67};
/// Reads a scripthash and executes the corresponding contract. If scripthash is zero, performs dynamic invoke by taking scripthash from main stack. Disposes the top item on invocation stack.
static const uint8_t TAILCALL{0x69};


// Stack
/// The item n back in the main stack is removed.
static const uint8_t XDROP{0x6D};
/// The item n back in the main stack in swapped with top stack item.
static const uint8_t XSWAP{0x72};
/// The item on top of the main stack is copied and inserted to the position n in the main stack.
static const uint8_t XTUCK{0x73};
/// Puts the number of stack items onto the stack.
static const uint8_t DEPTH{0x74};
/// Removes the top stack item.
static const uint8_t DROP{0x75};
/// Duplicates the top stack item.
static const uint8_t DUP{0x76};
/// Removes the second-to-top stack item.
static const uint8_t NIP{0x77};
/// Copies the second-to-top stack item to the top.
static const uint8_t OVER{0x78};
/// The item n back in the stack is copied to the top.
static const uint8_t PICK{0x79};
/// The item n back in the stack is moved to the top.
static const uint8_t ROLL{0x7A};
/// The top three items on the stack are rotated to the left.
static const uint8_t ROT{0x7B};
/// The item at the top of the stack is copied and inserted before the second-to-top item.
static const uint8_t TUCK{0x7D};


// Splice
/// Concatenates two strings.
static const uint8_t CAT{0x7E};
/// Returns a section of a string.
static const uint8_t SUBSTR{0x7F};
/// Keeps only characters left of the specified point in a string.
static const uint8_t LEFT{0x80};
/// Keeps only characters right of the specified point in a string.
static const uint8_t RIGHT{0x81};
/// Returns the length of the input string.
static const uint8_t SIZE{0x82};


// Bitwise logic
/// Flips all of the bits in the input.
static const uint8_t INVERT{0x83};
/// Boolean and between each bit in the inputs.
static const uint8_t AND{0x84};
/// Boolean or between each bit in the inputs.
static const uint8_t OR{0x85};
/// Boolean exclusive or between each bit in the inputs.
static const uint8_t XOR{0x86};
/// Returns 1 if the inputs are exactly equal, 0 otherwise.
static const uint8_t EQUAL{0x87};
// Same as OP_EQUAL, but runs OP_VERIFY afterward.
static const uint8_t OP_EQUALVERIFY{0x88};
// Transaction is invalid unless occuring in an unexecuted OP_IF branch
static const uint8_t OP_RESERVED1{0x89};
// Transaction is invalid unless occuring in an unexecuted OP_IF branch
static const uint8_t OP_RESERVED2{0x8A};


// Arithmetic
// Note: Arithmetic inputs are limited to signed 32-bit integers, but may overflow their output.
/// 1 is added to the input.
static const uint8_t INC{0x8B};
/// 1 is subtracted from the input.
static const uint8_t DEC{0x8C};
/// Puts the sign of top stack item on top of the main stack. If value is negative, put -1; if positive, put 1; if value is zero, put 0.
static const uint8_t SIGN{0x8D};
/// The sign of the input is flipped.
static const uint8_t NEGATE{0x8F};
/// The input is made positive.
static const uint8_t ABS{0x90};
/// If the input is 0 or 1, it is flipped. Otherwise the output will be 0.
static const uint8_t NOT{0x91};
/// Returns 0 if the input is 0. 1 otherwise.
static const uint8_t NZ{0x92};
/// a is added to b.
static const uint8_t ADD{0x93};
/// b is subtracted from a.
static const uint8_t SUB{0x94};
/// a is multiplied by b.
static const uint8_t MUL{0x95};
/// a is divided by b.
static const uint8_t DIV{0x96};
/// Returns the remainder after dividing a by b.
static const uint8_t MOD{0x97};
/// Shifts a left b bits, preserving sign.
static const uint8_t SHL{0x98};
/// Shifts a right b bits, preserving sign.
static const uint8_t SHR{0x99};
/// If both a and b are not 0, the output is 1. Otherwise 0.
static const uint8_t BOOLAND{0x9A};
/// If a or b is not 0, the output is 1. Otherwise 0.
static const uint8_t BOOLOR{0x9B};
/// Returns 1 if the numbers are equal, 0 otherwise.
static const uint8_t NUMEQUAL{0x9C};
/// Returns 1 if the numbers are not equal, 0 otherwise.
static const uint8_t NUMNOTEQUAL{0x9E};
/// Returns 1 if a is less than b, 0 otherwise.
static const uint8_t LT{0x9F};
/// Returns 1 if a is greater than b, 0 otherwise.
static const uint8_t GT{0xA0};
/// Returns 1 if a is less than or equal to b, 0 otherwise.
static const uint8_t LTE{0xA1};
/// Returns 1 if a is greater than or equal to b, 0 otherwise.
static const uint8_t GTE{0xA2};
/// Returns the smaller of a and b.
static const uint8_t MIN{0xA3};
/// Returns the larger of a and b.
static const uint8_t MAX{0xA4};
/// Returns 1 if x is within the specified range (left-inclusive), 0 otherwise.
static const uint8_t WITHIN{0xA5};


// Crypto
//static const uint8_t RIPEMD160{0xA6}; // The input is hashed using RIPEMD-160.
/// The input is hashed using SHA-1.
static const uint8_t SHA1{0xA7};
/// The input is hashed using SHA-256.
static const uint8_t SHA256{0xA8};
/// The input is hashed using Hash160: first with SHA-256 and then with RIPEMD-160.
static const uint8_t HASH160{0xA9};
/// The input is hashed using Hash256: twice with SHA-256.
static const uint8_t HASH256{0xAA};
/// The publickey, signature and message are taken from main stack. Verifies if given message was signed by given publickey and a boolean output is put on top of the main stack.
static const uint8_t VERIFY{0xAD};

// Array
/// An array is removed from top of the main stack. Its size is put on top of the main stack.
static const uint8_t ARRAYSIZE{0xC0};
/// An array is removed from top of the main stack. Its elements are put on top of the main stack (in reverse order) and the array size is also put on main stack.
static const uint8_t UNPACK{0xC2};
/// An input index n (or key) and an array (or map) are taken from main stack. Element array[n] (or map[n]) is put on top of the main stack.
static const uint8_t PICKITEM{0xC3};
/// A value v, index n (or key) and an array (or map) are taken from main stack. Attribution array[n]=v (or map[n]=v) is performed.
static const uint8_t SETITEM{0xC4};
///A value n is taken from top of main stack. A zero-filled array type with size n is put on top of the main stack.
static const uint8_t NEWARRAY{0xC5};
/// A Map is created and put on top of the main stack.
static const uint8_t NEWMAP{0xC7};
/// An array is removed from the top of the main stack and its elements are reversed.
static const uint8_t REVERSE{0xC9};
/// An input index n (or key) and an array (or map) are removed from the top of the main stack. Element array[n] (or map[n]) is removed.
static const uint8_t REMOVE{0xCA};
/// A map is taken from top of the main stack. The keys of this map are put on top of the main stack.
static const uint8_t KEYS{0xCC};
/// A map is taken from top of the main stack. The values of this map are put on top of the main stack.
static const uint8_t VALUES{0xCD};


// Stack isolation
/// Current context is copied to the invocation stack (similar to CALL). Read return count (1-byte), parameter count (1-byte) and a 2-byte value n for jump to be performed.
static const uint8_t CALL_I{0xE0};
/// Reads a scripthash and executes the corresponding contract (similar to APPCALL). Read return count (1-byte), parameter count (1-byte) and a 20-byte scripthash.
static const uint8_t CALL_E{0xE1};
/// Reads a scripthash and executes the corresponding contract (similar to dynamic invoke APPCALL). Read return count (1-byte) and parameter count (1-byte). The 20-byte scripthash is pop from main stack.
static const uint8_t CALL_ED{0xE2};
/// Reads a scripthash and executes the corresponding contract (similar to TAILCALL). Read return count (1-byte), parameter count (1-byte) and a 20-byte scripthash. Disposes the top item on invocation stack.
static const uint8_t CALL_ET{0xE3};
/// Reads a scripthash and executes the corresponding contract (similar to dynamic invoke TAILCALL). Read return count (1-byte) and parameter count (1-byte). The 20-byte scripthash is pop from main stack. Disposes the top item on invocation stack.
static const uint8_t CALL_EDT{0xE4};


// Exceptions
/// Halts the execution of the vm by setting VMState.FAULT.
static const uint8_t THROW{0xF0};
/// Removes top stack item n, and halts the execution of the vm by setting VMState.FAULT only if n is False.
static const uint8_t THROWIFNOT{0xF1};

} // namespace TW::NEO
