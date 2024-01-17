// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::eip712_types::{
    Eip712Domain, Eip712Fee, Eip712Transaction, Eip712TypedMsg, MsgPropertyName,
};
use crate::transaction::GreenfieldUnsignedTransaction;
use std::collections::BTreeMap;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_evm::message::eip712::eip712_message::Eip712Message;
use tw_evm::message::eip712::message_types::MessageTypesBuilder;
use tw_evm::message::EthMessage;
use tw_hash::H256;
use tw_number::U256;

pub struct Eip712TxPreimage {
    pub eip712_tx: String,
    pub tx_hash: H256,
}

pub struct Eip712Signer;

impl Eip712Signer {
    pub fn preimage_hash(
        unsigned: &GreenfieldUnsignedTransaction,
    ) -> SigningResult<Eip712TxPreimage> {
        // `types_builder` will be used to declare all custom types like `Tx`, `Fee`, `Msg1` etc.
        let mut types_builder = MessageTypesBuilder::default();

        // Step 1: Convert all [`TxBody::messages`] to a map of `msg1: GreenfieldTypedMsg`, `msg2: GreenfieldTypedMsg`.
        // at the same time, declare the message custom types.
        let mut msgs = BTreeMap::new();
        for (msg_idx, msg) in unsigned.tx_body.messages.iter().enumerate() {
            // Index of the transaction messages starts from 1.
            let msg_idx = msg_idx + 1;

            let property_name = MsgPropertyName(msg_idx);
            let property_value = Eip712TypedMsg::from(msg.to_json()?);

            msgs.insert(property_name, property_value);

            // Declare message custom types like `Msg1`, `TypeMsg1Amount`, etc.
            msg.declare_eip712_type(msg_idx, &mut types_builder);
        }

        // Step 2: Generate `Tx` and `Domain` objects - the main parts of the EIP712 message.
        let tx_to_sign = Eip712Transaction {
            account_number: U256::from(unsigned.account_number),
            chain_id: unsigned.eth_chain_id,
            fee: Eip712Fee::from(unsigned.fee.clone()),
            memo: unsigned.tx_body.memo.clone(),
            msgs,
            sequence: U256::from(unsigned.signer.sequence),
            timeout_height: U256::zero(),
        };
        let domain = Eip712Domain::new(unsigned.eth_chain_id);

        // Step 3: Declare `Tx`, `Domain` and all types they depend on.
        tx_to_sign.declare_eip712_types(&mut types_builder);
        domain.declare_eip712_types(&mut types_builder);

        // Step 4: Generate EIP712 message with all declared custom types, `Domain` and `Tx`,
        // and compute the EIP712 message hash.
        let msg_to_sign = Eip712Message {
            types: types_builder.build(),
            domain: serde_json::to_value(domain)
                .map_err(|_| SigningError(SigningErrorType::Error_internal))?,
            primary_type: Eip712Transaction::TYPE_NAME.to_string(),
            message: serde_json::to_value(tx_to_sign)
                .map_err(|_| SigningError(SigningErrorType::Error_internal))?,
        };

        let tx_hash = msg_to_sign.hash()?;
        let eip712_tx = serde_json::to_string(&msg_to_sign)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        Ok(Eip712TxPreimage { eip712_tx, tx_hash })
    }
}
