// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::prebuild::erc1155::Erc1155;
use crate::abi::prebuild::erc20::Erc20;
use crate::abi::prebuild::erc4337::{Erc4337SimpleAccount, ExecuteArgs};
use crate::abi::prebuild::erc721::Erc721;
use crate::address::Address;
use crate::transaction::transaction_eip1559::TransactionEip1559;
use crate::transaction::transaction_non_typed::TransactionNonTyped;
use crate::transaction::user_operation::UserOperation;
use crate::transaction::UnsignedTransactionBox;
use std::str::FromStr;
use tw_coin_entry::{SigningError, SigningErrorType, SigningResult};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;
use tw_proto::Common::Proto::SigningError as CommonError;
use tw_proto::Ethereum::Proto;

pub struct Signer;

impl Signer {
    pub fn sign_proto(input: Proto::SigningInput<'_>) -> Proto::SigningOutput<'_> {
        match Signer::sign_proto_impl(input) {
            Ok(output) => output,
            Err(error) => Proto::SigningOutput {
                error: error.0,
                error_message: error.to_string().into(),
                ..Proto::SigningOutput::default()
            },
        }
    }

    fn sign_proto_impl(input: Proto::SigningInput<'_>) -> SigningResult<Proto::SigningOutput<'_>> {
        let chain_id = U256::from_big_endian_slice(&input.chain_id)?;
        let private_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())?;

        let unsigned = Self::transaction_from_proto(&input)?;

        let pre_hash = unsigned.pre_hash(chain_id);
        let signature = private_key.sign(pre_hash)?;

        let signed = unsigned.into_signed(signature, chain_id);

        let eth_signature = signed.signature();

        Ok(Proto::SigningOutput {
            encoded: signed.encode().into(),
            v: eth_signature.v().to_big_endian_compact().into(),
            r: eth_signature.r().to_big_endian_compact().into(),
            s: eth_signature.s().to_big_endian_compact().into(),
            data: signed.payload().into(),
            pre_hash: pre_hash.to_vec().into(),
            ..Proto::SigningOutput::default()
        })
    }

    fn transaction_from_proto(
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<Box<dyn UnsignedTransactionBox>> {
        use Proto::mod_Transaction::OneOftransaction_oneof as Tx;
        use Proto::TransactionMode as TxMode;

        let Some(ref transaction) = input.transaction else {
            return Err(SigningError(CommonError::Error_invalid_params));
        };

        let (eth_amount, payload, to) = match transaction.transaction_oneof {
            Tx::transfer(ref transfer) => {
                let amount = U256::from_big_endian_slice(&transfer.amount)?;
                let to_address = Address::from_str(&input.to_address)?;
                (amount, transfer.data.to_vec(), Some(to_address))
            },
            Tx::erc20_transfer(ref erc20_transfer) => {
                let token_to_address = Address::from_str(&erc20_transfer.to)?;
                let token_amount = U256::from_big_endian_slice(&erc20_transfer.amount)?;
                let contract_address = Address::from_str(&input.to_address)?;

                let payload = Erc20::transfer(token_to_address, token_amount)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::erc20_approve(ref erc20_approve) => {
                let spender = Address::from_str(&erc20_approve.spender)?;
                let token_amount = U256::from_big_endian_slice(&erc20_approve.amount)?;
                let contract_address = Address::from_str(&input.to_address)?;

                let payload = Erc20::approve(spender, token_amount)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::erc721_transfer(ref erc721_transfer) => {
                let from = Address::from_str(&erc721_transfer.from)?;
                let token_to_address = Address::from_str(&erc721_transfer.to)?;
                let token_id = U256::from_big_endian_slice(&erc721_transfer.token_id)?;
                let contract_address = Address::from_str(&input.to_address)?;

                let payload = Erc721::encode_transfer_from(from, token_to_address, token_id)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::erc1155_transfer(ref erc1155_transfer) => {
                let from = Address::from_str(&erc1155_transfer.from)?;
                let to = Address::from_str(&erc1155_transfer.to)?;
                let token_id = U256::from_big_endian_slice(&erc1155_transfer.token_id)?;
                let value = U256::from_big_endian_slice(&erc1155_transfer.value)?;
                let data = erc1155_transfer.data.to_vec();
                let contract_address = Address::from_str(&input.to_address)?;

                let payload = Erc1155::encode_safe_transfer_from(from, to, token_id, value, data)?;
                (U256::zero(), payload, Some(contract_address))
            },
            Tx::contract_generic(ref contract_generic) => {
                let amount = U256::from_big_endian_slice(&contract_generic.amount)?;
                let payload = contract_generic.data.to_vec();
                // `to_address` can be omitted for the generic contract call.
                // For example, on creating a new smart contract.
                let to_address = Address::from_str_optional(&input.to_address)?;

                (amount, payload, to_address)
            },
            Tx::batch(ref batch) => {
                if input.tx_mode != TxMode::UserOp {
                    return Err(SigningError(SigningErrorType::Error_invalid_params));
                }

                // Payload should match ERC4337 standard.
                let calls: Vec<_> = batch
                    .calls
                    .iter()
                    .map(erc4337_execute_call_from_proto)
                    .collect::<Result<Vec<_>, _>>()?;
                let payload = Erc4337SimpleAccount::encode_execute_batch(calls)?;

                return user_operation_from_proto(input, payload).map(UserOperation::into_boxed);
            },
            Tx::None => return Err(SigningError(SigningErrorType::Error_invalid_params)),
        };

        let tx = match input.tx_mode {
            TxMode::Legacy => {
                transaction_non_typed_from_proto(input, eth_amount, payload, to)?.into_boxed()
            },
            TxMode::Enveloped => {
                transaction_eip1559_from_proto(input, eth_amount, payload, to)?.into_boxed()
            },
            TxMode::UserOp => {
                let to = to.ok_or(SigningError(SigningErrorType::Error_invalid_address))?;
                // Payload should match the ERC4337 standard.
                let payload = Erc4337SimpleAccount::encode_execute(ExecuteArgs {
                    to,
                    value: eth_amount,
                    data: payload,
                })?;

                user_operation_from_proto(input, payload)?.into_boxed()
            },
        };
        Ok(tx)
    }
}

fn erc4337_execute_call_from_proto(
    call: &Proto::mod_Transaction::mod_Batch::BatchedCall,
) -> SigningResult<ExecuteArgs> {
    let to = Address::from_str(&call.address)?;
    let value = U256::from_big_endian_slice(&call.amount)?;
    Ok(ExecuteArgs {
        to,
        value,
        data: call.payload.to_vec(),
    })
}

fn transaction_non_typed_from_proto(
    input: &Proto::SigningInput,
    eth_amount: U256,
    payload: Vec<u8>,
    to_address: Option<Address>,
) -> SigningResult<TransactionNonTyped> {
    let nonce = U256::from_big_endian_slice(&input.nonce)?;
    let gas_price = U256::from_big_endian_slice(&input.gas_price)?;
    let gas_limit = U256::from_big_endian_slice(&input.gas_limit)?;

    Ok(TransactionNonTyped {
        nonce,
        gas_price,
        gas_limit,
        to: to_address,
        amount: eth_amount,
        payload,
    })
}

fn transaction_eip1559_from_proto(
    input: &Proto::SigningInput,
    eth_amount: U256,
    payload: Vec<u8>,
    to_address: Option<Address>,
) -> SigningResult<TransactionEip1559> {
    let nonce = U256::from_big_endian_slice(&input.nonce)?;
    let gas_limit = U256::from_big_endian_slice(&input.gas_limit)?;
    let max_inclusion_fee_per_gas = U256::from_big_endian_slice(&input.max_inclusion_fee_per_gas)?;
    let max_fee_per_gas = U256::from_big_endian_slice(&input.max_fee_per_gas)?;

    Ok(TransactionEip1559 {
        nonce,
        max_inclusion_fee_per_gas,
        max_fee_per_gas,
        gas_limit,
        to: to_address,
        amount: eth_amount,
        payload,
    })
}

fn user_operation_from_proto(
    input: &Proto::SigningInput,
    erc4337_payload: Vec<u8>,
) -> SigningResult<UserOperation> {
    let Some(ref user_op) = input.user_operation else {
        return Err(SigningError(CommonError::Error_invalid_params))
    };

    let nonce = U256::from_big_endian_slice(&input.nonce)?;
    let gas_limit = U256::from_big_endian_slice(&input.gas_limit)?;
    let max_inclusion_fee_per_gas = U256::from_big_endian_slice(&input.max_inclusion_fee_per_gas)?;
    let max_fee_per_gas = U256::from_big_endian_slice(&input.max_fee_per_gas)?;

    let entry_point = Address::from_str(user_op.entry_point.as_ref())?;
    let sender = Address::from_str(user_op.sender.as_ref())?;
    let verification_gas_limit = U256::from_big_endian_slice(&user_op.verification_gas_limit)?;
    let pre_verification_gas = U256::from_big_endian_slice(&user_op.pre_verification_gas)?;

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
