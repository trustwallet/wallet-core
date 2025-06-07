// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::defined_addresses::{COMPUTE_BUDGET_ADDRESS, SYSTEM_PROGRAM_ID_ADDRESS};
use crate::instruction::Instruction;
use crate::modules::insert_instruction::InsertInstruction;
use crate::modules::instruction_builder::compute_budget_instruction::{
    ComputeBudgetInstruction, ComputeBudgetInstructionBuilder, UnitLimit, UnitPrice,
};
use crate::modules::instruction_builder::system_instruction::SystemInstruction;
use crate::modules::message_decompiler::{InstructionWithoutAccounts, MessageDecompiler};
use crate::modules::proto_builder::ProtoBuilder;
use crate::modules::tx_signer::TxSigner;
use crate::modules::PubkeySignatureMap;
use crate::transaction::versioned::VersionedTransaction;
use crate::SOLANA_ALPHABET;
use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_encoding::base58;
use tw_encoding::base64::{self, STANDARD};
use tw_hash::H256;
use tw_keypair::{ed25519, KeyPairResult};
use tw_memory::Data;
use tw_proto::Solana::Proto;

pub struct SolanaTransaction;

impl SolanaTransaction {
    pub fn update_blockhash_and_sign(
        encoded_tx: &str,
        recent_blockhash: &str,
        private_keys: &[Data],
    ) -> Proto::SigningOutput<'static> {
        Self::update_blockhash_and_sign_impl(encoded_tx, recent_blockhash, private_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn update_blockhash_and_sign_impl(
        encoded_tx: &str,
        recent_blockhash: &str,
        private_keys: &[Data],
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let tx_bytes = base64::decode(encoded_tx, STANDARD)?;

        let tx_to_sign: VersionedTransaction =
            bincode::deserialize(&tx_bytes).map_err(|_| SigningErrorType::Error_input_parse)?;
        let mut msg_to_sign = tx_to_sign.message;

        let new_blockchain_hash = base58::decode(recent_blockhash, SOLANA_ALPHABET)?;
        let new_blockchain_hash = H256::try_from(new_blockchain_hash.as_slice())
            .tw_err(SigningErrorType::Error_invalid_params)?;

        // Update the transaction's blockhash and re-sign it.
        msg_to_sign.set_recent_blockhash(new_blockchain_hash);

        let unsigned_encoded = TxSigner::preimage_versioned(&msg_to_sign)?;

        // Do not sign the transaction if there is no private keys, but set zeroed signatures.
        // It's needed to estimate the transaction fee with an updated blockhash without using real private keys.
        let signed_tx = if private_keys.is_empty() {
            VersionedTransaction::unsigned(msg_to_sign)
        } else {
            let private_keys = private_keys
                .iter()
                .map(|pk| ed25519::sha512::KeyPair::try_from(pk.as_slice()))
                .collect::<KeyPairResult<Vec<_>>>()?;

            let external_signatures = PubkeySignatureMap::default();
            TxSigner::sign_versioned(msg_to_sign, &private_keys, &external_signatures)?
        };

        let unsigned_encoded = base64::encode(&unsigned_encoded, STANDARD);
        let signed_encoded =
            bincode::serialize(&signed_tx).tw_err(SigningErrorType::Error_internal)?;
        let signed_encoded = base64::encode(&signed_encoded, STANDARD);

        Ok(Proto::SigningOutput {
            encoded: Cow::from(signed_encoded),
            unsigned_tx: Cow::from(unsigned_encoded),
            signatures: ProtoBuilder::build_signatures(&signed_tx),
            ..Proto::SigningOutput::default()
        })
    }

    pub fn get_compute_unit_price(encoded_tx: &str) -> SigningResult<Option<UnitPrice>> {
        let tx = VersionedTransaction::from_base64(encoded_tx)?;
        let instructions = MessageDecompiler::decompile_partly(&tx.message)?;
        Ok(instructions
            .iter()
            .find_map(try_instruction_as_set_unit_price))
    }

    pub fn get_compute_unit_limit(encoded_tx: &str) -> SigningResult<Option<UnitLimit>> {
        let tx = VersionedTransaction::from_base64(encoded_tx)?;
        let instructions = MessageDecompiler::decompile_partly(&tx.message)?;
        Ok(instructions
            .iter()
            .find_map(try_instruction_as_set_unit_limit))
    }

    pub fn set_compute_unit_price(encoded_tx: &str, price: UnitPrice) -> SigningResult<String> {
        let tx_bytes = base64::decode(encoded_tx, STANDARD)?;
        let mut tx: VersionedTransaction =
            bincode::deserialize(&tx_bytes).map_err(|_| SigningErrorType::Error_input_parse)?;
        let instructions = MessageDecompiler::decompile_partly(&tx.message)?;

        let set_price_ix = ComputeBudgetInstructionBuilder::set_compute_unit_price(price);

        // First, try to find a `ComputeBudgetInstruction::SetComputeUnitPrice` instruction.
        let ix_position = instructions
            .iter()
            .position(|ix| try_instruction_as_set_unit_price(ix).is_some());
        // If it presents already, it's enough to update the instruction data only.
        if let Some(pos) = ix_position {
            tx.message.instructions_mut()[pos].data = set_price_ix.data;
            return tx.to_base64().tw_err(SigningErrorType::Error_internal);
        }

        // `ComputeBudgetInstruction::SetComputeUnitPrice` can be pushed to the end of the instructions list.
        tx.message
            .push_simple_instruction(set_price_ix.program_id, set_price_ix.data)?;

        tx.to_base64().tw_err(SigningErrorType::Error_internal)
    }

    pub fn set_compute_unit_limit(encoded_tx: &str, limit: UnitLimit) -> SigningResult<String> {
        let tx_bytes = base64::decode(encoded_tx, STANDARD)?;
        let mut tx: VersionedTransaction =
            bincode::deserialize(&tx_bytes).map_err(|_| SigningErrorType::Error_input_parse)?;
        let instructions = MessageDecompiler::decompile_partly(&tx.message)?;

        let set_limit_ix = ComputeBudgetInstructionBuilder::set_compute_unit_limit(limit);

        // First, try to find a `ComputeBudgetInstruction::SetComputeUnitLimit` instruction.
        let ix_position = instructions
            .iter()
            .position(|ix| try_instruction_as_set_unit_limit(ix).is_some());
        // If it presents already, it's enough to update the instruction data only.
        if let Some(pos) = ix_position {
            tx.message.instructions_mut()[pos].data = set_limit_ix.data;
            return tx.to_base64().tw_err(SigningErrorType::Error_internal);
        }

        // `ComputeBudgetInstruction::SetComputeUnitLimit` should be at the beginning of the instructions list.
        // However `SystemInstruction::AdvanceNonceAccount` must be the first instruction.
        // So in case if the advance nonce instruction presents, we should insert unit limit as the second instruction.
        let insert_at = match instructions.first() {
            Some(first_ix) if is_instruction_advance_nonce_account(first_ix) => 1,
            _ => 0,
        };

        tx.message.insert_simple_instruction(
            insert_at,
            set_limit_ix.program_id,
            set_limit_ix.data,
        )?;

        tx.to_base64().tw_err(SigningErrorType::Error_internal)
    }

    pub fn set_fee_payer(encoded_tx: &str, fee_payer: SolanaAddress) -> SigningResult<String> {
        let tx_bytes = base64::decode(encoded_tx, STANDARD)?;
        let mut tx: VersionedTransaction =
            bincode::deserialize(&tx_bytes).map_err(|_| SigningErrorType::Error_input_parse)?;

        tx.message.set_fee_payer(fee_payer)?;

        // Set the correct number of zero signatures
        let unsigned_tx = VersionedTransaction::unsigned(tx.message);
        unsigned_tx
            .to_base64()
            .tw_err(SigningErrorType::Error_internal)
    }

    pub fn insert_instruction(
        encoded_tx: &str,
        insert_at: i32,
        instruction: &str,
    ) -> SigningResult<String> {
        let tx_bytes = base64::decode(encoded_tx, STANDARD)?;
        let mut tx: VersionedTransaction =
            bincode::deserialize(&tx_bytes).map_err(|_| SigningErrorType::Error_input_parse)?;

        let instruction: Instruction =
            serde_json::from_str(instruction).map_err(|_| SigningErrorType::Error_input_parse)?;

        if insert_at >= 0 && insert_at as usize > tx.message.instructions().len() {
            return Err(SigningError::from(SigningErrorType::Error_invalid_params));
        }

        let final_insert_at = if insert_at < 0 {
            tx.message.instructions().len() // Append to the end if negative
        } else {
            insert_at as usize // Use the specified position
        };

        tx.message.insert_instruction(
            final_insert_at,
            instruction.program_id,
            instruction.accounts,
            instruction.data,
        )?;

        // Set the correct number of zero signatures
        let unsigned_tx = VersionedTransaction::unsigned(tx.message);
        unsigned_tx
            .to_base64()
            .tw_err(SigningErrorType::Error_internal)
    }
}

fn try_instruction_as_compute_budget(
    ix: &InstructionWithoutAccounts,
) -> Option<ComputeBudgetInstruction> {
    if ix.program_id != *COMPUTE_BUDGET_ADDRESS {
        return None;
    }
    ComputeBudgetInstruction::try_from_borsh(&ix.data).ok()
}

fn try_instruction_as_set_unit_price(ix: &InstructionWithoutAccounts) -> Option<UnitPrice> {
    match try_instruction_as_compute_budget(ix)? {
        ComputeBudgetInstruction::SetComputeUnitPrice(price) => Some(price),
        _ => None,
    }
}

fn try_instruction_as_set_unit_limit(ix: &InstructionWithoutAccounts) -> Option<UnitLimit> {
    match try_instruction_as_compute_budget(ix)? {
        ComputeBudgetInstruction::SetComputeUnitLimit(limit) => Some(limit),
        _ => None,
    }
}

fn is_instruction_advance_nonce_account(ix: &InstructionWithoutAccounts) -> bool {
    if ix.program_id != *SYSTEM_PROGRAM_ID_ADDRESS {
        return false;
    }
    let Ok(system_ix) = SystemInstruction::try_from_bincode(&ix.data) else {
        return false;
    };
    system_ix == SystemInstruction::AdvanceNonceAccount
}
