// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;
use serde_json::{json, Value as Json};
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;

#[derive(Deserialize)]
struct ExpectedCell {
    data: String,
    bit_len: usize,
    references: Vec<ExpectedCell>,
}

struct TestCase {
    input_encoded: &'static str,
    expected: Json,
    expected_hash: &'static str,
    /// Expected encoded can be different from the original [`TestCase::input_encoded`]
    /// since same Cell can be BoC encoded differently.
    expected_encoded: &'static str,
}

/// Takes a JSON object that will be deserialized as `ExpectedCell`.
/// It's done to allow the function caller to use `json!` macro for readability.
#[track_caller]
fn test_boc_encode_decode(input: TestCase) {
    let boc_decoded = BagOfCells::parse_base64(input.input_encoded).unwrap();
    let root_cell = boc_decoded
        .single_root()
        .expect("Expected single root Cell");

    let expected: ExpectedCell = serde_json::from_value(input.expected)
        .expect("Error deserializing `ExpectedCell` from JSON");
    assert_eq_cell(root_cell, expected);

    let actual_hash = root_cell.cell_hash_base64();
    assert_eq!(actual_hash, input.expected_hash);

    // Wrap the Cell to the BoC again.
    let boc_encoded = BagOfCells::from_root(root_cell.as_ref().clone())
        .to_base64(true)
        .unwrap();
    assert_eq!(boc_encoded, input.expected_encoded);
}

#[track_caller]
fn assert_eq_cell(cell: &Cell, expected: ExpectedCell) {
    assert_eq!(cell.data().to_hex(), expected.data, "Invalid Cell.data");
    assert_eq!(cell.bit_len(), expected.bit_len, "Invalid Cell.bit_len");
    for (cell_ref, expected_ref) in cell.references().iter().zip(expected.references) {
        assert_eq_cell(cell_ref, expected_ref);
    }
}

#[test]
fn test_boc_encode_jetton_transfer_tx() {
    let expected = json!({
        "data": "8800b4510655c8136d4ff5be8ea40a9e161ab0f88321d4969cd828d453f22c7c4b2a08",
        "bit_len": 277,
        "references": [{
            "data": "688595a2c8b55e7bde026a06f72d3f98f78d52a52eec663bea675b44069578d78338f0e58793370446f6ea491ce97a180de915eb4a3688ea1b37e4c64bbea30529a9a3176a8e07f6000000010003",
            "bit_len": 624,
            "references": [{
                "data": "620031341f879da9b83ede2949836e1a9fb5ae1c75431117aeb6531a77cf3aae83f3202faf080000000000000000000000000001",
                "bit_len": 416,
                "references": [{
                    "data": "0f8a7ea5000000000000000041dcd65008000b8196730b5e1d033e99c42857e699fa6c827758d8a9489ac210b3bb131d133900168a20cab9026da9feb7d1d48153c2c3561f10643a92d39b051a8a7e458f89654202000000007465737420636f6d6d656e74",
                    "bit_len": 808,
                    "references": []
                }]
            }]
        }]
    });

    // The same Cell can be BoC encoded differently.
    // Try to decode the original encoded transaction: https://testnet.tonviewer.com/transaction/12bfe84f947740aec3faa54f04a50690900e3aae9ac9596cfa6804a61a48f429
    test_boc_encode_decode(TestCase {
        input_encoded: "te6ccgICAAQAAQAAARgAAAFFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKgwAAQGcaIWVosi1XnveAmoG9y0/mPeNUqUu7GY76mdbRAaVeNeDOPDlh5M3BEb26kkc6XoYDekV60o2iOobN+TGS76jBSmpoxdqjgf2AAAAAQADAAIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEAAwDKD4p+pQAAAAAAAAAAQdzWUAgAC4GWcwteHQM+mcQoV+aZ+myCd1jYqUiawhCzuxMdEzkAFoogyrkCban+t9HUgVPCw1YfEGQ6ktObBRqKfkWPiWVCAgAAAAB0ZXN0IGNvbW1lbnQ=",
        expected: expected.clone(),
        expected_hash: "yYwgXI3TfZpqtdthYvW503zvoGfeJKdlFUpet6NZ8i8",
        expected_encoded: "te6cckECBAEAARUAAUWIALRRBlXIE21P9b6OpAqeFhqw+IMh1Jac2CjUU/IsfEsqDAEBnGiFlaLItV573gJqBvctP5j3jVKlLuxmO+pnW0QGlXjXgzjw5YeTNwRG9upJHOl6GA3pFetKNojqGzfkxku+owUpqaMXao4H9gAAAAEAAwIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEDAMoPin6lAAAAAAAAAABB3NZQCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAAAAAHRlc3QgY29tbWVudG/bd5c=",
    });

    // Try to decode with same encoded data.
    test_boc_encode_decode(TestCase {
        input_encoded: "te6cckECBAEAARUAAUWIALRRBlXIE21P9b6OpAqeFhqw+IMh1Jac2CjUU/IsfEsqDAEBnGiFlaLItV573gJqBvctP5j3jVKlLuxmO+pnW0QGlXjXgzjw5YeTNwRG9upJHOl6GA3pFetKNojqGzfkxku+owUpqaMXao4H9gAAAAEAAwIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEDAMoPin6lAAAAAAAAAABB3NZQCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAAAAAHRlc3QgY29tbWVudG/bd5c=",
        expected,
        expected_hash: "yYwgXI3TfZpqtdthYvW503zvoGfeJKdlFUpet6NZ8i8",
        expected_encoded: "te6cckECBAEAARUAAUWIALRRBlXIE21P9b6OpAqeFhqw+IMh1Jac2CjUU/IsfEsqDAEBnGiFlaLItV573gJqBvctP5j3jVKlLuxmO+pnW0QGlXjXgzjw5YeTNwRG9upJHOl6GA3pFetKNojqGzfkxku+owUpqaMXao4H9gAAAAEAAwIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEDAMoPin6lAAAAAAAAAABB3NZQCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAAAAAHRlc3QgY29tbWVudG/bd5c=",
    });
}

