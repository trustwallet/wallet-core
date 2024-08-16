// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::blockhash::Blockhash;
use crate::defined_addresses::{TOKEN_2022_PROGRAM_ID_ADDRESS, TOKEN_PROGRAM_ID_ADDRESS};
use crate::instruction::Instruction;
use crate::modules::compiled_instructions::compile_instructions;
use crate::modules::compiled_keys::CompiledKeys;
use crate::modules::instruction_builder::compute_budget_instruction::{UnitLimit, UnitPrice};
use crate::modules::instruction_builder::stake_instruction::{
    DepositStakeArgs, StakeInstructionBuilder,
};
use crate::modules::instruction_builder::system_instruction::SystemInstructionBuilder;
use crate::modules::instruction_builder::token_instruction::TokenInstructionBuilder;
use crate::modules::instruction_builder::InstructionBuilder;
use crate::modules::PubkeySignatureMap;
use crate::transaction::versioned::VersionedMessage;
use crate::transaction::{legacy, v0, CompiledInstruction, MessageHeader, Signature};
use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
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
        let mut signing_keys = Vec::default();
        if !self.input.fee_payer_private_key.is_empty() {
            let fee_payer_private_key =
                ed25519::sha512::KeyPair::try_from(self.input.fee_payer_private_key.as_ref())
                    .into_tw()
                    .context("Invalid fee payer private key")?;
            signing_keys.push(fee_payer_private_key);
        }

        signing_keys.push(self.signer_key()?);

        // Consider matching other transaction types if they may contain other private keys.
        if let ProtoTransactionType::create_nonce_account(ref nonce) = self.input.transaction_type {
            let nonce_private_key =
                ed25519::sha512::KeyPair::try_from(nonce.nonce_account_private_key.as_ref())
                    .into_tw()
                    .context("Invalid nonce account private key")?;
            signing_keys.push(nonce_private_key);
        }

        Ok(signing_keys)
    }

    pub fn external_signatures(&self) -> SigningResult<PubkeySignatureMap> {
        match self.input.raw_message {
            Some(ref raw_message) => RawMessageBuilder::external_signatures(raw_message),
            None => Ok(PubkeySignatureMap::default()),
        }
    }

    pub fn build(self) -> SigningResult<VersionedMessage> {
        if let Some(ref raw_message) = self.input.raw_message {
            return RawMessageBuilder::build(raw_message);
        }

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
            ProtoTransactionType::advance_nonce_account(ref advance_nonce) => {
                self.advance_nonce_from_proto(advance_nonce)
            },
            ProtoTransactionType::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No transaction type specified"),
        }
    }

    fn transfer_from_proto(
        &self,
        transfer: &Proto::Transfer<'_>,
    ) -> SigningResult<Vec<Instruction>> {
        let from = self.signer_address()?;
        let to = SolanaAddress::from_str(&transfer.recipient)
            .into_tw()
            .context("Invalid recipient address")?;

        let references = Self::parse_references(&transfer.references)?;

        let transfer_ix = SystemInstructionBuilder::transfer(from, to, transfer.value)
            .with_references(references);

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, from)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .maybe_memo(transfer.memo.as_ref())
            .add_instruction(transfer_ix);
        Ok(builder.output())
    }

    fn delegate_stake_from_proto(
        &self,
        delegate: &Proto::DelegateStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        let validator = SolanaAddress::from_str(delegate.validator_pubkey.as_ref())
            .into_tw()
            .context("Invalid validator address")?;

        let stake_account = if delegate.stake_account.is_empty() {
            None
        } else {
            let stake_account = SolanaAddress::from_str(&delegate.stake_account)
                .into_tw()
                .context("Invalid stake account")?;
            Some(stake_account)
        };

        let deposit_ixs = StakeInstructionBuilder::deposit_stake(DepositStakeArgs {
            sender,
            validator,
            stake_account,
            recent_blockhash: self.recent_blockhash()?,
            lamports: delegate.value,
            space: DEFAULT_SPACE,
        });

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, sender)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .add_instructions(deposit_ixs);
        Ok(builder.output())
    }

    fn deactivate_stake_from_proto(
        &self,
        deactivate: &Proto::DeactivateStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        let stake_account = SolanaAddress::from_str(&deactivate.stake_account)
            .into_tw()
            .context("Invalid stake account")?;

        let deactivate_ix = StakeInstructionBuilder::deactivate(stake_account, sender);

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, sender)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .add_instruction(deactivate_ix);
        Ok(builder.output())
    }

    fn deactivate_all_stake_from_proto(
        &self,
        deactivate_all: &Proto::DeactivateAllStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        let deactivate_ixs = deactivate_all
            .stake_accounts
            .iter()
            .map(|stake_account| {
                let stake_account = SolanaAddress::from_str(stake_account.as_ref())?;
                Ok(StakeInstructionBuilder::deactivate(stake_account, sender))
            })
            .collect::<SigningResult<Vec<_>>>()
            .context("Invalid stake account(s)")?;

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, sender)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .add_instructions(deactivate_ixs);
        Ok(builder.output())
    }

    fn withdraw_from_proto(
        &self,
        withdraw: &Proto::WithdrawStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;
        let stake_account = SolanaAddress::from_str(withdraw.stake_account.as_ref())
            .into_tw()
            .context("Invalid stake account")?;

        let custodian_account = None;

        let withdraw_ix = StakeInstructionBuilder::withdraw(
            stake_account,
            sender,
            sender,
            withdraw.value,
            custodian_account,
        );

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, sender)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .add_instruction(withdraw_ix);
        Ok(builder.output())
    }

    fn withdraw_all_from_proto(
        &self,
        withdraw_all: &Proto::WithdrawAllStake,
    ) -> SigningResult<Vec<Instruction>> {
        let sender = self.signer_address()?;

        let withdraw_ixs = withdraw_all
            .stake_accounts
            .iter()
            .map(|withdraw| {
                let stake_account = SolanaAddress::from_str(withdraw.stake_account.as_ref())
                    .into_tw()
                    .context("Invalid stake account")?;

                let custodian_account = None;
                Ok(StakeInstructionBuilder::withdraw(
                    stake_account,
                    sender,
                    sender,
                    withdraw.value,
                    custodian_account,
                ))
            })
            .collect::<SigningResult<Vec<_>>>()?;

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, sender)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .add_instructions(withdraw_ixs);
        Ok(builder.output())
    }

    fn create_token_account_from_proto(
        &self,
        create_token_acc: &Proto::CreateTokenAccount,
    ) -> SigningResult<Vec<Instruction>> {
        let funding_account = self.signer_address()?;
        let other_main_address = SolanaAddress::from_str(create_token_acc.main_address.as_ref())
            .into_tw()
            .context("Invalid main address")?;

        let token_mint_address =
            SolanaAddress::from_str(create_token_acc.token_mint_address.as_ref())
                .into_tw()
                .context("Invalid token mint address")?;

        let token_address = SolanaAddress::from_str(create_token_acc.token_address.as_ref())
            .into_tw()
            .context("Invalid token address to be created")?;

        let instruction = TokenInstructionBuilder::create_account(
            funding_account,
            other_main_address,
            token_mint_address,
            token_address,
            match_program_id(create_token_acc.token_program_id),
        );
        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, funding_account)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .add_instruction(instruction);
        Ok(builder.output())
    }

    fn token_transfer_from_proto(
        &self,
        token_transfer: &Proto::TokenTransfer,
    ) -> SigningResult<Vec<Instruction>> {
        let signer = self.signer_address()?;
        let sender_token_address =
            SolanaAddress::from_str(token_transfer.sender_token_address.as_ref())
                .into_tw()
                .context("Invalid sender token address")?;

        let token_mint_address =
            SolanaAddress::from_str(token_transfer.token_mint_address.as_ref())
                .into_tw()
                .context("Invalid token mint address")?;

        let recipient_token_address =
            SolanaAddress::from_str(token_transfer.recipient_token_address.as_ref())
                .into_tw()
                .context("Invalid recipient token address")?;

        let decimals = token_transfer
            .decimals
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid token decimals. Expected lower than 256")?;

        let references = Self::parse_references(&token_transfer.references)?;
        let transfer_instruction = TokenInstructionBuilder::transfer_checked(
            sender_token_address,
            token_mint_address,
            recipient_token_address,
            signer,
            token_transfer.amount,
            decimals,
            match_program_id(token_transfer.token_program_id),
        )
        .with_references(references);

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, signer)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
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
            SolanaAddress::from_str(create_and_transfer.sender_token_address.as_ref())
                .into_tw()
                .context("Invalid sender token address")?;

        let token_mint_address =
            SolanaAddress::from_str(create_and_transfer.token_mint_address.as_ref())
                .into_tw()
                .context("Invalid token mint address")?;

        let recipient_main_address =
            SolanaAddress::from_str(create_and_transfer.recipient_main_address.as_ref())
                .into_tw()
                .context("Invalid recipient main address")?;

        let recipient_token_address =
            SolanaAddress::from_str(create_and_transfer.recipient_token_address.as_ref())
                .into_tw()
                .context("Invalid recipient token address")?;

        let references = Self::parse_references(&create_and_transfer.references)?;

        let decimals = create_and_transfer
            .decimals
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid token decimals. Expected lower than 256")?;

        let create_account_instruction = TokenInstructionBuilder::create_account(
            // Can be different from the actual signer.
            fee_payer,
            recipient_main_address,
            token_mint_address,
            recipient_token_address,
            match_program_id(create_and_transfer.token_program_id),
        );
        let transfer_instruction = TokenInstructionBuilder::transfer_checked(
            sender_token_address,
            token_mint_address,
            recipient_token_address,
            signer,
            create_and_transfer.amount,
            decimals,
            match_program_id(create_and_transfer.token_program_id),
        )
        .with_references(references);

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, signer)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
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
        let prev_nonce_account = self.nonce_account()?;

        let new_nonce_account = if create_nonce.nonce_account.is_empty() {
            let nonce_key =
                ed25519::sha512::KeyPair::try_from(create_nonce.nonce_account_private_key.as_ref())
                    .into_tw()
                    .context("Invalid nonce account private key")?;
            SolanaAddress::with_public_key_ed25519(nonce_key.public())
        } else {
            SolanaAddress::from_str(create_nonce.nonce_account.as_ref())
                .into_tw()
                .context("Invalid nonce account")?
        };

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(prev_nonce_account, signer)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
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
        let withdraw_from_nonce = SolanaAddress::from_str(withdraw_nonce.nonce_account.as_ref())
            .into_tw()
            .context("Invalid nonce account")?;

        let recipient = SolanaAddress::from_str(withdraw_nonce.recipient.as_ref())
            .into_tw()
            .context("Invalid recipient")?;

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(self.nonce_account()?, signer)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit())
            .add_instruction(SystemInstructionBuilder::withdraw_nonce_account(
                withdraw_from_nonce,
                signer,
                recipient,
                withdraw_nonce.value,
            ));
        Ok(builder.output())
    }

    fn advance_nonce_from_proto(
        &self,
        advance_nonce: &Proto::AdvanceNonceAccount,
    ) -> SigningResult<Vec<Instruction>> {
        let signer = self.signer_address()?;
        let nonce_account = SolanaAddress::from_str(advance_nonce.nonce_account.as_ref())
            .into_tw()
            .context("Invalid nonce account")?;

        let mut builder = InstructionBuilder::default();
        builder
            .maybe_advance_nonce(Some(nonce_account), signer)
            .maybe_priority_fee_price(self.priority_fee_price())
            .maybe_priority_fee_limit(self.priority_fee_limit());
        Ok(builder.output())
    }

    fn nonce_account(&self) -> SigningResult<Option<SolanaAddress>> {
        if self.input.nonce_account.is_empty() {
            Ok(None)
        } else {
            SolanaAddress::from_str(&self.input.nonce_account)
                .map(Some)
                .map_err(SigningError::from)
                .context("Invalid nonce account")
        }
    }

    fn signer_address(&self) -> SigningResult<SolanaAddress> {
        if self.input.private_key.is_empty() {
            SolanaAddress::from_str(&self.input.sender)
                .map_err(SigningError::from)
                .context("Sender address is either not set or invalid")
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
                ed25519::sha512::KeyPair::try_from(self.input.fee_payer_private_key.as_ref())
                    .into_tw()
                    .context("Invalid fee payer private key")?;
            return Ok(SolanaAddress::with_public_key_ed25519(
                fee_payer_key.public(),
            ));
        }
        if !self.input.fee_payer.is_empty() {
            return SolanaAddress::from_str(self.input.fee_payer.as_ref())
                .map_err(SigningError::from)
                .context("Invalid fee payer address");
        }
        self.signer_address()
    }

    fn recent_blockhash(&self) -> SigningResult<Blockhash> {
        Blockhash::from_str(&self.input.recent_blockhash)
            .map_err(SigningError::from)
            .context("Invalid recent blockhash")
    }

    fn signer_key(&self) -> SigningResult<ed25519::sha512::KeyPair> {
        ed25519::sha512::KeyPair::try_from(self.input.private_key.as_ref())
            .map_err(SigningError::from)
            .context("Invalid signer key")
    }

    fn priority_fee_price(&self) -> Option<UnitPrice> {
        self.input
            .priority_fee_price
            .as_ref()
            .map(|proto| proto.price)
    }

    fn priority_fee_limit(&self) -> Option<UnitLimit> {
        self.input
            .priority_fee_limit
            .as_ref()
            .map(|proto| proto.limit)
    }

    fn parse_references(refs: &[Cow<'_, str>]) -> SigningResult<Vec<SolanaAddress>> {
        refs.iter()
            .map(|addr| SolanaAddress::from_str(addr).map_err(SigningError::from))
            .collect::<SigningResult<Vec<_>>>()
            .context("Invalid transaction reference(s)")
    }
}

