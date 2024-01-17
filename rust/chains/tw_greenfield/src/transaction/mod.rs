// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::GreenfieldAddress;
use crate::context::GreenfieldContext;
use crate::public_key::GreenfieldPublicKey;
use crate::signature::GreenfieldSignature;
use crate::transaction::message::GreenfieldMessageBox;
use tw_cosmos_sdk::transaction::{
    Fee, SignMode, SignedTransaction, SignerInfo, TxBody, UnsignedTransaction,
};
use tw_misc::traits::ToBytesVec;
use tw_number::U256;

pub type GreenfieldSignerInfo = SignerInfo<GreenfieldPublicKey>;
pub type GreenfieldFee = Fee<GreenfieldAddress>;
pub type GreenfieldSignedTransaction = SignedTransaction<GreenfieldContext>;

pub mod message;

pub enum GreenfieldSignMode {
    Eip712,
}

impl From<GreenfieldSignMode> for SignMode {
    fn from(mode: GreenfieldSignMode) -> Self {
        use tw_cosmos_sdk::proto::cosmos::signing::v1beta1 as signing_proto;

        match mode {
            GreenfieldSignMode::Eip712 => {
                SignMode::Other(signing_proto::SignMode::SIGN_MODE_EIP_712 as i32)
            },
        }
    }
}

pub struct GreenfieldTxBody {
    pub messages: Vec<GreenfieldMessageBox>,
    pub memo: String,
    pub timeout_height: u64,
}

impl GreenfieldTxBody {
    fn into_cosmos_tx_body(self) -> TxBody {
        TxBody {
            messages: self
                .messages
                .into_iter()
                .map(|greenfield_msg| greenfield_msg.to_cosmos_message())
                .collect(),
            memo: self.memo,
            timeout_height: self.timeout_height,
        }
    }
}

pub struct GreenfieldUnsignedTransaction {
    pub signer: GreenfieldSignerInfo,
    pub fee: GreenfieldFee,
    pub cosmos_chain_id: String,
    pub eth_chain_id: U256,
    pub account_number: u64,
    pub tx_body: GreenfieldTxBody,
}

impl GreenfieldUnsignedTransaction {
    pub fn into_signed(self, signature: GreenfieldSignature) -> GreenfieldSignedTransaction {
        self.into_cosmos_unsigned().into_signed(signature.to_vec())
    }

    fn into_cosmos_unsigned(self) -> UnsignedTransaction<GreenfieldContext> {
        UnsignedTransaction {
            signer: self.signer,
            fee: self.fee,
            chain_id: self.cosmos_chain_id,
            account_number: self.account_number,
            tx_body: self.tx_body.into_cosmos_tx_body(),
        }
    }
}
