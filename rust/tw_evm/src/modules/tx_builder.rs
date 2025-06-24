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
use crate::message::{to_signing, EthMessage};
use crate::transaction::access_list::{Access, AccessList};
use crate::transaction::authorization_list::{
    Authorization, AuthorizationList, SignedAuthorization,
};
use crate::transaction::transaction_eip1559::TransactionEip1559;
use crate::transaction::transaction_eip7702::TransactionEip7702;
use crate::transaction::transaction_non_typed::TransactionNonTyped;
use crate::transaction::user_operation::UserOperation;
use crate::transaction::user_operation_v0_7::UserOperationV0_7;
use crate::transaction::UnsignedTransactionBox;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::DecodeHex;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::ecdsa::secp256k1::Signature;
use tw_keypair::traits::SigningKeyTrait;
use tw_keypair::KeyPairError;
use tw_memory::Data;
use tw_number::U256;
use tw_proto::Common::Proto::SigningError as CommonError;
use tw_proto::Ethereum::Proto;
use Proto::mod_SigningInput::OneOfuser_operation_oneof as UserOp;
use Proto::mod_Transaction::OneOftransaction_oneof as Tx;
use Proto::SCWalletType;
use Proto::TransactionMode as TxMode;

pub struct TransactionParts {
    pub eth_amount: U256,
    pub data: Data,
    pub to: Option<Address>,
}

impl TryFrom<TransactionParts> for ExecuteArgs {
    type Error = SigningError;

