use crate::grammar::{ensure, GDefine, GKeyword, ParseTree};
use crate::reader::Reader;
use crate::{must_err, must_ok};

#[test]
// TODO: Backslashes should be handled.
fn test_define_attribute_correct_separator_handling() {
    // Key only.
    let expected = GDefine {
        key: GKeyword("SOME_DEF".to_string()),
        value: None,
    };

    must_ok!(GDefine, "#define SOME_DEF");
    must_ok!(GDefine, "#define SOME_DEF", expected);
    must_ok!(GDefine, "#define SOME_DEF\n", expected);
    must_ok!(GDefine, "#define SOME_DEF\nsome_value", expected);
    must_ok!(GDefine, "#define SOME_DEF \nsome_value", expected);
    must_ok!(GDefine, "#define SOME_DEF\n some_value", expected);

    // Key and value.
    let expected = GDefine {
        key: GKeyword("SOME_DEF".to_string()),
        value: Some("some_value".to_string()),
    };

    must_ok!(GDefine, "#define SOME_DEF some_value", expected);
    must_ok!(GDefine, "#define SOME_DEF some_value\n", expected);

    // Fails
    must_err!(GDefine, "#define\nSOME_DEF some_value");
}

#[test]
// TODO: Backslashes should be handled.
fn test_define_attribute_reoccuring() {
    let driver = Reader::from("#define SOME_DEF\n#define OTHER_DEF with value");
    let (res, reader) = ensure::<GDefine>(driver).unwrap();
    assert_eq!(
        res,
        GDefine {
            key: GKeyword("SOME_DEF".to_string()),
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
    let driver = Reader::from("#define SOME_DEF");
    let der = GDefine::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GDefine {
            key: GKeyword("SOME_DEF".to_string()),
            value: None,
        }
    );

    let driver = Reader::from("#define SOME_DEF some value\n");
    let der = GDefine::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GDefine {
            key: GKeyword("SOME_DEF".to_string()),
            value: Some("some value".to_string()),
        }
    );

    // TODO: Should it behave like that?
    let driver = Reader::from("#define SOME_DEF(x)");
    let der = GDefine::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GDefine {
            key: GKeyword("SOME_DEF".to_string()),
            value: Some("(x)".to_string()),
        }
    );
}
