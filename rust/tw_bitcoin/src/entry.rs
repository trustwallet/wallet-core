use crate::utils::{proto_input_to_native, proto_output_to_native, proto_sighash_to_sig};
use crate::{bitcoin_output_error, Error, Result};
use bitcoin::address::NetworkChecked;
use std::fmt::Display;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_keypair::tw::PublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;
use tw_utxo::address::standard_bitcoin::{StandardBitcoinAddress, StandardBitcoinPrefix};
use tw_utxo::sighash_computer::SighashComputer;
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::transaction_fee::TransactionFee;
use tw_utxo::transaction::transaction_interface::TxInputInterface;
use tw_utxo::utxo_selector::SelectionBuilder;

pub struct Address(pub bitcoin::address::Address<NetworkChecked>);

impl Display for Address {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl CoinAddress for Address {
    fn data(&self) -> tw_memory::Data {
        self.0.to_string().into_bytes()
    }
}

pub struct BitcoinEntry;

impl CoinEntry for BitcoinEntry {
    type AddressPrefix = StandardBitcoinPrefix;
    type Address = StandardBitcoinAddress;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = Proto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;

    #[inline]
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        StandardBitcoinAddress::from_str_with_coin_and_prefix(coin, address, prefix)
    }

    #[inline]
    fn parse_address_unchecked(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
    ) -> AddressResult<Self::Address> {
        StandardBitcoinAddress::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        StandardBitcoinAddress::derive_as_tw(coin, &public_key, derivation, prefix)
    }

    #[inline]
    fn sign(&self, _coin: &dyn CoinContext, proto: Self::SigningInput<'_>) -> Self::SigningOutput {
        self.sign_impl(_coin, proto)
            .unwrap_or_else(|err| bitcoin_output_error!(Proto::SigningOutput, err))
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        self.preimage_hashes_impl(_coin, proto)
            .unwrap_or_else(|err| bitcoin_output_error!(Proto::PreSigningOutput, err))
    }

    #[inline]
    fn compile(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        self.compile_impl(_coin, proto, signatures, _public_keys)
            .unwrap_or_else(|err| bitcoin_output_error!(Proto::SigningOutput, err))
    }

    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        None
    }
}

impl BitcoinEntry {
    pub(crate) fn sign_impl(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Result<Proto::SigningOutput<'static>> {
        let pre_sign = self.preimage_hashes_impl(_coin, proto.clone())?;

        let mut sigs = vec![];
        for (input, sighash) in proto.inputs.iter().zip(pre_sign.sighashes.iter()) {
            let privkey = if !input.private_key.is_empty() {
                // Overwrite with individual key for input.
                &input.private_key
            } else {
                &proto.private_key
            };

            let sig = proto_sighash_to_sig(privkey, sighash, proto.dangerous_use_fixed_schnorr_rng)
                .unwrap();
            sigs.push(sig);
        }

        self.compile_impl(_coin, proto, sigs, vec![])
    }

