use tw_coin_entry::error::prelude::*;
use tw_hash::{blake2::blake2_b, H256, H512};
use tw_keypair::{
    ed25519::{sha512::KeyPair, Signature},
    traits::SigningKeyTrait,
    KeyPairError,
};
use tw_scale::{impl_enum_scale, impl_struct_scale, RawOwned, ToScale};

use crate::address::*;
use crate::extensions::*;
use crate::{EncodeError, EncodeResult};

/// Transaction hash type.
pub type TxHash = H256;
/// Block hash type.
pub type BlockHash = H256;
/// Block number type.
pub type BlockNumber = u32;

impl_enum_scale!(
    /// Supported signature types for Substrate transactions.
    #[derive(Clone, Debug)]
    pub enum MultiSignature {
        /// Ed25519 signature represented as 64 bytes.
        Ed25519(H512) = 0x0,
        /// Sr25519 signature represented as 64 bytes.
        Sr25519(H512) = 0x1,
        //Ecdsa([u8; 65]) = 0x2,
    }
);

impl From<Signature> for MultiSignature {
    fn from(sig: Signature) -> Self {
        Self::Ed25519(sig.to_bytes())
    }
}

/// Represents a module and method call index pair for a Substrate transaction.
#[derive(Clone, Debug)]
pub struct CallIndex(Option<(u8, u8)>);

impl CallIndex {
    /// Creates a CallIndex from TrustWallet's representation of call indices.
    pub fn from_tw(call_index: Option<(i32, i32)>) -> EncodeResult<Self> {
        let call_index = match call_index {
            Some((module_index, method_index)) => {
                if module_index > u8::MAX as i32 || method_index > u8::MAX as i32 {
                    Err(EncodeError::InvalidCallIndex)
                        .into_tw()
                        .context("Module or method call index too large.")?;
                }
                Some((module_index as u8, method_index as u8))
            },
            _ => None,
        };
        Ok(Self(call_index))
    }

    /// Creates a CallIndex that must contain valid indices.
    pub fn required_from_tw(call_index: Option<(i32, i32)>) -> EncodeResult<Self> {
        if call_index.is_none() {
            Err(EncodeError::MissingCallIndices)
                .into_tw()
                .context("Call indices are required.")?;
        }
        Self::from_tw(call_index)
    }

    /// Wraps a SCALE-encodable value with this CallIndex.
    pub fn wrap<T: ToScale>(self, value: T) -> WithCallIndex<T> {
        WithCallIndex {
            value,
            call_index: self,
        }
    }
}

/// Wrapper type that combines a SCALE-encodable value with optional call indices.
#[derive(Clone, Debug)]
pub struct WithCallIndex<T: ToScale> {
    /// The wrapped SCALE-encodable value.
    value: T,
    /// Associated call indices.
    call_index: CallIndex,
}

impl<T: ToScale> WithCallIndex<T> {
    pub fn map<U: ToScale, F: Fn(T) -> U>(self, f: F) -> WithCallIndex<U> {
        WithCallIndex {
            value: f(self.value),
            call_index: self.call_index,
        }
    }
}

impl<T: ToScale> ToScale for WithCallIndex<T> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        if let Some(call_index) = &self.call_index.0 {
            let mut value = self.value.to_scale();
            if value.len() < 2 {
                debug_assert!(
                    false,
                    "`WithCallIndex` inner value must include call indices in Scale representation"
                );
                return;
            }
            // Override the first two bytes with the custom call index.
            value[0] = call_index.0;
            value[1] = call_index.1;
            out.extend(&value);
        } else {
            self.value.to_scale_into(out);
        }
    }
}

/// Builder pattern implementation for creating Substrate transactions.
#[derive(Debug, Default)]
pub struct TransactionBuilder {
    /// Whether to use the newer multi-address format.
    multi_address: bool,
    /// The call data to be executed.
    call: RawOwned,
    /// Additional data attached to the transaction.
    extensions: TxExtensionData,
    /// The sender's address.
    account: MultiAddress,
}

impl TransactionBuilder {
    /// Creates a new transaction builder.
    ///
    /// * `multi_address` - Whether to use the newer multi-address format
    /// * `call` - The SCALE-encoded call data
    pub fn new(multi_address: bool, call: RawOwned) -> Self {
        Self {
            multi_address,
            call,
            ..Default::default()
        }
    }

