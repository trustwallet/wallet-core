// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::sync::Arc;
use tw_ton_sdk::address::address_data::AddressData;
use tw_ton_sdk::address::raw_address::RawAddress;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::message::state_init::StateInit;

pub fn comment_cell(comment: &str) -> String {
    let mut builder = CellBuilder::new();
    builder
        .store_u32(32, 0)
        .unwrap()
        .store_string(comment)
        .unwrap();
    let cell = builder.build().unwrap();
    BagOfCells::from_root(cell).to_base64(true).unwrap()
}

/// Doge chatbot example
/// https://docs.ton.org/develop/dapps/ton-connect/transactions#smart-contract-deployment
pub fn doge_chatbot_state_init(current_timestamp: u32) -> String {
    let current_timestamp_ms = current_timestamp as u64 * 1000;

    let doge_chatbot_boc = BagOfCells::parse_base64("te6cckEBAgEARAABFP8A9KQT9LzyyAsBAGrTMAGCCGlJILmRMODQ0wMx+kAwi0ZG9nZYcCCAGMjLBVAEzxaARfoCE8tqEssfAc8WyXP7AN4uuM8=").unwrap();
    let doge_chatbot_cell = doge_chatbot_boc.single_root().unwrap();

    let mut data_builder = CellBuilder::new();
    // Current timestamp makes this Doge chatbot unique across other bots with the same contract code.
    data_builder.store_u64(64, current_timestamp_ms).unwrap();
    let data = data_builder.build().unwrap();

    let state_init_cell = StateInit::default()
        .set_code(Arc::clone(doge_chatbot_cell))
        .set_data(data.into_arc())
        .to_cell()
        .unwrap();

    BagOfCells::from_root(state_init_cell)
        .to_base64(true)
        .unwrap()
}

pub fn contract_address_from_state_init(state_init: &str) -> String {
    let state_init_boc = BagOfCells::parse_base64(state_init).unwrap();
    let state_init_cell = state_init_boc.single_root().unwrap();
    let state_init_hash = state_init_cell.cell_hash();
    RawAddress::from(AddressData::new(0, state_init_hash)).to_string()
}
