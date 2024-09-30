// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::v0;
use crate::transaction::versioned::{VersionedMessage, VersionedTransaction};
use std::borrow::Cow;
use tw_proto::Solana::Proto::{self, mod_RawMessage::OneOfmessage as ProtoMessageType};

pub struct ProtoBuilder;

impl ProtoBuilder {
    pub fn build_from_tx(tx: &VersionedTransaction) -> Proto::RawMessage<'static> {
        let message_header = Proto::mod_RawMessage::MessageHeader {
            num_required_signatures: tx.message.header().num_required_signatures as u32,
            num_readonly_signed_accounts: tx.message.header().num_readonly_signed_accounts as u32,
            num_readonly_unsigned_accounts: tx.message.header().num_readonly_unsigned_accounts
                as u32,
        };

        let account_keys: Vec<_> = tx
            .message
            .account_keys()
            .iter()
            .map(|acc| Cow::from(acc.to_string()))
            .collect();

        let recent_blockhash = Cow::from(tx.message.recent_blockhash().to_string());

        let instructions = tx
            .message
            .instructions()
            .iter()
            .map(|ix| Proto::mod_RawMessage::Instruction {
                program_id: ix.program_id_index as u32,
                accounts: vec_u8_to_u32(&ix.accounts),
                program_data: Cow::from(ix.data.clone()),
            })
            .collect();

        let message = match tx.message {
            VersionedMessage::Legacy(_) => {
                ProtoMessageType::legacy(Proto::mod_RawMessage::MessageLegacy {
                    header: Some(message_header),
                    account_keys,
                    recent_blockhash,
                    instructions,
                })
            },
            VersionedMessage::V0(ref v0) => {
                let address_table_lookups =
                    Self::build_address_table_lookups(&v0.address_table_lookups);
                ProtoMessageType::v0(Proto::mod_RawMessage::MessageV0 {
                    header: Some(message_header),
                    account_keys,
                    recent_blockhash,
                    instructions,
                    address_table_lookups,
                })
            },
        };

        Proto::RawMessage {
            signatures: Self::build_signatures(tx),
            message,
        }
    }

    fn build_address_table_lookups(
        lookups: &[v0::MessageAddressTableLookup],
    ) -> Vec<Proto::mod_RawMessage::MessageAddressTableLookup<'static>> {
        lookups
            .iter()
            .map(|lookup| Proto::mod_RawMessage::MessageAddressTableLookup {
                account_key: Cow::from(lookup.account_key.to_string()),
                writable_indexes: vec_u8_to_u32(&lookup.writable_indexes),
                readonly_indexes: vec_u8_to_u32(&lookup.readonly_indexes),
            })
            .collect()
    }

    pub fn build_signatures(tx: &VersionedTransaction) -> Vec<Proto::PubkeySignature<'static>> {
        tx.message
            .account_keys()
            .iter()
            .zip(tx.signatures.iter())
            .map(|(pubkey, signature)| Proto::PubkeySignature {
                pubkey: Cow::from(pubkey.to_string()),
                signature: Cow::from(signature.to_string()),
            })
            .collect()
    }
}

fn vec_u8_to_u32(v: &[u8]) -> Vec<u32> {
    v.iter().map(|num| *num as u32).collect()
}
