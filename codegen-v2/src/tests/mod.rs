use crate::codegen::swift::{render_to_strings, RenderIntput};
use crate::manifest::parse_str;

const STRUCT_TEMPLATE: &str = include_str!("../codegen/templates/swift/struct.hbs");
const ENUM_TEMPLATE: &str = include_str!("../codegen/templates/swift/enum.hbs");
const EXTENSION_TEMPLATE: &str = include_str!("../codegen/templates/swift/extension.hbs");
const PROTO_TEMPLATE: &str = include_str!("../codegen/templates/swift/proto.hbs");

/// Convenience function.
fn create_intput(yaml: &str) -> RenderIntput {
    let file_info = parse_str(yaml).unwrap();

    RenderIntput {
        file_info,
        struct_template: STRUCT_TEMPLATE,
        enum_template: ENUM_TEMPLATE,
        extension_template: EXTENSION_TEMPLATE,
        proto_template: PROTO_TEMPLATE,
    }
}

#[test]
fn single_struct() {
    const INPUT: &str = include_str!("samples/single_struct.input.yaml");
    const EXPECTED: &str = include_str!("samples/single_struct.output.swift");

    let input = create_intput(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert_eq!(rendered.structs.len(), 1);
    assert!(rendered.enums.is_empty());
    assert!(rendered.extensions.is_empty());
    assert!(rendered.protos.is_empty());

    let (name, output) = &rendered.structs[0];
    println!("{output}");
    assert_eq!(name, "FirstStruct");
    assert_eq!(output, EXPECTED);
}

#[test]
fn optional() {
    const INPUT: &str = include_str!("samples/optional.input.yaml");
    const EXPECTED: &str = include_str!("samples/optional.output.swift");

    let input = create_intput(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert_eq!(rendered.structs.len(), 1);
    assert!(rendered.enums.is_empty());
    assert!(rendered.extensions.is_empty());
    assert!(rendered.protos.is_empty());

    let (name, output) = &rendered.structs[0];
    println!("{output}");
    assert_eq!(name, "FirstStruct");
    assert_eq!(output, EXPECTED);
}

