use tw_proto::Polkadot::Proto;

use tw_substrate::*;

use crate::network::*;

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
