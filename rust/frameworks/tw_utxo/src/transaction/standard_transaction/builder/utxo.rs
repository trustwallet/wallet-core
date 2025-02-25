use super::TransactionInput;
use crate::address::DEFAULT_PUBLIC_KEY_HASHER;
use crate::sighash::SighashType;
use crate::spending_data::{standard_constructor, SpendingDataConstructor};
use crate::transaction::standard_transaction::DEFAULT_TX_HASHER;
use crate::transaction::UtxoToSign;
use crate::{
    script::{standard_script::conditions, Script, Witness},
    signing_mode::SigningMethod,
    transaction::asset::brc20::{BRC20TransferInscription, Brc20Ticker},
    transaction::transaction_parts::{Amount, OutPoint},
};
use bitcoin::hashes::Hash;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::StatefulHasher;
use tw_hash::{hasher::Hasher, H160, H256};
use tw_keypair::{ecdsa, schnorr};
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;

pub struct UtxoBuilder {
    input: TransactionInput,
    prev_txid: Option<H256>,
    prev_index: Option<u32>,
    amount: Option<Amount>,
    sighash_ty: Option<SighashType>,
    /// Transaction hasher used to pre-image transaction or get a transaction hash.
    /// Note it's ignored for Taproot transactions as they require Sha256.
    tx_hasher: Hasher,
    public_key_hasher: Hasher,
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
            sighash_ty: None,
            tx_hasher: DEFAULT_TX_HASHER,
            public_key_hasher: DEFAULT_PUBLIC_KEY_HASHER,
        }
    }

    pub fn prev_txid(mut self, txid: H256) -> Self {
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

    pub fn sighash_type(mut self, sighash_ty: SighashType) -> Self {
        self.sighash_ty = Some(sighash_ty);
        self
    }

    /// Specify a transaction hash algorithm used to pre-image transaction or get a transaction hash.
    /// Note it doesn't affect Taproot transactions.
    pub fn tx_hasher(mut self, tx_hasher: Hasher) -> Self {
        self.tx_hasher = tx_hasher;
        self
    }

    pub fn public_key_hasher(mut self, public_key_hasher: Hasher) -> Self {
        self.public_key_hasher = public_key_hasher;
        self
    }

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

    fn finalize_sighash_type(&self) -> SigningResult<SighashType> {
        self.sighash_ty
            .or_tw_err(SigningErrorType::Error_internal)
            .context("'UtxoBuilder::sighash_ty' is not set")
    }

    // TODO next iteration.
    // pub fn p2sh(self, redeem_script: Script) -> SigningResult<(TransactionInput, UtxoToSign)> {
    //     let h = sha256_ripemd(redeem_script.as_slice());
    //     let redeem_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");
    //
    //     self.p2sh_with_hash(redeem_hash)
    // }
    //
    // pub fn p2sh_with_hash(
    //     mut self,
    //     redeem_hash: H160,
    // ) -> SigningResult<(TransactionInput, UtxoToSign)> {
    //     self.finalize_out_point()?;
    //     let amount = self.finalize_amount()?;
    //
    //     Ok((
    //         self.input,
    //         UtxoToSign {
    //             script_pubkey: conditions::new_p2sh(&redeem_hash),
    //             // P2SH output can be spent by a legacy address only.
    //             signing_method: SigningMethod::Legacy,
    //             amount,
    //             ..Default::default()
    //         },
    //     ))
    // }

    pub fn p2pk(
        mut self,
        pubkey: &ecdsa::secp256k1::PublicKey,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;
        let sighash_ty = self.finalize_sighash_type()?;

        // The scriptPubkey for signing is the same as declared at the unspent output.
        let script_pubkey = conditions::new_p2pk(&pubkey.compressed());

        Ok((
            self.input,
            UtxoToSign {
                prevout_script_pubkey: script_pubkey.clone(),
                reveal_script_pubkey: script_pubkey,
                taproot_reveal_script_pubkey: None,
                // P2PK output can be spent by a legacy address only.
                signing_method: SigningMethod::Legacy,
                // When the sighash is signed, build a P2PK script_sig.
                spending_data_constructor: SpendingDataConstructor::ecdsa(
                    standard_constructor::P2PK,
                ),
                spender_public_key: pubkey.compressed().to_vec(),
                amount,
                leaf_hash_code_separator: None,
                tx_hasher: self.tx_hasher,
                sighash_ty,
            },
        ))
    }

    pub fn p2pkh(
        mut self,
        pubkey: &ecdsa::secp256k1::PublicKey,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let h = self.public_key_hasher.hash(pubkey.compressed().as_slice());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;
        let sighash_ty = self.finalize_sighash_type()?;

        // The scriptPubkey for signing is the same as declared at the unspent output.
        let script_pubkey = conditions::new_p2pkh(&pubkey_hash);

        Ok((
            self.input,
            UtxoToSign {
                prevout_script_pubkey: script_pubkey.clone(),
                reveal_script_pubkey: script_pubkey,
                taproot_reveal_script_pubkey: None,
                // P2PK output can be spent by a legacy address only.
                signing_method: SigningMethod::Legacy,
                // When the sighash is signed, build a P2PKH script_sig.
                spending_data_constructor: SpendingDataConstructor::ecdsa(
                    standard_constructor::P2PKH {
                        pubkey: pubkey.compressed(),
                    },
                ),
                spender_public_key: pubkey.compressed().to_vec(),
                amount,
                leaf_hash_code_separator: None,
                tx_hasher: self.tx_hasher,
                sighash_ty,
            },
        ))
    }

    // TODO next iteration.
    // pub fn p2wsh(mut self, redeem_script: Script) -> SigningResult<(TransactionInput, UtxoToSign)> {
    //     let h = sha256(redeem_script.as_slice());
    //     let redeem_hash: H256 = h.as_slice().try_into().expect("hash length is 32 bytes");
    //
    //     self.finalize_out_point()?;
    //     let amount = self.finalize_amount()?;
    //     let sighash_ty = self.finalize_sighash_type()?;
    //
    //     Ok((
    //         self.input,
    //         UtxoToSign {
    //             script_pubkey: conditions::new_p2wsh(&redeem_hash),
    //             // P2WSH output can be spent by a legacy address only.
    //             signing_method: SigningMethod::Segwit,
    //             spending_data_constructor: SpendingDataConstructor::ecdsa(todo!()),
    //             amount,
    //             leaf_hash_code_separator: None,
    //             tx_hasher: self.tx_hasher,
    //             sighash_ty,
    //         },
    //     ))
    // }

    pub fn p2wpkh(
        mut self,
        pubkey: &ecdsa::secp256k1::PublicKey,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let h = self.public_key_hasher.hash(pubkey.compressed().as_slice());
        let pubkey_hash: H160 = h.as_slice().try_into().expect("hash length is 20 bytes");

        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;
        let sighash_ty = self.finalize_sighash_type()?;

        Ok((
            self.input,
            UtxoToSign {
                // Original P2WPKH scriptPubkey.
                prevout_script_pubkey: conditions::new_p2wpkh(&pubkey_hash),
                // To spend a P2WPKH UTXO, we need to sign the transaction with a corresponding P2PKH UTXO.
                // Then the result script_sig will be published as a witness.
                // Generating special scriptPubkey for P2WPKH.
                reveal_script_pubkey: conditions::new_p2pkh(&pubkey_hash),
                taproot_reveal_script_pubkey: None,
                // When the sighash is signed, build a P2WPKH witness.
                spending_data_constructor: SpendingDataConstructor::ecdsa(
                    standard_constructor::P2WPKH {
                        pubkey: pubkey.compressed(),
                    },
                ),
                spender_public_key: pubkey.compressed().to_vec(),
                // P2WPKH output can be spent by a Witness (eg "bc1") address only.
                signing_method: SigningMethod::Segwit,
                amount,
                leaf_hash_code_separator: None,
                tx_hasher: self.tx_hasher,
                sighash_ty,
            },
        ))
    }

    pub fn p2tr_key_path(
        self,
        pubkey: &schnorr::PublicKey,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let tweaked_pubkey = pubkey.tweak(None);
        self.p2tr_key_path_with_tweaked_pubkey(&tweaked_pubkey.x_only())
    }

    pub fn p2tr_key_path_with_tweaked_pubkey(
        mut self,
        tweaked_pubkey: &schnorr::XOnlyPublicKey,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        self.finalize_out_point()?;
        let amount = self.finalize_amount()?;
        let sighash_ty = self.finalize_sighash_type()?;

        // The scriptPubkey for signing is the same as declared at the unspent output.
        let script_pubkey = conditions::new_p2tr_dangerous_assume_tweaked(&tweaked_pubkey.bytes());

        Ok((
            self.input,
            UtxoToSign {
                prevout_script_pubkey: script_pubkey.clone(),
                reveal_script_pubkey: script_pubkey,
                taproot_reveal_script_pubkey: None,
                // P2TR output can be spent by a Witness (eg "bc1") address only.
                signing_method: SigningMethod::Taproot,
                // When the sighash is signed, build a P2TR witness.
                spending_data_constructor: SpendingDataConstructor::schnorr(
                    standard_constructor::P2TRKeyPath,
                ),
                spender_public_key: tweaked_pubkey.bytes().to_vec(),
                amount,
                leaf_hash_code_separator: None,
                // Note Taproot transactions use Sha256 hash, not [`UtxoToSign::tx_hasher`].
                tx_hasher: Hasher::Sha256,
                sighash_ty,
            },
        ))
    }

    pub fn p2tr_script_path(self) -> P2TRScriptPathUtxoBuilder {
        P2TRScriptPathUtxoBuilder::new(self)
    }

    pub fn brc20_transfer(
        self,
        pubkey: &schnorr::PublicKey,
        ticker: String,
        value: String,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let ticker = Brc20Ticker::new(ticker).unwrap();
        let transfer =
            BRC20TransferInscription::new(&pubkey.compressed(), &ticker, &value).unwrap();

        let control_block = transfer
            .spend_info
            .control_block(&(
                transfer.script.to_owned(),
                bitcoin::taproot::LeafVersion::TapScript,
            ))
            .or_tw_err(SigningErrorType::Error_internal)
            .context("'TaprootSpendInfo::control_block' is None")?;

        let merkle_root = transfer.merkle_root()?;
        let transfer_payload = Script::from(transfer.script.to_bytes());

        self.p2tr_script_path()
            .reveal_script_pubkey(transfer_payload.clone())
            .spender_public_key(pubkey)
            // BRC20 transfer UTXO should be signed by revealing actual script pubkey.
            .taproot_reveal_script_pubkey(transfer_payload)
            .restore_prevout_script_pubkey(pubkey, &merkle_root)
            .control_block(control_block.serialize())
            .build()
    }
}

