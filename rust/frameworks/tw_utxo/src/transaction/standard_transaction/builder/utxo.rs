use super::TransactionInput;
use crate::address::segwit::SegwitAddress;
use crate::{
    script::{standard_script::conditions, Script, Witness},
    sighash_computer::UtxoToSign,
    signing_mode::SigningMethod,
    transaction::asset::brc20::{BRC20TransferInscription, Brc20Ticker},
    transaction::transaction_parts::{Amount, OutPoint},
};
use bitcoin::hashes::Hash;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex;
use tw_hash::{hasher::Hasher, ripemd::bitcoin_hash_160, sha2::sha256, H160, H256, H264};
use tw_keypair::{ecdsa, schnorr, tw};
use tw_misc::traits::ToBytesVec;

pub struct UtxoBuilder {
    input: TransactionInput,
    prev_txid: Option<H256>,
    prev_index: Option<u32>,
    amount: Option<Amount>,
}

impl UtxoBuilder {
    pub fn new() -> Self {
        UtxoBuilder {
            input: TransactionInput {
                previous_output: OutPoint::default(),
                script_sig: Script::new(),
                sequence: u32::MAX,
                witness: Witness::new(),
            },
            prev_txid: None,
            prev_index: None,
            amount: None,
        }
    }

    pub fn prev_txid(mut self, txid: H256) -> Self {
        self.prev_txid = Some(txid);
        self
    }

    pub fn prev_txid_str_and_reverse(mut self, txid: &str) -> Self {
        let txid: H256 = hex::decode(txid)
            .unwrap()
            .into_iter()
            .rev()
            .collect::<Vec<u8>>()
            .as_slice()
            .try_into()
            .unwrap();

        self.prev_txid = Some(txid);
        self
    }

    pub fn prev_index(mut self, index: u32) -> Self {
        self.prev_index = Some(index);
        self
    }

    pub fn sequence(mut self, sequence: u32) -> Self {
        self.input.sequence = sequence;
        self
    }

    pub fn amount(mut self, amount: Amount) -> Self {
        self.amount = Some(amount);
        self
    }

    // TODO: Can we somehow get rid of this?
    fn finalize_out_point(&mut self) -> SigningResult<()> {
        // Populate the input with the previous output.
        self.input.previous_output.hash = self
            .prev_txid
            .or_tw_err(SigningErrorType::Error_internal)
            .context("'UtxoBuilder::prev_txid' is not set")?;
        self.input.previous_output.index = self
            .prev_index
            .or_tw_err(SigningErrorType::Error_internal)
            .context("'UtxoBuilder::prev_index' is not set")?;

        Ok(())
    }

    fn finalize_amount(&self) -> SigningResult<Amount> {
        self.amount
            .or_tw_err(SigningErrorType::Error_internal)
            .context("'UtxoBuilder::amount' is not set")
    }

    pub fn custom_script_pubkey(
        self,
        script_pubkey: Script,
        signing_method: SigningMethod,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let amount = self.finalize_amount()?;
        Ok((
            self.input,
            UtxoToSign {
                script_pubkey,
                signing_method,
                amount,
                ..Default::default()
            },
        ))
    }

