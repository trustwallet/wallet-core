// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Sui::Proto;

mod sui_address;
mod sui_compile;
mod sui_sign;
mod test_cases;

fn object_ref(id: &'static str, version: u64, digest: &'static str) -> Proto::ObjectRef<'static> {
    Proto::ObjectRef {
        object_id: id.into(),
        version,
        object_digest: digest.into(),
    }
}
