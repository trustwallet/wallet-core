// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::abi_to_signing_error;
use crate::abi::prebuild::biz::BizAccount;
use crate::abi::prebuild::erc1155::Erc1155;
use crate::abi::prebuild::erc20::Erc20;
use crate::abi::prebuild::erc4337::Erc4337SimpleAccount;
use crate::abi::prebuild::erc721::Erc721;
use crate::abi::prebuild::ExecuteArgs;
use crate::address::{Address, EvmAddress};
use crate::evm_context::EvmContext;
use crate::modules::authorization_signer::AuthorizationSigner;
use crate::transaction::access_list::{Access, AccessList};
use crate::transaction::authorization_list::{Authorization, AuthorizationList};
use crate::transaction::transaction_eip1559::TransactionEip1559;
use crate::transaction::transaction_eip7702::TransactionEip7702;
use crate::transaction::transaction_non_typed::TransactionNonTyped;
use crate::transaction::user_operation::UserOperation;
use crate::transaction::user_operation_v0_7::UserOperationV0_7;
use crate::transaction::UnsignedTransactionBox;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;
use tw_number::U256;
use tw_proto::Common::Proto::SigningError as CommonError;
use tw_proto::Ethereum::Proto;
use Proto::mod_SigningInput::OneOfuser_operation_oneof as UserOp;
use Proto::mod_Transaction::OneOftransaction_oneof as Tx;
use Proto::SCAccountType;
use Proto::TransactionMode as TxMode;

