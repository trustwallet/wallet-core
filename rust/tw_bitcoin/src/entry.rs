use crate::Result;
use bitcoin::address::{NetworkChecked, Payload};
use bitcoin::taproot::ControlBlock;
use bitcoin::{OutPoint, ScriptBuf, Sequence, TxIn, Txid, Witness};
use secp256k1::hashes::Hash;
use std::fmt::Display;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::{PrivateKey, PublicKey};
use tw_proto::BitcoinV2::Proto;

pub type PlaceHolderProto<'a> = tw_proto::Bitcoin::Proto::SigningInput<'a>;

pub struct PlaceHolder;

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
    type SigningOutput = PlaceHolderProto<'static>;
    type PreSigningOutput = PlaceHolderProto<'static>;

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
    fn sign(&self, _coin: &dyn CoinContext, _input: Self::SigningInput<'_>) -> Self::SigningOutput {
        todo!()
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        _input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        todo!()
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
            // Return error.
            todo!()
        }

        let mut txins: Vec<TxIn> = vec![];
        for (index, input) in proto.inputs.iter().enumerate() {
            // TODO:
            let pubkey = bitcoin::PublicKey::from_slice(&[]).unwrap();
            let sig_slice = &signatures[index];

            let (script_sig, witness) = match input.variant {
                Proto::mod_Input::Variant::P2Pkh => {
                    let sig = bitcoin::ecdsa::Signature::from_slice(sig_slice).unwrap();

                    (
                        ScriptBuf::builder()
                            .push_slice(sig.serialize())
                            .push_key(&pubkey)
                            .into_script(),
                        Witness::new(),
                    )
                },
                Proto::mod_Input::Variant::P2Wpkh => {
                    let sig = bitcoin::ecdsa::Signature::from_slice(sig_slice).unwrap();

                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.serialize());
                        w.push(pubkey.to_bytes());
                        w
                    })
                },
                Proto::mod_Input::Variant::P2TrKeyPath => {
                    let sig = bitcoin::taproot::Signature::from_slice(sig_slice).unwrap();

                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w
                    })
                },
                Proto::mod_Input::Variant::P2TrScriptPath => {
                    let sig = bitcoin::taproot::Signature::from_slice(sig_slice).unwrap();
                    let control_block = ControlBlock::decode(input.control_block.as_ref()).unwrap();

                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w.push(input.payload.as_ref());
                        w.push(control_block.serialize());
                        w
                    })
                },
                _ => panic!(),
            };

            txins.push(TxIn {
                previous_output: OutPoint {
                    txid: Txid::from_slice(input.txid.as_ref()).unwrap(),
                    vout: input.vout,
                },
                script_sig,
                // TODO:
                sequence: Sequence::default(),
                witness,
            })
        }

        todo!()
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