#[test]
fn test_wallet_code_hashes() {
    let wallet_v3r1_code = BagOfCells::parse_base64("te6cckEBAQEAYgAAwP8AIN0gggFMl7qXMO1E0NcLH+Ck8mCDCNcYINMf0x/TH/gjE7vyY+1E0NMf0x/T/9FRMrryoVFEuvKiBPkBVBBV+RDyo/gAkyDXSpbTB9QC+wDo0QGkyMsfyx/L/8ntVD++buA=").unwrap();
    assert_eq!(
        wallet_v3r1_code.single_root().unwrap().cell_hash_base64(),
        "thBBpYp5gLlG6PueGY48kE0keZ_6NldOpCUcQaVm9YE"
    );

    let wallet_v3r2_code = BagOfCells::parse_base64("te6cckEBAQEAcQAA3v8AIN0gggFMl7ohggEznLqxn3Gw7UTQ0x/THzHXC//jBOCk8mCDCNcYINMf0x/TH/gjE7vyY+1E0NMf0x/T/9FRMrryoVFEuvKiBPkBVBBV+RDyo/gAkyDXSpbTB9QC+wDo0QGkyMsfyx/L/8ntVBC9ba0=").unwrap();
    assert_eq!(
        wallet_v3r2_code.single_root().unwrap().cell_hash_base64(),
        "hNr6RJ-Ypph3ibojI1gHK8D3bcRSQAKl0JGLmnXS1Zk"
    );

    let wallet_v4r2_code = BagOfCells::parse_base64("te6cckECFAEAAtQAART/APSkE/S88sgLAQIBIAIDAgFIBAUE+PKDCNcYINMf0x/THwL4I7vyZO1E0NMf0x/T//QE0VFDuvKhUVG68qIF+QFUEGT5EPKj+AAkpMjLH1JAyx9SMMv/UhD0AMntVPgPAdMHIcAAn2xRkyDXSpbTB9QC+wDoMOAhwAHjACHAAuMAAcADkTDjDQOkyMsfEssfy/8QERITAubQAdDTAyFxsJJfBOAi10nBIJJfBOAC0x8hghBwbHVnvSKCEGRzdHK9sJJfBeAD+kAwIPpEAcjKB8v/ydDtRNCBAUDXIfQEMFyBAQj0Cm+hMbOSXwfgBdM/yCWCEHBsdWe6kjgw4w0DghBkc3RyupJfBuMNBgcCASAICQB4AfoA9AQw+CdvIjBQCqEhvvLgUIIQcGx1Z4MesXCAGFAEywUmzxZY+gIZ9ADLaRfLH1Jgyz8gyYBA+wAGAIpQBIEBCPRZMO1E0IEBQNcgyAHPFvQAye1UAXKwjiOCEGRzdHKDHrFwgBhQBcsFUAPPFiP6AhPLassfyz/JgED7AJJfA+ICASAKCwBZvSQrb2omhAgKBrkPoCGEcNQICEekk30pkQzmkD6f+YN4EoAbeBAUiYcVnzGEAgFYDA0AEbjJftRNDXCx+AA9sp37UTQgQFA1yH0BDACyMoHy//J0AGBAQj0Cm+hMYAIBIA4PABmtznaiaEAga5Drhf/AABmvHfaiaEAQa5DrhY/AAG7SB/oA1NQi+QAFyMoHFcv/ydB3dIAYyMsFywIizxZQBfoCFMtrEszMyXP7AMhAFIEBCPRR8qcCAHCBAQjXGPoA0z/IVCBHgQEI9FHyp4IQbm90ZXB0gBjIywXLAlAGzxZQBPoCFMtqEssfyz/Jc/sAAgBsgQEI1xj6ANM/MFIkgQEI9Fnyp4IQZHN0cnB0gBjIywXLAlAFzxZQA/oCE8tqyx8Syz/Jc/sAAAr0AMntVGliJeU=").unwrap();
    assert_eq!(
        wallet_v4r2_code.single_root().unwrap().cell_hash_base64(),
        "_rX_aCDi_w2Ug-fg1iyBfYRniftK5YDIeIZtlZ2r1cA"
    );
}

