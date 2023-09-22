// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_evm::address::Address;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::evm_entry::EvmEntry;
use tw_evm::modules::abi_encoder::AbiEncoder;
use tw_evm::modules::compiler::Compiler;
use tw_evm::modules::json_signer::EthJsonSigner;
use tw_evm::modules::rlp_encoder::RlpEncoder;
use tw_evm::modules::signer::Signer;
use tw_keypair::tw::PublicKey;
use tw_proto::Ethereum::Proto;
use tw_proto::EthereumAbi::Proto as AbiProto;
use tw_proto::EthereumRlp::Proto as RlpProto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct EthereumEntry;

impl CoinEntry for EthereumEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = CompilerProto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = EthJsonSigner<StandardEvmContext>;
    type PlanBuilder = NoPlanBuilder;

    #[inline]
    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        Address::from_str(address)
    }

    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        Ok(Address::with_secp256k1_pubkey(public_key))
    }

    #[inline]
    fn sign(&self, _coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        Signer::<StandardEvmContext>::sign_proto(input)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        Compiler::<StandardEvmContext>::preimage_hashes(input)
    }

    #[inline]
    fn compile(
        &self,
        _coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        Compiler::<StandardEvmContext>::compile(input, signatures, public_keys)
    }

    #[inline]
    fn json_signer(&self) -> Option<Self::JsonSigner> {
        Some(EthJsonSigner::default())
    }
}

impl EvmEntry for EthereumEntry {
    type RlpEncodingInput<'a> = RlpProto::EncodingInput<'a>;
    type RlpEncodingOutput = RlpProto::EncodingOutput<'static>;

    type DecodeContractCallInput<'a> = AbiProto::ContractCallDecodingInput<'a>;
    type DecodeContractCallOutput = AbiProto::ContractCallDecodingOutput<'static>;

    type DecodeParamsInput<'a> = AbiProto::ParamsDecodingInput<'a>;
    type DecodeParamsOutput = AbiProto::ParamsDecodingOutput<'static>;

    type GetFunctionSignatureInput<'a> = AbiProto::FunctionGetTypeInput<'a>;

    #[inline]
    fn encode_rlp(input: Self::RlpEncodingInput<'_>) -> Self::RlpEncodingOutput {
        RlpEncoder::<StandardEvmContext>::encode_with_proto(input)
    }

    #[inline]
    fn decode_contract_call(
        input: Self::DecodeContractCallInput<'_>,
    ) -> Self::DecodeContractCallOutput {
        AbiEncoder::<StandardEvmContext>::decode_contract_call(input)
    }

    #[inline]
    fn decode_params(input: Self::DecodeParamsInput<'_>) -> Self::DecodeParamsOutput {
        AbiEncoder::<StandardEvmContext>::decode_params(input)
    }

    #[inline]
    fn get_function_signature(input: Self::GetFunctionSignatureInput<'_>) -> String {
        AbiEncoder::<StandardEvmContext>::get_function_signature(input)
    }
}