pub struct RawMessageBuilder;

impl RawMessageBuilder {
    pub fn build(raw_message: &Proto::RawMessage) -> SigningResult<VersionedMessage> {
        use Proto::mod_RawMessage::OneOfmessage as RawMessageType;

        match raw_message.message {
            RawMessageType::legacy(ref legacy) => Self::build_legacy(legacy),
            RawMessageType::v0(ref v0) => Self::build_v0(v0),
            RawMessageType::None => SigningError::err(SigningErrorType::Error_invalid_params),
        }
    }

    pub fn external_signatures(
        raw_message: &Proto::RawMessage,
    ) -> SigningResult<PubkeySignatureMap> {
        let mut key_signs = PubkeySignatureMap::with_capacity(raw_message.signatures.len());
        for entry in raw_message.signatures.iter() {
            let pubkey = SolanaAddress::from_str(entry.pubkey.as_ref())
                .into_tw()
                .context("Invalid 'PubkeySignature::public'")?;

            let signature = Signature::from_str(entry.signature.as_ref())
                .context("Invalid 'PubkeySignature::signature'")?;

            let ed25519_signature = ed25519::Signature::try_from(signature.0.as_slice())
                .into_tw()
                .context("Invalid 'PubkeySignature::signature'")?;

            key_signs.insert(pubkey, ed25519_signature);
        }
        Ok(key_signs)
    }

