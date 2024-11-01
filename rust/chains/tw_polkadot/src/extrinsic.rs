use tw_proto::Polkadot::Proto;

use crate::substrate::*;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum EncodeError {
    InvalidNetworkId,
    MissingCallIndicesTable,
    InvalidCallIndex,
    InvalidAddress,
    InvalidValue,
}

pub type EncodeResult<T> = Result<T, EncodeError>;
pub type WithCallIndexResult<T> = Result<WithCallIndex<T>, EncodeError>;

// `Extrinsic` is (for now) just a lightweight wrapper over the actual protobuf object.
// In the future, we will refine the latter to let the caller specify arbitrary extrinsics.

#[derive(Debug, Clone)]
pub struct Extrinsic<'a> {
    inner: Proto::SigningInput<'a>,
}

impl<'a> Extrinsic<'a> {
    pub fn from_input(input: Proto::SigningInput<'a>) -> Self {
        Self { inner: input }
    }

    pub fn encode_call(&self) -> EncodeResult<Vec<u8>> {
        let ctx = PolkadotSigningContext::encode_input(&self.inner)?;
        Ok(ctx.0)
    }
}
