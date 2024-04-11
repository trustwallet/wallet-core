// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod envelope;
pub mod identity;
pub mod principal;
pub mod request_id;
pub mod rosetta;

use std::time::Duration;

/// This constant defines the maximum amount of time an ingress message can wait
/// to start executing after submission before it is expired.  Hence, if an
/// ingress message is submitted at time `t` and it has not been scheduled for
/// execution till time `t+MAX_INGRESS_TTL`, it will be expired.
///
/// At the time of writing, this constant is also used to control how long the
/// status of a completed ingress message (IngressStatus ∈ [Completed, Failed])
/// is maintained by the IC before it is deleted from the ingress history.
const MAX_INGRESS_TTL: Duration = Duration::from_secs(5 * 60);

/// An upper limit on the validity of the request, expressed in nanoseconds since 1970-01-01.
pub fn get_ingress_expiry(
    current_timestamp_duration: Duration,
    permitted_drift_in_seconds: Option<u64>,
) -> u64 {
    let permitted_drift = permitted_drift_in_seconds
        .map(Duration::from_secs)
        .unwrap_or(Duration::from_secs(60));

    current_timestamp_duration
        .saturating_add(MAX_INGRESS_TTL)
        .saturating_sub(permitted_drift)
        .as_nanos() as u64
}
