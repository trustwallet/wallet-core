// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub type UtxoErrorKind = tw_proto::Utxo::Proto::Error;
pub type UtxoResult<T> = Result<T, UtxoError>;

#[derive(Debug, Clone)]
pub struct UtxoError(pub UtxoErrorKind);
