// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::SOLANA_ALPHABET;
use serde::{Deserialize, Serialize};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base58;
use tw_hash::{as_byte_sequence, H512};

pub mod legacy;
pub mod short_vec;
pub mod v0;
pub mod versioned;

#[derive(Serialize, Deserialize, Default, Debug, PartialEq, Eq, Clone, Copy)]
#[serde(rename_all = "camelCase")]
pub struct MessageHeader {
    /// The number of signatures required for this message to be considered
    /// valid. The signers of those signatures must match the first
    /// `num_required_signatures` of [`Message::account_keys`].
    // NOTE: Serialization-related changes must be paired with the direct read at sigverify.
    pub num_required_signatures: u8,

    /// The last `num_readonly_signed_accounts` of the signed keys are read-only
    /// accounts.
    pub num_readonly_signed_accounts: u8,

    /// The last `num_readonly_unsigned_accounts` of the unsigned keys are
    /// read-only accounts.
    pub num_readonly_unsigned_accounts: u8,
}

#[derive(Serialize, Deserialize, Debug, PartialEq, Eq, Clone)]
#[serde(rename_all = "camelCase")]
pub struct CompiledInstruction {
    /// Index into the transaction keys array indicating the program account that executes this instruction.
    pub program_id_index: u8,
    /// Ordered indices into the transaction keys array indicating which accounts to pass to the program.
    #[serde(with = "short_vec")]
    pub accounts: Vec<u8>,
    /// The program input data.
    #[serde(with = "short_vec")]
    pub data: Vec<u8>,
}

#[derive(Serialize, Deserialize, Debug, Default, Clone, Copy, Eq, PartialEq, Hash)]
pub struct Signature(#[serde(with = "as_byte_sequence")] pub H512);

impl FromStr for Signature {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let data = base58::decode(s, SOLANA_ALPHABET)
            .tw_err(|_| SigningErrorType::Error_input_parse)
            .context("Error decoding Solana Signature from base58")?;
        H512::try_from(data.as_slice())
            .map(Signature)
            .tw_err(|_| SigningErrorType::Error_input_parse)
            .context("Solana Signature must be 64 byte length")
    }
}

impl fmt::Display for Signature {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", base58::encode(self.0.as_slice(), SOLANA_ALPHABET))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::address::SolanaAddress;
    use crate::transaction::v0::MessageAddressTableLookup;
    use crate::transaction::versioned::{VersionedMessage, VersionedTransaction};
    use crate::SOLANA_ALPHABET;
    use tw_encoding::base58;
    use tw_encoding::base64::{self, STANDARD};
    use tw_encoding::hex::ToHex;
    use tw_hash::H256;
    use tw_memory::Data;

    fn base58_decode(s: &'static str) -> Data {
        base58::decode(s, SOLANA_ALPHABET).unwrap()
    }

    fn base58_decode_h256(s: &'static str) -> H256 {
        let bytes = base58::decode(s, SOLANA_ALPHABET).unwrap();
        H256::try_from(bytes.as_slice()).unwrap()
    }

