mod proto {
    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}

use std::time::Duration;
use tw_encoding::hex;
use tw_keypair::ecdsa::secp256k1::PrivateKey;

use crate::{
    identity::{Identity, IdentityError},
    interface_spec::{
        envelope::{Envelope, EnvelopeContent},
        get_ingress_expiry,
        request_id::RequestId,
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
    pub fee: u64,
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
            max_fee: Some(Tokens { e8s: args.fee }),
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
    let (request_id, update_envelope) =
        create_update_envelope(&identity, canister_id, arg, ingress_expiry)?;

    // Create the read state envelope.
    let (_, read_state_envelope) =
        create_read_state_envelope(&identity, request_id, ingress_expiry)?;

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
    canister_id: Principal,
    arg: Vec<u8>,
    ingress_expiry: u64,
) -> Result<(RequestId, Envelope), SignTransferError> {
    let sender = identity.sender();
    let content = EnvelopeContent::Call {
        nonce: None, //TODO: do we need the nonce?
        ingress_expiry,
        sender: sender.0,
        canister_id: canister_id.0,
        method_name: METHOD_NAME.to_string(),
        arg,
    };

    let request_id = RequestId::from(&content);
    let signature = identity
        .sign(request_id.sig_data())
        .map_err(SignTransferError::Identity)?;

    let env = Envelope {
        content,
        sender_pubkey: Some(signature.public_key),
        sender_sig: Some(signature.signature),
    };
    Ok((request_id, env))
}

fn create_read_state_envelope(
    identity: &Identity,
    update_request_id: RequestId,
    ingress_expiry: u64,
) -> Result<(RequestId, Envelope), SignTransferError> {
    let sender = identity.sender();

    let content = EnvelopeContent::ReadState {
        ingress_expiry,
        sender: sender.0,
        paths: vec![vec![
            "request_status".into(),
            update_request_id.0.as_slice().into(),
        ]],
    };

    let request_id = RequestId::from(&content);
    let signature = identity
        .sign(request_id.sig_data())
        .map_err(SignTransferError::Identity)?;

    let env = Envelope {
        content,
        sender_pubkey: Some(signature.public_key),
        sender_sig: Some(signature.signature),
    };
    Ok((request_id, env))
}

#[cfg(test)]
mod test {
    use ic_ledger_types::{AccountIdentifier, DEFAULT_SUBACCOUNT};

    use super::*;

    pub const SIGNED_TRANSACTION: &str = "81826b5452414e53414354494f4e81a266757064617465a367636f6e74656e74a66c726571756573745f747970656463616c6c6e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d026b63616e69737465725f69644a000000000000000201016b6d6574686f645f6e616d656773656e645f70626361726758400a0012070a050880c2d72f1a0308904e2a220a20943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a3a0a088090caa5a3a78abd176d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f736967984018d8189d18ee188a1118a81858184018da188d188c18b800184c18f611182718ea18931899186f183318c518711848186718841718351825181e187c18710018a21871183618f6184b18cd18e618e418ea182c18d118c91857188d140c18b4182a188018c51871189f1418b518cf182e18b618a418fd18a36a726561645f7374617465a367636f6e74656e74a46c726571756573745f747970656a726561645f73746174656e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d0265706174687381824e726571756573745f7374617475735820b20f43257a5e87542693561e20a6076d515395e49623fcecd6c5b5640b8db8c36d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f736967984018a8189b12186d18a4188d18fb18f71869187918f70e1825181d185a0318440b18e0186e1820183f1834188016186818dd183b18d518de18941849187e1882186e18591861187218ac0a18de18df1858183718b6182818930c18431864183718f60a18ef1824185e18ed184e18841839185718d5091888";

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

        let signed_transaction = transfer(
            private_key,
            canister_id,
            TransferArgs {
                memo: 0,
                amount: 100000000,
                fee: 10_000,
                to: to_account_identifier.to_hex(),
                current_timestamp_secs: 1_691_709_940,
            },
        )
        .unwrap();
        let hex_encoded_signed_transaction = hex::encode(&signed_transaction, false);
        assert_eq!(hex_encoded_signed_transaction, SIGNED_TRANSACTION);
    }
}
