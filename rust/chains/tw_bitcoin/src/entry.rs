use crate::context::StandardBitcoinContext;
use crate::modules::compiler::BitcoinCompiler;
use crate::modules::planner::BitcoinPlanner;
use crate::modules::signer::BitcoinSigner;
use crate::modules::transaction_util::BitcoinTransactionUtil;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_keypair::tw::PublicKey;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::address::standard_bitcoin::{StandardBitcoinAddress, StandardBitcoinPrefix};

pub struct BitcoinEntry;

impl CoinEntry for BitcoinEntry {
    type AddressPrefix = StandardBitcoinPrefix;
    type Address = StandardBitcoinAddress;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = Proto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = BitcoinPlanner<StandardBitcoinContext>;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;
    type TransactionUtil = BitcoinTransactionUtil;

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
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
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
    fn sign(&self, coin: &dyn CoinContext, proto: Self::SigningInput<'_>) -> Self::SigningOutput {
        BitcoinSigner::<StandardBitcoinContext>::sign(coin, &proto)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        BitcoinCompiler::<StandardBitcoinContext>::preimage_hashes(coin, proto)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        BitcoinCompiler::<StandardBitcoinContext>::compile(coin, proto, signatures, public_keys)
    }

    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        Some(BitcoinPlanner::<StandardBitcoinContext>::default())
    }

    #[inline]
    fn transaction_util(&self) -> Option<Self::TransactionUtil> {
        Some(BitcoinTransactionUtil)
    }
}