    pub(crate) fn preimage_hashes_impl(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Result<Proto::PreSigningOutput<'static>> {
        let proto = pre_processor(proto);

        // Construct the transaction.
        let mut builder = TransactionBuilder::new();
        let mut dummy_claims = Vec::with_capacity(proto.inputs.len());

        // Process proto inputs.
        for input in &proto.inputs {
            // Note that the returned claims are invalid, respectively don't
            // contain any valid signatures yet. This is solely used to
            // calculate the transaction fee.
            let (utxo, arg, claim) = proto_input_to_native(input, None)?;

            builder = builder.push_input(utxo, arg);
            dummy_claims.push(claim);
        }

        // Process proto outputs.
        let mut utxo_outputs = vec![];
        for output in proto.outputs {
            let (out, tx_out) = proto_output_to_native(&output)?;

            builder = builder.push_output(out);
            utxo_outputs.push(tx_out);
        }

        // Process proto change output, if specified.
        if !proto.disable_change_output {
            let change_output = proto.change_output.unwrap();
            let (out, tx_out) = proto_output_to_native(&change_output)?;

            builder = builder.push_output(out);
            utxo_outputs.push(tx_out);
        }

        // Build the transaction.
        let (tx, tx_args) = builder.build();

        // Select the UTXOs required for the transaction, depending on outputs
        // and calculated fee.
        let selector = SelectionBuilder::new(tx, tx_args)
            .compile(dummy_claims)
            .unwrap();

        let (tx, tx_args) = if proto.disable_change_output {
            selector
                .select_inputs_do_not_set_change(
                    tw_utxo::utxo_selector::InputSelector::UseAll,
                    proto.fee_per_vb as i64,
                )
                .unwrap()
        } else {
            selector
                .select_inputs(
                    tw_utxo::utxo_selector::InputSelector::Ascending,
                    proto.fee_per_vb as i64,
                )
                .unwrap()
        };

        // Calculate the sighashes for the transaction.
        let computer = SighashComputer::new(tx, tx_args);
        let preiamge = computer.preimage_tx().unwrap();
        let (tx, tx_args) = computer.into_transaction();

        // Once the UTXO selection process is completed, we prepare the inputs
        // for the proto structure to be returned.
        let mut proto_inputs = vec![];
        for (input, arg) in tx.inputs.iter().zip(tx_args.utxos_to_sign.iter()) {
            let signing_method = match arg.signing_method {
                SigningMethod::Legacy => UtxoProto::SigningMethod::Legacy,
                SigningMethod::Segwit => UtxoProto::SigningMethod::Segwit,
                SigningMethod::TaprootAll => UtxoProto::SigningMethod::TaprootAll,
                SigningMethod::TaprootOnePrevout => UtxoProto::SigningMethod::TaprootOnePrevout,
            };

            proto_inputs.push(UtxoProto::TxIn {
                txid: input.previous_output.hash.to_vec().into(),
                vout: input.previous_output.index,
                value: arg.amount as u64,
                sequence: input.sequence,
                script_pubkey: arg.script_pubkey.as_data().to_vec().into(),
                sighash_type: UtxoProto::SighashType::All, // TODO
                signing_method,
                weight_estimate: input.weight() as u64,
                leaf_hash: arg
                    .leaf_hash_code_separator
                    .map(|(hash, _)| hash.to_vec().into())
                    .unwrap_or_default(),
            });
        }

        let mut proto_sighashes = vec![];
        for sighash in preiamge.sighashes {
            let signing_method = match sighash.signing_method {
                SigningMethod::Legacy => UtxoProto::SigningMethod::Legacy,
                SigningMethod::Segwit => UtxoProto::SigningMethod::Segwit,
                SigningMethod::TaprootAll => UtxoProto::SigningMethod::TaprootAll,
                SigningMethod::TaprootOnePrevout => UtxoProto::SigningMethod::TaprootOnePrevout,
            };

            proto_sighashes.push(UtxoProto::Sighash {
                sighash: sighash.sighash.into(),
                signing_method,
                sighash_type: UtxoProto::SighashType::UseDefault, // TODO
            });
        }

        Ok(Proto::PreSigningOutput {
            error: Proto::Error::OK,
            error_message: Default::default(),
            txid: tx.txid().into(),
            sighashes: proto_sighashes,
            // Update selected inputs.
            utxo_inputs: proto_inputs,
            utxo_outputs,
            weight_estimate: tx.weight() as u64,
            fee_estimate: tx.fee(proto.fee_per_vb as i64) as u64, // TODO
        })
    }

