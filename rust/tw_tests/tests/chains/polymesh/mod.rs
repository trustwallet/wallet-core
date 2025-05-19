// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ed25519::{sha512::PublicKey, Signature};
use tw_keypair::traits::VerifyingKeyTrait;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polkadot::Proto as PolkadotProto;
use tw_proto::Polymesh::Proto::{self, SigningInput};
use tw_proto::TxCompiler::Proto::{self as CompilerProto, PreSigningOutput};

mod polymesh_address;
mod polymesh_compile;
mod polymesh_sign;

pub const GENESIS_HASH: &str = "0x6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063";
pub const TESTNET_GENESIS_HASH: &str =
    "0x2ace05e703aa50b48c0ccccfc8b424f7aab9a1e2c424ed12e45d20b1e8ffd0d6";
/// Private key for testing.  DO NOT USE, since this is public.
pub const PRIVATE_KEY_1: &str =
    "0x790a0a01ec2e7c7db4abcaffc92ce70a960ef9ad3021dbe3bf327c1c6343aee4";
pub const PUBLIC_KEY_1: &str = "2EANwBfNsFu9KV8JsW5sbhF6ft8bzvw5EW1LCrgHhrqtK6Ys";
pub const PUBLIC_KEY_HEX_1: &str =
    "0x4bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c";

pub const PUBLIC_KEY_2: &str = "2CpqFh8VnwJAjenw4xSUWCaaJ2QwGdhnCikoSEczMhjgqyj7";

fn custom_call_indices(module: u8, method: u8) -> Option<PolkadotProto::CallIndices> {
    Some(PolkadotProto::CallIndices {
        variant: PolkadotProto::mod_CallIndices::OneOfvariant::custom(
            PolkadotProto::CustomCallIndices {
                module_index: module as i32,
                method_index: method as i32,
            },
        ),
    })
}

fn helper_sign(coin: CoinType, input: SigningInput<'_>) -> String {
    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let signed_output = signer.sign(coin, input);
    assert_eq!(signed_output.error, SigningError::OK);

    signed_output.encoded.to_hex()
}

fn helper_encode(coin: CoinType, input: &SigningInput<'_>) -> String {
    let mut pre_imager = PreImageHelper::<PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(coin, input);

    assert_eq!(preimage_output.error, SigningError::OK);
    preimage_output.data.to_hex()
}

fn helper_encode_and_compile(
    coin: CoinType,
    input: Proto::SigningInput,
    signature: &str,
    public_key: &str,
    ed25519: bool,
) -> (String, String) {
    // Step 1: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(coin, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    let preimage = preimage_output.data.to_hex();

    // Step 2: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature_bytes = signature.decode_hex().unwrap();
    let public_key = public_key.decode_hex().unwrap();

    // Verify signature (pubkey & hash & signature)
    if !ed25519 {
        let signature = Signature::try_from(signature_bytes.as_slice()).unwrap();
        let public = PublicKey::try_from(public_key.as_slice()).unwrap();
        assert!(public.verify(signature, preimage_output.data.into()));
    }

    // Compile transaction info
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(coin, &input, vec![signature_bytes], vec![public_key]);
    assert_eq!(output.error, SigningError::OK);
    let signed = output.encoded.to_hex();

    (preimage, signed)
}

fn balance_call(call: Proto::mod_Balance::OneOfmessage_oneof) -> Proto::RuntimeCall<'_> {
    Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::balance_call(Proto::Balance {
            message_oneof: call,
        }),
    }
}

fn identity_call(call: Proto::mod_Identity::OneOfmessage_oneof) -> Proto::RuntimeCall<'_> {
    Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::identity_call(Proto::Identity {
            message_oneof: call,
        }),
    }
}

fn identity_add_auth_call(
    add_auth: Proto::mod_Identity::AddAuthorization,
) -> Proto::RuntimeCall<'_> {
    identity_call(Proto::mod_Identity::OneOfmessage_oneof::add_authorization(
        add_auth,
    ))
}

fn identity_join_identity(
    auth_id: u64,
    call_indices: Option<PolkadotProto::CallIndices>,
) -> Proto::RuntimeCall<'static> {
    identity_call(
        Proto::mod_Identity::OneOfmessage_oneof::join_identity_as_key(
            Proto::mod_Identity::JoinIdentityAsKey {
                call_indices,
                auth_id,
            },
        ),
    )
}

fn staking_call(call: Proto::mod_Staking::OneOfmessage_oneof) -> Proto::RuntimeCall<'_> {
    Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::staking_call(Proto::Staking {
            message_oneof: call,
        }),
    }
}

fn batch_calls(
    kind: Proto::mod_Utility::BatchKind,
    calls: Vec<Proto::RuntimeCall<'static>>,
) -> Proto::RuntimeCall<'static> {
    Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::utility_call(Proto::Utility {
            message_oneof: Proto::mod_Utility::OneOfmessage_oneof::batch(
                Proto::mod_Utility::Batch {
                    kind,
                    calls,
                    ..Default::default()
                },
            ),
        }),
    }
}