pub struct P2TRScriptPathUtxoBuilder {
    /// Required.
    reveal_script_pubkey: Option<Script>,
    /// Required.
    spender_public_key: Option<Data>,
    /// Required.
    prevout_script_pubkey: Option<Script>,
    /// Required.
    control_block: Option<Data>,

    /// Optional.
    /// If not specified, `standard_constructor::P2TRScriptPath` will be created from [`P2TRScriptPathUtxoBuilder::control_block`].
    spending_data_ctor: Option<SpendingDataConstructor>,

    /// Optional.
    taproot_reveal_script_pubkey: Option<Script>,

    /// Inner UTXO builder.
    utxo_builder: UtxoBuilder,
}

impl P2TRScriptPathUtxoBuilder {
    pub fn new(utxo_builder: UtxoBuilder) -> P2TRScriptPathUtxoBuilder {
        P2TRScriptPathUtxoBuilder {
            reveal_script_pubkey: None,
            spender_public_key: None,
            prevout_script_pubkey: None,
            control_block: None,
            spending_data_ctor: None,
            taproot_reveal_script_pubkey: None,
            utxo_builder,
        }
    }

    pub fn reveal_script_pubkey(mut self, reveal_script_pubkey: Script) -> Self {
        self.reveal_script_pubkey = Some(reveal_script_pubkey);
        self
    }

