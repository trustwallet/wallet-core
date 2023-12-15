// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::GreenfieldAddress;
use crate::context::GreenfieldContext;
use crate::public_key::GreenfieldPublicKey;
use crate::transaction::message::GreenfieldMessageBox;
use tw_cosmos_sdk::signature::secp256k1::Secp256k1Signature;
use tw_cosmos_sdk::signature::CosmosSignature;
use tw_cosmos_sdk::transaction::{
    Fee, SignMode, SignedTransaction, SignerInfo, TxBody as CosmosTxBody,
    UnsignedTransaction as CosmosUnsignedTransaction,
};
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

/// TODO rename to `GreenfieldTxBody`.
pub struct TxBody {
    pub messages: Vec<GreenfieldMessageBox>,
    pub memo: String,
    pub timeout_height: u64,
}

impl TxBody {
    /// TODO rename `CosmosTxBody` to `TxBody`.
    fn into_cosmos_tx_body(self) -> CosmosTxBody {
        CosmosTxBody {
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

/// TODO rename to `GreenfieldUnsignedTransaction`.
pub struct UnsignedTransaction {
    pub signer: GreenfieldSignerInfo,
    pub fee: GreenfieldFee,
    pub cosmos_chain_id: String,
    pub eth_chain_id: U256,
    pub account_number: u64,
    pub tx_body: TxBody,
}

impl UnsignedTransaction {
    pub fn into_signed(self, signature: Secp256k1Signature) -> GreenfieldSignedTransaction {
        self.into_cosmos_unsigned()
            .into_signed(signature.to_bytes())
    }

    /// TODO rename `CosmosUnsignedTransaction` to `UnsignedTransaction`.
    fn into_cosmos_unsigned(self) -> CosmosUnsignedTransaction<GreenfieldContext> {
        CosmosUnsignedTransaction {
            signer: self.signer,
            fee: self.fee,
            chain_id: self.cosmos_chain_id,
            account_number: self.account_number,
            tx_body: self.tx_body.into_cosmos_tx_body(),
        }
    }
}
