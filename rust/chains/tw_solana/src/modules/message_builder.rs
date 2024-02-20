// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::blockhash::Blockhash;
use crate::instruction::Instruction;
use crate::modules::compiled_instructions::compile_instructions;
use crate::modules::compiled_keys::CompiledKeys;
use crate::modules::instruction_builder::stake_instruction::{
    DepositStakeArgs, StakeInstructionBuilder,
};
use crate::modules::instruction_builder::system_instruction::SystemInstructionBuilder;
use crate::modules::instruction_builder::token_instruction::TokenInstructionBuilder;
use crate::modules::instruction_builder::InstructionBuilder;
use crate::transaction::versioned::VersionedMessage;
use crate::transaction::{legacy, v0};
use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_keypair::ed25519;
use tw_keypair::traits::KeyPairTrait;
use tw_proto::Solana::Proto;
use Proto::mod_SigningInput::OneOftransaction_type as ProtoTransactionType;

const DEFAULT_SPACE: u64 = 200;
const DEFAULT_CREATE_NONCE_SPACE: u64 = 80;

pub struct MessageBuilder<'a> {
    input: Proto::SigningInput<'a>,
}

impl<'a> MessageBuilder<'a> {
    pub fn new(input: Proto::SigningInput<'a>) -> Self {
        MessageBuilder { input }
    }

    pub fn signing_keys(&self) -> SigningResult<Vec<ed25519::sha512::KeyPair>> {
        let mut signing_keys = vec![self.signer_key()?];

        if !self.input.fee_payer_private_key.is_empty() {
            let fee_payer_private_key =
                ed25519::sha512::KeyPair::try_from(self.input.fee_payer_private_key.as_ref())?;
            signing_keys.push(fee_payer_private_key);
        }

        // Consider matching other transaction types if they may contain other private keys.
        if let ProtoTransactionType::create_nonce_account(ref nonce) = self.input.transaction_type {
            let nonce_private_key =
                ed25519::sha512::KeyPair::try_from(nonce.nonce_account_private_key.as_ref())?;
            signing_keys.push(nonce_private_key);
        }

        Ok(signing_keys)
    }

    pub fn build(self) -> SigningResult<VersionedMessage> {
        let instructions = self.build_instructions()?;

        // Please note the fee payer can be different from the actual signer.
        let (message_header, account_keys) = CompiledKeys::with_fee_payer(self.fee_payer()?)
            .compile(&instructions)
            .try_into_message_components()?;

        let compiled_instructions = compile_instructions(&instructions, &account_keys)?;

        if self.input.v0_msg {
            Ok(VersionedMessage::V0(v0::Message {
                header: message_header,
                account_keys,
                recent_blockhash: self.recent_blockhash()?.to_bytes(),
                instructions: compiled_instructions,
                address_table_lookups: Vec::default(),
            }))
        } else {
            Ok(VersionedMessage::Legacy(legacy::Message {
                header: message_header,
                account_keys,
                recent_blockhash: self.recent_blockhash()?.to_bytes(),
                instructions: compiled_instructions,
            }))
        }
    }

    /// Please note that this method can add [`MessageBuilder::signer_keys`] if necessary.
    fn build_instructions(&self) -> SigningResult<Vec<Instruction>> {
        match self.input.transaction_type {
            ProtoTransactionType::transfer_transaction(ref transfer) => {
                self.transfer_from_proto(transfer)
            },
            ProtoTransactionType::delegate_stake_transaction(ref delegate) => {
                self.delegate_stake_from_proto(delegate)
            },
            ProtoTransactionType::deactivate_stake_transaction(ref deactivate) => {
                self.deactivate_stake_from_proto(deactivate)
            },
            ProtoTransactionType::deactivate_all_stake_transaction(ref deactivate_all) => {
                self.deactivate_all_stake_from_proto(deactivate_all)
            },
            ProtoTransactionType::withdraw_transaction(ref withdraw) => {
                self.withdraw_from_proto(withdraw)
            },
            ProtoTransactionType::withdraw_all_transaction(ref withdraw_all) => {
                self.withdraw_all_from_proto(withdraw_all)
            },
            ProtoTransactionType::create_token_account_transaction(ref create_token_acc) => {
                self.create_token_account_from_proto(create_token_acc)
            },
            ProtoTransactionType::token_transfer_transaction(ref token_transfer) => {
                self.token_transfer_from_proto(token_transfer)
            },
            ProtoTransactionType::create_and_transfer_token_transaction(
                ref create_and_transfer,
            ) => self.create_and_transfer_token_from_proto(create_and_transfer),
            ProtoTransactionType::create_nonce_account(ref create_nonce) => {
                self.create_nonce_account_from_proto(create_nonce)
            },
            ProtoTransactionType::withdraw_nonce_account(ref withdraw_nonce) => {
                self.withdraw_nonce_from_proto(withdraw_nonce)
            },
            _ => todo!(),
        }
    }

