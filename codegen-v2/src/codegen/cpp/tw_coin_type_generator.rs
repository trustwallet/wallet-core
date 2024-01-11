// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::cpp_include_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::Result;
use std::path::PathBuf;

const TW_COIN_TYPE_END: &str = "end_of_tw_coin_type_marker_do_not_modify";

pub fn tw_coin_type_path() -> PathBuf {
    cpp_include_directory().join("TWCoinType.h")
}

/// Represents `TWCoinType.h`.
pub struct TWCoinTypeGenerator;

impl TWCoinTypeGenerator {
    pub fn generate_coin_type_variant(coin: &CoinItem) -> Result<()> {
        let coin_type = coin.coin_type();
        let coin_id_number = coin.coin_id_number;
        let tw_coin_type_file_path = tw_coin_type_path();

        println!("[EDIT] {tw_coin_type_file_path:?}");
        let mut tw_coin_type_rs = FileContent::read(tw_coin_type_file_path)?;

        {
            let mut enum_region =
                tw_coin_type_rs.rfind_line(|line| line.contains(TW_COIN_TYPE_END))?;
            enum_region.push_line_before(format!("    TWCoinType{coin_type} = {coin_id_number},"));
        }

        tw_coin_type_rs.write()
    }
}
