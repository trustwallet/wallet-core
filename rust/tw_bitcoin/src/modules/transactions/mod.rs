use bitcoin::key::PublicKey;
use bitcoin::script::ScriptBuf;
use bitcoin::taproot::{TapNodeHash, TaprootSpendInfo};

mod brc20;
mod input_builder;
mod input_claim_builder;
mod ordinals;
mod output_builder;

// Re-exports
pub use brc20::{BRC20TransferInscription, Brc20Ticker};
pub use input_builder::InputBuilder;
pub use input_claim_builder::InputClaimBuilder;
pub use ordinals::{OrdinalNftInscription, OrdinalsInscription};
pub use output_builder::OutputBuilder;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct TaprootScript {
    pub pubkey: PublicKey,
    pub merkle_root: TapNodeHash,
}

#[derive(Debug, Clone)]
pub struct TaprootProgram {
    pub script: ScriptBuf,
    pub spend_info: TaprootSpendInfo,
}
