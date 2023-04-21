// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{
    GKeyword, GMarker, GMarkers, GPrimitive, GStaticVar, GStructName, GType, GTypeCategory,
};
use crate::must_ok;

#[test]
fn test_types_categories() {
    #[rustfmt::skip]
    must_ok!(
        GTypeCategory,
        "int",
        GTypeCategory::Scalar(GPrimitive::Int)
    );
    must_ok!(
        GTypeCategory,
        "int*",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
    );
    must_ok!(
        GTypeCategory,
        "int **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Scalar(GPrimitive::Int)
        ))))
    );
    must_ok!(
        GTypeCategory,
        "int * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
        ))))
    );
}

#[test]
fn test_types_categories_struct() {
    must_ok!(
        GTypeCategory,
        "struct SomeStruct",
        GTypeCategory::Struct(GStructName::from("SomeStruct"))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct*",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStructName::from(
            "SomeStruct"
        ))))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Struct(GStructName::from("SomeStruct"))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStructName::from(
                "SomeStruct"
            ))))
        ))))
    );
}

#[test]
fn test_types_categories_unrecognized() {
    must_ok!(
        GTypeCategory,
        "SomeType",
        GTypeCategory::Unrecognized(GKeyword::from("SomeType"))
    );
    must_ok!(
        GTypeCategory,
        "SomeType*",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Unrecognized(GKeyword::from(
            "SomeType"
        ))))
    );
    must_ok!(
        GTypeCategory,
        "SomeType **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unrecognized(GKeyword::from("SomeType"))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "SomeType * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unrecognized(GKeyword::from(
                "SomeType"
            ))))
        ))))
    );
}

#[test]
fn test_types_static() {
    let expected = GStaticVar {
        ty: GType::Const(GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(
            GPrimitive::Char,
        )))),
        name: GKeyword::from("SOME_VAR"),
        value: "some_value".to_string(),
        markers: GMarkers(vec![GMarker::NonNull]),
    };

    #[rustfmt::skip]
    must_ok!(
        GStaticVar,
        "static const char *_Nonnull SOME_VAR = \"some_value\";",
        expected
    );

    let expected = GStaticVar {
        ty: GType::Const(GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(
            GPrimitive::Char,
        )))),
        name: GKeyword::from("SOME_VAR"),
        // TODO: Should be a variant based on `ty`.
        value: "50".to_string(),
        markers: GMarkers(vec![GMarker::NonNull]),
    };

    #[rustfmt::skip]
    must_ok!(
        GStaticVar,
        "static const char *_Nonnull SOME_VAR = 50;",
        expected
    );
}
