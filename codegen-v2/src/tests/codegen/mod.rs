use handlebars::Handlebars;

mod swift;

const TEMPLATE_DIR: &str = "src/codegen/templates/swift/";

fn template_path(file: &str) -> String {
    format!("{}{}", TEMPLATE_DIR, file)
}

fn new_engine<'re>() -> Handlebars<'re> {
    let mut engine = Handlebars::new();
    engine.set_strict_mode(true);
    engine
}
