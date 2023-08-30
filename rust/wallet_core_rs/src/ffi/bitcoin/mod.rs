// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use tw_bitcoin::entry::BitcoinEntry;
use tw_coin_entry::coin_entry::CoinEntry;
use tw_coin_entry::test_utils::empty_context::EmptyCoinContext;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;
use tw_proto::BitcoinV2::Proto;

#[no_mangle]
pub unsafe extern "C" fn tw_bitcoin_sign(_coin: CoinType, input: *const TWData) -> *mut TWData {
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let signing: Proto::SigningInput = try_or_else!(
        tw_proto::deserialize(input_data.as_slice()),
        std::ptr::null_mut
    );

    let signed: Proto::SigningOutput = BitcoinEntry.sign(&EmptyCoinContext, signing);

    let serialized = tw_proto::serialize(&signed).expect("serialized invalid structure");
    TWData::from(serialized).into_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn tw_bitcoin_preimage_hashes(
    _coin: CoinType,
    input: *const TWData,
) -> *mut TWData {
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let signing: Proto::SigningInput = try_or_else!(
        tw_proto::deserialize(input_data.as_slice()),
        std::ptr::null_mut
    );

    let signed: Proto::PreSigningOutput = BitcoinEntry.preimage_hashes(&EmptyCoinContext, signing);

    let serialized = tw_proto::serialize(&signed).expect("serialized invalid structure");
    TWData::from(serialized).into_ptr()
}

#[no_mangle]
pub unsafe extern "C" fn tw_bitcoin_compile(_coin: CoinType, input: *const TWData) -> *mut TWData {
    let input_data = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let signing: Proto::SigningInput = try_or_else!(
        tw_proto::deserialize(input_data.as_slice()),
        std::ptr::null_mut
    );

    // TODO:
    let _signatures = vec![];
    let signed: Proto::SigningOutput =
        BitcoinEntry.compile(&EmptyCoinContext, signing, _signatures, vec![]);

    let serialized = tw_proto::serialize(&signed).expect("serialized invalid structure");
    TWData::from(serialized).into_ptr()
}
