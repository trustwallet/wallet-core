use super::transfer::SendArgs;
use ic_ledger_types;
use prost;
use prost::Message;

#[derive(Clone, PartialEq, prost::Message)]
pub struct AccountIdentifier {
    /// Can contain either:
    ///   * the 32 byte identifier (4 byte checksum + 28 byte hash)
    ///   * the 28 byte hash
    #[prost(bytes = "vec", tag = "1")]
    pub hash: prost::alloc::vec::Vec<u8>,
}

#[derive(Clone, PartialEq, prost::Message)]
pub struct Subaccount {
    #[prost(bytes = "vec", tag = "1")]
    pub sub_account: prost::alloc::vec::Vec<u8>,
}

#[derive(Clone, PartialEq, prost::Message)]
pub struct Memo {
    #[prost(uint64, tag = "1")]
    pub memo: u64,
}

#[derive(Clone, PartialEq, prost::Message)]
pub struct Tokens {
    #[prost(uint64, tag = "1")]
    pub e8s: u64,
}

#[derive(Clone, PartialEq, prost::Message)]
pub struct Payment {
    #[prost(message, optional, tag = "1")]
    pub receiver_gets: ::core::option::Option<Tokens>,
}

#[derive(Clone, PartialEq, prost::Message)]
pub struct BlockIndex {
    #[prost(uint64, tag = "1")]
    pub height: u64,
}
#[derive(Clone, PartialEq, prost::Message)]
pub struct TimeStamp {
    #[prost(uint64, tag = "1")]
    pub timestamp_nanos: u64,
}
/// Make a payment
#[derive(Clone, PartialEq, prost::Message)]
pub struct SendRequest {
    #[prost(message, optional, tag = "1")]
    pub memo: ::core::option::Option<Memo>,
    #[prost(message, optional, tag = "2")]
    pub payment: ::core::option::Option<Payment>,
    #[prost(message, optional, tag = "3")]
    pub max_fee: ::core::option::Option<Tokens>,
    #[prost(message, optional, tag = "4")]
    pub from_subaccount: ::core::option::Option<Subaccount>,
    #[prost(message, optional, tag = "5")]
    pub to: ::core::option::Option<AccountIdentifier>,
    #[prost(message, optional, tag = "6")]
    pub created_at: ::core::option::Option<BlockIndex>,
    #[prost(message, optional, tag = "7")]
    pub created_at_time: ::core::option::Option<TimeStamp>,
}

pub fn into_bytes(args: SendArgs) -> Result<Vec<u8>, String> {
    let proto_type = into_proto(args);
    let mut buf = Vec::with_capacity(proto_type.encoded_len());
    proto_type.encode(&mut buf).map_err(|e| e.to_string())?;
    Ok(buf)
}

pub fn tokens_into_proto(tokens: ic_ledger_types::Tokens) -> Tokens {
    Tokens { e8s: tokens.e8s() }
}

pub fn timestamp_into_proto(ts: ic_ledger_types::Timestamp) -> TimeStamp {
    TimeStamp {
        timestamp_nanos: ts.timestamp_nanos,
    }
}

fn into_proto(args: SendArgs) -> SendRequest {
    let SendArgs {
        memo,
        amount,
        fee,
        to,
        created_at_time,
    } = args;
    let amount = tokens_into_proto(amount);
    let payment = Some(Payment {
        receiver_gets: Some(amount),
    });

    SendRequest {
        memo: Some(Memo { memo: memo.0 }),
        payment,
        max_fee: Some(tokens_into_proto(fee)),
        from_subaccount: None,
        to: Some(to).map(|ai| AccountIdentifier {
            hash: ai.as_ref().to_vec(),
        }),
        created_at: None,
        created_at_time: created_at_time.map(timestamp_into_proto),
    }
}