    pub fn p2sh(self, redeem_script: Script) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let h = bitcoin_hash_160(redeem_script.as_data());
        let redeem_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.p2sh_with_hash(redeem_hash)
    }

    pub fn p2sh_with_hash(
        mut self,
        redeem_hash: H160,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2sh(&redeem_hash),
                // P2SH output can be spent by a legacy address only.
                signing_method: SigningMethod::Legacy,
                amount,
                ..Default::default()
            },
        ))
    }

    pub fn p2pk(
        mut self,
        pubkey: ecdsa::secp256k1::PublicKey,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2pk(&pubkey.compressed()),
                // P2PK output can be spent by a legacy address only.
                signing_method: SigningMethod::Legacy,
                amount,
                ..Default::default()
            },
        ))
    }

    /// TODO consider removing this.
    pub fn p2pkh(self, pubkey: tw::PublicKey) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.p2pkh_with_hash(pubkey_hash)
    }

    /// TODO consider renaming this as `p2pkh`.
    pub fn p2pkh_with_hash(
        mut self,
        pubkey_hash: H160,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2pkh(&pubkey_hash),
                // P2PKH output can be spent by a legacy address only.
                signing_method: SigningMethod::Legacy,
                amount,
                ..Default::default()
            },
        ))
    }

    pub fn p2wsh(self, redeem_script: Script) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let h = sha256(redeem_script.as_data());
        let redeem_hash: H256 = h.as_slice().try_into().expect("hash length is 32 bytes");

        self.p2wsh_with_hash(redeem_hash)
    }

    pub fn p2wsh_with_hash(
        mut self,
        redeem_hash: H256,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2wsh(&redeem_hash),
                // P2WSH output can be spent by a legacy address only.
                signing_method: SigningMethod::Segwit,
                amount,
                ..Default::default()
            },
        ))
    }

    /// TODO consider removing this.
    pub fn p2wpkh(self, pubkey: tw::PublicKey) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.p2wpkh_with_hash(&pubkey_hash)
    }

    /// TODO consider renaming this as `p2wpkh`.
    pub fn p2wpkh_with_hash(
        mut self,
        pubkey_hash: &H160,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2wpkh(pubkey_hash),
                // P2WPKH output can be spent by a Witness (eg "bc1") address only.
                signing_method: SigningMethod::Segwit,
                amount,
                ..Default::default()
            },
        ))
    }

    pub fn p2witness_program_with_addr(
        self,
        addr: &SegwitAddress,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let witness_program = addr.witness_program();
        match witness_program.len() {
            H160::LEN => {
                let pubkey_hash = H160::try_from(witness_program)
                    .expect("'witness_program' length must be checked already");
                self.p2wpkh_with_hash(&pubkey_hash)
            },
            H256::LEN => {
                let redeem_hash = H256::try_from(witness_program)
                    .expect("'witness_program' length must be checked already");
                self.p2wsh_with_hash(redeem_hash)
            },
            _ => SigningError::err(SigningErrorType::Error_invalid_address)
                .context(format!("The given '{addr}' segwit address should have 20 or 32 byte length witness program")),
        }
    }

    pub fn p2tr_key_path(
        mut self,
        pubkey: &schnorr::PublicKey,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let pubkey = pubkey.compressed();

        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2tr_key_path(&pubkey),
                // P2TR output can be spent by a Witness (eg "bc1") address only.
                signing_method: SigningMethod::Taproot,
                amount,
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }

    pub fn p2tr_script_path(
        mut self,
        payload: Script,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        // Construct the leaf hash.
        let script_buf = bitcoin::ScriptBuf::from_bytes(payload.as_data().to_vec());
        let leaf_hash = bitcoin::taproot::TapLeafHash::from_script(
            &script_buf,
            bitcoin::taproot::LeafVersion::TapScript,
        );

        // Convert to native.
        let leaf_hash: H256 = H256::from(leaf_hash.to_byte_array());

        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                // We use the full (revealed) script as scriptPubkey here.
                script_pubkey: payload,
                signing_method: SigningMethod::Taproot,
                amount,
                leaf_hash_code_separator: Some((leaf_hash, u32::MAX)),
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }

    pub fn p2tr_dangerous_assume_tweaked(
        mut self,
        tweaked_pubkey_xonly: &H256,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2tr_dangerous_assume_tweaked(tweaked_pubkey_xonly),
                // P2TR output can be spent by a Witness (eg "bc1") address only.
                signing_method: SigningMethod::Taproot,
                amount,
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }

    pub fn brc20_transfer(
        mut self,
        pubkey: &schnorr::PublicKey,
        ticker: String,
        value: String,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let pubkey: H264 = pubkey.compressed();

        let ticker = Brc20Ticker::new(ticker).unwrap();
        let transfer = BRC20TransferInscription::new(&pubkey, &ticker, &value).unwrap();

        let leaf_hash = bitcoin::taproot::TapLeafHash::from_script(
            &transfer.script,
            bitcoin::taproot::LeafVersion::TapScript,
        );

        // Convert to native.
        let leaf_hash = H256::from(leaf_hash.to_byte_array());

        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;

        Ok((
            self.input,
            UtxoToSign {
                // We use the full (revealed) script as scriptPubkey here.
                script_pubkey: Script::from(transfer.script.to_vec()),
                signing_method: SigningMethod::Taproot,
                amount,
                leaf_hash_code_separator: Some((leaf_hash, u32::MAX)),
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }
}

impl Default for UtxoBuilder {
    fn default() -> Self {
        UtxoBuilder::new()
    }
}
