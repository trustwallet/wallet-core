// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Interfacing to wallet-core: interface methods, type utilites, wrappers

use libc::c_char;
use std::ffi::CString;
use std::ffi::CStr;
use hex::ToHex;

// signatures
extern "C" {
    fn TWStringCreateWithUTF8Bytes(twstring: *const c_char) -> *const u8;
    fn TWStringUTF8Bytes(twstring: *const u8) -> *const c_char;

    fn TWDataCreateWithBytes(bytes: *const u8, size: usize) -> *const u8;
    fn TWDataSize(data: *const u8) -> usize;
    fn TWDataBytes(data: *const u8) -> *const u8;

    fn TWPrivateKeyData(private_key: *const u8) -> *const u8;
    fn TWPrivateKeyGetPublicKeySecp256k1(private_key: *const u8, compressed: bool) -> *const u8;

    fn TWPublicKeyData(private_key: *const u8) -> *const u8;

    fn TWHDWalletCreateWithMnemonic(mnemonic: *const u8, passphrase: *const u8) -> *const u8;
    fn TWHDWalletGetAddressForCoin(wallet: *const u8, coin: u32) -> *const u8;
    fn TWHDWalletGetKeyForCoin(wallet: *const u8, coin: u32) -> *const u8;

    fn TWAnySignerSign(input: *const u8, coin: u32) -> *const u8;

    fn TWMnemonicIsValid(mnemonic: *const u8) -> bool;
}

// type utilities
pub struct TWString {
    wrapped: *const u8
    // TODO delete when destructing with TWStringDelete
}

pub trait FromString {
    fn from_str(s: &str) -> Self;
}

pub trait ToVec {
    fn to_vec(self: &Self) -> Vec<u8>;
}

pub trait FromVec {
    fn from_vec(vec: &Vec<u8>) -> Self;
}

pub trait ToHexString {
    fn to_hex(self: &Self) -> String;
}

impl FromString for TWString {
    fn from_str(s: &str) -> Self {
        let cstring = CString::new(s).unwrap();
        let ptr = unsafe { TWStringCreateWithUTF8Bytes(cstring.as_ptr()) };
        TWString { wrapped: ptr }
    }
}

impl ToString for TWString {
    fn to_string(&self) -> String {
        let s1 = unsafe { TWStringUTF8Bytes(self.wrapped) };
        let c_str: &CStr = unsafe { CStr::from_ptr(s1) };
        let str_slice: &str = c_str.to_str().unwrap();
        str_slice.to_owned()
    }
}

pub struct TWData {
    wrapped: *const u8
    // TODO delete when destructing
}

fn tw_data_size(data: &TWData) -> usize {
    unsafe { TWDataSize(data.wrapped) }
}

impl ToVec for TWData {
    fn to_vec(&self) -> Vec<u8> {
        let size = tw_data_size(self);
        let ptr = unsafe { TWDataBytes(self.wrapped) };
        let slice: &[u8] = unsafe { std::slice::from_raw_parts(ptr, size) };
        slice.to_vec()
    }
}

impl FromVec for TWData {
    fn from_vec(v: &Vec<u8>) -> Self {
        let ptr = unsafe { TWDataCreateWithBytes(v.as_ptr(), v.len()) };
        TWData { wrapped: ptr }
    }
}

impl ToHexString for TWData {
    fn to_hex(&self) -> String {
        self.to_vec().encode_hex::<String>()
    }
}

pub struct PrivateKey {
    wrapped: *const u8
    // TODO delete when destructing
}

pub fn private_key_data(private_key: &PrivateKey) -> TWData {
    let ptr = unsafe { TWPrivateKeyData(private_key.wrapped) };
    TWData { wrapped: ptr }
}

pub fn private_key_get_public_key_secp256k1(private_key: &PrivateKey, compressed: bool) -> PublicKey {
    let ptr = unsafe { TWPrivateKeyGetPublicKeySecp256k1(private_key.wrapped, compressed) };
    PublicKey { wrapped: ptr }
}

pub struct PublicKey {
    wrapped: *const u8
    // TODO delete when destructing
}

pub fn public_key_data(public_key: &PublicKey) -> TWData {
    let ptr = unsafe { TWPublicKeyData(public_key.wrapped) };
    TWData { wrapped: ptr }
}

pub struct HDWallet {
    wrapped: *const u8
    // TODO delete when destructing
}

// wrappers
pub fn hd_wallet_create_with_mnemonic(mnemonic: &TWString, passphrase: &TWString) -> HDWallet {
    let ptr = unsafe { TWHDWalletCreateWithMnemonic(mnemonic.wrapped, passphrase.wrapped) };
    HDWallet { wrapped: ptr }
}

pub fn hd_wallet_get_address_for_coin(wallet: &HDWallet, coin: u32) -> TWString {
    let ptr = unsafe { TWHDWalletGetAddressForCoin(wallet.wrapped, coin) };
    TWString { wrapped: ptr }
}

pub fn hd_wallet_get_key_for_coin(wallet: &HDWallet, coin: u32) -> PrivateKey {
    let ptr = unsafe { TWHDWalletGetKeyForCoin(wallet.wrapped, coin) };
    PrivateKey { wrapped: ptr }
}

pub fn any_signer_sign(input: &TWData, coin: u32) -> TWData {
    let ptr = unsafe { TWAnySignerSign(input.wrapped, coin) };
    TWData { wrapped: ptr }
}

pub fn mnemonic_is_valid(mnemonic: &TWString) -> bool {
    unsafe { TWMnemonicIsValid(mnemonic.wrapped) }
}
