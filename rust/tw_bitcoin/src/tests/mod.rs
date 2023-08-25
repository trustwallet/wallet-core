mod address;
mod brc20;
mod data;
mod free_estimate;
mod ordinal_nft;
mod p2tr_key_path;
mod p2wpkh;
mod send_to_address;
mod legacy_scripts;
mod legacy_build_sign;

const ONE_BTC: u64 = 100_000_000;

fn hex(string: &str) -> Vec<u8> {
    tw_encoding::hex::decode(string).unwrap()
}
