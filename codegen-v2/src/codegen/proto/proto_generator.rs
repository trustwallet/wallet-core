// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::proto::proto_source_directory;
use crate::codegen::template_generator::TemplateGenerator;
use crate::registry::CoinItem;
use crate::Result;
use std::path::PathBuf;

const PROTO_TEMPLATE: &str = include_str!("templates/Blockchain.proto");

pub fn blockchain_proto_path(coin: &CoinItem) -> PathBuf {
    let blockchain_type = coin.blockchain_type();
    proto_source_directory().join(format!("{blockchain_type}.proto"))
}

pub struct ProtoGenerator;

impl ProtoGenerator {
    pub fn generate(coin: &CoinItem) -> Result<()> {
        let proto_path = blockchain_proto_path(coin);

        if proto_path.exists() {
            return Ok(());
        }

        TemplateGenerator::new(PROTO_TEMPLATE)
            .write_to(proto_path)
            .with_default_patterns(coin)
            .write()?;

        Ok(())
    }
}
