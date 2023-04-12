use crate::grammar::{ensure, GDefine, GKeyword, ParseTree};
use crate::reader::Reader;

#[test]
// TODO: Backslashes should be handled.
fn test_define_attribute_correct_separator_handling() {
    let driver = Reader::from("#define SOME_DEF");
    let der = GDefine::derive(driver);
    assert!(der.is_ok());

    let driver = Reader::from("#define SOME_DEF\n");
    let der = GDefine::derive(driver);
    assert!(der.is_ok());

    let driver = Reader::from("#define SOME_DEF some_value");
    let der = GDefine::derive(driver);
    assert!(der.is_ok());

    let driver = Reader::from("#define SOME_DEF some_value\n");
    let der = GDefine::derive(driver);
    assert!(der.is_ok());

    // OK! The "some_value" part could be another component.
    let driver = Reader::from("#define SOME_DEF\nsome_value");
    let der = GDefine::derive(driver);
    assert!(der.is_ok());

    let driver = Reader::from("#define SOME_DEF \nsome_value");
    let der = GDefine::derive(driver);
    assert!(der.is_ok());

    let driver = Reader::from("#define SOME_DEF\n some_value");
    let der = GDefine::derive(driver);
    assert!(der.is_ok());

    // ERR
    let driver = Reader::from("#define\nSOME_DEF some_value");
    let der = GDefine::derive(driver);
    assert!(der.is_err());

    let driver = Reader::from("#define \nSOME_DEF some_value");
    let der = GDefine::derive(driver);
    assert!(der.is_err());

    let driver = Reader::from("#define\n SOME_DEF some_value");
    let der = GDefine::derive(driver);
    assert!(der.is_err());
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