    fn build_legacy(
        legacy: &Proto::mod_RawMessage::MessageLegacy,
    ) -> SigningResult<VersionedMessage> {
        let header = Self::build_message_header(&legacy.header)?;
        let account_keys = Self::build_account_keys(&legacy.account_keys)?;
        let recent_blockhash = Blockhash::from_str(legacy.recent_blockhash.as_ref())
            .into_tw()
            .context("Invalid recent blockhash")?;

        let instructions: Vec<_> = Self::build_instructions(&legacy.instructions)?;

        Ok(VersionedMessage::Legacy(legacy::Message {
            header,
            account_keys,
            recent_blockhash: recent_blockhash.to_bytes(),
            instructions,
        }))
    }

    fn build_v0(v0: &Proto::mod_RawMessage::MessageV0) -> SigningResult<VersionedMessage> {
        let header = Self::build_message_header(&v0.header)?;
        let account_keys = Self::build_account_keys(&v0.account_keys)?;
        let recent_blockhash = Blockhash::from_str(v0.recent_blockhash.as_ref())
            .into_tw()
            .context("Invalid recent blockhash")?;

        let instructions: Vec<_> = Self::build_instructions(&v0.instructions)?;
        let address_table_lookups = v0
            .address_table_lookups
            .iter()
            .map(Self::build_address_lookup_table)
            .collect::<SigningResult<Vec<_>>>()?;

        Ok(VersionedMessage::V0(v0::Message {
            header,
            account_keys,
            recent_blockhash: recent_blockhash.to_bytes(),
            instructions,
            address_table_lookups,
        }))
    }

