// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
pub struct TWCoinType {
    coin: CoinItem,
}

impl TWCoinType {
    pub fn new(coin: CoinItem) -> TWCoinType {
        TWCoinType { coin }
    }

    pub fn add_coin_type_variant(self) -> Result<()> {
        let coin_type = self.coin.coin_type();
        let coin_id_number = self.coin.coin_id_number;

        let mut tw_coin_type_rs = FileContent::read(tw_coin_type_path())?;

        {
            let mut enum_region =
                tw_coin_type_rs.rfind_line(|line| line.contains(TW_COIN_TYPE_END))?;
            enum_region.push_line_before(format!("    TWCoinType{coin_type} = {coin_id_number},"));
        }

        tw_coin_type_rs.write()
    }
}
