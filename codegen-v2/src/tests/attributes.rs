// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{ensure, GDefine, GKeyword};
use crate::reader::Reader;
use crate::{must_err, must_ok};

#[test]
// TODO: Backslashes should be handled.
fn test_define_attribute_correct_separator_handling() {
    // Key only.
    let expected = GDefine {
        key: GKeyword("some_def".to_string()),
        value: None,
    };

    must_ok!(GDefine, "#define some_def", expected);
    must_ok!(GDefine, "#define some_def\n", expected);
    // Note that "some_value" is not parsed.
    must_ok!(GDefine, "#define some_def\nsome_value", expected);
    must_ok!(GDefine, "#define some_def \nsome_value", expected);
    must_ok!(GDefine, "#define some_def\n some_value", expected);

    // Key and value.
    let expected = GDefine {
        key: GKeyword("some_def".to_string()),
        value: Some("some_value".to_string()),
    };

    must_ok!(GDefine, "#define some_def some_value", expected);
    must_ok!(GDefine, "#define some_def some_value\n", expected);

    // Fails
    must_err!(GDefine, "#define\nSOME_DEF some_value");
}

#[test]
fn test_define_attribute_continued() {
    let driver = Reader::from("#define some_def\n#define OTHER_DEF with value");
    let (res, reader) = ensure::<GDefine>(driver).unwrap();
    assert_eq!(
        res,
        GDefine {
            key: GKeyword("some_def".to_string()),
            value: None,
        }
    );

    let (res, _) = ensure::<GDefine>(reader).unwrap();
    assert_eq!(
        res,
        GDefine {
            key: GKeyword("OTHER_DEF".to_string()),
            value: Some("with value".to_string()),
        }
    );
}

#[test]
fn test_define_attribute() {
    must_ok!(
        GDefine,
        "#define some_def",
        GDefine {
            key: GKeyword("some_def".to_string()),
            value: None,
        }
    );

    must_ok!(
        GDefine,
        "#define some_def some value\n",
        GDefine {
            key: GKeyword("some_def".to_string()),
            value: Some("some value".to_string()),
        }
    );

    // TODO: Should it behave like that?
    must_ok!(
        GDefine,
        "#define some_def(x)",
        GDefine {
            key: GKeyword("some_def".to_string()),
            value: Some("(x)".to_string()),
        }
    );
}
