trait ParseTree {
    type Derivation;

    fn derive(input: &str) -> Result<Self::Derivation>;
    fn generate(&self) -> String;
}

enum GType {
    Bool,
    Char,
    Int,
    Struct(GStruct),
}

impl ParseTree for GType {
    type Derivation = Self;

    fn derive(input: &str) -> Result<Self::Derivation> {
        let der = match input {
            "bool" => GType::Bool,
            "char" => GType::Char,
            "int" => GType::Int,
            _ => GType::Struct(GStruct::derive(input)?),
        };

        Ok(der)
    }
    fn generate(&self) -> String {
        todo!()
    }
}

struct GStruct;

impl ParseTree for GStruct {
    type Derivation = Self;

    fn derive(input: &str) -> Result<Self::Derivation> {
        todo!()
    }
    fn generate(&self) -> String {
        todo!()
    }
}

struct GTag;

enum GSeparator {
    Space,
}

struct GParam {
    ty: GType,
    s1: GSeparator,
    tag: GTag,
    s2: GSeparator,
    name: (),
}

type Result<T> = std::result::Result<T, ()>;