#[test]
fn test_boc_encode_cell_builder() {
    let leaf = {
        let mut builder = CellBuilder::new();
        builder.store_byte(10).unwrap();
        builder.build().unwrap()
    };
    let inter = {
        let mut builder = CellBuilder::new();
        builder.store_byte(20).unwrap().store_child(leaf).unwrap();
        builder.build().unwrap()
    };
    let root = {
        let mut builder = CellBuilder::new();
        builder.store_byte(30).unwrap().store_child(inter).unwrap();
        builder.build().unwrap()
    };

    let boc = BagOfCells::from_root(root);
    assert_eq!(
        boc.to_base64(true).unwrap(),
        "te6cckEBAwEACwABAh4BAQIUAgACCjHga8U="
    );
}

fn typical_boc_test(boc_base64: &str, expected_hash: &str) {
    let boc = BagOfCells::parse_base64(boc_base64).unwrap();
    let root_cell = boc.single_root().unwrap();
    let hash = root_cell.cell_hash();
    assert_eq!(hash.to_string(), expected_hash);

    let boc_base64_again = boc.to_base64(false).unwrap();
    assert_eq!(boc_base64_again, boc_base64);
}

#[test]
fn test_boc_encode_pruned_block() {
    let boc = "te6ccgEBBAEArwAJRgPIr248LcbQSSCsDD5Rb27WLhRGYiTEGG+uChgAAXoNHAAIASJxwAtrH/x8t+GjDO5/X/f1fk4Rw3oYx+9S1gRE8vya04qzwiyFkEMdYglgAAAaNN8fbBluIJfFw9NAAgMoSAEB/rX/aCDi/w2Ug+fg1iyBfYRniftK5YDIeIZtlZ2r1cAAByhIAQEg0z54hgTX/ohMEnHs6qluCydagWgxQoxSyLwK8qfAOQAA";
    let hash = "a6f4b8afa43a9ee61f6d89050d665d164c94c5eca658ddb6c2ab34b4118ab34c";
    typical_boc_test(boc, hash);
}

/// Checks whether BoC encoding doesn't panic because of invalid input.
#[test]
fn test_boc_decode_invalid() {
    #[track_caller]
    fn test_invalid(input_hex: &str) {
        let input = input_hex.decode_hex().unwrap();
        BagOfCells::parse(&input).unwrap_err();
    }

    test_invalid("b5ee9c725e0000030000000000000000000000000000000000005e");

    // Errors in `BagOfCells::parse()`.
    test_invalid("b5ee9c72c9000001000000000000100000000000000000ff20d1fffe20000052180000001926");
    test_invalid("b5ee9c7201000001000056600000000c000c0cff5e0000005eb5ee9c72ca0c0c0c0c0c0c00");
    test_invalid("b5ee9c72ca0000010000560c0c130c0c0c0c0c0c0c0c000c0c0c5e5e0c0c00b5ee0c5e5e");

    // Errors in `cell::get_bit_descriptor()`.
    test_invalid("b5ee9c72ca0000010000560c0c130c0c0c0c0c0c0c0c000c0c0c5e5e0c0c00b5ee0c5e5e");
    test_invalid("b5ee9c72ca0000230000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff0000000c000c0cffffffffffff0000000000000000000000000000000000000000000600080c");

    // Error in `cell::get_refs_descriptor()`.
    test_invalid("b5ee9c72d1000c0c0c0c20260cba5e0900002a2600000000000000090909090909090909090909090909090909090909091f1f1f1f090909090909090909090971ee31310909090909090909090200000900090909090901680909090909090909090909090909090909090909090000000000000000000000000c88f3");
    // Errors in `CellType::level_mask()`.
    test_invalid("b5ee9c72ca0000180000250125000000000000000b0b0b0b0b0b0404040404040404030404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040408080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808040404040c04040404040404040404040404040404040404040404040404040404040404040404040404270404040404040404040404040404040404040400005204040404040404040404000404040404040404040404040404040403fb04040404040404040404040404040404040404040404040400002501250b4b0b0800ca00250c00000c000c100c0c0c26");
    // Error in `cell::calculate_hashes_and_depths()`.
    test_invalid("b5ee9c72d1000a000000000000000008860101ff041cffff000100000000000010081c01000000000000000000000000000000000000b5ee00000000ff9c72d1000a0000000000000000000000ac0000000006060606060606060606060606000008d60104ff031cff530000002e0000080000000000000000b0504f4f4ab0b0b0b0b0b0b0b0b00f00b00500000f0000000000030053a900002f00000000000000feffffffff0000000000009ce4ee6100000000000000000000000000000886fc00ff041cffff00000000000063000000000000eeee9c72069c720606060000060600");
}
