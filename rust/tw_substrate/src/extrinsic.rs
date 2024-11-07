use tw_hash::{blake2::blake2_b, H256, H512};
use tw_keypair::{
    ed25519::{sha512::KeyPair, Signature},
    traits::SigningKeyTrait,
    KeyPairError,
};
use tw_scale::{impl_enum_scale, impl_struct_scale, ToScale};

use crate::address::*;
use crate::extensions::*;
use crate::EncodeError;

pub type TxHash = H256;
pub type BlockHash = H256;
pub type BlockNumber = u32;

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum MultiSignature {
        Ed25519(H512),
        Sr25519(H512),
        //Ecdsa([u8; 65]),
    }
);

impl From<Signature> for MultiSignature {
    fn from(sig: Signature) -> Self {
        Self::Ed25519(sig.to_bytes())
    }
}

#[derive(Clone, Debug)]
pub struct CallIndex(Option<(u8, u8)>);

impl CallIndex {
    pub fn from_tw(call_index: Option<(i32, i32)>) -> Result<Self, EncodeError> {
        let call_index = match call_index {
            Some((module_index, method_index)) => {
                if module_index > 0xff || method_index > 0xff {
                    return Err(EncodeError::InvalidCallIndex);
                }
                Some((module_index as u8, method_index as u8))
            },
            _ => None,
        };
        Ok(Self(call_index))
    }

    pub fn required_from_tw(call_index: Option<(i32, i32)>) -> Result<Self, EncodeError> {
        if call_index.is_none() {
            return Err(EncodeError::MissingCallIndicesTable);
        }
        Self::from_tw(call_index)
    }

    pub fn has_call_index(&self) -> bool {
        self.0.is_some()
    }

    pub fn wrap<T: ToScale>(self, value: T) -> WithCallIndex<T> {
        WithCallIndex {
            value,
            call_index: self,
        }
    }
}

#[derive(Clone, Debug)]
pub struct WithCallIndex<T: ToScale> {
    value: T,
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
            assert!(value.len() >= 2);
            // Override the first two bytes with the custom call index.
            value[0] = call_index.0;
            value[1] = call_index.1;
            out.extend(&value);
        } else {
            self.value.to_scale_into(out);
        }
    }
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct Encoded(pub Vec<u8>);

impl Encoded {
    pub fn new<T: ToScale>(val: T) -> Self {
        Self(val.to_scale())
    }
}

impl ToScale for Encoded {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend(&self.0);
    }
}

pub type EncodedExtra = Encoded;
pub type EncodedAdditionalSigned = Encoded;
pub struct SignedPayload<'a>((&'a Encoded, &'a EncodedExtra, &'a EncodedAdditionalSigned));

impl<'a> ToScale for SignedPayload<'a> {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        self.0.to_scale_into(out);
    }
}

impl<'a> SignedPayload<'a> {
    pub fn new(
        call: &'a Encoded,
        extra: &'a EncodedExtra,
        additional: &'a EncodedAdditionalSigned,
    ) -> Self {
        Self((call, extra, additional))
    }
}

/// Helper to build transaction.
#[derive(Clone, Debug, Default)]
pub struct TransactionBuilder {
    call: Encoded,
    extensions: TxExtensionData,
}

impl TransactionBuilder {
    pub fn new(call: Encoded) -> Self {
        Self {
            call,
            ..Default::default()
        }
    }

    pub fn extension<E: TxExtension>(&mut self, extension: E) {
        extension.encode(&mut self.extensions);
    }

    pub fn build(self) -> PrepareTransaction {
        PrepareTransaction::new(self.extensions.signed, self.extensions.data, self.call)
    }
}

/// PrepareTransaction holds all data needed to sign a transaction.
#[derive(Clone, Debug)]
pub struct PrepareTransaction {
    call: Encoded,
    pub extra: EncodedExtra,
    pub additional: EncodedAdditionalSigned,
}

impl PrepareTransaction {
    pub fn new(additional: EncodedAdditionalSigned, extra: EncodedExtra, call: Encoded) -> Self {
        Self {
            additional,
            extra,
            call,
        }
    }

    pub fn encode_payload(&self) -> Result<Vec<u8>, KeyPairError> {
        let payload = SignedPayload::new(&self.call, &self.extra, &self.additional);
        // SCALE encode the SignedPayload and if the payload is large then we sign a hash
        // of the payload.
        let encoded = payload.to_scale();
        if encoded.len() > MAX_PAYLOAD_SIZE {
            Ok(blake2_b(&encoded, PAYLOAD_HASH_SIZE).map_err(|_| KeyPairError::InternalError)?)
        } else {
            Ok(encoded)
        }
    }

    pub fn sign(
        self,
        account: MultiAddress,
        signer: &KeyPair,
    ) -> Result<ExtrinsicV4, KeyPairError> {
        let payload = self.encode_payload()?;
        let signature = signer.sign(payload)?;
        self.into_signed(account, signature)
    }

    pub fn into_signed(
        self,
        account: MultiAddress,
        signature: Signature,
    ) -> Result<ExtrinsicV4, KeyPairError> {
        Ok(ExtrinsicV4::signed(
            account,
            signature.into(),
            self.extra,
            self.call,
        ))
    }
}

impl_struct_scale!(
    #[derive(Clone, Debug)]
    pub struct ExtrinsicSignature {
        pub account: MultiAddress,
        pub signature: MultiSignature,
        pub extra: EncodedExtra,
    }
);

/// Current version of the `UncheckedExtrinsic` format.
pub const EXTRINSIC_VERSION: u8 = 4;
pub const SIGNED_EXTRINSIC_BIT: u8 = 0b1000_0000;
pub const UNSIGNED_EXTRINSIC_MASK: u8 = 0b0111_1111;
pub const MAX_PAYLOAD_SIZE: usize = 256;
pub const PAYLOAD_HASH_SIZE: usize = 32;

#[derive(Clone, Debug)]
pub struct ExtrinsicV4 {
    pub signature: Option<ExtrinsicSignature>,
    pub call: Encoded,
}

impl ExtrinsicV4 {
    pub fn signed(
        account: MultiAddress,
        sig: MultiSignature,
        extra: EncodedExtra,
        call: Encoded,
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
