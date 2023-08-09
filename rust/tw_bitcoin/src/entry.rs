use crate::Result;
use std::fmt::Display;
use bitcoin::key::TweakedPublicKey;
use bitcoin::{ScriptBuf, WPubkeyHash, VarInt};
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
use tw_proto::Bitcoin::Proto::{self as BitcoinProto, UnspentTransaction};
use std::borrow::Cow;

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

impl LegacyPlanBuilder {
    fn plan<'a>(&self, _coin: &dyn CoinContext, proto: BitcoinProto::SigningInput<'a>) -> BitcoinProto::TransactionPlan<'a> {
        let mut inputs = proto.utxo.to_vec();
        inputs.sort_by(|a, b| a.amount.partial_cmp(&b.amount).unwrap());

        let mut total_selected_amount = 0;
        let mut remaining = proto.amount as u64;
        let selected: Vec<UnspentTransaction<'_>> = inputs.into_iter().take_while(|input| {
            remaining = remaining.saturating_sub(input.amount as u64);
            total_selected_amount += input.amount as u64;

            remaining != 0
        }).collect();

        if remaining != 0 {
            // Return error
            todo!()
        }

        let mut weight = 0;
        let mut witness_weight = 0;

        let scale_factor = 1;
        for input in &selected {
            weight += scale_factor*(
                // Outpoint
                32 + 4
                // Sequence
                + 4
                + VarInt(input.script.as_ref().len() as u64).len()
                + input.script.len()
            );

            if !input.spendingScript.is_empty() {
                witness_weight += 1;
                weight += input.spendingScript.len();
            }
        }

        let change = total_selected_amount - proto.amount as u64;

        BitcoinProto::TransactionPlan {
            amount: proto.amount as i64,
            available_amount: total_selected_amount as i64,
            fee: 0,
            change: change as i64,
            utxos: selected,
            // Used for Zcash
            branch_id: Cow::default(),
            error: tw_proto::Common::Proto::SigningError::OK,
            // Used for other chain(s).
            output_op_return: Cow::default(),
            // Used for other chain(s).
            preblockhash: Cow::default(),
            // Used for other chain(s).
            preblockheight: 0,
        }
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