    /// Sets the sender's account.
    pub fn set_account(&mut self, account: AccountId) {
        self.account = MultiAddress::new(account, self.multi_address);
    }

    /// Adds an extension to the transaction.
    pub fn extension<E: TxExtension>(&mut self, extension: E) {
        extension.encode(&mut self.extensions);
    }

    /// Encodes the payload that needs to be signed.
    /// Returns the encoded bytes or a hash of the payload if it exceeds MAX_PAYLOAD_SIZE.
    pub fn encode_payload(&self) -> Result<Vec<u8>, KeyPairError> {
        // SCALE encode the payload that needs to be signed: (call, extensions.data, extensions.signed).
        let mut payload = self.call.to_scale();
        self.extensions.data.to_scale_into(&mut payload);
        self.extensions.signed.to_scale_into(&mut payload);

        // if the payload is large then we sign a hash of the payload.
        if payload.len() > MAX_PAYLOAD_SIZE {
            Ok(blake2_b(&payload, PAYLOAD_HASH_SIZE).map_err(|_| KeyPairError::InternalError)?)
        } else {
            Ok(payload)
        }
    }

    /// Signs the transaction with the given keypair.
    pub fn sign(self, keypair: &KeyPair) -> Result<ExtrinsicV4, KeyPairError> {
        let payload = self.encode_payload()?;
        let signature = keypair.sign(payload)?;
        self.into_signed(signature)
    }

    /// Creates a signed extrinsic using a pre-computed signature.
    pub fn into_signed(self, signature: Signature) -> Result<ExtrinsicV4, KeyPairError> {
        Ok(ExtrinsicV4::signed(
            self.account,
            signature.into(),
            self.extensions.data,
            self.call,
        ))
    }
}

impl_struct_scale!(
    #[derive(Clone, Debug)]
    pub struct ExtrinsicSignature {
        /// The sender's address.
        pub account: MultiAddress,
        /// The signature of the extrinsic.
        pub signature: MultiSignature,
        /// Additional signed data (e.g., era, nonce, tip).
        pub extra: RawOwned,
    }
);

/// Current version of the `UncheckedExtrinsic` format.
pub const EXTRINSIC_VERSION: u8 = 4;
pub const SIGNED_EXTRINSIC_BIT: u8 = 0b1000_0000;
pub const UNSIGNED_EXTRINSIC_MASK: u8 = 0b0111_1111;
pub const MAX_PAYLOAD_SIZE: usize = 256;
pub const PAYLOAD_HASH_SIZE: usize = 32;

/// Represents a Substrate transaction (extrinsic) using format version 4.
#[derive(Clone, Debug)]
pub struct ExtrinsicV4 {
    /// Optional signature data for signed transactions.
    pub signature: Option<ExtrinsicSignature>,
    /// The actual call data to be executed.
    pub call: RawOwned,
}

impl ExtrinsicV4 {
    /// Creates a new signed extrinsic.
    ///
    /// * `account` - The sender's address
    /// * `sig` - The signature of the extrinsic
    /// * `extra` - Additional signed data
    /// * `call` - The call data to be executed
    pub fn signed(
        account: MultiAddress,
        sig: MultiSignature,
        extra: RawOwned,
        call: RawOwned,
    ) -> Self {
        Self {
            signature: Some(ExtrinsicSignature {
                account,
                signature: sig,
                extra,
            }),
            call,
        }
    }
}

impl ToScale for ExtrinsicV4 {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        // We use a temp buffer here for the `Compact<u32>` length prefix.
        let mut buf = Vec::with_capacity(512);

        // 1 byte version id and signature if signed.
        match &self.signature {
            Some(sig) => {
                buf.push(EXTRINSIC_VERSION | SIGNED_EXTRINSIC_BIT);
                sig.to_scale_into(&mut buf);
            },
            None => {
                buf.push(EXTRINSIC_VERSION & UNSIGNED_EXTRINSIC_MASK);
            },
        }
        self.call.to_scale_into(&mut buf);

        // SCALE encode the tmp buffer to `out`.
        buf.to_scale_into(out);
    }
}
