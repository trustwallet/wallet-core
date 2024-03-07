use crate::modules::signer::Signer;
use crate::{Error, Result};
use bitcoin::address::NetworkChecked;
use std::borrow::Cow;
use std::fmt::Display;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_coin_entry::signing_output_error;
use tw_hash::H256;
use tw_keypair::ecdsa::signature::Signature;
use tw_keypair::schnorr;
use tw_keypair::tw::PublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto::mod_Input::mod_InputBuilder::OneOfvariant;
use tw_proto::BitcoinV2::Proto::mod_Input::OneOfto_recipient;
use tw_proto::BitcoinV2::Proto::mod_ToPublicKeyOrHash::OneOfto_address;
use tw_proto::BitcoinV2::Proto::{self, mod_Output};
use tw_proto::Utxo::Proto as UtxoProto;
use tw_utxo::address::standard_bitcoin::{StandardBitcoinAddress, StandardBitcoinPrefix};
use tw_utxo::script::{Script, Witness};
use tw_utxo::sighash_computer::SighashComputer;
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::standard_transaction::builder::{
    OutputBuilder, SpendingScriptBuilder, TransactionBuilder, UtxoBuilder,
};
use tw_utxo::transaction::transaction_fee::TransactionFee;
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
        Signer::sign_proto(_coin, proto)
            .unwrap_or_else(|err| signing_output_error!(Proto::SigningOutput, err))
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        self.preimage_hashes_impl(_coin, proto)
            .unwrap_or_else(|err| signing_output_error!(Proto::PreSigningOutput, err))
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
            .unwrap_or_else(|err| signing_output_error!(Proto::SigningOutput, err))
    }

    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        None
    }
}

// TODO: Adjust error type
fn pubkey_from_raw(pubkey: &[u8]) -> Result<PublicKey> {
    PublicKey::new(pubkey.to_vec(), tw_keypair::tw::PublicKeyType::Secp256k1)
        .map_err(|_| Error::from(Proto::Error::Error_internal))
}

impl BitcoinEntry {
    pub(crate) fn preimage_hashes_impl(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Result<Proto::PreSigningOutput<'static>> {
        let proto = pre_processor(proto);

        // Construct the transaction.
        let mut builder = TransactionBuilder::new();
        let mut dummy_claims = vec![]; // TODO

        for input in proto.inputs {
            match input.to_recipient {
                Proto::mod_Input::OneOfto_recipient::builder(b) => {
                    match b.variant {
                        OneOfvariant::p2sh(_) => todo!(),
                        OneOfvariant::p2pkh(pubkey) => {
                            let pubkey = pubkey_from_raw(&pubkey).unwrap();

                            let (utxo, arg) = UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .p2pkh(pubkey.clone())
                                .unwrap();

                            let sig = Signature::from_bytes(&vec![1; 65]).unwrap();
                            let claim = SpendingScriptBuilder::new().p2pkh(sig, pubkey).unwrap();

                            builder = builder.push_input(utxo, arg);
                            dummy_claims.push(claim);
                        },
                        OneOfvariant::p2wsh(_) => todo!(),
                        OneOfvariant::p2wpkh(pubkey) => {
                            let pubkey = pubkey_from_raw(&pubkey).unwrap();

                            let (utxo, arg) = UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .p2wpkh(pubkey.clone())
                                .unwrap();

                            let sig = Signature::from_bytes(&vec![1; 65]).unwrap();
                            let claim = SpendingScriptBuilder::new().p2wpkh(sig, pubkey).unwrap();

                            builder = builder.push_input(utxo, arg);
                            dummy_claims.push(claim);
                        },
                        OneOfvariant::p2tr_key_path(payload) => {
                            // TODO: Rename field to `pubkey`?
                            let pubkey = pubkey_from_raw(&payload.public_key).unwrap();

                            let (utxo, arg) = UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .p2tr_key_path(pubkey)
                                .unwrap();

                            let sig = schnorr::Signature::from_bytes(&vec![1; 64]).unwrap();
                            let claim = SpendingScriptBuilder::new().p2tr_key_path(sig).unwrap();

                            builder = builder.push_input(utxo, arg);
                            dummy_claims.push(claim);
                        },
                        OneOfvariant::p2tr_script_path(_) => todo!(),
                        OneOfvariant::brc20_inscribe(payload) => {
                            // TODO: Rename field to `pubkey`?
                            let pubkey = pubkey_from_raw(&payload.inscribe_to).unwrap();
                            let ticker = payload.ticker.to_string();
                            let value = payload.transfer_amount.to_string();

                            let (utxo, arg) = UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .brc20_transfer(pubkey.clone(), ticker.clone(), value.clone())
                                .unwrap();

                            let sig = schnorr::Signature::from_bytes(&vec![1; 64]).unwrap();
                            let claim = SpendingScriptBuilder::new()
                                .brc20_transfer(sig, pubkey, ticker, value)
                                .unwrap();

                            builder = builder.push_input(utxo, arg);
                            dummy_claims.push(claim);
                        },
                        OneOfvariant::ordinal_inscribe(_) => todo!(),
                        OneOfvariant::None => todo!(),
                    }
                },
                Proto::mod_Input::OneOfto_recipient::custom_script(payload) => {
                    let script_pubkey = Script::from(payload.script_pubkey.to_vec());
                    let script_sig = Script::from(payload.script_sig.to_vec());
                    let mut witness = Witness::new();
                    for item in payload.witness_items {
                        witness.push_item(Script::from(item.to_vec()));
                    }

                    let (utxo, arg) = UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64) // TODO: Just use u64 to begin with?
                        // TODO: Signing method:
                        .custom_script_pubkey(
                            script_pubkey,
                            tw_utxo::signing_mode::SigningMethod::Legacy,
                        )
                        .unwrap();

                    let claim = SpendingScriptBuilder::custom_script_sig_witness(
                        Some(script_sig),
                        Some(witness),
                    );

                    builder = builder.push_input(utxo, arg);
                    dummy_claims.push(claim);
                },
                Proto::mod_Input::OneOfto_recipient::None => todo!(),
            };
        }

