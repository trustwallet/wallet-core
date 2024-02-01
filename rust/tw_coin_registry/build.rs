// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use itertools::Itertools;
use serde::Deserialize;
use std::io::Write;
use std::path::PathBuf;
use std::{env, fs};

/// We're only interested in `id` and `name` of the coin to generate `CoinType` enum.
#[derive(Deserialize)]
struct CoinItem {
    #[serde(rename = "coinId")]
    coin_id: u64,
    name: String,
}

/// Transforms a coin name to a Rust name.
/// https://github.com/trustwallet/wallet-core/blob/3769f31b7d0c75126b2f426bb065364429aaa379/codegen/lib/coin_skeleton_gen.rb#L15-L22
fn format_name(name: &str) -> String {
    name.replace([' ', '.', '-'], "")
}

fn generate_coin_type(coins: &[CoinItem]) -> String {
    const RAW_TYPE: &str = "u32";
    const ENUM_NAME: &str = "CoinType";

    let coin_types_variants = coins
        .iter()
        .map(|coin| format!("\t{} = {},\n", format_name(&coin.name), coin.coin_id))
        .join("");

    format!(
        r#"#[allow(non_camel_case_types)]
#[derive(Clone, Copy, Debug, Eq, Hash, PartialEq)]
#[derive(strum_macros::EnumIter, strum_macros::FromRepr)]
#[repr({RAW_TYPE})]
pub enum CoinType {{
{coin_types_variants}
}}

impl {ENUM_NAME} {{
    pub fn iter() -> impl IntoIterator<Item = {ENUM_NAME}> {{
        use strum::IntoEnumIterator;

        <Self as IntoEnumIterator>::iter()
    }}
}}

impl TryFrom<{RAW_TYPE}> for {ENUM_NAME} {{
    type Error = ();

    fn try_from(num: {RAW_TYPE}) -> Result<{ENUM_NAME}, ()> {{
        {ENUM_NAME}::from_repr(num).ok_or(())
    }}
}}

impl<'de> serde::Deserialize<'de> for {ENUM_NAME} {{
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: serde::de::Deserializer<'de>,
    {{
        let num_value: {RAW_TYPE} = {RAW_TYPE}::deserialize(deserializer)?;
        {ENUM_NAME}::try_from(num_value).map_err(|_| serde::de::Error::custom("Invalid `CoinType` value"))
    }}
}}
"#
    )
}

fn generate_and_write_coin_type(coins: &[CoinItem]) {
    let coin_type_path = PathBuf::from(env::var("OUT_DIR").unwrap()).join("coin_type.rs");

    let coin_type_content = generate_coin_type(coins);

    let mut file = fs::OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open(coin_type_path)
        .expect("Error creating/opening coin_type.rs");
    file.write_all(coin_type_content.as_bytes())
        .expect("Error writing coin_type.rs");
}

fn main() {
    let registry_path = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("..")
        .join("registry.json");
    let registry_path_str = registry_path.to_str().unwrap();

    // Re-run this build.rs if the `proto` directory has been changed (i.e. a new file is added).
    println!("cargo:rerun-if-changed={registry_path_str}");

    let registry_bytes = fs::read(registry_path).expect("Error reading registry.json");

    let coins: Vec<CoinItem> =
        serde_json::from_slice(&registry_bytes).expect("registry.json expected to be valid");

    generate_and_write_coin_type(&coins);
}
