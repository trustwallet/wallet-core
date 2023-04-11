use crate::grammar::{
    GFuncName, GFunctionDecl, GHeaderDefine, GMarker, GParamItem, GParamName, GPrimitive,
    GReturnValue, GStruct, GType, GTypeCategory, ParseTree,
};
use crate::reader::Reader;

#[test]
fn test_define_attribute() {
    let driver = Reader::from("#define SOME_DEF");
    let der = GHeaderDefine::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GHeaderDefine {
            key: "SOME_DEF".to_string(),
            value: None,
        }
    );

    let driver = Reader::from("#define SOME_DEF some value");
    let der = GHeaderDefine::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GHeaderDefine {
            key: "SOME_DEF".to_string(),
            value: Some("some value".to_string()),
        }
    );

    let driver = Reader::from("#define SOME_DEF(x)");
    let der = GHeaderDefine::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GHeaderDefine {
            key: "SOME_DEF(x)".to_string(),
            value: None,
        }
    );
}
