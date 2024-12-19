// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::multi_sig_ordered::MultiSigOrderedKeys;
use tw_hash::H32;
use tw_keypair::schnorr;
use tw_utxo::script::standard_script::opcodes::*;
use tw_utxo::script::Script;

const VERIFY: bool = true;
const NO_VERIFY: bool = false;

/// https://github.com/babylonchain/babylon/blob/dev/docs/transaction-impl-spec.md#op_return-output-description
/// ```txt
/// OP_RETURN OP_DATA_71 <Tag> <Version> <StakerPublicKey> <FinalityProviderPublicKey> <StakingTime>
/// ```
pub fn new_op_return_script(
    tag: &H32,
    version: u8,
    staker_key: &schnorr::XOnlyPublicKey,
    finality_provider_key: &schnorr::XOnlyPublicKey,
    locktime: u16,
) -> Script {
    let mut buf = Vec::with_capacity(71);
    buf.extend_from_slice(tag.as_slice());
    buf.push(version);
    buf.extend_from_slice(staker_key.bytes().as_slice());
    buf.extend_from_slice(finality_provider_key.bytes().as_slice());
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
pub fn new_timelock_script(staker_key: &schnorr::XOnlyPublicKey, locktime: u16) -> Script {
    let mut s = Script::with_capacity(64);
    append_single_sig(&mut s, staker_key, VERIFY);
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
    staker_key: &schnorr::XOnlyPublicKey,
    covenants: &MultiSigOrderedKeys,
) -> Script {
    let mut s = Script::with_capacity(64);
    append_single_sig(&mut s, staker_key, VERIFY);
    // Covenant multisig is always last in script so we do not run verify and leave
    // last value on the stack. If we do not leave at least one element on the stack
    // script will always error.
    append_multi_sig(
        &mut s,
        covenants.public_keys_ordered(),
        covenants.quorum(),
        NO_VERIFY,
    );
    s
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
    staker_key: &schnorr::XOnlyPublicKey,
    finality_providers_keys: &MultiSigOrderedKeys,
    covenants: &MultiSigOrderedKeys,
) -> Script {
    let mut s = Script::with_capacity(64);
    append_single_sig(&mut s, staker_key, VERIFY);
    // We need to run verify to clear the stack, as finality provider multisig is in the middle of the script.
    append_multi_sig(
        &mut s,
        finality_providers_keys.public_keys_ordered(),
        finality_providers_keys.quorum(),
        VERIFY,
    );
    // Covenant multisig is always last in script so we do not run verify and leave
    // last value on the stack. If we do not leave at least one element on the stack
    // script will always error.
    append_multi_sig(
        &mut s,
        covenants.public_keys_ordered(),
        covenants.quorum(),
        NO_VERIFY,
    );
    s
}

fn append_single_sig(dst: &mut Script, key: &schnorr::XOnlyPublicKey, verify: bool) {
    dst.push_slice(key.bytes().as_slice());
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
/// Note: It is up to the caller to ensure that the keys are unique and sorted.
fn append_multi_sig(
    dst: &mut Script,
    pubkeys: &[schnorr::XOnlyPublicKey],
    quorum: u32,
    verify: bool,
) {
    if pubkeys.len() == 1 {
        return append_single_sig(dst, &pubkeys[0], verify);
    }

    for (i, pk_xonly) in pubkeys.iter().enumerate() {
        dst.push_slice(pk_xonly.bytes().as_slice());
        if i == 0 {
            dst.push(OP_CHECKSIG);
        } else {
            dst.push(OP_CHECKSIGADD);
        }
    }

    dst.push_int(quorum as i64);
    if verify {
        dst.push(OP_NUMEQUALVERIFY);
    } else {
        dst.push(OP_NUMEQUAL);
    }
}
