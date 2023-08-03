use bitcoin::{PubkeyHash, PublicKey, WPubkeyHash};
use secp256k1::hashes::Hash;
use tw_encoding::hex;

pub fn pubkey_hash_from_hex(hex: &str) -> PubkeyHash {
    PubkeyHash::from_byte_array(hex::decode(hex).unwrap().try_into().unwrap())
}

pub fn witness_pubkey_hash(hex: &str) -> WPubkeyHash {
    WPubkeyHash::from_byte_array(hex::decode(hex).unwrap().try_into().unwrap())
}

pub fn txid_rev(hex: &str) -> Vec<u8> {
    hex::decode(hex).unwrap().into_iter().rev().collect()
}
