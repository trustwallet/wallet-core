use crate::codegen::kotlin::{RenderIntput};
use crate::manifest::parse_str;

fn create_input(yaml: &str) -> RenderIntput {
	let file_info = parse_str(yaml).unwrap();

	RenderIntput {
		file_info,
		android_main_template: include_str!("../../codegen/kotlin/templates/androidmain_struct.hbs"),
	}
}
