use crate::codegen::swift::{register_renderer, MethodInfo};
use handlebars::Handlebars;
use serde_json::{json, Value};

const INPUT: &str = r#"{
    "method_name": "someFunc",
    "is_static": true,
    "return": {
        "type": "String",
        "is_nullable": true,
        "wrap_as": "TWStringNSString(result)"
    },
    "params": [
        {
            "name": "one",
            "type": "String",
            "is_nullable": false,
            "wrap_as": "TWStringCreateWithNSString(one)",
            "deter_as": "StringDelete(one)"
        },
        {
            "name": "two",
            "type": "SomeEnum",
            "is_nullable": false,
            "wrap_as": "TWSomeEnum(rawValue: two.rawValue)"
        }
    ],
    "c_ffi_name": "TWSomeFunc"
}"#;

const OUTPUT: &str =
r#"public static func someFunc(one: String, two: SomeEnum) -> String? {
     // Prepare 'one'.
    let one = TWStringCreateWithNSString(one)
    defer {
        StringDelete(one)
    }

     // Prepare 'two'.
    let two = TWSomeEnum(rawValue: two.rawValue)

    // Call the underlying C function.
    let result = TWSomeFunc(one, two)

    // Check nullable result.
    guard let result = result else {
        return nil
    }

    // Return with conversion function.
    return TWStringNSString(result)
}"#;

#[test]
fn test_swift_method_from_json() {
	let mut engine = Handlebars::new();
	engine.set_strict_mode(true);

	register_renderer(&mut engine).unwrap();

	let data: MethodInfo = serde_json::from_str(INPUT).unwrap();

	let out = engine.render("part_method.hbs", &data).unwrap();
	println!("{}", out);
	assert_eq!(out, OUTPUT);
}
