use crate::grammar::{GDefine, GKeyword, ParseTree};
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

    let driver = Reader::from("#define\nSOME_DEF some_value");
    let der = GDefine::derive(driver);
    assert!(der.is_err());

    let driver = Reader::from("#define SOME_DEF\nsome_value");
    let der = GDefine::derive(driver);
    assert!(der.is_err());
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

    let driver = Reader::from("#define SOME_DEF some value");
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
