//! Extensions for Substrate transactions.
//!
//! This module provides types and traits for handling Substrate transaction extensions. Extensions are a way to
//! include additional data and checks in a transaction, such as nonce, fee payment, era, and version checks.
//!
//! # Example
//!
//! ```rust
//! use tw_substrate::extensions::*;
//! use tw_substrate::TransactionBuilder;
//! use tw_scale::RawOwned;
//!
//! // Create a transaction builder
//! let mut builder = TransactionBuilder::new(true, RawOwned::default());
//!
//! // Add various extensions
//! builder.extension(CheckVersion(1)); // Spec version
//! builder.extension(CheckVersion(1)); // Transaction version
//! builder.extension(CheckGenesis(Default::default()));
//! builder.extension(CheckEra {
//!     era: Era::immortal(),
//!     current_hash: Default::default(),
//! });
//! builder.extension(CheckNonce::new(1));
//! builder.extension(ChargeTransactionPayment::new(0));
//! ```

use crate::extrinsic::BlockHash;
use tw_scale::{impl_enum_scale, Compact, RawOwned, ToScale};

/// Data container for transaction extensions.
///
/// Contains two parts:
/// - `data`: Raw extension data included in the transaction
/// - `signed`: Additional data included in the signature payload
#[derive(Clone, Debug, Default)]
pub struct TxExtensionData {
    /// Raw extension data included in the transaction
    pub data: RawOwned,
    /// Additional data included in the signature payload
    pub signed: RawOwned,
}

impl TxExtensionData {
    /// Encode extension data into the transaction data field
    pub fn encode_data<T: ToScale>(&mut self, data: &T) {
        data.to_scale_into(&mut self.data.0);
    }

    /// Encode extension data into the signature payload field
    pub fn encode_signed<T: ToScale>(&mut self, signed: &T) {
        signed.to_scale_into(&mut self.signed.0);
    }
}

/// Trait for types that can be used as transaction extensions.
pub trait TxExtension {
    /// Encodes the extension data into the transaction.
    fn encode(&self, tx: &mut TxExtensionData);
}

impl TxExtension for () {
    fn encode(&self, _tx: &mut TxExtensionData) {}
}

/// Version check extension.
///
/// Used for both spec version and transaction version checks.
#[derive(Clone, Debug, Default)]
pub struct CheckVersion(
    /// Version number to check
    pub u32,
);
pub type CheckSpecVersion = CheckVersion;
pub type CheckTxVersion = CheckVersion;

impl TxExtension for CheckVersion {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_signed(&self.0);
    }
}

/// Extension for checking block genesis hash
#[derive(Clone, Debug, Default)]
pub struct CheckGenesis(
    /// Genesis block hash
    pub BlockHash,
);

impl TxExtension for CheckGenesis {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_signed(&self.0);
    }
}

/// Transaction era for time-based validity.
///
/// Can be either:
/// - `Immortal`: Transaction never expires
/// - `Mortal(period, phase)`: Transaction is valid for a specific time window
#[derive(Clone, Copy, Default, Debug, PartialEq)]
pub enum Era {
    #[default]
    Immortal,
    Mortal(u64, u64),
}

impl Era {
    /// Creates a mortal era with the given period and current block number
    ///
    /// # Arguments
    /// * `period` - The number of blocks the transaction remains valid for
    /// * `block` - Current block number used to calculate the era phase
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

    /// Creates an immortal era (transaction never expires)
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

/// Extension for checking transaction era and block hash.
#[derive(Clone, Debug, Default)]
pub struct CheckEra {
    /// Transaction validity period
    pub era: Era,
    /// Current block hash
    pub current_hash: BlockHash,
}

impl TxExtension for CheckEra {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.era);
        tx.encode_signed(&self.current_hash);
    }
}

/// Extension for checking transaction nonce.
#[derive(Clone, Debug)]
pub struct CheckNonce(
    /// Account nonce as compact encoding
    pub Compact<u32>,
);

impl CheckNonce {
    /// Creates a new nonce check extension
    ///
    /// # Arguments
    /// * `nonce` - Account nonce value
    pub fn new(nonce: u32) -> Self {
        Self(Compact(nonce))
    }
}

impl TxExtension for CheckNonce {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.0);
    }
}

/// Extension for handling native token transaction fees.
#[derive(Clone, Debug)]
pub struct ChargeTransactionPayment(
    /// Transaction tip amount as compact encoding
    pub Compact<u128>,
);

impl ChargeTransactionPayment {
    /// Creates a new payment extension with the given tip amount
    ///
    /// # Arguments
    /// * `tip` - Optional tip amount in addition to the base fee
    pub fn new(tip: u128) -> Self {
        Self(Compact(tip))
    }
}

impl TxExtension for ChargeTransactionPayment {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.0);
    }
}

/// Extension for handling asset-based transaction fees.
#[derive(Clone, Debug)]
pub struct ChargeAssetTxPayment {
    /// Transaction tip amount as compact encoding
    tip: Compact<u128>,
    /// Optional asset ID for fee payment (None = native token)
    asset_id: Option<u32>,
}

impl ChargeAssetTxPayment {
    /// Creates a new asset payment extension
    ///
    /// # Arguments
    /// * `tip` - Optional tip amount in addition to the base fee
    /// * `asset_id` - Asset ID to pay fees with (0 = native token)
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
    /// Mode for checking runtime metadata.
    #[derive(Clone, Copy, Debug, Default)]
    pub enum CheckMetadataMode {
        /// Metadata check is disabled
        #[default]
        Disabled = 0x00,
        /// Metadata check is enabled
        Enabled = 0x01,
    }
);

/// Extension for checking runtime metadata hash.
#[derive(Clone, Debug, Default)]
pub struct CheckMetadataHash {
    /// Whether metadata checking is enabled
    pub mode: CheckMetadataMode,
    /// Optional metadata hash to check against
    pub hash: Option<BlockHash>,
}

impl TxExtension for CheckMetadataHash {
    fn encode(&self, tx: &mut TxExtensionData) {
        tx.encode_data(&self.mode);
        tx.encode_signed(&self.hash);
    }
}
