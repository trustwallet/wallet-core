// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{
    GKeyword, GMarker, GMarkers, GPrimitive, GStructDecl, GStructName, GType, GTypeCategory,
};
use crate::{must_err, must_ok};

#[test]
fn test_struct_decls_separator_handling() {
    let expected = GStructDecl {
        name: GStructName::from("SomeStruct"),
        fields: vec![
            (
                GKeyword::from("one"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
            ),
            (
                GKeyword::from("two"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
            ),
            (
                GKeyword::from("three"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Char)),
            ),
        ],
        markers: GMarkers(vec![]),
    };

    must_ok!(
        GStructDecl,
        "struct SomeStruct { int one; bool two; char three; };",
        expected
    );
    must_ok!(
        GStructDecl,
        " struct SomeStruct { int one; bool two; char three; };",
        expected
    );
    must_ok!(
        GStructDecl,
        "struct SomeStruct { int one ; bool two ; char three ; } ;",
        expected
    );
    must_ok!(
        GStructDecl,
        "struct SomeStruct{int one;bool two;char three;};",
        expected
    );
    must_ok!(
        GStructDecl,
        "struct\nSomeStruct\n{\nint\none\n;\nbool\ntwo\n;\nchar\nthree\n;\n}\n;",
        expected
    );

    // ERR!
    must_err!(
        GStructDecl,
        "structSomeStruct { int one; bool two; char three; };"
    );
    must_err!(
        GStructDecl,
        "struct SomeStruct { int; bool two; char three; };"
    );
    // No semicolon on last field.
    must_err!(
        GStructDecl,
        "structSomeStruct { int one; bool two; char three };"
    );
    // No semicolon termination.
    must_err!(
        GStructDecl,
        "struct SomeStruct { int one; bool two; char three; }"
    );
}

#[test]
fn test_struct_decls_with_comment() {
    let expected = GStructDecl {
        name: GStructName::from("SomeStruct"),
        fields: vec![
            (
                GKeyword::from("one"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
            ),
            (
                GKeyword::from("two"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
            ),
            (
                GKeyword::from("three"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Char)),
            ),
        ],
        markers: GMarkers(vec![]),
    };

    must_ok!(
        GStructDecl,
        r#"struct SomeStruct {
			int one; // Comment
			bool two; // Comment
			char three; // Comment
		};"#,
        expected
    );

    must_ok!(
        GStructDecl,
        r#"struct SomeStruct {
			int one; // Comment
			bool two;
			char three; // Comment
		};"#,
        expected
    );

    must_ok!(
        GStructDecl,
        r#"struct SomeStruct {
			int one; // Comment
			bool two; // Comment
			char three;
		};"#,
        expected
    );

    must_ok!(
        GStructDecl,
        r#"struct SomeStruct {
			int one;
			// Comment
			bool two;
			char three;
		};"#,
        expected
    );
}

#[test]
fn test_struct_decls_with_marker() {
    let expected = GStructDecl {
        name: GStructName::from("TWHash"),
        fields: vec![(
            GKeyword::from("unused"),
            GType::Mutable(GTypeCategory::Scalar(GPrimitive::UInt8T)),
        )],
        markers: GMarkers(vec![GMarker::TwExportStruct]),
    };

    must_ok!(
        GStructDecl,
        r#"
		TW_EXPORT_STRUCT
		struct TWHash {
			uint8_t unused; // C doesn't allow zero-sized struct
		};
		"#,
        expected
    );
}
