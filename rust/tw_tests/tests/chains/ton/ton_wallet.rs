// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_keypair::test_utils::tw_public_key_helper::TWPublicKeyHelper;
use tw_keypair::tw::PublicKeyType;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;
use tw_ton::resources::WALLET_ID_V5R1_TON_MAINNET;
use wallet_core_rs::ffi::ton::wallet::{
    tw_ton_wallet_build_v4_r2_state_init, tw_ton_wallet_build_v5_r1_state_init,
};

#[test]
fn test_ton_wallet_v4_r2_create_state_init() {
    let public_key = TWPublicKeyHelper::with_hex(
        "f229a9371fa7c2108b3d90ea22c9be705ff5d0cfeaee9cbb9366ff0171579357",
        PublicKeyType::Ed25519,
    );
    assert!(!public_key.is_null());
    let state_init_boc = TWStringHelper::wrap(unsafe {
        tw_ton_wallet_build_v4_r2_state_init(public_key.ptr(), 0, 0x29a9a317)
    });
    assert_eq!(state_init_boc.to_string().unwrap(), "te6cckECFgEAAwQAAgE0AQIBFP8A9KQT9LzyyAsDAFEAAAAAKamjF/IpqTcfp8IQiz2Q6iLJvnBf9dDP6u6cu5Nm/wFxV5NXQAIBIAQFAgFIBgcE+PKDCNcYINMf0x/THwL4I7vyZO1E0NMf0x/T//QE0VFDuvKhUVG68qIF+QFUEGT5EPKj+AAkpMjLH1JAyx9SMMv/UhD0AMntVPgPAdMHIcAAn2xRkyDXSpbTB9QC+wDoMOAhwAHjACHAAuMAAcADkTDjDQOkyMsfEssfy/8ICQoLAubQAdDTAyFxsJJfBOAi10nBIJJfBOAC0x8hghBwbHVnvSKCEGRzdHK9sJJfBeAD+kAwIPpEAcjKB8v/ydDtRNCBAUDXIfQEMFyBAQj0Cm+hMbOSXwfgBdM/yCWCEHBsdWe6kjgw4w0DghBkc3RyupJfBuMNDA0CASAODwBu0gf6ANTUIvkABcjKBxXL/8nQd3SAGMjLBcsCIs8WUAX6AhTLaxLMzMlz+wDIQBSBAQj0UfKnAgBwgQEI1xj6ANM/yFQgR4EBCPRR8qeCEG5vdGVwdIAYyMsFywJQBs8WUAT6AhTLahLLH8s/yXP7AAIAbIEBCNcY+gDTPzBSJIEBCPRZ8qeCEGRzdHJwdIAYyMsFywJQBc8WUAP6AhPLassfEss/yXP7AAAK9ADJ7VQAeAH6APQEMPgnbyIwUAqhIb7y4FCCEHBsdWeDHrFwgBhQBMsFJs8WWPoCGfQAy2kXyx9SYMs/IMmAQPsABgCKUASBAQj0WTDtRNCBAUDXIMgBzxb0AMntVAFysI4jghBkc3Rygx6xcIAYUAXLBVADzxYj+gITy2rLH8s/yYBA+wCSXwPiAgEgEBEAWb0kK29qJoQICga5D6AhhHDUCAhHpJN9KZEM5pA+n/mDeBKAG3gQFImHFZ8xhAIBWBITABG4yX7UTQ1wsfgAPbKd+1E0IEBQNch9AQwAsjKB8v/ydABgQEI9ApvoTGACASAUFQAZrc52omhAIGuQ64X/wAAZrx32omhAEGuQ64WPwEXtMkg=");
}

#[test]
fn test_ton_wallet_v5_r1_create_state_init() {
    let public_key = TWPublicKeyHelper::with_hex(
        "f229a9371fa7c2108b3d90ea22c9be705ff5d0cfeaee9cbb9366ff0171579357",
        PublicKeyType::Ed25519,
    );
    assert!(!public_key.is_null());
    let state_init_boc = TWStringHelper::wrap(unsafe {
        tw_ton_wallet_build_v5_r1_state_init(public_key.ptr(), 0, WALLET_ID_V5R1_TON_MAINNET)
    });
    assert_eq!(state_init_boc.to_string().unwrap(), "te6cckECFgEAArEAAgE0AQIBFP8A9KQT9LzyyAsDAFGAAAAAP///iPkU1JuP0+EIRZ7IdRFk3zgv+uhn9XdOXcmzf4C4q8mroAIBIAQFAgFIBgcBAvIIAtzQINdJwSCRW49jINcLHyCCEGV4dG69IYIQc2ludL2wkl8D4IIQZXh0brqOtIAg1yEB0HTXIfpAMPpE+Cj6RDBYvZFb4O1E0IEBQdch9AWDB/QOb6ExkTDhgEDXIXB/2zzgMSDXSYECgLmRMOBw4hIJAgEgCgsBHiDXCx+CEHNpZ2668uCKfwkB5o7w7aLt+yGDCNciAoMI1yMggCDXIdMf0x/TH+1E0NIA0x8g0x/T/9cKAAr5AUDM+RCaKJRfCtsx4fLAh98Cs1AHsPLQhFEluvLghVA2uvLghvgju/LQiCKS+ADeAaR/yMoAyx8BzxbJ7VQgkvgP3nDbPNgSAgEgDA0AGb5fD2omhAgKDrkPoCwCAW4ODwIBSBARABmtznaiaEAg65Drhf/AABmvHfaiaEAQ65DrhY/AABezJftRNBx1yHXCx+AAEbJi+1E0NcKAIAP27aLt+wL0BCFukmwhjkwCIdc5MHCUIccAs44tAdcoIHYeQ2wg10nACPLgkyDXSsAC8uCTINcdBscSwgBSMLDy0InXTNc5MAGk6GwShAe78uCT10rAAPLgk+1V4tIAAcAAkVvg69csCBQgkXCWAdcsCBwS4lIQseMPINdKExQVAJYB+kAB+kT4KPpEMFi68uCR7UTQgQFB1xj0BQSdf8jKAEAEgwf0U/Lgi44UA4MH9Fvy4Iwi1woAIW4Bs7Dy0JDiyFADzxYS9ADJ7VQAcjDXLAgkji0h8uCS0gDtRNDSAFETuvLQj1RQMJExnAGBAUDXIdcKAPLgjuLIygBYzxbJ7VST8sCN4gAQk1vbMeHXTNBAlw92");
}
