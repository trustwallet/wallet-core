// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::ton::ton_sign::assert_eq_boc;
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[test]
fn test_ton_compile_wallet_v4r2_transfer_and_deploy() {
    let public_key = "f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41";

    let transfer = Proto::Transfer {
        dest: "EQDYW_1eScJVxtitoBRksvoV9cCYo4uKGWLVNIHB1JqRR3n0".into(),
        amount: 10,
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        public_key: public_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        expire_at: 1671135440,
        wallet_version: Proto::WalletVersion::WALLET_V4_R2,
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::TON, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "9da0ab29120fd96b48b2bb76843e77c0a1770b47c2f78da23299e2c32f50192e"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature_bytes = "e7c6f18186845784fa20c4d63c4261e58a1256c60e6de8aa6da9c008665564b35afe67eaf56af0963427aa78b0bfa1a895378983c7f135f99862df71b6e6b708".decode_hex().unwrap();
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::TON, &input, vec![signature_bytes], vec![]);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/6ZzWOFKZt_m3kZjbwfbATwLaVwmUOdDp0xjhuY7PO3k=
    assert_eq_boc(&output.encoded, "te6ccgICABoAAQAAA8sAAAJFiADN98eLgHfrkE8l8gmT8X5REpTVR6QnqDhArTbKlVvbZh4ABAABAZznxvGBhoRXhPogxNY8QmHlihJWxg5t6KptqcAIZlVks1r+Z+r1avCWNCeqeLC/oaiVN4mDx/E1+Zhi33G25rcIKamjF/////8AAAAAAAMAAgFiYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4hQAAAAAAAAAAAAAAAAAQADAAACATQABgAFAFEAAAAAKamjF/Qsd/kxvqIOxdAVBzEna7suKGCUdmEkWyMZ74Ez7o1BQAEU/wD0pBP0vPLICwAHAgEgAA0ACAT48oMI1xgg0x/TH9MfAvgju/Jk7UTQ0x/TH9P/9ATRUUO68qFRUbryogX5AVQQZPkQ8qP4ACSkyMsfUkDLH1Iwy/9SEPQAye1U+A8B0wchwACfbFGTINdKltMH1AL7AOgw4CHAAeMAIcAC4wABwAORMOMNA6TIyx8Syx/L/wAMAAsACgAJAAr0AMntVABsgQEI1xj6ANM/MFIkgQEI9Fnyp4IQZHN0cnB0gBjIywXLAlAFzxZQA/oCE8tqyx8Syz/Jc/sAAHCBAQjXGPoA0z/IVCBHgQEI9FHyp4IQbm90ZXB0gBjIywXLAlAGzxZQBPoCFMtqEssfyz/Jc/sAAgBu0gf6ANTUIvkABcjKBxXL/8nQd3SAGMjLBcsCIs8WUAX6AhTLaxLMzMlz+wDIQBSBAQj0UfKnAgIBSAAXAA4CASAAEAAPAFm9JCtvaiaECAoGuQ+gIYRw1AgIR6STfSmRDOaQPp/5g3gSgBt4EBSJhxWfMYQCASAAEgARABG4yX7UTQ1wsfgCAVgAFgATAgEgABUAFAAZrx32omhAEGuQ64WPwAAZrc52omhAIGuQ64X/wAA9sp37UTQgQFA1yH0BDACyMoHy//J0AGBAQj0Cm+hMYALm0AHQ0wMhcbCSXwTgItdJwSCSXwTgAtMfIYIQcGx1Z70ighBkc3RyvbCSXwXgA/pAMCD6RAHIygfL/8nQ7UTQgQFA1yH0BDBcgQEI9ApvoTGzkl8H4AXTP8glghBwbHVnupI4MOMNA4IQZHN0crqSXwbjDQAZABgAilAEgQEI9Fkw7UTQgQFA1yDIAc8W9ADJ7VQBcrCOI4IQZHN0coMesXCAGFAFywVQA88WI/oCE8tqyx/LP8mAQPsAkl8D4gB4AfoA9AQw+CdvIjBQCqEhvvLgUIIQcGx1Z4MesXCAGFAEywUmzxZY+gIZ9ADLaRfLH1Jgyz8gyYBA+wAG");
    assert_eq!(
        output.hash.to_hex(),
        "b3d9462c13a8c67e19b62002447839c386de51415ace3ff6473b1e6294299819"
    );
}

