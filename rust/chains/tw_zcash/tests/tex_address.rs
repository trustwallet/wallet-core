// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_zcash::tex_address::TexAddress;

#[test]
fn test_from_to_t_address() {
    const TEX_ADDRESS: &str = "tex1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0";
    const T_ADDRESS: &str = "t1ffSG8KGpAmsCU3Rh14bV8fUazFK6xUc3P";
    const TEX_HRP: &str = "tex";

    let tex_address = TexAddress::from_str(TEX_ADDRESS).unwrap();
    let t_address = tex_address.to_t_address(184).unwrap();
    assert_eq!(t_address.to_string(), T_ADDRESS);
    let parsed_tex_address =
        TexAddress::from_t_address_with_hrp(&t_address, TEX_HRP.to_string()).unwrap();
    assert_eq!(parsed_tex_address.to_string(), TEX_ADDRESS);
}
