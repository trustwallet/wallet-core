// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use tw_coin_entry::error::prelude::ResultContext;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;

const HAS_CRC32: bool = true;

pub struct AddressConverter;

impl AddressConverter {
    /// Converts a TON user address into a single root Cell.
    pub fn convert_to_cell(addr: &TonAddress) -> CellResult<Cell> {
        let mut builder = CellBuilder::new();
        builder
            .store_address(addr)
            .context("Error storing given address to CellBuilder")?;
        builder.build()
    }

    /// Converts a TON user address into a Bag of Cells (BoC) with a single root Cell.
    /// The function is mostly used to request a Jetton user address via `get_wallet_address` RPC.
    /// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
    pub fn convert_to_boc(addr: &TonAddress) -> CellResult<BagOfCells> {
        Self::convert_to_cell(addr).map(BagOfCells::from_root)
    }

    /// Converts a TON user address into a Bag of Cells (BoC) with a single root Cell.
    /// The function is mostly used to request a Jetton user address via `get_wallet_address` RPC.
    /// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
    pub fn convert_to_boc_base64(addr: &TonAddress) -> CellResult<String> {
        Self::convert_to_boc(addr).and_then(|boc| boc.to_base64(HAS_CRC32))
    }

    /// Parses a TON address from a single root Cell.
    pub fn parse_from_cell(cell: &Cell) -> CellResult<TonAddress> {
        cell.parse_fully(|parser| parser.load_address())
            .map(TonAddress::with_address_data)
    }

    /// Parses a TON address from a Bag of Cells (BoC) with a single root Cell.
    /// The function is mostly used to parse a Jetton user address received on `get_wallet_address` RPC.
    /// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
    pub fn parse_from_boc(boc: &BagOfCells) -> CellResult<TonAddress> {
        boc.single_root()
            .and_then(|cell| Self::parse_from_cell(cell))
    }

    /// Parses a TON address from a Bag of Cells (BoC) with a single root Cell.
    /// The function is mostly used to parse a Jetton user address received on `get_wallet_address` RPC.
    /// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
    pub fn parse_from_boc_base64(boc: &str) -> CellResult<TonAddress> {
        BagOfCells::parse_base64(boc).and_then(|boc| Self::parse_from_boc(&boc))
    }
}
