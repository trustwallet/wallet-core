// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{Error, Result};
use std::fs;
use std::path::PathBuf;

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
            to_replace: Vec::default(),
            replace_with: Vec::default(),
        }
    }

    pub fn write_to(mut self, write_to: PathBuf) -> TemplateGenerator {
        self.write_to = Some(write_to);
        self
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

    pub fn replace_all(self) -> Result<()> {
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
