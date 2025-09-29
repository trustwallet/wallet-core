// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_keypair::tw::{PrivateKey, PublicKeyType};
use tw_memory::Data;
use tw_proto::Tezos::Proto::mod_Operation::OneOfoperation_data;
use tw_proto::Tezos::Proto::Operation;

use crate::binary_coding::decode_check;
use crate::forging::{forge_operation, ForgeError};

pub struct OperationList<'a> {
    branch: String,
    operation_list: Vec<Operation<'a>>,
}

impl<'a> OperationList<'a> {
    pub fn new(branch: String) -> Self {
        Self {
            branch,
            operation_list: Vec::new(),
        }
    }

    pub fn add_operation(&mut self, operation: Operation<'a>) {
        self.operation_list.push(operation);
    }

    // Forge the given branch to a hex encoded string.
    pub fn forge_branch(&self) -> Result<Data, ForgeError> {
        let prefix = [1, 52];
        let decoded = decode_check(&self.branch, tw_encoding::base58::Alphabet::Bitcoin)?;

        if decoded.len() != 34 || !prefix.iter().zip(decoded.iter()).all(|(a, b)| a == b) {
            return Err(ForgeError::InvalidBranch);
        }

        let mut forged = Data::new();
        forged.extend_from_slice(&decoded[prefix.len()..]);
        Ok(forged)
    }

    pub fn forge(&self, private_key: Option<&PrivateKey>) -> Result<Data, ForgeError> {
        let mut forged = self.forge_branch()?;

        for mut operation in self.operation_list.iter().cloned() {
            // If it's REVEAL operation, inject the public key if not specified
            if let OneOfoperation_data::reveal_operation_data(reveal_data) =
                &mut operation.operation_data
            {
                if reveal_data.public_key.is_empty() {
                    if let Some(key) = private_key {
                        reveal_data.public_key = key
                            .get_public_key_by_type(PublicKeyType::Ed25519)
                            .map_err(|_| ForgeError::InvalidPublicKey)?
                            .to_bytes()
                            .to_vec()
                            .into();
                    }
                }
            }

            forged.extend_from_slice(&forge_operation(&operation)?);
        }

        Ok(forged)
    }
}