    fn try_from(value: TransactionParts) -> Result<Self, Self::Error> {
        let to = value
            .to
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("Error creating a Smart Contract Wallet execute call - no destination address specified")?;
        Ok(ExecuteArgs {
            to,
            value: value.eth_amount,
            data: value.data,
        })
    }
}

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

        let TransactionParts {
            eth_amount,
            data,
            to,
        } = Self::handle_transaction_type(input, &transaction.transaction_oneof)?;

        let tx = match input.tx_mode {
            TxMode::Legacy => {
                Self::transaction_non_typed_from_proto(input, eth_amount, data, to)?.into_boxed()
            },
            TxMode::Enveloped => {
                Self::transaction_eip1559_from_proto(input, eth_amount, data, to)?.into_boxed()
            },
            TxMode::UserOp => Self::user_operation_from_proto(input, data)?,
            TxMode::SetCode => Self::transaction_eip7702_from_proto(input, eth_amount, data, to)?,
        };
        Ok(tx)
    }

    fn handle_transaction_type(
        input: &Proto::SigningInput,
        transaction: &Tx,
    ) -> SigningResult<TransactionParts> {
        match transaction {
            Tx::transfer(ref transfer) => {
                let eth_amount = U256::from_big_endian_slice(&transfer.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let to_address = Self::parse_address(&input.to_address)
                    .context("Invalid destination address")?;

                Ok(TransactionParts {
                    eth_amount,
                    data: transfer.data.to_vec(),
                    to: Some(to_address),
                })
            },
            Tx::erc20_transfer(ref erc20_transfer) => {
                let token_to_address = Self::parse_address(&erc20_transfer.to)
                    .context("Invalid destination address")?;

                let token_amount = U256::from_big_endian_slice(&erc20_transfer.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let contract_address =
                    Self::parse_address(&input.to_address).context("Invalid Contract address")?;

                let data = Erc20::transfer(token_to_address, token_amount)
                    .map_err(abi_to_signing_error)?;

                Ok(TransactionParts {
                    eth_amount: U256::zero(),
                    data,
                    to: Some(contract_address),
                })
            },
            Tx::erc20_approve(ref erc20_approve) => {
                let spender = Self::parse_address(&erc20_approve.spender)
                    .context("Invalid sender address")?;

                let token_amount = U256::from_big_endian_slice(&erc20_approve.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let contract_address =
                    Self::parse_address(&input.to_address).context("Invalid Contract address")?;

                let data = Erc20::approve(spender, token_amount).map_err(abi_to_signing_error)?;

                Ok(TransactionParts {
                    eth_amount: U256::zero(),
                    data,
                    to: Some(contract_address),
                })
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

                let data = Erc721::encode_transfer_from(from, token_to_address, token_id)
                    .map_err(abi_to_signing_error)?;

                Ok(TransactionParts {
                    eth_amount: U256::zero(),
                    data,
                    to: Some(contract_address),
                })
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

                let data = Erc1155::encode_safe_transfer_from(from, to, token_id, value, data)
                    .map_err(abi_to_signing_error)?;

                Ok(TransactionParts {
                    eth_amount: U256::zero(),
                    data,
                    to: Some(contract_address),
                })
            },
            Tx::contract_generic(ref contract_generic) => {
                let eth_amount = U256::from_big_endian_slice(&contract_generic.amount)
                    .into_tw()
                    .context("Invalid amount")?;

                let data = contract_generic.data.to_vec();
                // `to_address` can be omitted for the generic contract call.
                // For example, on creating a new smart contract.
                let to = Self::parse_address_optional(&input.to_address)
                    .context("Invalid destination address")?;

                Ok(TransactionParts {
                    eth_amount,
                    data,
                    to,
                })
            },
            Tx::scw_batch(ref batch) => {
                // Payload should match ERC4337 standard.
                let calls: Vec<_> = batch
                    .calls
                    .iter()
                    .map(Self::erc4337_execute_call_from_proto)
                    .collect::<Result<Vec<_>, _>>()?;
                let execute_payload = Self::encode_execute_batch(batch.wallet_type, calls)?;
                let to = Self::sc_tx_destination(input, batch.wallet_type)?;

                Ok(TransactionParts {
                    eth_amount: U256::zero(),
                    data: execute_payload,
                    to,
                })
            },
            Tx::scw_execute(ref execute) => {
                let inner_transaction = execute
                    .transaction
                    .as_ref()
                    .or_tw_err(SigningErrorType::Error_invalid_params)
                    .context("`Execute.transaction` must be provided")?;

                let execute_args =
                    Self::handle_transaction_type(input, &inner_transaction.transaction_oneof)?
                        .try_into()?;
                let execute_call_payload = Self::encode_execute(execute.wallet_type, execute_args)?;

                let to = Self::sc_tx_destination(input, execute.wallet_type)?;
                Ok(TransactionParts {
                    eth_amount: U256::zero(),
                    data: execute_call_payload,
                    to,
                })
            },
            Tx::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No transaction specified"),
        }
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
        call: &Proto::mod_Transaction::mod_SCWalletBatch::BatchedCall,
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
        to_address: Option<Address>,
    ) -> SigningResult<Box<dyn UnsignedTransactionBox>> {
        let nonce = U256::from_big_endian_slice(&input.nonce)
            .into_tw()
            .context("Invalid nonce")?;

        let gas_limit = U256::from_big_endian_slice(&input.gas_limit)
            .into_tw()
            .context("Invalid gas limit")?;

        let to = to_address
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("'to' must be provided for `SetCode` transaction")?;

        let max_inclusion_fee_per_gas =
            U256::from_big_endian_slice(&input.max_inclusion_fee_per_gas)
                .into_tw()
                .context("Invalid max inclusion fee per gas")?;

        let max_fee_per_gas = U256::from_big_endian_slice(&input.max_fee_per_gas)
            .into_tw()
            .context("Invalid max fee per gas")?;

        let access_list =
            Self::parse_access_list(&input.access_list).context("Invalid access list")?;

        let authorization_list = Self::build_authorization_list(input, to)?;

        Ok(TransactionEip7702 {
            nonce,
            max_inclusion_fee_per_gas,
            max_fee_per_gas,
            gas_limit,
            to,
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

        let call_gas_limit = U256::from_big_endian_slice(&input.gas_limit)
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

        let eip7702_auth = Self::build_authorization_list(input, sender)
            .ok()
            .and_then(|auth_list| auth_list.0.first().cloned());

        let entry_point = Self::parse_address(user_op_v0_7.entry_point.as_ref())
            .context("Invalid entry point")?;

        Ok(UserOperationV0_7 {
            sender,
            nonce,
            factory,
            factory_data: user_op_v0_7.factory_data.to_vec(),
            call_data: erc4337_payload,
            call_gas_limit,
            verification_gas_limit,
            pre_verification_gas,
            max_fee_per_gas,
            max_priority_fee_per_gas,
            paymaster,
            paymaster_verification_gas_limit,
            paymaster_post_op_gas_limit,
            paymaster_data: user_op_v0_7.paymaster_data.to_vec(),
            eip7702_auth,
            entry_point,
        })
    }

    /// Returns a destination address of the Smart Contract Wallet transaction.
    /// Returns:
    /// - `Ok(Some(address))` when generating a transaction calling a function of the account itself through EIP-7702 authorized code.
    /// - `Ok(None)` when generating a UserOperation.
    /// - `Err(e)` when the account type is not supported for the given transaction mode.
    #[inline]
    fn sc_tx_destination(
        input: &Proto::SigningInput,
        wallet_type: SCWalletType,
    ) -> SigningResult<Option<Address>> {
        match (input.tx_mode, wallet_type) {
            // Destination address is not used when generating UserOperation.
            (TxMode::UserOp, SCWalletType::SimpleAccount | SCWalletType::Biz4337) => Ok(None),
            (TxMode::UserOp, _) => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Biz account cannot be used in UserOperation flow"),
            (TxMode::Legacy | TxMode::Enveloped | TxMode::SetCode, SCWalletType::Biz) => {
                Self::signer_address(input).map(Some)
            },
            (TxMode::Legacy | TxMode::Enveloped | TxMode::SetCode, _) => SigningError::err(
                SigningErrorType::Error_invalid_params,
            )
            .context("Biz account can only be used in Legacy/Enveloped/SetCode transactions flow"),
        }
    }

    #[inline]
    fn encode_execute(wallet_type: SCWalletType, args: ExecuteArgs) -> SigningResult<Data> {
        match wallet_type {
            SCWalletType::SimpleAccount => Erc4337SimpleAccount::encode_execute(args),
            SCWalletType::Biz4337 => BizAccount::encode_execute_4337_op(args),
            SCWalletType::Biz => BizAccount::encode_execute(args),
        }
        .map_err(abi_to_signing_error)
    }

    #[inline]
    fn encode_execute_batch(
        wallet_type: SCWalletType,
        calls: Vec<ExecuteArgs>,
    ) -> SigningResult<Data> {
        match wallet_type {
            SCWalletType::SimpleAccount => Erc4337SimpleAccount::encode_execute_batch(calls),
            SCWalletType::Biz4337 => BizAccount::encode_execute_4337_ops(calls),
            SCWalletType::Biz => BizAccount::encode_execute_batch(calls),
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

    fn signer_address(input: &Proto::SigningInput) -> SigningResult<Address> {
        let signer_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())
            .into_tw()
            .context("Sender's private key must be provided to generate an EIP-7702 transaction")?;
        let signer = Address::with_secp256k1_pubkey(&signer_key.public());
        Ok(signer)
    }

    fn build_authorization_list(
        input: &Proto::SigningInput,
        destination: Address, // Field `destination` is only used for sanity check
    ) -> SigningResult<AuthorizationList> {
        let eip7702_authorization = input
            .eip7702_authorization
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("'eip7702Authorization' must be provided for `SetCode` transaction")?;

        let address = eip7702_authorization
            .address
            .parse()
            .into_tw()
            .context("Invalid authority address")?;

        let (authorization, signature) =
            if let Some(other_auth_fields) = &eip7702_authorization.custom_signature {
                // If field `custom_signature` is provided, it means that the authorization is already signed.
                let chain_id = U256::from_big_endian_slice(&other_auth_fields.chain_id)
                    .into_tw()
                    .context("Invalid chain ID")?;
                let nonce = U256::from_big_endian_slice(&other_auth_fields.nonce)
                    .into_tw()
                    .context("Invalid nonce")?;
                let signature = Signature::try_from(
                    other_auth_fields
                        .signature
                        .decode_hex()
                        .map_err(|_| KeyPairError::InvalidSignature)?
                        .as_slice(),
                )
                .tw_err(SigningErrorType::Error_invalid_params)
                .context("Invalid signature")?;

                (
                    Authorization {
                        chain_id,
                        address,
                        nonce,
                    },
                    signature,
                )
            } else {
                // If field `custom_signature` is not provided, the authorization will be signed with the provided private key, nonce and chainId
                let signer_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())
                    .into_tw()
                    .context(
                        "Sender's private key must be provided to generate an EIP-7702 transaction",
                    )?;
                let signer = Address::with_secp256k1_pubkey(&signer_key.public());
                if destination != signer {
                    return SigningError::err(SigningErrorType::Error_invalid_params).context(
                        "Unexpected 'destination'. Expected to be the same as the signer address",
                    );
                }

                let chain_id = U256::from_big_endian_slice(&input.chain_id)
                    .into_tw()
                    .context("Invalid chain ID")?;
                let nonce = U256::from_big_endian_slice(&input.nonce)
                    .into_tw()
                    .context("Invalid nonce")?;

                let authorization = Authorization {
                    chain_id,
                    address,
                    // `authorization.nonce` must be incremented by 1 over `transaction.nonce`.
                    nonce: nonce + 1,
                };

                let pre_hash = authorization.hash().map_err(to_signing)?;
                let signature = signer_key.sign(pre_hash)?;

                (authorization, signature)
            };

        let signed_authorization = SignedAuthorization {
            authorization,
            y_parity: signature.v(),
            r: U256::from_big_endian(signature.r()),
            s: U256::from_big_endian(signature.s()),
        };

        Ok(AuthorizationList::from(vec![signed_authorization]))
    }
}
