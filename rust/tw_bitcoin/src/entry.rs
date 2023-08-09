use crate::Result;
use std::fmt::Display;
use bitcoin::key::TweakedPublicKey;
use bitcoin::{ScriptBuf, WPubkeyHash};
use secp256k1::XOnlyPublicKey;
use secp256k1::hashes::Hash;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_coin_entry::modules::plan_builder::PlanBuilder;
use bitcoin::address::{NetworkUnchecked, NetworkChecked, Payload, WitnessVersion};
use tw_keypair::tw::{PrivateKey, PublicKey};
use tw_proto::Utxo::Proto as UtxoProto;
use tw_proto::Bitcoin::Proto as BitcoinProto;

pub type PlaceHolderProto<'a> = tw_proto::Bitcoin::Proto::SigningInput<'a>;

pub struct PlaceHolder;

impl Display for PlaceHolder {
    fn fmt(&self, _f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

impl CoinAddress for PlaceHolder {
    fn data(&self) -> tw_memory::Data {
        todo!()
    }
}

impl JsonSigner for PlaceHolder {
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

pub struct LegacyPlanBuilder;

impl PlanBuilder for LegacyPlanBuilder {
    type SigningInput<'a> = BitcoinProto::SigningInput<'a>;
    type Plan = BitcoinProto::TransactionPlan<'static>;

    fn plan(&self, _coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::Plan {
        let address = Address::from_str(input.to_address.as_ref()).unwrap().0;

        let script_pubkey = match address.address_type().unwrap() {
            bitcoin::AddressType::P2pkh => {
                if let Payload::PubkeyHash(ref hash) = address.payload {
                    ScriptBuf::new_p2pkh(hash)
                } else {
                    panic!()
                }
            },
            bitcoin::AddressType::P2sh => {
                if let Payload::ScriptHash(ref hash) = address.payload {
                    ScriptBuf::new_p2sh(hash)
                } else {
                    panic!()
                }
            },
            bitcoin::AddressType::P2wpkh => {
                if let Payload::WitnessProgram(program) = address.payload {
                    // TODO
                    assert_eq!(program.version(), WitnessVersion::V0);
                    let slice = program.program().as_bytes();
                    let wpubkey_hash = WPubkeyHash::from_slice(slice);
                    ScriptBuf::new_v0_p2wpkh(wpubkey_hash)
                } else {
                    panic!()
                }
            },
            bitcoin::AddressType::P2wsh => todo!(),
            bitcoin::AddressType::P2tr => {
                if let Payload::WitnessProgram(program) = address.payload {
                    // TODO
                    assert_eq!(program.version(), WitnessVersion::V1);
                    let slice = program.program().as_bytes();
                    let xonly = XOnlyPublicKey::from_slice(slice).unwrap();
                    // TODO: another way to do this?
                    let output_key = TweakedPublicKey::dangerous_assume_tweaked(xonly);
                    ScriptBuf::new_v1_p2tr_tweaked(output_key)
                } else {
                    panic!()
                }
            },
            _ => todo!()
        };

        let plan = BitcoinProto::TransactionPlan {
            amount: ,
            available_amount: ,
            fee: ,
            change: ,
            utxos: ,
            branch_id: ,
            error: ,
            output_op_return: ,
            
        };

        todo!()
    }
}

// Legacy implementations used for `Bitcoin.proto` backwards-compatibility.
impl BitcoinEntry {
    #[inline]
    fn compile_legacy(
        &self,
        _coin: &dyn CoinContext,
        _input: PlaceHolderProto<'static>,
        _signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> PlaceHolderProto<'static> {
        todo!()
    }

    #[inline]
    fn plan_builder_legacy(&self) -> Option<LegacyPlanBuilder> {
        None
    }
}

pub struct Address(bitcoin::address::Address<NetworkChecked>);

impl Address {
    // TODO: Implement `FromStr`
    pub fn from_str(string: &str) -> Result<Self> {
        let addr: bitcoin::address::Address<NetworkChecked> = string.parse::<bitcoin::address::Address<_>>().unwrap().require_network(bitcoin::Network::Bitcoin).unwrap();
        Ok(Address(addr))
    }
}

#[test]
fn address() {
    let x = Address::from_str("bc1qunq74p3h8425hr6wllevlvqqr6sezfxj262rff");
}

impl CoinEntry for BitcoinEntry {
    type AddressPrefix = NoPrefix;
    type Address = PlaceHolder;
    type SigningInput<'a> = PlaceHolderProto<'a>;
    type SigningOutput = PlaceHolderProto<'static>;
    type PreSigningOutput = PlaceHolderProto<'static>;

    // Optional modules:
    type JsonSigner = PlaceHolder;
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
        _input: Self::SigningInput<'_>,
        _signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
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
