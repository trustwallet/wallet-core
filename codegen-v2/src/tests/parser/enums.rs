// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{GEnumDecl, GKeyword};
use crate::{must_err, must_ok};

#[test]
fn test_enum_decls_separator_handling() {
    // Variants without values.
    let expected = GEnumDecl {
        name: GKeyword::from("SomeEnum"),
        variants: vec![
            (GKeyword::from("ONE"), None),
            (GKeyword::from("TWO"), None),
            (GKeyword::from("THREE"), None),
        ],
    };

    must_ok!(GEnumDecl, "enum SomeEnum { ONE, TWO, THREE };", expected);
    must_ok!(GEnumDecl, " enum SomeEnum { ONE, TWO, THREE };", expected);
    must_ok!(
        GEnumDecl,
        " enum SomeEnum { ONE , TWO , THREE , } ; ",
        expected
    );
    must_ok!(GEnumDecl, "enum SomeEnum{ONE,TWO,THREE};", expected);
    must_ok!(GEnumDecl, "enum SomeEnum{ONE,TWO,THREE,};", expected);
    must_ok!(
        GEnumDecl,
        "enum SomeEnum\n{\nONE\n,\nTWO\n,\nTHREE\n}\n;",
        expected
    );
    must_ok!(
        GEnumDecl,
        "enum SomeEnum\n{\nONE\n,\nTWO\n,\nTHREE\n,\n}\n;",
        expected
    );

    // Variants with values.
    let expected = GEnumDecl {
        name: GKeyword::from("SomeEnum"),
        variants: vec![
            (GKeyword::from("ONE"), Some(1)),
            (GKeyword::from("TWO"), Some(2)),
            (GKeyword::from("THREE"), Some(3)),
        ],
    };

    must_ok!(
        GEnumDecl,
        "enum SomeEnum { ONE = 1, TWO = 2, THREE = 3 };",
        expected
    );
    must_ok!(
        GEnumDecl,
        " enum SomeEnum { ONE = 1, TWO = 2, THREE = 3 };",
        expected
    );
    must_ok!(
        GEnumDecl,
        "enum SomeEnum { ONE = 1, TWO = 2, THREE = 3 , };",
        expected
    );
    must_ok!(
        GEnumDecl,
        "enum SomeEnum { ONE = 1, TWO = 2, THREE = 3 };",
        expected
    );
    must_ok!(GEnumDecl, "enum SomeEnum{ONE=1,TWO=2,THREE=3};", expected);
    must_ok!(
        GEnumDecl,
        "enum SomeEnum\n{\nONE\n=\n1\n,\nTWO\n=\n2,\nTHREE\n=\n3\n}\n;",
        expected
    );
    must_ok!(
        GEnumDecl,
        "enum SomeEnum\n{\nONE\n=\n1\n,\nTWO\n=\n2,\nTHREE\n=\n3\n,\n}\n;",
        expected
    );
    // Hex values.
    must_ok!(
        GEnumDecl,
        "enum SomeEnum { ONE = 0x1, TWO = 0x2, THREE = 0x3 };",
        expected
    );

    // ERR!
    must_err!(GEnumDecl, "enum SomeEnum { } ");
    // No semicolon.
    must_err!(GEnumDecl, "enum SomeEnum { ONE, TWO, THREE }");
    must_err!(GEnumDecl, "enumSomeEnum { ONE, TWO, THREE };");
    must_err!(GEnumDecl, "enumSomeEnum { ONE = 1, TWO = 2, THREE = 3 };");
}

#[test]
// TODO: Comments should be parsed as well.
fn test_enum_decls_with_comment() {
    // Variants without values.
    let expected = GEnumDecl {
        name: GKeyword::from("SomeEnum"),
        variants: vec![
            (GKeyword::from("ONE"), None),
            (GKeyword::from("TWO"), None),
            (GKeyword::from("THREE"), None),
        ],
    };

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE, // Comment
        TWO, // Comment
        THREE // Comment
    };"#,
        expected
    );

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE, // Comment
        TWO,
        THREE // Comment
    };"#,
        expected
    );

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE, // Comment
        TWO, // Comment
        THREE
    };"#,
        expected
    );

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE,
        // Comment
        TWO,
        THREE
    };"#,
        expected
    );

    // Variants with values.
    let expected = GEnumDecl {
        name: GKeyword::from("SomeEnum"),
        variants: vec![
            (GKeyword::from("ONE"), Some(1)),
            (GKeyword::from("TWO"), Some(2)),
            (GKeyword::from("THREE"), Some(3)),
        ],
    };

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE = 1, // Comment
        TWO = 2, // Comment
        THREE = 3, // Comment
    };"#,
        expected
    );

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE = 1, // Comment
        TWO = 2,
        THREE = 3, // Comment
    };"#,
        expected
    );

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE = 1, // Comment
        TWO = 2, // Comment
        THREE = 3,
    };"#,
        expected
    );

    must_ok!(
        GEnumDecl,
        r#"enum SomeEnum {
        ONE = 1,
        // Comment
        TWO = 2,
        THREE = 3,
    };"#,
        expected
    );
}
