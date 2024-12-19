// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::covenant_committee::CovenantCommittee;
use crate::babylon::tx_builder::BabylonStakingParams;
use std::borrow::Cow;
use tw_coin_entry::error::prelude::*;
use tw_keypair::schnorr;
use tw_proto::BabylonStaking::Proto;
use tw_utxo::sighash::SighashType;
use tw_utxo::signature::BitcoinSchnorrSignature;

pub mod output_protobuf;
pub mod utxo_protobuf;

pub fn staking_params_from_proto(
    params: &Option<Proto::StakingInfo>,
) -> SigningResult<BabylonStakingParams> {
    let params = params
        .as_ref()
        .or_tw_err(SigningErrorType::Error_invalid_params)
        .context("No 'StakingInfo' params provided")?;

    let staker = schnorr::PublicKey::try_from(params.staker_public_key.as_ref())
        .into_tw()
        .context("Invalid stakerPublicKey")?;
    let staking_locktime: u16 = params
        .staking_time
        .try_into()
        .tw_err(|_| SigningErrorType::Error_invalid_params)
        .context("stakingTime cannot be greater than 65535")?;
    let finality_provider = parse_schnorr_pk(&params.finality_provider_public_key)
        .context("Invalid finalityProviderPublicKeys")?;

    let covenants_pks = parse_schnorr_pks(&params.covenant_committee_public_keys)
        .context("Invalid covenantCommitteePublicKeys")?;
    let covenants = CovenantCommittee::new(covenants_pks, params.covenant_quorum)?;

    Ok(BabylonStakingParams {
        staker,
        staking_locktime,
        finality_provider,
        covenants,
    })
}

pub fn parse_schnorr_pk(bytes: &Cow<[u8]>) -> SigningResult<schnorr::XOnlyPublicKey> {
    schnorr::XOnlyPublicKey::try_from(bytes.as_ref()).into_tw()
}

pub fn parse_schnorr_pks(pks: &[Cow<[u8]>]) -> SigningResult<Vec<schnorr::XOnlyPublicKey>> {
    pks.iter().map(parse_schnorr_pk).collect()
}

pub fn parse_schnorr_pubkey_sig(
    pubkey_sig: &Proto::PublicKeySignature,
    sighash_ty: SighashType,
) -> SigningResult<(schnorr::XOnlyPublicKey, BitcoinSchnorrSignature)> {
    let pk = parse_schnorr_pk(&pubkey_sig.public_key)?;
    let sig = schnorr::Signature::try_from(pubkey_sig.signature.as_ref())
        .tw_err(|_| SigningErrorType::Error_invalid_params)
        .context("Invalid signature")?;
    let btc_sign = BitcoinSchnorrSignature::new(sig, sighash_ty)?;
    Ok((pk, btc_sign))
}