    pub fn spender_public_key(mut self, spender: &schnorr::PublicKey) -> Self {
        self.spender_public_key = Some(spender.compressed().to_vec());
        self
    }

    /// Restore the original scriptPubkey declared at the unspent P2TR output.
    ///
    /// # Note
    ///
    /// `internal_key` can differ from [`P2TRScriptPathUtxoBuilder::spender_public_key`].
    pub fn restore_prevout_script_pubkey(
        mut self,
        internal_key: &schnorr::PublicKey,
        merkle_root: &H256,
    ) -> Self {
        self.prevout_script_pubkey = Some(conditions::new_p2tr_script_path(
            &internal_key.compressed(),
            merkle_root,
        ));
        self
    }

    pub fn control_block(mut self, control_block: Data) -> Self {
        self.control_block = Some(control_block);
        self
    }

    /// Set custom spending data constructor.
    pub fn custom_spending_data_ctor(
        mut self,
        spending_data_ctor: SpendingDataConstructor,
    ) -> Self {
        self.spending_data_ctor = Some(spending_data_ctor);
        self
    }

    pub fn taproot_reveal_script_pubkey(mut self, taproot_reveal_script_pubkey: Script) -> Self {
        self.taproot_reveal_script_pubkey = Some(taproot_reveal_script_pubkey);
        self
    }

