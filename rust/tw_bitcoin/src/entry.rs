use crate::modules::signer::Signer;
use crate::{Error, Result};
use bitcoin::address::NetworkChecked;
use std::borrow::Cow;
use std::fmt::Display;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_coin_entry::prefix::NoPrefix;
use tw_coin_entry::signing_output_error;
use tw_keypair::tw::PublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

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
    type AddressPrefix = NoPrefix;
    type Address = Address;
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
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        self.parse_address_unchecked(coin, address)
    }

    #[inline]
    fn parse_address_unchecked(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
    ) -> AddressResult<Self::Address> {
        let address = bitcoin::address::Address::from_str(address)
            .map_err(|_| AddressError::FromHexError)?
            // At this moment, we support mainnet only.
            // This check will be removed in coming PRs.
            .require_network(bitcoin::Network::Bitcoin)
            .map_err(|_| AddressError::InvalidInput)?;

        Ok(Address(address))
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let pubkey = match public_key {
            PublicKey::Secp256k1(pubkey) | PublicKey::Secp256k1Extended(pubkey) => pubkey,
            _ => return Err(AddressError::InvalidInput),
        };

        let pubkey = bitcoin::PublicKey::from_slice(pubkey.to_vec().as_ref())
            .map_err(|_| AddressError::InvalidInput)?;

        let address: bitcoin::address::Address<NetworkChecked> = bitcoin::address::Address::new(
            bitcoin::Network::Bitcoin,
            bitcoin::address::Payload::PubkeyHash(pubkey.pubkey_hash()),
        );

        Ok(Address(address))
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

impl BitcoinEntry {
    pub(crate) fn preimage_hashes_impl(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Result<Proto::PreSigningOutput<'static>> {
        let proto = pre_processor(proto);

        // Convert input builders into Utxo inputs.
        let utxo_inputs = proto
            .inputs
            .iter()
            .map(crate::modules::transactions::InputBuilder::utxo_from_proto)
            .collect::<Result<Vec<_>>>()?;

        // Convert output builders into Utxo outputs (does not contain the change output).
        let mut utxo_outputs = proto
            .outputs
            .iter()
            .map(crate::modules::transactions::OutputBuilder::utxo_from_proto)
            .collect::<Result<Vec<_>>>()?;

        // If automatic change output creation is enabled (by default), a change
        // script must be provided.
        let change_script_pubkey = if proto.disable_change_output {
            Cow::default()
        } else {
            // Convert output builder to Utxo output.
            let output = crate::modules::transactions::OutputBuilder::utxo_from_proto(
                &proto
                    .change_output
                    .ok_or_else(|| Error::from(Proto::Error::Error_invalid_change_output))?,
            )?;

            output.script_pubkey
        };

        // Prepare SigningInput for Utxo sighash generation.
        let utxo_signing = UtxoProto::SigningInput {
            version: proto.version,
            lock_time: proto.lock_time,
            inputs: utxo_inputs.clone(),
            outputs: utxo_outputs
                .iter()
                .map(|output| UtxoProto::TxOut {
                    value: output.value,
                    script_pubkey: Cow::Borrowed(&output.script_pubkey),
                })
                .collect(),
            input_selector: proto.input_selector,
            weight_base: proto.fee_per_vb,
            change_script_pubkey,
            disable_change_output: proto.disable_change_output,
        };

        // Generate the sighashes to be signed. This also selects the inputs
        // according to the input selecter and appends a change output, if
        // enabled.
        let utxo_presigning = tw_utxo::compiler::Compiler::preimage_hashes(utxo_signing);
        handle_utxo_error(&utxo_presigning.error)?;

        // Check whether the change output is present.
        if !proto.disable_change_output {
            // Change output has been added.
            debug_assert_eq!(utxo_presigning.outputs.len(), utxo_outputs.len() + 1);

            let change_output = utxo_presigning
                .outputs
                .last()
                .expect("expected change output");

            // Push it to the list of outputs.
            utxo_outputs.push(Proto::mod_PreSigningOutput::TxOut {
                value: change_output.value,
                script_pubkey: change_output.script_pubkey.to_vec().into(),
                control_block: Default::default(),
                taproot_payload: Default::default(),
            })
        }

        // Sanity check.
        debug_assert!(utxo_presigning.inputs.len() <= proto.inputs.len());
        debug_assert_eq!(utxo_presigning.outputs.len(), utxo_outputs.len());

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
    };

    Err(Error::from(bitcoin_err))
}
