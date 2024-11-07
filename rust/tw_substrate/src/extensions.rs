use crate::extrinsic::{BlockHash, Encoded};
use tw_scale::{impl_enum_scale, Compact, ToScale};

#[derive(Clone, Debug, Default)]
pub struct TxExtensionData {
    pub data: Encoded,
    pub signed: Encoded,
}

impl TxExtensionData {
    pub fn encode_data<T: ToScale>(&mut self, data: &T) {
        data.to_scale_into(&mut self.data.0);
    }

    pub fn encode_signed<T: ToScale>(&mut self, signed: &T) {
        signed.to_scale_into(&mut self.signed.0);
    }
}

pub trait TxExtension {
    fn encode(&self, tx: &mut TxExtensionData);
}

impl TxExtension for () {
    fn encode(&self, _tx: &mut TxExtensionData) {}
}

#[derive(Clone, Debug, Default)]
pub struct CheckVersion(pub u32);
pub type CheckSpecVersion = CheckVersion;
pub type CheckTxVersion = CheckVersion;

impl TxExtension for CheckVersion {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_signed(&self.0);
    }
}

#[derive(Clone, Debug, Default)]
pub struct CheckGenesis(pub BlockHash);

impl TxExtension for CheckGenesis {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_signed(&self.0);
    }
}

#[derive(Clone, Copy, Default, Debug, PartialEq)]
pub enum Era {
    #[default]
    Immortal,
    Mortal(u64, u64),
}

impl Era {
    pub fn mortal(period: u64, block: u64) -> Self {
        // Based off `sp_runtime::generic::Era`:
        // See https://github.com/paritytech/polkadot-sdk/blob/657b5503a04e97737696fa7344641019350fb521/substrate/primitives/runtime/src/generic/era.rs#L65
        let period = period
            .checked_next_power_of_two()
            .unwrap_or(1 << 16)
            .clamp(4, 1 << 16);
        let phase = block % period;
        let quantize_factor = (period >> 12).max(1);
        let quantized_phase = phase / quantize_factor * quantize_factor;
        Self::Mortal(period, quantized_phase)
    }

    pub fn immortal() -> Self {
        Self::Immortal
    }
}

impl ToScale for Era {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        match self {
            Self::Immortal => {
                out.push(0);
            },
            Self::Mortal(period, phase) => {
                // Based off `sp_runtime::generic::Era`:
                // See https://github.com/paritytech/polkadot-sdk/blob/657b5503a04e97737696fa7344641019350fb521/substrate/primitives/runtime/src/generic/era.rs#L107
                let quantize_factor = (period >> 12).max(1);

                let encoded = (period.trailing_zeros() - 1).clamp(1, 15) as u16
                    | ((phase / quantize_factor) << 4) as u16;
                encoded.to_scale_into(out);
            },
        }
    }
}

#[derive(Clone, Debug, Default)]
pub struct CheckEra {
    pub era: Era,
    pub current_hash: BlockHash,
}

impl TxExtension for CheckEra {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.era);
        tx.encode_signed(&self.current_hash);
    }
}

#[derive(Clone, Debug)]
pub struct CheckNonce(pub Compact<u32>);

impl CheckNonce {
    pub fn new(nonce: u32) -> Self {
        Self(Compact(nonce))
    }
}

impl TxExtension for CheckNonce {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.0);
    }
}

#[derive(Clone, Debug)]
pub struct ChargeTransactionPayment(pub Compact<u128>);

impl ChargeTransactionPayment {
    pub fn new(tip: u128) -> Self {
        Self(Compact(tip))
    }
}

impl TxExtension for ChargeTransactionPayment {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.0);
    }
}

#[derive(Clone, Debug)]
pub struct ChargeAssetTxPayment {
    tip: Compact<u128>,
    asset_id: Option<u32>,
}

impl ChargeAssetTxPayment {
    pub fn new(tip: u128, asset_id: u32) -> Self {
        Self {
            tip: Compact(tip),
            asset_id: if asset_id > 0 {
                Some(asset_id)
            } else {
                // native token
                None
            },
        }
    }
}

impl TxExtension for ChargeAssetTxPayment {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.tip);
        tx.encode_data(&self.asset_id);
    }
}

impl_enum_scale!(
    #[derive(Clone, Copy, Debug, Default)]
    pub enum CheckMetadataMode {
        #[default]
        Disabled,
        Enabled,
    }
);

#[derive(Clone, Debug, Default)]
pub struct CheckMetadataHash {
    pub mode: CheckMetadataMode,
    pub hash: Option<BlockHash>,
}

impl TxExtension for CheckMetadataHash {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.mode);
        tx.encode_signed(&self.hash);
    }
}