#[test]
fn test_ton_compile_wallet_v5r1_transfer_and_deploy() {
    let public_key = "55b837ceefc94d1865b763149c4751b43e297cea65dabfe20febed13d56072e4";

    let transfer = Proto::Transfer {
        dest: "EQBe6DtCpJZe8M4t-crMXe93JlEYgSl30S5OUuMSLOfeQfBu".into(),
        amount: 10,
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        public_key: public_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        expire_at: 0xffffffff,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::TON, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "d3b8dbedb049074d90bcae341484e0385789e19f69cd86d570939b94043739a7"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature_bytes = "00239fa98ecbde5dda29ce7c78dc6e25369e1ff7e0070d9a1b472833e8630e98d684929b60a9ac003f2fd39e07612b8eeaf3776d09d5e54d7d061b067ded4c0e".decode_hex().unwrap();
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(CoinType::TON, &input, vec![signature_bytes], vec![]);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/6ZzWOFKZt_m3kZjbwfbATwLaVwmUOdDp0xjhuY7PO3k=
    assert_eq_boc(&output.encoded, "te6cckECGwEAA2sAAkWIACm9HPyVOpjCNOG6nbf+EwCONRHHpeMQsIlCoWNKhUaaHgECAgE0AwQBoXNpZ25///8R/////wAAAACACOfqY7L3l3aKc58eNxuJTaeH/fgBw2aG0coM+hjDpjWhJKbYKmsAD8v054HYSuO6vN3bQnV5U19BhsGfe1MDoAUBFP8A9KQT9LzyyAsGAFGAAAAAP///iKrcG+d35KaMMtuxik4jqNofFL51Mu1f8Qf19onqsDlyIAIKDsPIbQMWBwIBIAgJAWJiAC90HaFSSy94Zxb85WYu97uTKIxAlLvolycpcYkWc+8giFAAAAAAAAAAAAAAAAABFgIBSAoLAQLyDALc0CDXScEgkVuPYyDXCx8gghBleHRuvSGCEHNpbnS9sJJfA+CCEGV4dG66jrSAINchAdB01yH6QDD6RPgo+kQwWL2RW+DtRNCBAUHXIfQFgwf0Dm+hMZEw4YBA1yFwf9s84DEg10mBAoC5kTDgcOIXDQIBIA4PAR4g1wsfghBzaWduuvLgin8NAeaO8O2i7fshgwjXIgKDCNcjIIAg1yHTH9Mf0x/tRNDSANMfINMf0//XCgAK+QFAzPkQmiiUXwrbMeHywIffArNQB7Dy0IRRJbry4IVQNrry4Ib4I7vy0IgikvgA3gGkf8jKAMsfAc8Wye1UIJL4D95w2zzYFwIBIBARABm+Xw9qJoQICg65D6AsAgFuEhMCAUgUFQAZrc52omhAIOuQ64X/wAAZrx32omhAEOuQ64WPwAAXsyX7UTQcdch1wsfgABGyYvtRNDXCgCAAAAP27aLt+wL0BCFukmwhjkwCIdc5MHCUIccAs44tAdcoIHYeQ2wg10nACPLgkyDXSsAC8uCTINcdBscSwgBSMLDy0InXTNc5MAGk6GwShAe78uCT10rAAPLgk+1V4tIAAcAAkVvg69csCBQgkXCWAdcsCBwS4lIQseMPINdKGBkaAJYB+kAB+kT4KPpEMFi68uCR7UTQgQFB1xj0BQSdf8jKAEAEgwf0U/Lgi44UA4MH9Fvy4Iwi1woAIW4Bs7Dy0JDiyFADzxYS9ADJ7VQAcjDXLAgkji0h8uCS0gDtRNDSAFETuvLQj1RQMJExnAGBAUDXIdcKAPLgjuLIygBYzxbJ7VST8sCN4gAQk1vbMeHXTNB8Ui06");
    assert_eq!(
        output.hash.to_hex(),
        "437dae441a95a6bccdcdcea2560c313de24f13dd85c76d5d7ecaab1e70a1e52b"
    );
}
