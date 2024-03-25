// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::registry::CoinItem;
use crate::{current_year, Error, Result};
use std::fs;
use std::path::PathBuf;

const PATTERNS_CAPACITY: usize = 20;

pub struct TemplateGenerator {
    template_content: &'static str,
    write_to: Option<PathBuf>,
    to_replace: Vec<String>,
    replace_with: Vec<String>,
}

impl TemplateGenerator {
    pub fn new(template_content: &'static str) -> TemplateGenerator {
        TemplateGenerator {
            template_content,
            write_to: None,
            to_replace: Vec::with_capacity(PATTERNS_CAPACITY),
            replace_with: Vec::with_capacity(PATTERNS_CAPACITY),
        }
    }

    pub fn write_to(mut self, write_to: PathBuf) -> TemplateGenerator {
        self.write_to = Some(write_to);
        self
    }

    /// Use default patterns.
    pub fn with_default_patterns(self, coin: &CoinItem) -> TemplateGenerator {
        self.add_pattern("{YEAR}", current_year())
            .add_pattern("{BLOCKCHAIN}", coin.blockchain_type())
            .add_pattern("{TW_CRATE_NAME}", coin.id.to_tw_crate_name())
            .add_pattern("{COIN_ID}", coin.id.as_str())
            .add_pattern("{COIN_TYPE}", coin.coin_type())
            .add_pattern("{COIN_NAME}", if coin.display_name.len() > 0 { &coin.display_name } else { &coin.name })
            .add_pattern("{SYMBOL}", &coin.symbol)
            .add_pattern("{DECIMALS}", coin.decimals)
            .add_pattern("{P2PKH_PREFIX}", coin.p2pkh_prefix)
            .add_pattern("{P2SH_PREFIX}", coin.p2sh_prefix)
            .add_pattern("{HRP}", coin.hrp.as_str())
            .add_pattern("{STATIC_PREFIX}", coin.static_prefix)
            .add_pattern("{EXPLORER_URL}", &coin.explorer.url)
            .add_pattern("{EXPLORER_TX_PATH}", &coin.explorer.tx_path)
            .add_pattern("{EXPLORER_ACCOUNT_PATH}", &coin.explorer.account_path)
            .add_pattern("{EXPLORER_SAMPLE_TX}", &coin.explorer.sample_tx)
            .add_pattern("{EXPLORER_SAMPLE_ACCOUNT}", &coin.explorer.sample_account)
    }

    pub fn add_pattern<K: ToString, V: ToString>(
        mut self,
        to_replace: K,
        replace_with: V,
    ) -> TemplateGenerator {
        self.to_replace.push(to_replace.to_string());
        self.replace_with.push(replace_with.to_string());
        self
    }

    pub fn write(self) -> Result<()> {
        let write_to_path = self.write_to.ok_or_else(|| {
            Error::io_error_other("Incorrect use of 'TemplateGenerator'".to_string())
        })?;
        let file_to_write = fs::File::create(write_to_path)?;

        aho_corasick::AhoCorasick::new(self.to_replace)
            .map_err(|e| Error::io_error_other(format!("Invalid patterns: {e}")))?
            .try_stream_replace_all(
                self.template_content.as_bytes(),
                file_to_write,
                &self.replace_with,
            )
            .map_err(Error::from)
    }
}
