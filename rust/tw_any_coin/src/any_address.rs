// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::prefix::AddressPrefix;
use tw_coin_registry::{coin_dispatcher, CoinType};
use tw_keypair::tw::PublicKey;
use tw_misc::try_or_false;

#[derive(Debug, PartialEq)]
pub struct AnyAddress {
    coin: CoinType,
    address: String,
}

impl AnyAddress {
    pub fn is_valid(coin: CoinType, address: &str, prefix: Option<AddressPrefix>) -> bool {
        let (ctx, entry) = try_or_false!(coin_dispatcher(coin));
        entry.validate_address(&ctx, address, prefix).is_ok()
    }

    pub fn with_string(
        coin: CoinType,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<AnyAddress> {
        let (ctx, entry) = coin_dispatcher(coin).map_err(|_| AddressError::UnknownCoinType)?;
        let address = entry.normalize_address(&ctx, address, prefix)?;
        Ok(AnyAddress { coin, address })
    }

    pub fn with_public_key(
        coin: CoinType,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<AnyAddress> {
        let (ctx, entry) = coin_dispatcher(coin).map_err(|_| AddressError::UnknownCoinType)?;
        let address = entry.derive_address(&ctx, public_key, derivation, prefix)?;
        Ok(AnyAddress { coin, address })
    }

    pub fn get_data(&self) -> AddressResult<Vec<u8>> {
        let (ctx, entry) = coin_dispatcher(self.coin).map_err(|_| AddressError::UnknownCoinType)?;
        entry.address_to_data(&ctx, &self.address, None)
    }

    pub fn description(&self) -> &str {
        &self.address
    }
}
