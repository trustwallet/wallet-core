use super::TransactionOutput;
use crate::{
    script::{standard_script::conditions, Script},
    transaction::{
        asset::brc20::{BRC20TransferInscription, Brc20Ticker},
        transaction_parts::Amount,
    },
};
use bitcoin::hashes::Hash;
use tw_coin_entry::error::prelude::*;
use tw_hash::{ripemd::bitcoin_hash_160, sha2::sha256, H160, H256, H264};
use tw_keypair::tw;

pub struct OutputBuilder {
    amount: Amount,
}

impl OutputBuilder {
    pub fn new(amount: Amount) -> Self {
        OutputBuilder { amount }
    }

    pub fn custom_script_pubkey(&self, script_pubkey: Script) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey,
        }
    }

    pub fn p2sh(self, redeem_script: &[u8]) -> TransactionOutput {
        let h = bitcoin_hash_160(redeem_script);
        let redeem_hash: H160 = h
            .as_slice()
            .try_into()
            .expect("hash length must be 20 bytes");

        self.p2sh_from_hash(&redeem_hash)
    }

    pub fn p2sh_from_hash(self, redeem_hash: &H160) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2sh(redeem_hash),
        }
    }

    // TODO: Be more precise with PublicKey type?.
    pub fn p2pkh(self, pubkey: &tw::PublicKey) -> TransactionOutput {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h
            .as_slice()
            .try_into()
            .expect("hash length must be 20 bytes");

        self.p2pkh_from_hash(&pubkey_hash)
    }

    pub fn p2pkh_from_hash(self, pubkey_hash: &H160) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2pkh(pubkey_hash),
        }
    }

    pub fn p2wsh(self, redeem_script: &Script) -> TransactionOutput {
        let h = sha256(redeem_script.as_data());
        let redeem_hash: H256 = h.as_slice().try_into().expect("hash length is 32 bytes");

        self.p2wsh_from_hash(&redeem_hash)
    }

    pub fn p2wsh_from_hash(self, redeem_hash: &H256) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2wsh(redeem_hash),
        }
    }

    // TODO: Be more precise with PublicKey type?.
    pub fn p2wpkh(self, pubkey: &tw::PublicKey) -> TransactionOutput {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.p2wpkh_from_hash(&pubkey_hash)
    }

    pub fn p2wpkh_from_hash(self, pubkey_hash: &H160) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2wpkh(pubkey_hash),
        }
    }

    pub fn p2tr_key_path(self, pubkey: &tw::PublicKey) -> TransactionOutput {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2tr_key_path(&pubkey),
        }
    }

    pub fn p2tr_dangerous_assume_tweaked(self, xonly: &H256) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2tr_dangerous_assume_tweaked(xonly),
        }
    }

    pub fn p2tr_script_path(self, pubkey: tw::PublicKey, merkle_root: H256) -> TransactionOutput {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2tr_script_path(&pubkey, &merkle_root),
        }
    }

    // TODO: Be more precise with PublicKey type?.
    pub fn brc20_transfer(
        self,
        pubkey: tw::PublicKey,
        ticker: String,
        value: String,
    ) -> SigningResult<TransactionOutput> {
        let pubkey: H264 = pubkey
            .to_secp256k1()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("Expected secp256k1 public key")?
            .compressed();

        let ticker = Brc20Ticker::new(ticker)?;
        let transfer = BRC20TransferInscription::new(&pubkey, &ticker, &value)?;

        let merkle_root: H256 = transfer
            .spend_info
            .merkle_root()
            .or_tw_err(SigningErrorType::Error_internal)
            .context("No merkle root of the BRC20 Transfer spend info")?
            .to_byte_array()
            .into();

        Ok(TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2tr_script_path(&pubkey, &merkle_root),
        })
    }
}