        for output in proto.outputs {
            let out = match output.to_recipient {
                Proto::mod_Output::OneOfto_recipient::builder(builder) => match builder.variant {
                    mod_Output::mod_OutputBuilder::OneOfvariant::p2sh(_) => todo!(),
                    mod_Output::mod_OutputBuilder::OneOfvariant::p2pkh(payload) => {
                        match payload.to_address {
                            OneOfto_address::pubkey(pubkey) => {
                                let pubkey = pubkey_from_raw(&pubkey).unwrap();

                                OutputBuilder::new()
                                    .amount(output.value as i64)
                                    .p2pkh(pubkey)
                                    .unwrap()
                            },
                            OneOfto_address::hash(_) => todo!(),
                            OneOfto_address::None => todo!(),
                        }
                    },
                    mod_Output::mod_OutputBuilder::OneOfvariant::p2wsh(_) => todo!(),
                    mod_Output::mod_OutputBuilder::OneOfvariant::p2wpkh(payload) => {
                        match payload.to_address {
                            OneOfto_address::pubkey(pubkey) => {
                                let pubkey = pubkey_from_raw(&pubkey).unwrap();

                                OutputBuilder::new()
                                    .amount(output.value as i64)
                                    .p2wpkh(pubkey)
                                    .unwrap()
                            },
                            OneOfto_address::hash(_) => todo!(),
                            OneOfto_address::None => todo!(),
                        }
                    },
                    mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_key_path(pubkey) => {
                        let pubkey = pubkey_from_raw(&pubkey).unwrap();

                        OutputBuilder::new()
                            .amount(output.value as i64)
                            .p2tr_key_path(pubkey)
                            .unwrap()
                    },
                    mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_script_path(payload) => {
                        let pubkey = pubkey_from_raw(&payload.internal_key).unwrap();
                        let merkle_root: H256 = payload.merkle_root.as_ref().try_into().unwrap();

                        OutputBuilder::new()
                            .amount(output.value as i64)
                            .p2tr_script_path(pubkey, merkle_root)
                            .unwrap()
                    },
                    mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_dangerous_assume_tweaked(
                        _,
                    ) => todo!(),
                    mod_Output::mod_OutputBuilder::OneOfvariant::brc20_inscribe(payload) => {
                        let pubkey = pubkey_from_raw(&payload.inscribe_to).unwrap();
                        let ticker = payload.ticker.to_string();
                        let value = payload.transfer_amount.to_string();

                        OutputBuilder::new()
                            .amount(output.value as i64)
                            .brc20_transfer(pubkey, ticker, value)
                            .unwrap()
                    },
                    mod_Output::mod_OutputBuilder::OneOfvariant::ordinal_inscribe(_) => todo!(),
                    mod_Output::mod_OutputBuilder::OneOfvariant::None => todo!(),
                },
                _ => todo!(),
            };

            // Add output to builder
            builder = builder.push_output(out);
        }

        // Build the transaction.
        let (tx, tx_args) = builder.build();

        dbg!(&tx, &tx_args.utxos_to_sign);

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

        let computer = SighashComputer::new(tx, tx_args);
        let preiamge = computer.preimage_tx().unwrap();
        let (tx, tx_args) = computer.into_transaction();

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

        let mut utxo_outputs = vec![];
        // TODO

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
                sighash_type: UtxoProto::SighashType::All, // TODO
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

        // There must be a signature for each input.
        if proto.inputs.len() != signatures.len() {
            return Err(Error::from(
                Proto::Error::Error_unmatched_input_signature_count,
            ));
        }

        // Generate claims for all the inputs.
        let mut utxo_input_claims: Vec<UtxoProto::TxInClaim> = vec![];
        for (input, signature) in proto.inputs.iter().zip(signatures.into_iter()) {
            let utxo_claim =
                crate::modules::transactions::InputClaimBuilder::utxo_claim_from_proto(
                    input, signature,
                )?;

            utxo_input_claims.push(utxo_claim);
        }

