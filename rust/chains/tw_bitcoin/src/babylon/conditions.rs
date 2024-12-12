// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_hash::{H256, H32};
use tw_utxo::script::standard_script::opcodes::*;
use tw_utxo::script::Script;

const VERIFY: bool = true;
const NO_VERIFY: bool = false;
/// We always require only one finality provider to sign.
const FINALITY_PROVIDERS_QUORUM: u32 = 1;

/// https://github.com/babylonchain/babylon/blob/dev/docs/transaction-impl-spec.md#op_return-output-description
/// ```txt
/// OP_RETURN OP_DATA_71 <Tag> <Version> <StakerPublicKey> <FinalityProviderPublicKey> <StakingTime>
/// ```
pub fn new_op_return_script(
    tag: &H32,
    version: u8,
    staker_xonly: &H256,
    finality_provider_xonly: &H256,
    locktime: u16,
) -> Script {
    let mut buf = Vec::with_capacity(71);
    buf.extend_from_slice(tag.as_slice());
    buf.push(version);
    buf.extend_from_slice(staker_xonly.as_slice());
    buf.extend_from_slice(finality_provider_xonly.as_slice());
    buf.extend_from_slice(&locktime.to_be_bytes());

    let mut s = Script::new();
    s.push(OP_RETURN);
    s.push_slice(&buf);
    s
}

/// The timelock path locks the staker's Bitcoin for a pre-determined number of Bitcoin blocks.
/// https://github.com/babylonchain/babylon/blob/dev/docs/staking-script.md#1-timelock-path
///
/// ```txt
/// <StakerPK> OP_CHECKSIGVERIFY  <TimelockBlocks> OP_CHECKSEQUENCEVERIFY
/// ```
pub fn new_timelock_script(staker_xonly: &H256, locktime: u16) -> Script {
    let mut s = Script::with_capacity(64);
    append_single_sig(&mut s, staker_xonly, VERIFY);
    s.push_int(locktime as i64);
    s.push(OP_CHECKSEQUENCEVERIFY);
    s
}

/// The unbonding path allows the staker to on-demand unlock their locked Bitcoin before the timelock expires.
/// https://github.com/babylonchain/babylon/blob/dev/docs/staking-script.md#2-unbonding-path
///
/// ```txt
/// <StakerPk> OP_CHECKSIGVERIFY
/// <CovenantPk1> OP_CHECKSIG <CovenantPk2> OP_CHECKSIGADD ... <CovenantPkN> OP_CHECKSIGADD
/// <CovenantThreshold> OP_NUMEQUAL
/// ```
pub fn new_unbonding_script(
    staker_xonly: &H256,
    covenants_xonly: Vec<H256>,
    covenant_quorum: u32,
) -> SigningResult<Script> {
    let mut s = Script::with_capacity(64);
    append_single_sig(&mut s, staker_xonly, VERIFY);
    // Covenant multisig is always last in script so we do not run verify and leave
    // last value on the stack. If we do not leave at least one element on the stack
    // script will always error.
    append_multi_sig(&mut s, covenants_xonly, covenant_quorum, NO_VERIFY)?;
    Ok(s)
}

/// The slashing path is utilized for punishing finality providers and their delegators in the case of double signing.
/// https://github.com/babylonchain/babylon/blob/dev/docs/staking-script.md#3-slashing-path
///
/// ```txt
/// <StakerPk> OP_CHECKSIGVERIFY
/// <FinalityProviderPk1> OP_CHECKSIG <FinalityProviderPk2> OP_CHECKSIGADD ... <FinalityProviderPkN> OP_CHECKSIGADD
/// 1 OP_NUMEQUAL
/// <CovenantPk1> OP_CHECKSIG <CovenantPk2> OP_CHECKSIGADD ... <CovenantPkN> OP_CHECKSIGADD
/// <CovenantThreshold> OP_NUMEQUAL
/// ```
pub fn new_slashing_script(
    staker_xonly: &H256,
    finality_providers_xonly: Vec<H256>,
    covenants_xonly: Vec<H256>,
    covenant_quorum: u32,
) -> SigningResult<Script> {
    let mut s = Script::with_capacity(64);
    append_single_sig(&mut s, staker_xonly, VERIFY);
    // We need to run verify to clear the stack, as finality provider multisig is in the middle of the script.
    append_multi_sig(
        &mut s,
        finality_providers_xonly,
        FINALITY_PROVIDERS_QUORUM,
        VERIFY,
    )?;
    // Covenant multisig is always last in script so we do not run verify and leave
    // last value on the stack. If we do not leave at least one element on the stack
    // script will always error.
    append_multi_sig(&mut s, covenants_xonly, covenant_quorum, NO_VERIFY)?;
    Ok(s)
}

fn append_single_sig(dst: &mut Script, xonly: &H256, verify: bool) {
    dst.push_slice(xonly.as_slice());
    if verify {
        dst.push(OP_CHECKSIGVERIFY);
    } else {
        dst.push(OP_CHECKSIG);
    }
}

/// Creates a multisig script with given keys and signer threshold to
/// successfully execute script.
/// It validates whether threshold is not greater than number of keys.
/// If there is only one key provided it will return single key sig script.
/// Note: It is up to the caller to ensure that the keys are unique.
fn append_multi_sig(
    dst: &mut Script,
    mut covenants_xonly: Vec<H256>,
    covenant_quorum: u32,
    verify: bool,
) -> SigningResult<()> {
    if covenants_xonly.is_empty() {
        return SigningError::err(SigningErrorType::Error_invalid_params)
            .context("No covenant public keys provided");
    }
    if covenants_xonly.len() < covenant_quorum as usize {
        return SigningError::err(SigningErrorType::Error_invalid_params)
            .context("Required number of valid signers is greater than number of provided keys");
    }
    if covenants_xonly.len() == 1 {
        append_single_sig(dst, &covenants_xonly[0], verify);
        return Ok(());
    }

    // Sort the keys in lexicographical order bytes.
    covenants_xonly.sort();
    for (i, covenant_xonly) in covenants_xonly.into_iter().enumerate() {
        dst.push_slice(covenant_xonly.as_slice());
        if i == 0 {
            dst.push(OP_CHECKSIG);
        } else {
            dst.push(OP_CHECKSIGADD);
        }
    }

    dst.push_int(covenant_quorum as i64);
    if verify {
        dst.push(OP_NUMEQUALVERIFY);
    } else {
        dst.push(OP_NUMEQUAL);
    }

    Ok(())
}
