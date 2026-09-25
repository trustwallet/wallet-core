use super::TransactionOutput;
use crate::address::DEFAULT_PUBLIC_KEY_HASHER;
use crate::{
    script::{standard_script::conditions, Script},
    transaction::{
        asset::brc20::{BRC20TransferInscription, Brc20Ticker},
        transaction_parts::Amount,
    },
};
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::{Hasher, StatefulHasher};
use tw_hash::ripemd::sha256_ripemd;
use tw_hash::sha2::sha256;
use tw_hash::{H160, H256};
use tw_keypair::{ecdsa, schnorr};

pub const OP_RETURN_DATA_LIMIT: usize = 80;

pub struct OutputBuilder {
    amount: Amount,
    public_key_hasher: Hasher,
}

impl OutputBuilder {
    pub fn new(amount: Amount) -> Self {
        OutputBuilder {
            amount,
            public_key_hasher: DEFAULT_PUBLIC_KEY_HASHER,
        }
    }

    pub fn with_public_key_hasher(amount: Amount, public_key_hasher: Hasher) -> Self {
        OutputBuilder {
            amount,
            public_key_hasher,
        }
    }

    pub fn get_amount(&self) -> Amount {
        self.amount
    }

    pub fn custom_script_pubkey(&self, script_pubkey: Script) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey,
        }
    }

    pub fn p2sh(self, redeem_script: &[u8]) -> TransactionOutput {
        // TODO consider adding [`OutputBuilder::script_hasher`] if needed.
        let h = sha256_ripemd(redeem_script);
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

    pub fn p2pk(self, pubkey: &ecdsa::secp256k1::PublicKey) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2pk(&pubkey.compressed()),
        }
    }

    pub fn p2pkh(self, pubkey: &ecdsa::secp256k1::PublicKey) -> TransactionOutput {
        let h = self.public_key_hasher.hash(pubkey.compressed().as_slice());
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
        // TODO consider adding [`OutputBuilder::witness_script_hasher`] if needed.
        let h = sha256(redeem_script.as_slice());
        let redeem_hash: H256 = h.as_slice().try_into().expect("hash length is 32 bytes");

        self.p2wsh_from_hash(&redeem_hash)
    }

    pub fn p2wsh_from_hash(self, redeem_hash: &H256) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2wsh(redeem_hash),
        }
    }

    pub fn p2wpkh(self, pubkey: &ecdsa::secp256k1::PublicKey) -> TransactionOutput {
        let h = self.public_key_hasher.hash(pubkey.compressed().as_slice());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.p2wpkh_from_hash(&pubkey_hash)
    }

    pub fn p2wpkh_from_hash(self, pubkey_hash: &H160) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2wpkh(pubkey_hash),
        }
    }

    pub fn p2tr_key_path(self, pubkey: &schnorr::PublicKey) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2tr_key_path(&pubkey.compressed()),
        }
    }

    pub fn p2tr_dangerous_assume_tweaked(self, xonly: &H256) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2tr_dangerous_assume_tweaked(xonly),
        }
    }

    pub fn p2tr_script_path(
        self,
        pubkey: &schnorr::PublicKey,
        merkle_root: H256,
    ) -> TransactionOutput {
        TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_p2tr_script_path(&pubkey.compressed(), &merkle_root),
        }
    }

    pub fn brc20_transfer(
        self,
        pubkey: &schnorr::PublicKey,
        ticker: String,
        value: String,
    ) -> SigningResult<TransactionOutput> {
        let pubkey_data = pubkey.compressed();
        let ticker = Brc20Ticker::new(ticker)?;
        let transfer = BRC20TransferInscription::new(&pubkey_data, &ticker, &value)?;
        let merkle_root = transfer.merkle_root()?;
        Ok(self.p2tr_script_path(pubkey, merkle_root))
    }

    pub fn op_return(self, data: &[u8]) -> SigningResult<TransactionOutput> {
        if data.len() > OP_RETURN_DATA_LIMIT {
            return SigningError::err(SigningErrorType::Error_invalid_memo).context(format!(
                "OP_RETURN data can be up to {OP_RETURN_DATA_LIMIT} bytes"
            ));
        }

        Ok(TransactionOutput {
            value: self.amount,
            script_pubkey: conditions::new_op_return(data),
        })
    }
}
