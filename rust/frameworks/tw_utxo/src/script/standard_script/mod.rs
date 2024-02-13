use super::Script;

pub mod claims;
pub mod conditions;
pub mod opcodes;

const SEGWIT_VERSION: u8 = 0;
const TAPROOT_VERSION: u8 = 1;