    pub(crate) fn compile_impl(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> Result<Proto::SigningOutput<'static>> {
        let proto = pre_processor(proto);

        if signatures.len() != proto.inputs.len() {
            return Err(Error::from(
                Proto::Error::Error_unmatched_input_signature_count,
            ));
        }

        // Construct the transaction.
        let mut builder = TransactionBuilder::new();
        let mut claims = vec![]; // TODO

        // Process proto inputs WITH signatures.
        for (sig, input) in signatures.into_iter().zip(proto.inputs.iter()) {
            let (utxo, arg, claim) = proto_input_to_native(input, Some(sig))?;

            builder = builder.push_input(utxo, arg);
            claims.push(claim);
        }

        // Process proto outputs.
        let mut utxo_outputs = vec![];
        for output in proto.outputs {
            let (out, tx_out) = proto_output_to_native(&output)?;

            builder = builder.push_output(out);
            utxo_outputs.push(tx_out);
        }

        // Process proto change output, if specified.
        if !proto.disable_change_output {
            let change_output = proto.change_output.unwrap();
            let (out, tx_out) = proto_output_to_native(&change_output)?;

            builder = builder.push_output(out);
            utxo_outputs.push(tx_out);
        }

        // Build the transaction.
        let (tx, tx_args) = builder.build();

        let computer = SighashComputer::new(tx, tx_args);
        let tx = computer.compile(claims).unwrap();

        let mut utxo_inputs = vec![];
        for input in &tx.inputs {
            utxo_inputs.push(Proto::TransactionInput {
                txid: input.previous_output.hash.to_vec().into(),
                vout: input.previous_output.index,
                sequence: input.sequence,
                script_sig: input.script_sig.as_data().to_vec().into(),
                witness_items: input
                    .witness_items()
                    .iter()
                    .map(|w| w.as_data().to_vec().into())
                    .collect(),
            })
        }

        let mut utxo_outputs = vec![];
        for output in &tx.outputs {
            utxo_outputs.push(Proto::TransactionOutput {
                script_pubkey: output.script_pubkey.as_data().to_vec().into(),
                value: output.value as u64,
                // TODO
                taproot_payload: vec![].into(),
                control_block: vec![].into(),
            });
        }

        let transaction = Proto::Transaction {
            version: proto.version,
            lock_time: proto.lock_time,
            inputs: utxo_inputs,
            outputs: utxo_outputs,
        };

        // Return the full protobuf output.
        Ok(Proto::SigningOutput {
            error: Proto::Error::OK,
            error_message: Default::default(),
            transaction: Some(transaction),
            encoded: tx.encode_out().into(),
            txid: tx.txid().into(),
            weight: tx.weight() as u64,
            fee: tx.fee(proto.fee_per_vb as i64) as u64,
        })
    }
}

// Convenience function for pre-processing of certain fields that must be
// executed on each `CoinEntry` call.
pub(crate) fn pre_processor(mut proto: Proto::SigningInput<'_>) -> Proto::SigningInput<'_> {
    // We automatically set the transaction version to 2.
    if proto.version == 0 {
        proto.version = 2;
    }

    // If an input sequence (timelock, replace-by-fee, etc) of zero is not
    // expliclity enabled, we interpreted a sequence of zero as the max value
    // (default).
    proto.inputs.iter_mut().for_each(|txin| {
        if !txin.sequence_enable_zero && txin.sequence == 0 {
            txin.sequence = u32::MAX
        }
    });

    proto
}

/// Convert `Utxo.proto` error type to `BitcoinV2.proto` error type.
fn handle_utxo_error(utxo_err: &UtxoProto::Error) -> Result<()> {
    let bitcoin_err = match utxo_err {
        UtxoProto::Error::OK => return Ok(()),
        UtxoProto::Error::Error_invalid_leaf_hash => Proto::Error::Error_utxo_invalid_leaf_hash,
        UtxoProto::Error::Error_invalid_sighash_type => {
            Proto::Error::Error_utxo_invalid_sighash_type
        },
        UtxoProto::Error::Error_invalid_lock_time => Proto::Error::Error_utxo_invalid_lock_time,
        UtxoProto::Error::Error_invalid_txid => Proto::Error::Error_utxo_invalid_txid,
        UtxoProto::Error::Error_sighash_failed => Proto::Error::Error_utxo_sighash_failed,
        UtxoProto::Error::Error_missing_sighash_method => {
            Proto::Error::Error_utxo_missing_sighash_method
        },
        UtxoProto::Error::Error_failed_encoding => Proto::Error::Error_utxo_failed_encoding,
        UtxoProto::Error::Error_insufficient_inputs => Proto::Error::Error_utxo_insufficient_inputs,
        UtxoProto::Error::Error_no_outputs_specified => {
            Proto::Error::Error_utxo_no_outputs_specified
        },
        UtxoProto::Error::Error_missing_change_script_pubkey => {
            Proto::Error::Error_utxo_missing_change_script_pubkey
        },
        UtxoProto::Error::Error_signatures_count => {
            Proto::Error::Error_unmatched_input_signature_count
        },
        UtxoProto::Error::Error_invalid_public_key => Proto::Error::Error_invalid_public_key,
        UtxoProto::Error::Error_invalid_private_key => Proto::Error::Error_invalid_private_key,
        UtxoProto::Error::Error_verifying_signature | UtxoProto::Error::Error_invalid_signature => {
            Proto::Error::Error_invalid_sighash
        },
        UtxoProto::Error::Error_internal => Proto::Error::Error_internal,
    };

    Err(Error::from(bitcoin_err))
}
