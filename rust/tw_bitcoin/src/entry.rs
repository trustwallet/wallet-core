use std::fmt::Display;

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_coin_entry::modules::plan_builder::PlanBuilder;
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

    fn plan(&self, _coin: &dyn CoinContext, _input: Self::SigningInput<'_>) -> Self::Plan {
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
