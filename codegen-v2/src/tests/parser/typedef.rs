// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{GMarker, GMarkers, GPrimitive, GType, GTypeCategory, GTypedef};
use crate::{must_err, must_ok};

#[test]
// TODO: Backslashes should be handled.
fn test_keyword_separator_handling() {
    let expected = GTypedef {
        ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
        name: "var".to_string(),
        markers: GMarkers(vec![]),
    };

    must_ok!(GTypedef, "typedef int var;", expected);
    must_ok!(GTypedef, " typedef int var;", expected);
    must_ok!(GTypedef, "typedef int var; ", expected);
    must_ok!(GTypedef, "typedef int var;\n", expected);
    must_ok!(GTypedef, "typedef int var ;", expected);
    must_ok!(GTypedef, "typedef\nint\nvar\n;", expected);

    // ERR
    must_err!(GTypedef, "\ntypedef int var ;");
    // No semicolon.
    must_err!(GTypedef, "typedef int var");
    must_err!(GTypedef, "typedef int var word;");
}

#[test]
// TODO: Update/expand
fn test_typedef() {
    must_ok!(
        GTypedef,
        "typedef int *_Nonnull TW_Aeternity_Proto_SigningInput;",
        GTypedef {
            ty: GType::Mutable(GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(
                GPrimitive::Int
            )))),
            name: "TW_Aeternity_Proto_SigningInput".to_string(),
            markers: GMarkers(vec![GMarker::NonNull])
        }
    );
    must_ok!(
        GTypedef,
        "typedef const void TWData;",
        GTypedef {
            ty: GType::Const(GTypeCategory::Scalar(GPrimitive::Void)),
            name: "TWData".to_string(),
            markers: GMarkers(vec![])
        }
    );
}
