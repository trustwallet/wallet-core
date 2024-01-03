// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
use crate::context::BinanceContext;
use crate::modules::preimager::{JsonPreimager, JsonTxPreimage};
use crate::modules::wallet_connect::methods::COSMOS_SIGN_AMINO;
use crate::modules::wallet_connect::types::{SignAminoRequest, SignAminoResponse};
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::modules::wallet_connect_signer::WalletConnectSigner;
use tw_coin_entry::signing_output_error;
use tw_cosmos_sdk::modules::serializer::json_serializer::JsonSerializer;
use tw_cosmos_sdk::public_key::secp256k1::Secp256PublicKey;
use tw_cosmos_sdk::public_key::CosmosPublicKey;
use tw_keypair::tw;
use tw_keypair::tw::Curve;
use tw_misc::traits::ToBytesVec;
use tw_proto::WalletConnect::Proto as WCProto;

pub struct BinanceWalletConnectSigner;

impl WalletConnectSigner for BinanceWalletConnectSigner {
    type SigningInput<'a> = WCProto::SigningInput<'a>;
    type SigningOutput = WCProto::SigningOutput<'static>;

    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(WCProto::SigningOutput, e))
    }
}

impl BinanceWalletConnectSigner {
    pub fn sign_impl(
        coin: &dyn CoinContext,
        input: WCProto::SigningInput<'_>,
    ) -> SigningResult<WCProto::SigningOutput<'static>> {
        match input.method.as_ref() {
            COSMOS_SIGN_AMINO => Self::sign_amino(coin, input),
            _ => Err(SigningError(SigningErrorType::Error_not_supported)),
        }
    }

    pub fn sign_amino(
        coin: &dyn CoinContext,
        input: WCProto::SigningInput<'_>,
    ) -> SigningResult<WCProto::SigningOutput<'static>> {
        let request: SignAminoRequest = serde_json::from_str(&input.payload)
            .map_err(|_| SigningError(SigningErrorType::Error_input_parse))?;

        let private_key = tw::PrivateKey::new(input.private_key.to_vec())?;

        // First, check if the `SignAminoRequest::signer_address` belongs to the `SigningInput::private_key`.
        if let Some(ref expected_addr) = request.signer_address {
            Self::validate_signer_address(coin, &private_key, expected_addr)?;
        }

        let JsonTxPreimage { tx_hash, .. } = JsonPreimager::preimage_hash(&request.sign_doc)?;

        let signature_bytes = private_key.sign(tx_hash.as_slice(), Curve::Secp256k1)?;

        let public_key = Secp256PublicKey::from_private_key(coin, &private_key)?;
        let signature =
            JsonSerializer::<BinanceContext>::serialize_signature(&public_key, signature_bytes);

        let result = SignAminoResponse {
            signature,
            signed: request.sign_doc,
        };
        let result_json = serde_json::to_string(&result)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        Ok(WCProto::SigningOutput {
            result: Cow::from(result_json),
            ..WCProto::SigningOutput::default()
        })
    }

    fn validate_signer_address(
        coin: &dyn CoinContext,
        private_key: &tw::PrivateKey,
        expected_addr: &BinanceAddress,
    ) -> SigningResult<()> {
        let tw_public_key = private_key.get_public_key_by_type(coin.public_key_type())?;
        let prefix = None;
        let actual_address =
            BinanceAddress::with_public_key_coin_context(coin, &tw_public_key, prefix)?;

        if actual_address == *expected_addr {
            return Ok(());
        }

        Err(SigningError(SigningErrorType::Error_invalid_address))
    }
}
