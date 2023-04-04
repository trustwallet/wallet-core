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

enum GSeparator {
    Item(GSeparatorItem),
    Continued(GSeparatorContinued),
}

impl GSeparator {
    fn add(self, new_item: GSeparatorItem) -> Self {
        match self {
            GSeparator::Item(item) => GSeparator::Continued(GSeparatorContinued {
                item,
                next: Box::new(GSeparator::Item(new_item)),
            }),
            GSeparator::Continued(items) => GSeparator::Continued(GSeparatorContinued {
                item: items.item,
                next: Box::new(items.next.add(new_item)),
            }),
        }
    }
}

impl ParseTree for GSeparator {
    type Derivation = Self;

    fn derive(input: &str) -> Result<Self::Derivation> {
        let mut sep_items: Option<GSeparator> = None;

        for chr in input.chars() {
            if let Ok(item) = GSeparatorItem::derive(chr.to_string().as_str()) {
                if let Some(sep) = sep_items {
                    sep_items = Some(sep.add(item));
                }
            }
        }

        sep_items.ok_or(())
    }
}

enum GSeparatorItem {
    Space,
    Newline,
    Tab,
}

impl ParseTree for GSeparatorItem {
    type Derivation = Self;

    fn derive(input: &str) -> Result<Self::Derivation> {
        let der = match input {
            " " => GSeparatorItem::Space,
            "\n" => GSeparatorItem::Newline,
            "\t" => GSeparatorItem::Tab,
            _ => return Err(()),
        };

        Ok(der)
    }
}

struct GSeparatorContinued {
    item: GSeparatorItem,
    next: Box<GSeparator>,
}

struct GParam {
    ty: GType,
    tag: GTag,
    name: (),
}

type Result<T> = std::result::Result<T, ()>;