    fn build_message_header(
        raw_header: &Option<Proto::mod_RawMessage::MessageHeader>,
    ) -> SigningResult<MessageHeader> {
        let raw_header = raw_header
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("No message header provided")?;

        Ok(MessageHeader {
            num_required_signatures: try_into_u8(raw_header.num_required_signatures)?,
            num_readonly_signed_accounts: try_into_u8(raw_header.num_readonly_signed_accounts)?,
            num_readonly_unsigned_accounts: try_into_u8(raw_header.num_readonly_unsigned_accounts)?,
        })
    }

    fn build_account_keys(raw_keys: &[Cow<'_, str>]) -> SigningResult<Vec<SolanaAddress>> {
        raw_keys
            .iter()
            .map(|s| SolanaAddress::from_str(s.as_ref()))
            .collect::<AddressResult<Vec<_>>>()
            .map_err(SigningError::from)
            .context("Cannot build account keys")
    }

    fn build_instructions(
        ixs: &[Proto::mod_RawMessage::Instruction],
    ) -> SigningResult<Vec<CompiledInstruction>> {
        ixs.iter().map(Self::build_instruction).collect()
    }

    fn build_instruction(
        ix: &Proto::mod_RawMessage::Instruction,
    ) -> SigningResult<CompiledInstruction> {
        let accounts = ix
            .accounts
            .iter()
            .map(|idx| try_into_u8(*idx))
            .collect::<SigningResult<Vec<_>>>()
            .context("Cannot build account metas")?;

        Ok(CompiledInstruction {
            program_id_index: try_into_u8(ix.program_id).context("Invalid program ID")?,
            accounts,
            data: ix.program_data.to_vec(),
        })
    }

    fn build_address_lookup_table(
        lookup: &Proto::mod_RawMessage::MessageAddressTableLookup,
    ) -> SigningResult<v0::MessageAddressTableLookup> {
        let account_key = SolanaAddress::from_str(lookup.account_key.as_ref())
            .into_tw()
            .context("Invalid lookup's account key")?;

        let writable_indexes = lookup
            .writable_indexes
            .iter()
            .copied()
            .map(try_into_u8)
            .collect::<SigningResult<Vec<_>>>()
            .context("Invalid writable index(s)")?;

        let readonly_indexes = lookup
            .readonly_indexes
            .iter()
            .copied()
            .map(try_into_u8)
            .collect::<SigningResult<Vec<_>>>()
            .context("Invalid readonly index(s)")?;

        Ok(v0::MessageAddressTableLookup {
            account_key,
            writable_indexes,
            readonly_indexes,
        })
    }
}

fn try_into_u8<T>(num: T) -> SigningResult<u8>
where
    u8: TryFrom<T>,
{
    u8::try_from(num).tw_err(|_| SigningErrorType::Error_tx_too_big)
}

fn match_program_id(program_id: Proto::TokenProgramId) -> SolanaAddress {
    match program_id {
        Proto::TokenProgramId::TokenProgram => *TOKEN_PROGRAM_ID_ADDRESS,
        Proto::TokenProgramId::Token2022Program => *TOKEN_2022_PROGRAM_ID_ADDRESS,
    }
}
