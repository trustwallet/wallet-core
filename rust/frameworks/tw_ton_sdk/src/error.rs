// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;

pub type CellResult<T> = Result<T, CellError>;
pub type CellError = TWError<CellErrorType>;

#[derive(Debug)]
pub enum CellErrorType {
    BagOfCellsDeserializationError,
    BagOfCellsSerializationError,
    CellBuilderError,
    CellParserError,
    InvalidAddressType,
    InvalidExoticCell,
    NonEmptyReader,
    InternalError,
}
