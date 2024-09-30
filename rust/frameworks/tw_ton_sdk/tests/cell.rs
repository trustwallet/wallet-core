// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_ton_sdk::boc::BagOfCells;

#[test]
fn test_cell_format() {
    let boc = BagOfCells::parse_base64(
        "te6ccgEBAgEALQABDv8AiNDtHtgBCEIC5wowbAAnJ5YkP1ac4Mko6kz8nxtlxbAGbjghWfXoDfU=",
    )
    .unwrap();
    let cell = boc.single_root().unwrap();

    let actual_fmt = format!("{cell:?}");
    let expected =
"Cell(root) { data=ff0088d0ed1ed8, bit_len=56 }
	-> Cell(ref 1) { data=02e70a306c00272796243f569ce0c928ea4cfc9f1b65c5b0066e382159f5e80df5, bit_len=264 exotic=Library }
";
    assert_eq!(actual_fmt, expected);
}