    pub fn build(mut self) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let reveal_script_pubkey = self
            .reveal_script_pubkey
            .or_tw_err(SigningErrorType::Error_internal)
            .context("P2TRScriptPathUtxoBuilder::reveal_script_pubkey must be specified")?;

        let spender_public_key = self
            .spender_public_key
            .or_tw_err(SigningErrorType::Error_internal)
            .context("P2TRScriptPathUtxoBuilder::spender_public_key must be specified")?;

        let prevout_script_pubkey = self
            .prevout_script_pubkey
            .or_tw_err(SigningErrorType::Error_internal)
            .context("P2TRScriptPathUtxoBuilder::prevout_script_pubkey must be specified")?;

        let control_block = self
            .control_block
            .or_tw_err(SigningErrorType::Error_internal)
            .context("P2TRScriptPathUtxoBuilder::control_block must be specified")?;

        let spending_data_constructor = match self.spending_data_ctor {
            Some(ctor) => ctor,
            None => SpendingDataConstructor::schnorr(standard_constructor::P2TRScriptPath {
                payload: reveal_script_pubkey.clone(),
                control_block,
            }),
        };

        let leaf_hash = Self::leaf_hash(&reveal_script_pubkey);

        self.utxo_builder.finalize_out_point()?;
        let amount = self.utxo_builder.finalize_amount()?;
        let sighash_ty = self.utxo_builder.finalize_sighash_type()?;

        Ok((
            self.utxo_builder.input,
            UtxoToSign {
                prevout_script_pubkey,
                // We use the full (revealed) script as scriptPubkey here.
                reveal_script_pubkey,
                taproot_reveal_script_pubkey: self.taproot_reveal_script_pubkey,
                signing_method: SigningMethod::Taproot,
                spending_data_constructor,
                // Taproot ScriptPath input should be signed with a non-tweaked private key.
                spender_public_key,
                amount,
                leaf_hash_code_separator: Some((leaf_hash, u32::MAX)),
                // Note Taproot transactions use Sha256 hash, not [`UtxoToSign::tx_hasher`].
                tx_hasher: Hasher::Sha256,
                sighash_ty,
            },
        ))
    }

    /// Construct the leaf hash.
    fn leaf_hash(reveal_script_pubkey: &Script) -> H256 {
        let script_buf = bitcoin::ScriptBuf::from_bytes(reveal_script_pubkey.to_vec());
        let leaf_hash = bitcoin::taproot::TapLeafHash::from_script(
            &script_buf,
            bitcoin::taproot::LeafVersion::TapScript,
        );
        H256::from(leaf_hash.to_byte_array())
    }
}

impl Default for UtxoBuilder {
    fn default() -> Self {
        UtxoBuilder::new()
    }
}
