// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::private_key::SignatureData;
use tw_number::U256;

pub mod message;

use message::SerializeMessageBox;

/// At this moment, TW only supports the Direct signing mode.
#[derive(Clone, Copy, Debug)]
pub enum SignMode {
    Direct,
}

pub struct Fee<Address> {
    pub amounts: Vec<Coin>,
    pub gas_limit: u64,
    pub payer: Option<Address>,
    pub granter: Option<Address>,
}

pub struct Coin {
    pub denom: String,
    pub amount: U256,
}

pub struct SignerInfo<PublicKey> {
    pub public_key: PublicKey,
    pub sequence: u64,
    pub sign_mode: SignMode,
}

pub struct TxBody {
    pub messages: Vec<SerializeMessageBox>,
    pub memo: String,
    pub timeout_height: u64,
}

pub struct UnsignedTransaction<Address, PublicKey> {
    pub signer: SignerInfo<PublicKey>,
    pub fee: Fee<Address>,
    pub chain_id: String,
    pub account_number: u64,
    pub tx_body: TxBody,
}

impl<Address, PublicKey> UnsignedTransaction<Address, PublicKey> {
    pub fn into_signed(
        self,
        signature: SignatureData,
    ) -> SignedTransaction<Address, PublicKey> {
        SignedTransaction {
            signer: self.signer,
            fee: self.fee,
            tx_body: self.tx_body,
            signature,
        }
    }
}

pub struct SignedTransaction<Address, PublicKey> {
    pub signer: SignerInfo<PublicKey>,
    pub fee: Fee<Address>,
    pub tx_body: TxBody,
    pub signature: SignatureData,
}
