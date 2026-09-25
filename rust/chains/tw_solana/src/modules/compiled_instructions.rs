// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::instruction::Instruction;
use crate::transaction::CompiledInstruction;
use tw_coin_entry::error::prelude::*;

pub fn compile_instructions(
    ixs: &[Instruction],
    keys: &[SolanaAddress],
) -> SigningResult<Vec<CompiledInstruction>> {
    ixs.iter().map(|ix| compile_instruction(ix, keys)).collect()
}

fn position(keys: &[SolanaAddress], key: &SolanaAddress) -> SigningResult<u8> {
    keys.iter()
        .position(|k| k == key)
        .map(|k| k as u8)
        .or_tw_err(SigningErrorType::Error_internal)
}

/// https://github.com/solana-labs/solana/blob/4b65cc8eef6ef79cb9b9cbc534a99b4900e58cf7/sdk/program/src/message/legacy.rs#L72-L84
pub(crate) fn compile_instruction(
    ix: &Instruction,
    keys: &[SolanaAddress],
) -> SigningResult<CompiledInstruction> {
    let accounts = ix
        .accounts
        .iter()
        .map(|account_meta| position(keys, &account_meta.pubkey))
        .collect::<SigningResult<Vec<_>>>()
        .context("Cannot build account metas")?;

    Ok(CompiledInstruction {
        program_id_index: position(keys, &ix.program_id)
            .context("Program ID account is not provided")?,
        data: ix.data.clone(),
        accounts,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::instruction::AccountMeta;
    use crate::SOLANA_ALPHABET;
    use std::str::FromStr;
    use tw_encoding::base58;
    use tw_keypair::ed25519;

    #[test]
    fn test_compile_instruction() {
        let public_0 = base58::decode(
            "GymAh18wHuFTytfSJWi8eYTA9x5S3sNb9CJSGBWoPRE3",
            SOLANA_ALPHABET,
        )
        .unwrap();
        let public_0 = ed25519::sha512::PublicKey::try_from(public_0.as_slice()).unwrap();
        let address_0 = SolanaAddress::with_public_key_ed25519(&public_0);

        let public_1 = base58::decode(
            "2oKoYSAHgveX91917v4DUEuN8BNKXDg8KJWpaGyEay9V",
            SOLANA_ALPHABET,
        )
        .unwrap();
        let public_1 = ed25519::sha512::PublicKey::try_from(public_1.as_slice()).unwrap();
        let address_1 = SolanaAddress::with_public_key_ed25519(&public_1);

        let program_id = SolanaAddress::from_str("11111111111111111111111111111111").unwrap();

        let addresses = vec![address_0, address_1, program_id];
        let ixs_addresses = vec![
            AccountMeta::new(address_1, false),
            AccountMeta::new(address_0, false),
            AccountMeta::new(program_id, false),
            AccountMeta::new(address_1, false),
            AccountMeta::new(address_0, false),
        ];
        let data = vec![0_u8, 1, 2, 4];
        let instruction = Instruction::new(program_id, data.clone(), ixs_addresses);

        let compiled_ix = compile_instruction(&instruction, &addresses).unwrap();
        let expected = CompiledInstruction {
            program_id_index: 2,
            accounts: vec![1, 0, 2, 1, 0],
            data,
        };
        assert_eq!(compiled_ix, expected);
    }
}
