use crate::{script::Script, signing_mode::SigningMethod, transaction::transaction_parts::Amount};

pub struct UtxoToSign {
	pub script_pubkey: Script,
	pub signing_method: SigningMethod,
	pub amount: Amount,
}

pub struct PreImageComputer {

}
