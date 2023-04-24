/*
use super::{new_engine, template_path};
use crate::codegen::swift::{self, SwiftFunction};
use handlebars::Handlebars;
use serde_json::{json, Value};

#[test]
fn test_swift_method_from_json() {
    // TODO: Use MethodInfo directly?
    let input = r#"{
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

    let output = r#"public static func someFunc(one: String, two: SomeEnum) -> String? {
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

    let mut engine = new_engine();

    engine
        .register_template_file(swift::METHOD_INFO, template_path(swift::METHOD_INFO))
        .unwrap();

    let data: MethodInfo = serde_json::from_str(input).unwrap();
    let out = engine.render("part_method.hbs", &data).unwrap();

    assert_eq!(out, output);
}

#[test]
fn test_swift_method_from_json_2() {
    // TODO: Use MethodInfo directly?
    let input = r#"{
		"method_name": "someFunc",
		"is_static": false,
		"return": {
			"type": "String",
			"is_nullable": false,
			"wrap_as": "TWStringNSString(result)"
		},
		"params": [
			{
				"name": "one",
				"type": "String",
				"is_nullable": true,
				"wrap_as": "TWStringCreateWithNSString(one)",
				"deter_as": "StringDelete(one)"
			},
			{
				"name": "two",
				"type": "UInt32",
				"is_nullable": true
			}
		],
		"c_ffi_name": "TWSomeFunc"
	}"#;

    let output = r#"public func someFunc(one: String, two: UInt32) -> String {
    // Prepare 'one'.
    let one = TWStringCreateWithNSString(one)
    defer {
        StringDelete(one)
    }

    let two = two

    // Call the underlying C function.
    let result = TWSomeFunc(one, two)

    // Return with conversion function.
    return TWStringNSString(result)
}"#;

    let mut engine = new_engine();

    engine
        .register_template_file(swift::METHOD_INFO, template_path(swift::METHOD_INFO))
        .unwrap();

    let data: MethodInfo = serde_json::from_str(input).unwrap();
    let out = engine.render("part_method.hbs", &data).unwrap();

    assert_eq!(out, output);
}
*/
