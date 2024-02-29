use super::TransactionInput;
use crate::{
    error::UtxoError,
    error::{UtxoErrorKind, UtxoResult},
    script::{standard_script::conditions, Script, Witness},
    sighash_computer::UtxoToSign,
    signing_mode::SigningMethod,
    transaction::asset::brc20::{BRC20TransferInscription, Brc20Ticker},
    transaction::transaction_parts::{Amount, OutPoint},
};
use bitcoin::hashes::Hash;
use tw_encoding::hex;
use tw_hash::{hasher::Hasher, ripemd::bitcoin_hash_160, H160, H256, H264};
use tw_keypair::tw;
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
    pub fn custom_script(self, _custom: Script) -> Self {
        todo!()
    }
    pub fn amount(mut self, amount: Amount) -> Self {
        self.amount = Some(amount);
        self
    }
    // TODO: Can we somehow get rid of this?
    fn finalize_out_point(&mut self) -> UtxoResult<()> {
        // Populate the input with the previous output.
        self.input.previous_output.hash = self
            .prev_txid
            .ok_or(UtxoError(UtxoErrorKind::Error_internal))?;
        self.input.previous_output.index = self
            .prev_index
            .ok_or(UtxoError(UtxoErrorKind::Error_internal))?;

        Ok(())
    }
    pub fn p2pkh(mut self, pubkey: tw::PublicKey) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2pkh(&pubkey_hash),
                signing_method: SigningMethod::Legacy,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                ..Default::default()
            },
        ))
    }
    pub fn p2wpkh(mut self, pubkey: tw::PublicKey) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let h = bitcoin_hash_160(&pubkey.to_bytes());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2wpkh_script_code(&pubkey_hash),
                signing_method: SigningMethod::Segwit,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                ..Default::default()
            },
        ))
    }
    pub fn p2tr_key_path(
        mut self,
        pubkey: tw::PublicKey,
    ) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                // Generating special scriptPubkey for P2WPKH.
                script_pubkey: conditions::new_p2tr_key_path(&pubkey),
                signing_method: SigningMethod::TaprootAll,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }
    pub fn p2tr_script_path(
        mut self,
        pubkey: tw::PublicKey,
        payload: Script,
        merkle_root: H256,
    ) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        /*
        // TODO: This logic, including merkle root calculation, should be moved somewhere else.

        let mut builder = bitcoin::taproot::TaprootBuilder::new();

        for (depth, payload) in payloads {
            let payload = bitcoin::ScriptBuf::from_bytes(payload.as_data().to_vec());
            builder.add_leaf(*depth, payload).unwrap();
        }

        // The internal key.
        let xonly = bitcoin::secp256k1::XOnlyPublicKey::from(
            bitcoin::PublicKey::from_slice(pubkey.as_slice())
                .unwrap()
                .inner,
        );

        let spend_info = builder
            .finalize(&bitcoin::secp256k1::Secp256k1::new(), xonly)
            .unwrap();

        let merkle_root: H256 = spend_info
            .merkle_root()
            .unwrap()
            .to_byte_array()
            .as_slice()
            .try_into()
            .unwrap();
        */

        // Construct the leaf hash.
        let payload = bitcoin::ScriptBuf::from_bytes(payload.as_data().to_vec());
        let leaf_hash = bitcoin::taproot::TapLeafHash::from_script(
            &payload,
            bitcoin::taproot::LeafVersion::TapScript,
        );

        // Convert to native.
        let leaf_hash: H256 = leaf_hash
            .to_byte_array()
            .try_into()
            .expect("leaf hash length is 32 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                script_pubkey: conditions::new_p2tr_script_path(&pubkey, &merkle_root),
                signing_method: SigningMethod::TaprootAll,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                leaf_hash_code_separator: Some((leaf_hash, 0)),
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }
    pub fn brc20_transfer(
        mut self,
        pubkey: tw::PublicKey,
        ticker: String,
        value: String,
    ) -> UtxoResult<(TransactionInput, UtxoToSign)> {
        let pubkey: H264 = pubkey
            .to_bytes()
            .as_slice()
            .try_into()
            .expect("pubkey length is 33 bytes");

        let ticker = Brc20Ticker::new(ticker).unwrap();
        let transfer = BRC20TransferInscription::new(&pubkey, &ticker, &value).unwrap();

        let leaf_hash = bitcoin::taproot::TapLeafHash::from_script(
            &transfer.script,
            bitcoin::taproot::LeafVersion::TapScript,
        );

        // Convert to native.
        let leaf_hash: H256 = leaf_hash
            .to_byte_array()
            .try_into()
            .expect("leaf hash length is 32 bytes");

        self.finalize_out_point()?;

        Ok((
            self.input,
            UtxoToSign {
                // We reveal the full script.
                script_pubkey: Script::from(transfer.script.to_vec()),
                signing_method: SigningMethod::TaprootAll,
                amount: self
                    .amount
                    .ok_or(UtxoError(UtxoErrorKind::Error_internal))?,
                leaf_hash_code_separator: Some((leaf_hash, u32::MAX)),
                // Note that we don't use the default double-hasher.
                tx_hasher: Hasher::Sha256,
                ..Default::default()
            },
        ))
    }
}