    #[test]
    fn test_rango_transaction_ser_de() {
        let serialized = base64::decode("AQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAQAHEIoR5xuWyrvjIW4xU7CWlPOfyFAiy8B295hGo6tNjBmRCgUkQaFYTleMcAX2p74eBXQZd1dwDyQZAPJfSv2KGc5kcFLJj5qd2BVMaSNGVPfVBm74GbLwUq5/U1Ccdqc2gokZQxRDpMq7aeToP3nRaWIP4RXMxN+LJetccXMPq/QumgOqt7kkqk07cyPCKgYoQ4fQtOqqZn5sEqjWHYj3CDS5ha48uggePWu090s1ff4yoCjAvULeZ+cqYFn+Adk5Teyfw71W3u/F6VTnLQEPW96gJr5Kcm3bGi08n224JyF++PTko52VL0CIM2xtl0WkvNslD6Wawxr7yd9HYllN4Lz8lFwXilWGgyJdOq1qqBuZbE49glHeCO/sJHNnIHC0BgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwZGb+UhFzL/7K26csOb57yM5bvF9xJrLEObOkAAAAAEedVb8jHAbu50xW7OaBUH/bGy3qP0jlECsc2iVrwTjwbd9uHXZaGT2cvhRs7reawctIXtX1s3kTqM9YV+/wCpjJclj04kifG7PRApFI4NgwtaE5na/xCEBI572Nvp+Fm0P/on9df2SnTAmx8pWHneSwmrNt/J3VFLMhqns4zl6OL4d+g9rsaIj0Orta57MRu3jDSWCJf85ae4LBbiD/GXvOojZjsHekJrpRUuPggLJr943hDVD5UareeEucjCvaoHCgAFAsBcFQAKAAkDBBcBAAAAAAANBgAGACMJDAEBCQIABgwCAAAAAMqaOwAAAAAMAQYBEQs1DA8ABgEFAiMhCwsOCx0MDxoBGQcYBAgDJBscDB4PBwUQEhEfFR8UFwcFISITHw8MDCAfFgstwSCbM0HWnIEAAwAAABEBZAABCh0BAyZHAQMAypo7AAAAAJaWFAYAAAAAMgAADAMGAAABCQPZoILFk7gfE2y5bt3AC+g/4OwNzdiHKBhIbdeYvYFEjQPKyMkExMUkx0R25UNa/g5KsG0vfUwdUJ8e8HecK/Jkd3qm9XefBOB0BaD1+J+dBJz09vfyGuRYZH09HfdE/kL8v6Ql+H03+tO+9lMmmVg8O1c6gAN6eX0Cbn4=", STANDARD).unwrap();
        let actual: VersionedTransaction = bincode::deserialize(&serialized).unwrap();

        let expected = VersionedTransaction {
            signatures: vec![Signature(H512::default())],
            message: VersionedMessage::V0(v0::Message {
                header: MessageHeader {
                    num_required_signatures: 1,
                    num_readonly_signed_accounts: 0,
                    num_readonly_unsigned_accounts: 7,
                },
                account_keys: vec![
                    SolanaAddress::from("AHy6YZA8BsHgQfVkk7MbwpAN94iyN7Nf1zN4nPqUN32Q"),
                    SolanaAddress::from("g7dD1FHSemkUQrX1Eak37wzvDjscgBW2pFCENwjLdMX"),
                    SolanaAddress::from("7m57LBTxtzhWn6WdFxKtnoJLBQXyNERLYebebXLVaKy3"),
                    SolanaAddress::from("AEBCPtV8FFkWFAKxrz7mbYvobpkZuWaRWQCyJVRaheUD"),
                    SolanaAddress::from("BND2ehwWVeHVA5EtMm2b7Vu51AT8f2PNWusS9KQX5moy"),
                    SolanaAddress::from("DVCeozFGbe6ew3eWTnZByjHeYqTq1cvbrB7JJhkLxaRJ"),
                    SolanaAddress::from("GvgWmk8iPACw1AEMt47WzkuTkKoSGbn4Xk3aLM8vdbJD"),
                    SolanaAddress::from("HkphEpUqnFBxBuCPEq5j1HA9L8EwmsmRT6UcFKziptM1"),
                    SolanaAddress::from("Hzxx6b5a7dmmJeDXLQzr4dTrc2HGK9ar5YRakZgr3ZZ7"),
                    SolanaAddress::from("11111111111111111111111111111111"),
                    SolanaAddress::from("ComputeBudget111111111111111111111111111111"),
                    SolanaAddress::from("JUP6LkbZbjS1jKKwapdHNy74zcZ3tLUZoi5QNyVTaV4"),
                    SolanaAddress::from("TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA"),
                    SolanaAddress::from("ATokenGPvbdGVxr1b2hvZbsiqW5xWH25efTNsLJA8knL"),
                    SolanaAddress::from("D8cy77BBepLMngZx6ZukaTff5hCt1HrWyKk3Hnd9oitf"),
                    SolanaAddress::from("GGztQqQ6pCPaJQnNpXBgELr5cs3WwDakRbh1iEMzjgSJ"),
                ],
                recent_blockhash: base58_decode_h256(
                    "DiSimxK2z1cRa6yD4goqte3rDMmghJAD8WDUZEab2CzD",
                ),
                instructions: vec![
                    CompiledInstruction {
                        program_id_index: 10,
                        accounts: vec![],
                        data: base58_decode("K1FDJ7"),
                    },
                    CompiledInstruction {
                        program_id_index: 10,
                        accounts: vec![],
                        data: base58_decode("3E9ErJ5MrzbZ"),
                    },
                    CompiledInstruction {
                        program_id_index: 13,
                        accounts: vec![0, 6, 0, 35, 9, 12],
                        data: base58_decode("2"),
                    },
                    CompiledInstruction {
                        program_id_index: 9,
                        accounts: vec![0, 6],
                        data: base58_decode("3Bxs3zzLZLuLQEYX"),
                    },
                    CompiledInstruction {
                        program_id_index: 12,
                        accounts: vec![6],
                        data: base58_decode("J"),
                    },
                    CompiledInstruction {
                        program_id_index: 11,
                        accounts: vec![
                            12, 15, 0, 6, 1, 5, 2, 35, 33, 11, 11, 14, 11, 29, 12, 15, 26, 1, 25,
                            7, 24, 4, 8, 3, 36, 27, 28, 12, 30, 15, 7, 5, 16, 18, 17, 31, 21, 31,
                            20, 23, 7, 5, 33, 34, 19, 31, 15, 12, 12, 32, 31, 22, 11,
                        ],
                        data: base58_decode(
                            "5n9zLuyvSGkuf4iDD6PfDvzvzehUkDghmApUkZSXSx57jF9RGSH5Y23tzFJDG3",
                        ),
                    },
                    CompiledInstruction {
                        program_id_index: 12,
                        accounts: vec![6, 0, 0],
                        data: base58_decode("A"),
                    },
                ],
                address_table_lookups: vec![
                    MessageAddressTableLookup {
                        account_key: SolanaAddress::from(
                            "FeXRmSWmwChZbB2EC7Qjw9XKk28yBrPj3k3nzT1DKfak",
                        ),
                        writable_indexes: vec![202, 200, 201],
                        readonly_indexes: vec![196, 197, 36, 199],
                    },
                    MessageAddressTableLookup {
                        account_key: SolanaAddress::from(
                            "5cFsmTCEfmvpBUBHqsWZnf9n5vTWLYH2LT8X7HdShwxP",
                        ),
                        writable_indexes: vec![160, 245, 248, 159, 157],
                        readonly_indexes: vec![156, 244, 246, 247],
                    },
                    MessageAddressTableLookup {
                        account_key: SolanaAddress::from(
                            "HJ5StCvsDU4JsvK39VcsHjaoTRTtQU749MQ9qUsJaG1m",
                        ),
                        writable_indexes: vec![122, 121, 125],
                        readonly_indexes: vec![110, 126],
                    },
                ],
            }),
        };

        assert_eq!(actual, expected);

        let serialized_again = bincode::serialize(&actual).unwrap();
        assert_eq!(serialized_again.to_hex(), serialized.to_hex());
    }
}
