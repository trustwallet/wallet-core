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

pub fn cell_to_signing_error(cell_err: CellError) -> SigningError {
    cell_err.map_err(|cell_ty| match cell_ty {
        CellErrorType::BagOfCellsDeserializationError
        | CellErrorType::CellParserError
        | CellErrorType::InvalidExoticCell
        | CellErrorType::NonEmptyReader => SigningErrorType::Error_input_parse,
        CellErrorType::BagOfCellsSerializationError | CellErrorType::CellBuilderError => {
            SigningErrorType::Error_internal
        },
        CellErrorType::InvalidAddressType => SigningErrorType::Error_invalid_address,
        CellErrorType::InternalError => SigningErrorType::Error_internal,
    })
}
