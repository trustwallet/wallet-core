// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::prebuild::erc20::Erc20;
use crate::address::Address;
use crate::transaction::transaction_eip1559::TransactionEip1559;
use crate::transaction::transaction_non_typed::TransactionNonTyped;
use crate::transaction::user_operation::UserOperation;
use crate::transaction::UnsignedTransactionBox;
use std::str::FromStr;
use tw_coin_entry::{SigningError, SigningResult};
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
        let chain_id = U256::from_little_endian_slice(&input.chain_id)?;
        let private_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())?;

        let unsigned = Self::transaction_from_proto(&input)?;

        let pre_hash = unsigned.pre_hash(chain_id);
        let signature = private_key.sign(pre_hash)?;

        let signed = unsigned.into_signed(signature, chain_id);

        let eth_signature = signed.signature();

        Ok(Proto::SigningOutput {
            encoded: signed.encode().into(),
            v: eth_signature.v().to_little_endian_compact().into(),
            r: eth_signature.r().to_little_endian_compact().into(),
            s: eth_signature.s().to_little_endian_compact().into(),
            data: signed.payload().into(),
            pre_hash: pre_hash.to_vec().into(),
            ..Proto::SigningOutput::default()
        })
    }

    fn transaction_from_proto(
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<Box<dyn UnsignedTransactionBox>> {
        let Some(ref transaction) = input.transaction else {
            return Err(SigningError(CommonError::Error_invalid_params));
        };

        let to = Address::from_str(&input.to_address)?;
        let nonce = U256::from_little_endian_slice(&input.nonce)?;
        let gas_price = U256::from_little_endian_slice(&input.gas_price)?;
        let gas_limit = U256::from_little_endian_slice(&input.gas_limit)?;
        let max_inclusion_fee_per_gas =
            U256::from_little_endian_slice(&input.max_inclusion_fee_per_gas)?;
        let max_fee_per_gas = U256::from_little_endian_slice(&input.max_fee_per_gas)?;

        use Proto::mod_Transaction::OneOftransaction_oneof as Tx;
        use Proto::TransactionMode as TxMode;

        let (amount, payload) = match transaction.transaction_oneof {
            Tx::transfer(ref transfer) => {
                let amount = U256::from_little_endian_slice(&transfer.amount)?;
                (amount, transfer.data.to_vec())
            },
            Tx::erc20_transfer(ref erc20_transfer) => {
                let token_to_address = Address::from_str(&erc20_transfer.to)?;
                let token_amount = U256::from_little_endian_slice(&erc20_transfer.amount)?;
                let payload = Erc20::transfer(token_to_address, token_amount)
                    .map_err(|_| SigningError(CommonError::Error_internal))?;
                (U256::zero(), payload)
            },
            Tx::erc20_approve(ref erc20_approve) => {
                let spender = Address::from_str(&erc20_approve.spender)?;
                let token_amount = U256::from_little_endian_slice(&erc20_approve.amount)?;
                let payload = Erc20::approve(spender, token_amount)
                    .map_err(|_| SigningError(CommonError::Error_internal))?;
                (U256::zero(), payload)
            },
            _ => todo!(),
        };

        let tx = match input.tx_mode {
            TxMode::Legacy => TransactionNonTyped {
                nonce,
                gas_price,
                gas_limit,
                to,
                amount,
                payload,
            }
            .into_boxed(),
            TxMode::Enveloped => TransactionEip1559 {
                nonce,
                max_inclusion_fee_per_gas,
                max_fee_per_gas,
                gas_limit,
                to,
                amount,
                payload,
            }
            .into_boxed(),
            TxMode::UserOp => {
                let Some(ref user_op) = input.user_operation else {
                    return Err(SigningError(CommonError::Error_invalid_params))
                };

                let entry_point = Address::from_str(user_op.entry_point.as_ref())?;
                let sender = Address::from_str(user_op.sender.as_ref())?;
                let verification_gas_limit =
                    U256::from_little_endian_slice(&user_op.verification_gas_limit)?;
                let pre_verification_gas =
                    U256::from_little_endian_slice(&user_op.pre_verification_gas)?;

                UserOperation {
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
                    payload,
                }
                .into_boxed()
            },
        };
        Ok(tx)
    }
}
