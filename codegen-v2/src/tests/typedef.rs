use crate::grammar::{GMarker, GType, GTypeCategory, GTypedef, ParseTree};
use crate::reader::Reader;

#[test]
fn test_typedef() {
    let driver = Reader::from("typedef TWData *_Nonnull TW_Aeternity_Proto_SigningInput;");
    let res = GTypedef::derive(driver).unwrap();
    assert_eq!(
        res.derived,
        GTypedef {
            ty: GType::Mutable(GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown(
                "TWData".to_string()
            )))),
            name: "TW_Aeternity_Proto_SigningInput".to_string(),
            markers: crate::grammar::GMarkers(vec![GMarker::NonNull])
        }
    )
}

#[test]
fn test_typedef_invalid() {
    // ERR: No typedef name.
    let driver = Reader::from("typedef TWData *_Nonnull;");
    let res = GTypedef::derive(driver);
    assert!(res.is_err());

    let driver = Reader::from("typedef TWData *_Nonnull ;");
    let res = GTypedef::derive(driver);
    assert!(res.is_err());

    // ERR: No semicolon
    let driver = Reader::from("typedef TWData *_Nonnull TW_Aeternity_Proto_SigningInput");
    let res = GTypedef::derive(driver);
    assert!(res.is_err());
}
