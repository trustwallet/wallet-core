mod address;
mod brc20;
mod data;
mod free_estimate;
mod legacy;
mod ordinal_nft;
mod p2tr_key_path;
mod p2wpkh;
mod send_to_address;

fn hex(string: &str) -> Vec<u8> {
    tw_encoding::hex::decode(string).unwrap()
}
