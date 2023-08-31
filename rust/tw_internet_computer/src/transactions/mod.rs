mod transfer;

mod proto {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}

use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_proto::InternetComputer::Proto::mod_Transaction::OneOftransaction_oneof as Tx;

use crate::protocol::{identity, principal::Principal, rosetta};

#[derive(Debug)]
pub enum SignTransactionError {
    Identity(identity::SigningError),
    EncodingArgsFailed,
    InvalidToAccountIdentifier,
}

pub fn sign_transaction(
    private_key: PrivateKey,
    canister_id: Principal,
    transaction: &Tx,
) -> Result<rosetta::SignedTransaction, SignTransactionError> {
    match transaction {
        Tx::transfer(transfer_args) => transfer::transfer(
            private_key,
            canister_id,
            transfer::TransferArgs {
                memo: transfer_args.memo,
                amount: transfer_args.amount,
                max_fee: None,
                to: transfer_args.to_account_identifier.to_string(),
                current_timestamp_secs: transfer_args.current_timestamp_secs,
            },
        ),
        Tx::None => todo!(),
    }
}