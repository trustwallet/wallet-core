trait ParseTree {
    type Derivation;

    fn derive(input: &str) -> Result<Self::Derivation>;
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
}

struct GStruct;

impl ParseTree for GStruct {
    type Derivation = Self;

    fn derive(input: &str) -> Result<Self::Derivation> {
        todo!()
    }
}

struct GTag;

struct GSeparators(Vec<GSeparator>);

impl ParseTree for GSeparators {
    type Derivation = Self;

    fn derive(input: &str) -> Result<Self::Derivation> {
        let mut seps = vec![];

        for chr in input.chars() {
            // TODO: Find better way to do `.to_string().as_str().
            let sep = GSeparator::derive(chr.to_string().as_str())?;
            seps.push(sep);
        }

        Ok(GSeparators(seps))
    }
}

enum GSeparator {
    Space,
    Newline,
    Tab,
}

impl ParseTree for GSeparator {
    type Derivation = Self;

    fn derive(input: &str) -> Result<Self::Derivation> {
        let der = match input {
            " " => GSeparator::Space,
            "\n" => GSeparator::Newline,
            "\t" => GSeparator::Tab,
            _ => return Err(()),
        };

        Ok(der)
    }
}

struct GParam {
    ty: GType,
    s1: GSeparator,
    tag: GTag,
    s2: GSeparator,
    name: (),
}

type Result<T> = std::result::Result<T, ()>;
