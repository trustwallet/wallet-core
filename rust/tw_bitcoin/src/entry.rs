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
use tw_keypair::tw::PublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::BitcoinV2::Proto::mod_Input::mod_InputBuilder::OneOfvariant;
use tw_proto::Utxo::Proto as UtxoProto;
use tw_utxo::address::standard_bitcoin::{StandardBitcoinAddress, StandardBitcoinPrefix};
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;

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

        for input in proto.inputs {
            match input.to_recipient {
                Proto::mod_Input::OneOfto_recipient::builder(builder) => {
                    match builder.variant {
                        OneOfvariant::p2sh(_) => todo!(),
                        OneOfvariant::p2pkh(pubkey) => {
                            let pubkey = pubkey_from_raw(pubkey.as_ref()).unwrap();

                            UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .p2pkh(pubkey)
                                .unwrap();
                        },
                        OneOfvariant::p2wsh(_) => todo!(),
                        OneOfvariant::p2wpkh(pubkey) => {
                            let pubkey = pubkey_from_raw(pubkey.as_ref()).unwrap();

                            UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .p2wpkh(pubkey)
                                .unwrap();
                        },
                        OneOfvariant::p2tr_key_path(payload) => {
                            // TODO: Rename field to `pubkey`?
                            let pubkey = pubkey_from_raw(payload.public_key.as_ref()).unwrap();

                            UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .p2tr_key_path(pubkey)
                                .unwrap();
                        },
                        OneOfvariant::p2tr_script_path(_) => todo!(),
                        OneOfvariant::brc20_inscribe(payload) => {
                            // TODO: Rename field to `pubkey`?
                            let pubkey = pubkey_from_raw(payload.inscribe_to.as_ref()).unwrap();
                            let ticker = payload.ticker.to_string();
                            let value = payload.transfer_amount.to_string();

                            UtxoBuilder::new()
                                .prev_txid(input.txid.as_ref().try_into().unwrap())
                                .prev_index(input.vout)
                                .amount(input.value as i64) // TODO: Just use u64 to begin with?
                                .brc20_transfer(pubkey, ticker, value)
                                .unwrap();
                        },
                        OneOfvariant::ordinal_inscribe(_) => todo!(),
                        OneOfvariant::None => todo!(),
                    }
                },
                Proto::mod_Input::OneOfto_recipient::custom_script(payload) => {
                    let script = Script::from(payload.script_sig.to_vec());

                    UtxoBuilder::new()
                        .prev_txid(input.txid.as_ref().try_into().unwrap())
                        .prev_index(input.vout)
                        .amount(input.value as i64) // TODO: Just use u64 to begin with?
                        // TODO: Signing method:
                        .custom_script_pubkey(script, tw_utxo::signing_mode::SigningMethod::Legacy)
                        .unwrap();
                },
                Proto::mod_Input::OneOfto_recipient::None => todo!(),
            }
        }

        /*
        Ok(Proto::PreSigningOutput {
            error: Proto::Error::OK,
            error_message: Default::default(),
            txid: utxo_presigning.txid,
            sighashes: utxo_presigning.sighashes,
            // Update selected inputs.
            utxo_inputs: utxo_presigning.inputs,
            utxo_outputs,
            weight_estimate: utxo_presigning.weight_estimate,
            fee_estimate: utxo_presigning.fee_estimate,
        })
        */
        todo!()
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
