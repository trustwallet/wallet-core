use convert_case::{Case, Casing};
use heck::ToLowerCamelCase;

pub(super) fn pretty_name(name: String) -> String {
    name.replace("_", "").replace("TW", "").replace("Proto", "")
}
pub(super) fn pretty_func_name(name: &str, object_name: &str) -> String {
    let pretty_name = name
        .strip_prefix(object_name)
        // Panicking implies bug, checked at the start of the loop.
        .unwrap()
        .to_lower_camel_case();

    if object_name == "TWStoredKey" {
        pretty_name
            .replace("Json", "JSON")
            .replace("Hd", "HD")
    } else if object_name == "TWPublicKey" {
        pretty_name
            .replace("Der", "DER")
    } else if object_name == "TWHash" {
        pretty_name
            .replace("ripemd", "RIPEMD")
            .replace("Ripemd", "RIPEMD")
            .replace("sha512256", "sha512_256")
            .replace("sha3256", "sha3_256")
            .replace("sha256sha256", "sha256SHA256")
    } else if object_name == "TWAES" {
        pretty_name
            .replace("Cbc", "CBC")
            .replace("Ctr", "CTR")
    } else {
        pretty_name
    }
}

pub(super) fn pretty_file_name(name: String) -> String {
    let new_name = name
        .replace("+", "_")
        .replace("TW", "")
        .replace("Proto", "");

    new_name.to_case(Case::Snake)
}
