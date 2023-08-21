use std::time::Duration;

use ic_ledger_types::{AccountIdentifier, Memo, Timestamp, Tokens};

use super::{
    identity::{Identity, IdentityError},
    interface_spec::{
        envelope::{Envelope, EnvelopeContent},
        get_ingress_expiry,
        request_id::{self, RequestId},
    },
    proto, rosetta,
};

/// The endpoint on the ledger canister that is used to make transfers.
const METHOD_NAME: &str = "send_pb";

/// The fee for a transfer is always 10_000 e8s.
const FEE: Tokens = Tokens::from_e8s(10_000);

#[derive(Debug)]
pub enum SignTransferError {
    Identity(IdentityError),
    EncodingArgsFailed,
    EncodingSignedTransactionFailed,
}

#[derive(Clone, Debug)]
pub struct SendArgs {
    pub memo: Memo,
    pub amount: Tokens,
    pub fee: Tokens,
    pub to: AccountIdentifier,
    pub created_at_time: Option<Timestamp>,
}

pub fn transfer(
    identity: Identity,
    to_account_identifier: AccountIdentifier,
    amount: u64,
    memo: u64,
    current_timestamp_secs: u64,
) -> Result<Vec<u8>, SignTransferError> {
    let current_timestamp_duration = Duration::from_secs(current_timestamp_secs);
    let ingress_expiry = get_ingress_expiry(current_timestamp_duration);
    let timestamp_nanos = current_timestamp_duration.as_nanos() as u64;

    // Build the send arguments and then serialize to proto.
    let args = SendArgs {
        memo: Memo(memo),
        amount: Tokens::from_e8s(amount),
        fee: FEE,
        to: to_account_identifier,
        created_at_time: Some(Timestamp { timestamp_nanos }),
    };
    let arg = proto::into_bytes(args).map_err(|_| SignTransferError::EncodingArgsFailed)?;

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
        sender,
        canister_id: ic_ledger_types::MAINNET_LEDGER_CANISTER_ID,
        method_name: METHOD_NAME.to_string(),
        arg,
    };

    let request_id = content.to_request_id();
    let signature = identity
        .sign(request_id::make_sig_data(&request_id))
        .map_err(SignTransferError::Identity)?;

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
        sender,
        paths: vec![vec![
            "request_status".into(),
            request_id.0.as_slice().into(),
        ]],
    };

    let request_id = content.to_request_id();
    let signature = identity
        .sign(request_id::make_sig_data(&request_id))
        .map_err(SignTransferError::Identity)?;

    let env = Envelope {
        content,
        sender_pubkey: Some(signature.public_key),
        sender_sig: Some(signature.signature),
    };
    Ok(env)
}

#[cfg(test)]
mod test {
    use super::*;

    use candid::Principal;
    use ic_ledger_types::DEFAULT_SUBACCOUNT;
    use k256::SecretKey;
    use tw_encoding::hex;

    pub const ECDSA_SECP256K1: &str = "-----BEGIN EC PRIVATE KEY-----
MHQCAQEEICJxApEbuZznKFpV+VKACRK30i6+7u5Z13/DOl18cIC+oAcGBSuBBAAK
oUQDQgAEPas6Iag4TUx+Uop+3NhE6s3FlayFtbwdhRVjvOar0kPTfE/N8N6btRnd
74ly5xXEBNSXiENyxhEuzOZrIWMCNQ==
-----END EC PRIVATE KEY-----";

    pub const SIGNED_TRANSACTION: &str = "81826b5452414e53414354494f4e81a266757064617465a367636f6e74656e74a66c726571756573745f747970656463616c6c6e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d026b63616e69737465725f69644a000000000000000201016b6d6574686f645f6e616d656773656e645f70626361726758400a0012070a050880c2d72f1a0308904e2a220a20943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a3a0a088090caa5a3a78abd176d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f736967984018d8189d18ee188a1118a81858184018da188d188c18b800184c18f611182718ea18931899186f183318c518711848186718841718351825181e187c18710018a21871183618f6184b18cd18e618e418ea182c18d118c91857188d140c18b4182a188018c51871189f1418b518cf182e18b618a418fd18a36a726561645f7374617465a367636f6e74656e74a46c726571756573745f747970656a726561645f73746174656e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d0265706174687381824e726571756573745f7374617475735820b20f43257a5e87542693561e20a6076d515395e49623fcecd6c5b5640b8db8c36d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f736967984018a8189b12186d18a4188d18fb18f71869187918f70e1825181d185a0318440b18e0186e1820183f1834188016186818dd183b18d518de18941849187e1882186e18591861187218ac0a18de18df1858183718b6182818930c18431864183718f60a18ef1824185e18ed184e18841839185718d5091888";

    #[test]
    fn transfer_signature() {
        let secret_key = SecretKey::from_sec1_pem(ECDSA_SECP256K1).unwrap();

        let identity = crate::sign::identity::Identity::new(secret_key).unwrap();
        let to_account_identifier = AccountIdentifier::new(
            &Principal::from_text(
                "t4u4z-y3dur-j63pk-nw4rv-yxdbt-agtt6-nygn7-ywh6y-zm2f4-sdzle-3qe",
            )
            .unwrap(),
            &DEFAULT_SUBACCOUNT,
        );
        let signed_transaction =
            transfer(identity, to_account_identifier, 100000000, 0, 1_691_709_940).unwrap();
        let hex_encoded_signed_transaction = hex::encode(&signed_transaction, false);

        assert_eq!(hex_encoded_signed_transaction, SIGNED_TRANSACTION);
    }
}
