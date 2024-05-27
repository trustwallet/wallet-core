use tw_hash::H32;
use tw_proto::Polkadot::Proto;

#[derive(Debug, Clone)]
pub struct Extrinsic;

impl Extrinsic {
    pub fn from_input(input: Proto::SigningInput<'_>) -> Self {
        // let x = H32::from(input.block_hash);

        Self {}
    }
}
