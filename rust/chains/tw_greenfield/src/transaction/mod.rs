// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::GreenfieldAddress;
use crate::transaction::message::GreenfieldMessageBox;
use tw_cosmos_sdk::transaction::{Fee, SignerInfo};
use tw_number::U256;

pub mod message;

pub struct TxBody {
    pub messages: Vec<GreenfieldMessageBox>,
    pub memo: String,
    pub timeout_height: u64,
}

pub struct UnsignedTransaction {
    // TODO add PublicKey
    pub signer: SignerInfo<()>,
    pub fee: Fee<GreenfieldAddress>,
    pub cosmos_chain_id: String,
    pub eth_chain_id: U256,
    pub account_number: u64,
    pub tx_body: TxBody,
}