    fn transfer_from_proto(
        &self,
        transfer: &Proto::Transfer<'_>,
    ) -> SigningResult<Vec<Instruction>> {
        let from = self.signer_address()?;
        let to = SolanaAddress::from_str(&transfer.recipient)?;
        let references = Self::parse_references(&transfer.references)?;

        let transfer_instruction = SystemInstructionBuilder::transfer(from, to, transfer.value)
            .with_references(references);

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, from)
            .maybe_memo(transfer.memo.as_ref())
            .add_instruction(transfer_instruction);
        Ok(builder.output())
    }

    fn delegate_stake_from_proto(
        &self,
        delegate: &Proto::DelegateStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        let validator = SolanaAddress::from_str(delegate.validator_pubkey.as_ref())?;

        let stake_account = if delegate.stake_account.is_empty() {
            None
        } else {
            let stake_account = SolanaAddress::from_str(&delegate.stake_account)?;
            Some(stake_account)
        };

        StakeInstructionBuilder::deposit_stake(DepositStakeArgs {
            sender,
            validator,
            stake_account,
            recent_blockhash: self.recent_blockhash()?,
            lamports: delegate.value,
            space: DEFAULT_SPACE,
        })
    }

    fn deactivate_stake_from_proto(
        &self,
        deactivate: &Proto::DeactivateStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        let stake_account = SolanaAddress::from_str(&deactivate.stake_account)?;
        Ok(vec![StakeInstructionBuilder::deactivate(
            stake_account,
            sender,
        )])
    }

    fn deactivate_all_stake_from_proto(
        &self,
        deactivate_all: &Proto::DeactivateAllStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        deactivate_all
            .stake_accounts
            .iter()
            .map(|stake_account| {
                let stake_account = SolanaAddress::from_str(stake_account.as_ref())?;
                Ok(StakeInstructionBuilder::deactivate(stake_account, sender))
            })
            .collect()
    }

    fn withdraw_from_proto(
        &self,
        withdraw: &Proto::WithdrawStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        let stake_account = SolanaAddress::from_str(withdraw.stake_account.as_ref())?;
        let custodian_account = None;
        Ok(vec![StakeInstructionBuilder::withdraw(
            stake_account,
            sender,
            sender,
            withdraw.value,
            custodian_account,
        )])
    }

    fn withdraw_all_from_proto(
        &self,
        withdraw_all: &Proto::WithdrawAllStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        withdraw_all
            .stake_accounts
            .iter()
            .map(|withdraw| {
                let stake_account = SolanaAddress::from_str(withdraw.stake_account.as_ref())?;
                let custodian_account = None;
                Ok(StakeInstructionBuilder::withdraw(
                    stake_account,
                    sender,
                    sender,
                    withdraw.value,
                    custodian_account,
                ))
            })
            .collect()
    }

    fn create_token_account_from_proto(
        &self,
        create_token_acc: &Proto::CreateTokenAccount,
    ) -> SigningResult<Vec<Instruction>> {
        let funding_account = self.signer_address()?;
        let other_main_address = SolanaAddress::from_str(create_token_acc.main_address.as_ref())?;
        let token_mint_address =
            SolanaAddress::from_str(create_token_acc.token_mint_address.as_ref())?;
        let token_address = SolanaAddress::from_str(create_token_acc.token_address.as_ref())?;

        let instruction = TokenInstructionBuilder::create_account(
            funding_account,
            other_main_address,
            token_mint_address,
            token_address,
        );
        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, funding_account)
            .add_instruction(instruction);
        Ok(builder.output())
    }

    fn token_transfer_from_proto(
        &self,
        token_transfer: &Proto::TokenTransfer,
    ) -> SigningResult<Vec<Instruction>> {
        let signer = self.signer_address()?;
        let sender_token_address =
            SolanaAddress::from_str(token_transfer.sender_token_address.as_ref())?;
        let token_mint_address =
            SolanaAddress::from_str(token_transfer.token_mint_address.as_ref())?;
        let recipient_token_address =
            SolanaAddress::from_str(token_transfer.recipient_token_address.as_ref())?;

        let decimals = token_transfer
            .decimals
            .try_into()
            .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))?;
        let references = Self::parse_references(&token_transfer.references)?;

        let transfer_instruction = TokenInstructionBuilder::transfer_checked(
            sender_token_address,
            token_mint_address,
            recipient_token_address,
            signer,
            token_transfer.amount,
            decimals,
        )
        .with_references(references);

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, signer)
            .maybe_memo(token_transfer.memo.as_ref())
            .add_instruction(transfer_instruction);
        Ok(builder.output())
    }

    fn create_and_transfer_token_from_proto(
        &self,
        create_and_transfer: &Proto::CreateAndTransferToken,
    ) -> SigningResult<Vec<Instruction>> {
        let signer = self.signer_address()?;
        let fee_payer = self.fee_payer()?;
        let sender_token_address =
            SolanaAddress::from_str(create_and_transfer.sender_token_address.as_ref())?;
        let token_mint_address =
            SolanaAddress::from_str(create_and_transfer.token_mint_address.as_ref())?;
        let recipient_main_address =
            SolanaAddress::from_str(create_and_transfer.recipient_main_address.as_ref())?;
        let recipient_token_address =
            SolanaAddress::from_str(create_and_transfer.recipient_token_address.as_ref())?;
        let references = Self::parse_references(&create_and_transfer.references)?;

        let decimals = create_and_transfer
            .decimals
            .try_into()
            .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))?;

        let create_account_instruction = TokenInstructionBuilder::create_account(
            // Can be different from the actual signer.
            fee_payer,
            recipient_main_address,
            token_mint_address,
            recipient_token_address,
        );
        let transfer_instruction = TokenInstructionBuilder::transfer_checked(
            sender_token_address,
            token_mint_address,
            recipient_token_address,
            signer,
            create_and_transfer.amount,
            decimals,
        )
        .with_references(references);

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, signer)
            .add_instruction(create_account_instruction)
            // Optional memo. Order: before transfer, as per documentation.
            .maybe_memo(create_and_transfer.memo.as_ref())
            .add_instruction(transfer_instruction);
        Ok(builder.output())
    }

    fn create_nonce_account_from_proto(
        &self,
        create_nonce: &Proto::CreateNonceAccount,
    ) -> SigningResult<Vec<Instruction>> {
        let signer = self.signer_address()?;
        let new_nonce_account = SolanaAddress::from_str(create_nonce.nonce_account.as_ref())?;
        let prev_nonce_account = self.nonce_account()?;

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(prev_nonce_account, signer)
            .add_instructions(SystemInstructionBuilder::create_nonce_account(
                signer,
                new_nonce_account,
                create_nonce.rent,
                DEFAULT_CREATE_NONCE_SPACE,
            ));
        Ok(builder.output())
    }

    fn withdraw_nonce_from_proto(
        &self,
        withdraw_nonce: &Proto::WithdrawNonceAccount,
    ) -> SigningResult<Vec<Instruction>> {
        let signer = self.signer_address()?;
        let withdraw_from_nonce = SolanaAddress::from_str(withdraw_nonce.nonce_account.as_ref())?;
        let recipient = SolanaAddress::from_str(withdraw_nonce.recipient.as_ref())?;

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, signer)
            .add_instruction(SystemInstructionBuilder::withdraw_nonce_account(
                withdraw_from_nonce,
                signer,
                recipient,
                withdraw_nonce.value,
            ));
        Ok(builder.output())
    }

    fn nonce_account(&self) -> SigningResult<Option<SolanaAddress>> {
        if self.input.nonce_account.is_empty() {
            Ok(None)
        } else {
            SolanaAddress::from_str(&self.input.nonce_account)
                .map(Some)
                .map_err(SigningError::from)
        }
    }

    fn signer_address(&self) -> SigningResult<SolanaAddress> {
        if self.input.private_key.is_empty() {
            SolanaAddress::from_str(&self.input.sender).map_err(SigningError::from)
        } else {
            Ok(SolanaAddress::with_public_key_ed25519(
                self.signer_key()?.public(),
            ))
        }
    }

    /// Returns a fee payer of the transaction.
    /// Please note it can be different the transaction signer if [`Proto::SigningInput::fee_payer`] is set.
    pub fn fee_payer(&self) -> SigningResult<SolanaAddress> {
        if !self.input.fee_payer_private_key.is_empty() {
            let fee_payer_key =
                ed25519::sha512::KeyPair::try_from(self.input.fee_payer_private_key.as_ref())?;
            return Ok(SolanaAddress::with_public_key_ed25519(
                fee_payer_key.public(),
            ));
        }
        if !self.input.fee_payer.is_empty() {
            return SolanaAddress::from_str(self.input.fee_payer.as_ref())
                .map_err(SigningError::from);
        }
        self.signer_address()
    }

    fn recent_blockhash(&self) -> SigningResult<Blockhash> {
        Ok(Blockhash::from_str(&self.input.recent_blockhash)?)
    }

    fn signer_key(&self) -> SigningResult<ed25519::sha512::KeyPair> {
        ed25519::sha512::KeyPair::try_from(self.input.private_key.as_ref())
            .map_err(SigningError::from)
    }

    fn parse_references(refs: &[Cow<'_, str>]) -> SigningResult<Vec<SolanaAddress>> {
        refs.iter()
            .map(|addr| SolanaAddress::from_str(addr).map_err(SigningError::from))
            .collect()
    }
}
