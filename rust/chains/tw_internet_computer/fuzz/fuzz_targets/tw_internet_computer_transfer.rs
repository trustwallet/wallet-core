#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_internet_computer::{
    address::AccountIdentifier,
    protocol::principal::Principal,
    transactions::transfer::{transfer, TransferArgs},
};
use tw_keypair::ecdsa::secp256k1;

#[derive(Debug, arbitrary::Arbitrary)]
struct ArbitraryTransferArgs {
    memo: u64,
    amount: u64,
    max_fee: Option<u64>,
    #[arbitrary(with = arbitrary_to_field)]
    to: String,
    current_timestamp_nanos: u64,
    permitted_drift: Option<u64>,
}

fn arbitrary_to_field(u: &mut arbitrary::Unstructured) -> arbitrary::Result<String> {
    let mut buf = [0; 29];
    u.fill_buffer(&mut buf)?;
    let principal = Principal::from_slice(&buf);
    let account_identifier = AccountIdentifier::new(&principal);
    Ok(account_identifier.to_hex())
}

impl From<&ArbitraryTransferArgs> for TransferArgs {
    fn from(prev: &ArbitraryTransferArgs) -> TransferArgs {
        TransferArgs {
            memo: prev.memo,
            amount: prev.amount,
            max_fee: prev.max_fee,
            to: prev.to.clone(),
            current_timestamp_nanos: prev.current_timestamp_nanos,
        }
    }
}

#[derive(Debug, arbitrary::Arbitrary)]
struct TWInternetComputerTransactionsTransferInput {
    #[arbitrary(with = arbitrary_private_key)]
    private: Vec<u8>,
    #[arbitrary(with = arbitrary_canister_id)]
    canister_id: Principal,
    args: ArbitraryTransferArgs,
}

fn arbitrary_private_key(u: &mut arbitrary::Unstructured) -> arbitrary::Result<Vec<u8>> {
    let mut buf = [0; 32];
    u.fill_buffer(&mut buf)?;
    Ok(Vec::from(buf.as_slice()))
}

fn arbitrary_canister_id(u: &mut arbitrary::Unstructured) -> arbitrary::Result<Principal> {
    let mut buf = [0; 29];
    u.fill_buffer(&mut buf)?;
    let principal = Principal::from_slice(&buf);
    Ok(principal)
}

fuzz_target!(|input: TWInternetComputerTransactionsTransferInput| {
    let Ok(private_key) = secp256k1::PrivateKey::try_from(input.private.as_slice()) else {
        return;
    };

    let args = TransferArgs::from(&input.args);
    transfer(private_key, input.canister_id, args).ok();
});
