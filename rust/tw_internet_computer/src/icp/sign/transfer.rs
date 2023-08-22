mod proto {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}

use std::time::Duration;
use tw_encoding::hex;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1::PrivateKey;

use crate::{
    identity::{Identity, IdentityError},
    interface_spec::{
        envelope::{Envelope, EnvelopeContent},
        get_ingress_expiry,
        request_id::{self, RequestId},
    },
    principal::Principal,
    rosetta,
};

use self::proto::ic_ledger::pb::v1::{
    AccountIdentifier as ProtoAccountIdentifier, Memo, Payment, SendRequest, TimeStamp, Tokens,
};

#[derive(Clone, Debug)]
pub struct TransferArgs {
    pub memo: u64,
    pub amount: u64,
    pub to: String,
    pub current_timestamp_secs: u64,
}

impl From<TransferArgs> for SendRequest<'_> {
    fn from(args: TransferArgs) -> Self {
        let current_timestamp_duration = Duration::from_secs(args.current_timestamp_secs);
        let timestamp_nanos = current_timestamp_duration.as_nanos() as u64;
        let to_hash = hex::decode(&args.to).expect("Failed to decode account identifier.");

        Self {
            memo: Some(Memo { memo: args.memo }),
            payment: Some(Payment {
                receiver_gets: Some(Tokens { e8s: args.amount }),
            }),
            max_fee: Some(Tokens { e8s: FEE }),
            from_subaccount: None,
            to: Some(ProtoAccountIdentifier {
                hash: to_hash.into(),
            }),
            created_at: None,
            created_at_time: Some(TimeStamp { timestamp_nanos }),
        }
    }
}

#[derive(Debug)]
pub enum SignTransferError {
    Identity(IdentityError),
    EncodingArgsFailed,
    EncodingSignedTransactionFailed,
}

/// The endpoint on the ledger canister that is used to make transfers.
const METHOD_NAME: &str = "send_pb";

/// The fee for a transfer is always 10_000 e8s.
const FEE: u64 = 10_000;

pub fn transfer(
    private_key: PrivateKey,
    canister_id: Principal,
    args: TransferArgs,
) -> Result<Vec<u8>, SignTransferError> {
    let current_timestamp_duration = Duration::from_secs(args.current_timestamp_secs);
    let ingress_expiry = get_ingress_expiry(current_timestamp_duration);
    let identity = Identity::new(private_key);

    // Encode the arguments for the ledger `send_pb` endpoint.
    let send_request = SendRequest::from(args);
    let arg =
        tw_proto::serialize(&send_request).map_err(|_| SignTransferError::EncodingArgsFailed)?;
    // Create the update envelope.
    let update_envelope = create_update_envelope(&identity, arg, ingress_expiry)?;
    let request_id = update_envelope.content.to_request_id();

    // Create the read state envelope.
    let read_state_envelope = create_read_state_envelope(&identity, request_id, ingress_expiry)?;

    // Create a new EnvelopePair with the update call and read_state envelopes.
    let envelope_pair = rosetta::EnvelopePair::new(update_envelope, read_state_envelope);

    // Create a signed transaction containing the envelope pair.
    let request: rosetta::Request = (rosetta::RequestType::Send, vec![envelope_pair]);
    let signed_transaction: rosetta::SignedTransaction = vec![request];
    // Encode the signed transaction.
    let cbor_encoded_signed_transaction = serde_cbor::to_vec(&signed_transaction)
        .map_err(|_| SignTransferError::EncodingSignedTransactionFailed)?;

    Ok(cbor_encoded_signed_transaction)
}

fn create_update_envelope(
    identity: &Identity,
    arg: Vec<u8>,
    ingress_expiry: u64,
) -> Result<Envelope, SignTransferError> {
    let sender = identity.sender();
    let content = EnvelopeContent::Call {
        nonce: None, //TODO: do we need the nonce?
        ingress_expiry,
        sender: sender.0,
        canister_id: ic_ledger_types::MAINNET_LEDGER_CANISTER_ID,
        method_name: METHOD_NAME.to_string(),
        arg,
    };

    let request_id = content.to_request_id();
    let value = request_id::make_sig_data(&request_id);
    println!("update request id: {}", hex::encode(request_id.0, false));
    let signature = identity.sign(&value).map_err(SignTransferError::Identity)?;
    println!(
        "update signature: {}",
        hex::encode(&signature.signature, false)
    );

    let env = Envelope {
        content,
        sender_pubkey: Some(signature.public_key),
        sender_sig: Some(signature.signature),
    };
    Ok(env)
}

fn create_read_state_envelope(
    identity: &Identity,
    request_id: RequestId,
    ingress_expiry: u64,
) -> Result<Envelope, SignTransferError> {
    let sender = identity.sender();

    let content = EnvelopeContent::ReadState {
        ingress_expiry,
        sender: sender.0,
        paths: vec![vec![
            "request_status".into(),
            request_id.0.as_slice().into(),
        ]],
    };

    let request_id = content.to_request_id();
    let value = request_id::make_sig_data(&request_id);
    println!("request sig data len: {}", value.len());
    let signature = identity.sign(&value).map_err(SignTransferError::Identity)?;

    let env = Envelope {
        content,
        sender_pubkey: Some(signature.public_key),
        sender_sig: Some(signature.signature),
    };
    Ok(env)
}

#[cfg(test)]
mod test {
    use ic_ledger_types::{AccountIdentifier, DEFAULT_SUBACCOUNT};

    use super::*;

    #[test]
    fn transfer_successful() {
        let private_key = PrivateKey::try_from(
            "227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be",
        )
        .unwrap();
        let canister_id = Principal(ic_ledger_types::MAINNET_LEDGER_CANISTER_ID);
        let to_account_identifier = AccountIdentifier::new(
            &candid::Principal::from_text(
                "t4u4z-y3dur-j63pk-nw4rv-yxdbt-agtt6-nygn7-ywh6y-zm2f4-sdzle-3qe",
            )
            .unwrap(),
            &DEFAULT_SUBACCOUNT,
        );

        let value = transfer(
            private_key,
            canister_id,
            TransferArgs {
                memo: 0,
                amount: 100000000,
                to: to_account_identifier.to_hex(),
                current_timestamp_secs: 1_691_709_940,
            },
        )
        .unwrap();
        println!("{}", hex::encode(value, false));
    }
}