pub struct TxBuilder<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> TxBuilder<Context> {
    pub fn tx_from_proto(
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<Box<dyn UnsignedTransactionBox>> {
        let Some(ref transaction) = input.transaction else {
            return SigningError::err(CommonError::Error_invalid_params)
                .context("No transaction specified");
        };

        let (eth_amount, payload, to) = match transaction.transaction_oneof {
            Tx::transfer(ref transfer) => {
                let amount = U256::from_big_endian_slice(&transfer.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let to_address = Self::parse_address(&input.to_address)
                    .context("Invalid destination address")?;

                (amount, transfer.data.to_vec(), Some(to_address))
            },
            Tx::erc20_transfer(ref erc20_transfer) => {
                let token_to_address = Self::parse_address(&erc20_transfer.to)
                    .context("Invalid destination address")?;

                let token_amount = U256::from_big_endian_slice(&erc20_transfer.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let contract_address =
                    Self::parse_address(&input.to_address).context("Invalid Contract address")?;

                let payload = Erc20::transfer(token_to_address, token_amount)
                    .map_err(abi_to_signing_error)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::erc20_approve(ref erc20_approve) => {
                let spender = Self::parse_address(&erc20_approve.spender)
                    .context("Invalid sender address")?;

                let token_amount = U256::from_big_endian_slice(&erc20_approve.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let contract_address =
                    Self::parse_address(&input.to_address).context("Invalid Contract address")?;

                let payload =
                    Erc20::approve(spender, token_amount).map_err(abi_to_signing_error)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::erc721_transfer(ref erc721_transfer) => {
                let from =
                    Self::parse_address(&erc721_transfer.from).context("Invalid sender address")?;
                let token_to_address = Self::parse_address(&erc721_transfer.to)
                    .context("Invalid destination address")?;

                let token_id = U256::from_big_endian_slice(&erc721_transfer.token_id)
                    .into_tw()
                    .context("Invalid token ID")?;

                let contract_address =
                    Self::parse_address(&input.to_address).context("Invalid Contract address")?;

                let payload = Erc721::encode_transfer_from(from, token_to_address, token_id)
                    .map_err(abi_to_signing_error)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::erc1155_transfer(ref erc1155_transfer) => {
                let from = Self::parse_address(&erc1155_transfer.from)
                    .context("Invalid sender address")?;

                let to = Self::parse_address(&erc1155_transfer.to)
                    .context("Invalid destination address")?;

                let token_id = U256::from_big_endian_slice(&erc1155_transfer.token_id)
                    .into_tw()
                    .context("Invalid token ID")?;

                let value = U256::from_big_endian_slice(&erc1155_transfer.value)
                    .into_tw()
                    .context("Invalid value")?;

                let data = erc1155_transfer.data.to_vec();
                let contract_address =
                    Self::parse_address(&input.to_address).context("Invalid Contract address")?;

                let payload = Erc1155::encode_safe_transfer_from(from, to, token_id, value, data)
                    .map_err(abi_to_signing_error)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::contract_generic(ref contract_generic) => {
                let amount = U256::from_big_endian_slice(&contract_generic.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let payload = contract_generic.data.to_vec();
                // `to_address` can be omitted for the generic contract call.
                // For example, on creating a new smart contract.
                let to_address = Self::parse_address_optional(&input.to_address)
                    .context("Invalid destination address")?;

                (amount, payload, to_address)
            },
            Tx::batch(ref batch) => {
                // Payload should match ERC4337 standard.
                let calls: Vec<_> = batch
                    .calls
                    .iter()
                    .map(Self::erc4337_execute_call_from_proto)
                    .collect::<Result<Vec<_>, _>>()?;
                let execute_payload = Self::encode_execute_batch(input.user_operation_mode, calls)?;

                return match input.tx_mode {
                    TxMode::UserOp => Self::user_operation_from_proto(input, execute_payload),
                    TxMode::SetCode => {
                        Self::transaction_eip7702_from_proto(input, U256::zero(), execute_payload)
                    },
                    _ => SigningError::err(SigningErrorType::Error_invalid_params).context(
                        "Transaction batch can be used in `UserOp` or `SetCode` modes only",
                    ),
                };
            },
            Tx::None => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("No transaction specified")
            },
        };

        let tx = match input.tx_mode {
            TxMode::Legacy => {
                Self::transaction_non_typed_from_proto(input, eth_amount, payload, to)?.into_boxed()
            },
            TxMode::Enveloped => {
                Self::transaction_eip1559_from_proto(input, eth_amount, payload, to)?.into_boxed()
            },
            TxMode::UserOp => {
                let to = to
                    .or_tw_err(SigningErrorType::Error_invalid_address)
                    .context("No contract/destination address specified")?;
                let args = ExecuteArgs {
                    to,
                    value: eth_amount,
                    data: payload,
                };

                let user_op_payload = Self::encode_execute(input.user_operation_mode, args)?;
                Self::user_operation_from_proto(input, user_op_payload)?
            },
            TxMode::SetCode => {
                let to = to
                    .or_tw_err(SigningErrorType::Error_invalid_address)
                    .context("No contract/destination address specified")?;
                let args = ExecuteArgs {
                    to,
                    value: eth_amount,
                    data: payload,
                };

                let execute_payload = Self::encode_execute(input.user_operation_mode, args)?;
                Self::transaction_eip7702_from_proto(input, eth_amount, execute_payload)?
            },
        };
        Ok(tx)
    }

    #[inline]
    fn user_operation_from_proto(
        input: &Proto::SigningInput<'_>,
        user_op_payload: Data,
    ) -> SigningResult<Box<dyn UnsignedTransactionBox>> {
        match input.user_operation_oneof {
            UserOp::user_operation_v0_7(ref user_op_v0_7) => {
                Self::user_operation_v0_7_from_proto(input, user_op_v0_7, user_op_payload)
                    .map(UserOperationV0_7::into_boxed)
            },
            UserOp::user_operation(ref user_op) => {
                Self::user_operation_v0_6_from_proto(input, user_op, user_op_payload)
                    .map(UserOperation::into_boxed)
            },
            UserOp::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No user operation specified"),
        }
    }

    #[inline]
    fn erc4337_execute_call_from_proto(
        call: &Proto::mod_Transaction::mod_Batch::BatchedCall,
    ) -> SigningResult<ExecuteArgs> {
        let to = Self::parse_address(&call.address)
            .context("Invalid 'BatchedCall' destination address")?;
        let value = U256::from_big_endian_slice(&call.amount)
            .into_tw()
            .context("Invalid 'BatchedCall' amount")?;
        Ok(ExecuteArgs {
            to,
            value,
            data: call.payload.to_vec(),
        })
    }

    #[inline]
    fn transaction_non_typed_from_proto(
        input: &Proto::SigningInput,
        eth_amount: U256,
        payload: Data,
        to_address: Option<Address>,
    ) -> SigningResult<TransactionNonTyped> {
        let nonce = U256::from_big_endian_slice(&input.nonce)
            .into_tw()
            .context("Invalid nonce")?;

        let gas_price = U256::from_big_endian_slice(&input.gas_price)
            .into_tw()
            .context("Invalid gas price")?;

        let gas_limit = U256::from_big_endian_slice(&input.gas_limit)
            .into_tw()
            .context("Invalid gas limit")?;

        Ok(TransactionNonTyped {
            nonce,
            gas_price,
            gas_limit,
            to: to_address,
            amount: eth_amount,
            payload,
        })
    }

    #[inline]
    fn transaction_eip1559_from_proto(
        input: &Proto::SigningInput,
        eth_amount: U256,
        payload: Data,
        to_address: Option<Address>,
    ) -> SigningResult<TransactionEip1559> {
        let nonce = U256::from_big_endian_slice(&input.nonce)
            .into_tw()
            .context("Invalid nonce")?;

        let gas_limit = U256::from_big_endian_slice(&input.gas_limit)
            .into_tw()
            .context("Invalid gas limit")?;

        let max_inclusion_fee_per_gas =
            U256::from_big_endian_slice(&input.max_inclusion_fee_per_gas)
                .into_tw()
                .context("Invalid max inclusion fee per gas")?;

        let max_fee_per_gas = U256::from_big_endian_slice(&input.max_fee_per_gas)
            .into_tw()
            .context("Invalid max fee per gas")?;

        let access_list =
            Self::parse_access_list(&input.access_list).context("Invalid access list")?;

        Ok(TransactionEip1559 {
            nonce,
            max_inclusion_fee_per_gas,
            max_fee_per_gas,
            gas_limit,
            to: to_address,
            amount: eth_amount,
            payload,
            access_list,
        })
    }

    #[inline]
    fn transaction_eip7702_from_proto(
        input: &Proto::SigningInput,
        eth_amount: U256,
        payload: Data,
    ) -> SigningResult<Box<dyn UnsignedTransactionBox>> {
        let signer_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())
            .into_tw()
            .context("Sender's private key must be provided to generate an EIP-7702 transaction")?;
        let signer = Address::with_secp256k1_pubkey(&signer_key.public());

        let nonce = U256::from_big_endian_slice(&input.nonce)
            .into_tw()
            .context("Invalid nonce")?;

        let gas_limit = U256::from_big_endian_slice(&input.gas_limit)
            .into_tw()
            .context("Invalid gas limit")?;

        let max_inclusion_fee_per_gas =
            U256::from_big_endian_slice(&input.max_inclusion_fee_per_gas)
                .into_tw()
                .context("Invalid max inclusion fee per gas")?;

        let max_fee_per_gas = U256::from_big_endian_slice(&input.max_fee_per_gas)
            .into_tw()
            .context("Invalid max fee per gas")?;

        let access_list =
            Self::parse_access_list(&input.access_list).context("Invalid access list")?;

        let authority: Address = input
            .eip7702_authority
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("'eip7702Authority' must be provided for `SetCode` transaction")?
            .address
            // Parse `Address`
            .parse()
            .into_tw()
            .context("Invalid authority address")?;

        let chain_id = U256::from_big_endian_slice(&input.chain_id)
            .into_tw()
            .context("Invalid chain ID")?;

        let authorization = Authorization {
            chain_id,
            address: authority,
            // `authorization.nonce` must be incremented by 1 over `transaction.nonce`.
            nonce: nonce + 1,
        };
        let signed_authorization = AuthorizationSigner::sign(&signer_key, authorization)?;
        let authorization_list = AuthorizationList::from(vec![signed_authorization]);

        Ok(TransactionEip7702 {
            nonce,
            max_inclusion_fee_per_gas,
            max_fee_per_gas,
            gas_limit,
            // EIP-7702 transaction calls a smart contract function of the authorized address.
            to: Some(signer),
            amount: eth_amount,
            payload,
            access_list,
            authorization_list,
        }
        .into_boxed())
    }

    fn user_operation_v0_6_from_proto(
        input: &Proto::SigningInput,
        user_op: &Proto::UserOperation,
        erc4337_payload: Data,
    ) -> SigningResult<UserOperation> {
        let nonce = U256::from_big_endian_slice(&input.nonce)
            .into_tw()
            .context("Invalid nonce")?;

        let gas_limit = U256::from_big_endian_slice(&input.gas_limit)
            .into_tw()
            .context("Invalid gas limit")?;

        let max_inclusion_fee_per_gas =
            U256::from_big_endian_slice(&input.max_inclusion_fee_per_gas)
                .into_tw()
                .context("Invalid max inclusion fee per gas")?;

        let max_fee_per_gas = U256::from_big_endian_slice(&input.max_fee_per_gas)
            .into_tw()
            .context("Invalid max fee per gas")?;

        let entry_point =
            Self::parse_address(user_op.entry_point.as_ref()).context("Invalid entry point")?;

        let sender = Self::parse_address(user_op.sender.as_ref())
            .context("Invalid User Operation sender")?;

        let verification_gas_limit = U256::from_big_endian_slice(&user_op.verification_gas_limit)
            .into_tw()
            .context("Invalid verification gas limit")?;

        let pre_verification_gas = U256::from_big_endian_slice(&user_op.pre_verification_gas)
            .into_tw()
            .context("Invalid pre-verification gas")?;

        Ok(UserOperation {
            nonce,
            entry_point,
            sender,
            init_code: user_op.init_code.to_vec(),
            gas_limit,
            verification_gas_limit,
            max_fee_per_gas,
            max_inclusion_fee_per_gas,
            pre_verification_gas,
            paymaster_and_data: user_op.paymaster_and_data.to_vec(),
            payload: erc4337_payload,
        })
    }

    fn user_operation_v0_7_from_proto(
        input: &Proto::SigningInput,
        user_op_v0_7: &Proto::UserOperationV0_7,
        erc4337_payload: Data,
    ) -> SigningResult<UserOperationV0_7> {
        let sender = Self::parse_address(user_op_v0_7.sender.as_ref())
            .context("Invalid User Operation sender")?;

        let nonce = U256::from_big_endian_slice(&input.nonce)
            .into_tw()
            .context("Invalid nonce")?;

        let factory = Self::parse_address_optional(user_op_v0_7.factory.as_ref())
            .context("Invalid factory address")?;

        let call_data_gas_limit = U256::from_big_endian_slice(&input.gas_limit)
            .into_tw()
            .context("Invalid gas limit")?
            .try_into()
            .into_tw()
            .context("Gas limit exceeds u128")?;

        let verification_gas_limit =
            U256::from_big_endian_slice(&user_op_v0_7.verification_gas_limit)
                .into_tw()
                .context("Invalid verification gas limit")?
                .try_into()
                .into_tw()
                .context("Verification gas limit exceeds u128")?;

        let pre_verification_gas = U256::from_big_endian_slice(&user_op_v0_7.pre_verification_gas)
            .into_tw()
            .context("Invalid pre-verification gas")?;

        let max_fee_per_gas = U256::from_big_endian_slice(&input.max_fee_per_gas)
            .into_tw()
            .context("Invalid max fee per gas")?
            .try_into()
            .into_tw()
            .context("Max fee per gas exceeds u128")?;

        let max_priority_fee_per_gas =
            U256::from_big_endian_slice(&input.max_inclusion_fee_per_gas)
                .into_tw()
                .context("Invalid max inclusion fee per gas")?
                .try_into()
                .into_tw()
                .context("Max inclusion fee per gas exceeds u128")?;

        let paymaster = Self::parse_address_optional(user_op_v0_7.paymaster.as_ref())
            .context("Invalid paymaster address")?;

        let paymaster_verification_gas_limit =
            U256::from_big_endian_slice(&user_op_v0_7.paymaster_verification_gas_limit)
                .into_tw()
                .context("Invalid paymaster verification gas limit")?
                .try_into()
                .into_tw()
                .context("Paymaster verification gas limit exceeds u128")?;

        let paymaster_post_op_gas_limit =
            U256::from_big_endian_slice(&user_op_v0_7.paymaster_post_op_gas_limit)
                .into_tw()
                .context("Invalid paymaster post-op gas limit")?
                .try_into()
                .into_tw()
                .context("Paymaster post-op gas limit exceeds u128")?;

        let entry_point = Self::parse_address(user_op_v0_7.entry_point.as_ref())
            .context("Invalid entry point")?;

        Ok(UserOperationV0_7 {
            sender,
            nonce,
            factory,
            factory_data: user_op_v0_7.factory_data.to_vec(),
            call_data: erc4337_payload,
            call_data_gas_limit,
            verification_gas_limit,
            pre_verification_gas,
            max_fee_per_gas,
            max_priority_fee_per_gas,
            paymaster,
            paymaster_verification_gas_limit,
            paymaster_post_op_gas_limit,
            paymaster_data: user_op_v0_7.paymaster_data.to_vec(),
            entry_point,
        })
    }

    #[inline]
    fn encode_execute(account_type: SCAccountType, args: ExecuteArgs) -> SigningResult<Data> {
        match account_type {
            SCAccountType::SimpleAccount => Erc4337SimpleAccount::encode_execute(args),
            SCAccountType::Biz4337 => BizAccount::encode_execute_4337_op(args),
            SCAccountType::Biz => BizAccount::encode_execute(args),
        }
        .map_err(abi_to_signing_error)
    }

    #[inline]
    fn encode_execute_batch(
        account_type: SCAccountType,
        calls: Vec<ExecuteArgs>,
    ) -> SigningResult<Data> {
        match account_type {
            SCAccountType::SimpleAccount => Erc4337SimpleAccount::encode_execute_batch(calls),
            SCAccountType::Biz4337 => BizAccount::encode_execute_4337_ops(calls),
            SCAccountType::Biz => BizAccount::encode_execute_batch(calls),
        }
        .map_err(abi_to_signing_error)
    }

    fn parse_address(addr: &str) -> SigningResult<Address> {
        Context::Address::from_str(addr)
            .map(Context::Address::into)
            .map_err(SigningError::from)
    }

    fn parse_address_optional(addr: &str) -> SigningResult<Option<Address>> {
        match Context::Address::from_str_optional(addr) {
            Ok(Some(addr)) => Ok(Some(addr.into())),
            Ok(None) => Ok(None),
            Err(e) => Err(SigningError::from(e)),
        }
    }

    fn parse_access_list(list_proto: &[Proto::Access]) -> SigningResult<AccessList> {
        let mut access_list = AccessList::default();
        for access_proto in list_proto.iter() {
            access_list.add_access(Self::parse_access(access_proto)?);
        }
        Ok(access_list)
    }

    fn parse_access(access_proto: &Proto::Access) -> SigningResult<Access> {
        let addr =
            Self::parse_address(access_proto.address.as_ref()).context("Invalid access address")?;

        let mut access = Access::new(addr);
        for key_proto in access_proto.stored_keys.iter() {
            let storage_key = H256::try_from(key_proto.as_ref())
                .tw_err(SigningErrorType::Error_invalid_params)
                .context("Invalid storage key")?;
            access.add_storage_key(storage_key);
        }
        Ok(access)
    }
}
