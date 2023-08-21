use ic_ledger_types::AccountIdentifier;

pub fn is_address_valid(address: &str) -> bool {
    AccountIdentifier::from_hex(address).is_ok()
}
