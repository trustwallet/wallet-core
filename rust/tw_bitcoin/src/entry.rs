use bitcoin::address::{NetworkChecked, Payload};
use secp256k1::hashes::Hash;
use std::borrow::Cow;
use std::fmt::Display;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::{PrivateKey, PublicKey, PublicKeyType};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

// Convenience aliases.
pub use aliases::*;
pub mod aliases {
    use super::Proto;

    pub type ProtoOutputRecipient<'a> = Proto::mod_Output::OneOfto_recipient<'a>;
    pub type ProtoOutputBuilder<'a> = Proto::mod_Output::mod_Builder::OneOfvariant<'a>;
    pub type ProtoPubkeyOrHash<'a> = Proto::mod_ToPublicKeyOrHash::OneOfto_address<'a>;
    pub type ProtoInputRecipient<'a> = Proto::mod_Input::OneOfto_recipient<'a>;
    pub type ProtoInputBuilder<'a> = Proto::mod_Input::mod_Builder::OneOfvariant<'a>;
}

pub type PlaceHolderProto<'a> = tw_proto::Bitcoin::Proto::SigningInput<'a>;

pub struct PlaceHolder;

impl CoinContext for PlaceHolder {
    fn public_key_type(&self) -> PublicKeyType {
        todo!()
    }
}

pub struct NoJsonSigner;

impl JsonSigner for NoJsonSigner {
    fn sign_json(
        &self,
        _coin: &dyn tw_coin_entry::coin_context::CoinContext,
        _input_json: &str,
        _key: &PrivateKey,
    ) -> tw_coin_entry::error::SigningResult<String> {
        todo!()
    }
}

pub struct BitcoinEntry;

pub struct Address(bitcoin::address::Address<NetworkChecked>);

impl Display for Address {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl CoinAddress for Address {
    fn data(&self) -> tw_memory::Data {
        match &self.0.payload {
            Payload::PubkeyHash(hash) => hash.to_byte_array().into(),
            Payload::ScriptHash(hash) => hash.to_byte_array().into(),
            Payload::WitnessProgram(wp) => wp.program().as_bytes().into(),
            _ => todo!(), // Payload is non-exhaustive
        }
    }
}

impl CoinEntry for BitcoinEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = Proto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;

    #[inline]
    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        _address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        todo!()
    }

    #[inline]
    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        _public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        todo!()
    }

    #[inline]
    fn sign(&self, _coin: &dyn CoinContext, proto: Self::SigningInput<'_>) -> Self::SigningOutput {
        // TODO: Can we avoid cloning here?
        let pre_signed = self.preimage_hashes(_coin, proto.clone());
        // TODO: Check error

        let signatures =
            crate::modules::Signer::signatures_from_proto(&pre_signed, proto.private_key.to_vec())
                .unwrap();

        self.compile(_coin, proto, signatures, vec![])
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        let mut utxo_inputs = vec![];
        for input in proto.inputs {
            let txin = crate::modules::InputBuilder::utxo_from_proto(&input).unwrap();
            utxo_inputs.push(txin);
        }

        let mut utxo_outputs = vec![];
        for output in proto.outputs {
            let utxo = crate::modules::OutputBuilder::utxo_from_proto(&output).unwrap();
            utxo_outputs.push(utxo);
        }

        let change_output = if proto.disable_change_output {
            Cow::default()
        } else {
            crate::modules::OutputBuilder::utxo_from_proto(&proto.change_output.unwrap())
                .unwrap()
                .script_pubkey
        };

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
            weight_base: proto.sat_vb,
            change_script_pubkey: change_output,
            disable_change_output: proto.disable_change_output,
        };

        let utxo_presigning = tw_utxo::compiler::Compiler::preimage_hashes(utxo_signing);

        Proto::PreSigningOutput {
            error: 0,
            sighashes: utxo_presigning.sighashes,
            // Update selected inputs.
            utxo_inputs: utxo_presigning.inputs,
            utxo_outputs,
        }
    }

    #[inline]
    fn compile(
        &self,
        _coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        if proto.inputs.len() != signatures.len() {
            // Error
            //todo!()
        }

        let mut utxo_input_claims: Vec<UtxoProto::TxInClaim> = vec![];

        // Generate claims for all the inputs.
        for (input, signature) in proto.inputs.iter().zip(signatures.into_iter()) {
            let utxo_claim =
                crate::modules::InputBuilder::utxo_claim_from_proto(input, signature).unwrap();
            utxo_input_claims.push(utxo_claim);
        }

        // Process all the outputs.
        let mut utxo_outputs = vec![];
        for output in proto.outputs {
            let utxo = crate::modules::OutputBuilder::utxo_from_proto(&output).unwrap();

            utxo_outputs.push(utxo);
        }

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
            weight_base: proto.sat_vb,
        };

        let utxo_serialized = tw_utxo::compiler::Compiler::compile(utxo_preserializtion);

        // Prepare `Proto::TransactionInput` protobufs for signing output.
        let mut proto_inputs = vec![];
        for input in utxo_input_claims {
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

        // Prepare `Proto::TransactionOutput` protobufs for output.
        let mut proto_outputs = vec![];
        for output in utxo_outputs {
            proto_outputs.push(Proto::TransactionOutput {
                recipient: Cow::default(),
                script_pubkey: output.script_pubkey,
                amount: output.value,
                control_block: output.control_block,
            });
        }

        // Prepare `Proto::Transaction` protobuf for output.
        let transaction = Proto::Transaction {
            version: proto.version,
            lock_time: proto.lock_time,
            inputs: proto_inputs,
            outputs: proto_outputs,
        };

        // Return the full protobuf output.
        Proto::SigningOutput {
            transaction: Some(transaction),
            encoded: utxo_serialized.encoded,
            // TODO: Should be returned by `tw_utxo`.
            transaction_id: Cow::default(),
            error: 0,
            fee: 0,
        }
    }

    #[inline]
    fn json_signer(&self) -> Option<Self::JsonSigner> {
        None
    }

    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        None
    }
}
