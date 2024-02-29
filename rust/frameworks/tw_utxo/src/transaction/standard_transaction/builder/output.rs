use super::TransactionOutput;
use crate::{
    error::{UtxoError, UtxoErrorKind, UtxoResult},
    script::standard_script::conditions,
    transaction::{
        asset::brc20::{BRC20TransferInscription, Brc20Ticker},
        transaction_parts::Amount,
    },
};

use bitcoin::hashes::Hash;
use tw_hash::{ripemd::bitcoin_hash_160, H160, H256, H264};
use tw_keypair::tw;

pub struct OutputBuilder {
    amount: Option<Amount>,
}

impl OutputBuilder {
    pub fn new() -> Self {
        OutputBuilder { amount: None }
    }
    pub fn amount(mut self, amount: Amount) -> Self {
        self.amount = Some(amount);
        self
    }
    // TODO: Be more precise with PublicKey type?.
    // TODO: There should be a hash-equivalent.
    pub fn p2pkh(self, pubkey: tw::PublicKey) -> UtxoResult<TransactionOutput> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2pkh(&pubkey_hash),
        })
    }
    // TODO: Be more precise with PublicKey type?.
    // TODO: There should be a hash-equivalent.
    pub fn p2wpkh(self, pubkey: tw::PublicKey) -> UtxoResult<TransactionOutput> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2wpkh(&pubkey_hash),
        })
    }
    pub fn p2tr_key_path(self, pubkey: tw::PublicKey) -> UtxoResult<TransactionOutput> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2tr_key_path(&pubkey),
        })
    }
    pub fn p2tr_script_path(
        self,
        pubkey: tw::PublicKey,
        merkle_root: H256,
    ) -> UtxoResult<TransactionOutput> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2tr_script_path(&pubkey, &merkle_root),
        })
    }
    pub fn brc20_transfer(
        self,
        pubkey: tw::PublicKey,
        ticker: String,
        value: String,
    ) -> UtxoResult<TransactionOutput> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        let ticker = Brc20Ticker::new(ticker).unwrap();
        let transfer = BRC20TransferInscription::new(&pubkey, &ticker, &value).unwrap();

        let merkle_root: H256 = transfer
            .spend_info
            .merkle_root()
            .unwrap()
            .to_byte_array()
            .as_slice()
            .try_into()
            .unwrap();

        Ok(TransactionOutput {
            value: self
                .amount
                .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
            script_pubkey: conditions::new_p2tr_script_path(&pubkey, &merkle_root),
        })
    }
}