        // Prepare all the outputs.
        let mut utxo_outputs = vec![];
        for output in &proto.outputs {
            let utxo = crate::modules::transactions::OutputBuilder::utxo_from_proto(output)?;

            utxo_outputs.push(utxo);
        }

        // Prepare PreSerialization input for Utxo compiler.
        let utxo_preserializtion = UtxoProto::PreSerialization {
            version: proto.version,
            lock_time: proto.lock_time.clone(),
            inputs: utxo_input_claims.clone(),
            outputs: utxo_outputs
                .iter()
                .map(|out| UtxoProto::TxOut {
                    value: out.value,
                    script_pubkey: Cow::Borrowed(&out.script_pubkey),
                })
                .collect(),
            weight_base: proto.fee_per_vb,
        };

        // Compile the transaction, build the final encoded transaction
        // containing the signatures/scriptSigs/witnesses.
        let utxo_serialized = tw_utxo::compiler::Compiler::compile(utxo_preserializtion);
        handle_utxo_error(&utxo_serialized.error)?;

        let mut total_input_amount = 0;

        // Prepare `Proto::TransactionInput` for end result.
        let mut proto_inputs = vec![];
        for input in utxo_input_claims {
            total_input_amount += input.value;

            proto_inputs.push(Proto::TransactionInput {
                txid: Cow::Owned(input.txid.to_vec()),
                vout: input.vout,
                sequence: input.sequence,
                script_sig: Cow::Owned(input.script_sig.into_owned()),
                witness_items: input
                    .witness_items
                    .into_iter()
                    .map(|item| Cow::Owned(item.into_owned()))
                    .collect(),
            });
        }

        // Prepare `Proto::TransactionOutput` for end result.
        let mut proto_outputs = vec![];
        for output in utxo_outputs {
            proto_outputs.push(Proto::TransactionOutput {
                script_pubkey: output.script_pubkey,
                value: output.value,
                taproot_payload: output.taproot_payload,
                control_block: output.control_block,
            });
        }

        // Prepare `Proto::Transaction` for end result.
        let transaction = Proto::Transaction {
            version: proto.version,
            lock_time: proto.lock_time,
            inputs: proto_inputs,
            outputs: proto_outputs,
        };

        let total_output_amount = transaction
            .outputs
            .iter()
            .map(|output| output.value)
            .sum::<u64>();

        // Sanity check.
        debug_assert_eq!(transaction.inputs.len(), proto.inputs.len());
        debug_assert_eq!(transaction.outputs.len(), proto.outputs.len());
        // Every output is accounted for, including the fee.
        debug_assert_eq!(
            total_input_amount,
            total_output_amount + utxo_serialized.fee
        );

        // Return the full protobuf output.
        Ok(Proto::SigningOutput {
            error: Proto::Error::OK,
            error_message: Default::default(),
            transaction: Some(transaction),
            encoded: utxo_serialized.encoded,
            txid: utxo_serialized.txid,
            weight: utxo_serialized.weight,
            fee: utxo_serialized.fee,
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

#[rustfmt::skip]
/// Convert `Utxo.proto` error type to `BitcoinV2.proto` error type.
fn handle_utxo_error(utxo_err: &UtxoProto::Error) -> Result<()> {
    let bitcoin_err = match utxo_err {
        UtxoProto::Error::OK => return Ok(()),
        UtxoProto::Error::Error_invalid_leaf_hash => Proto::Error::Error_utxo_invalid_leaf_hash,
        UtxoProto::Error::Error_invalid_sighash_type => Proto::Error::Error_utxo_invalid_sighash_type,
        UtxoProto::Error::Error_invalid_lock_time => Proto::Error::Error_utxo_invalid_lock_time,
        UtxoProto::Error::Error_invalid_txid => Proto::Error::Error_utxo_invalid_txid,
        UtxoProto::Error::Error_sighash_failed => Proto::Error::Error_utxo_sighash_failed,
        UtxoProto::Error::Error_missing_sighash_method => Proto::Error::Error_utxo_missing_sighash_method,
        UtxoProto::Error::Error_failed_encoding => Proto::Error::Error_utxo_failed_encoding,
        UtxoProto::Error::Error_insufficient_inputs => Proto::Error::Error_utxo_insufficient_inputs,
        UtxoProto::Error::Error_no_outputs_specified => Proto::Error::Error_utxo_no_outputs_specified,
        UtxoProto::Error::Error_missing_change_script_pubkey => Proto::Error::Error_utxo_missing_change_script_pubkey,
        UtxoProto::Error::Error_signatures_count => Proto::Error::Error_unmatched_input_signature_count,
        UtxoProto::Error::Error_internal => Proto::Error::Error_internal,
    };

    Err(Error::from(bitcoin_err))
}
