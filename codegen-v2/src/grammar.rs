use super::reader::{Reader, ReaderBranch};
use super::{Error, Result};

pub trait ParseTree {
    type Derivation;

    fn derive(reader: Reader) -> Result<DerivationResult<Self::Derivation>>;
}

// Convenience function. Removes a derived type from the reader, returning the
// updated reader.
pub fn wipe<T>(reader: Reader) -> (Option<T::Derivation>, Reader)
where
    T: ParseTree,
{
    let (pending, checked_out) = reader.checkout();
    if let Ok(res) = T::derive(checked_out) {
        (Some(res.derived), pending.merge(res.branch))
    } else {
        (None, pending.discard())
    }
}

// Convenience function. Tries to successfully derive a type from the reader,
// returning the updated reader.
pub fn ensure<T>(reader: Reader) -> Result<(T::Derivation, Reader)>
where
    T: ParseTree,
{
    let (pending, checked_out) = reader.checkout();
    let res = T::derive(checked_out)?;
    Ok((res.derived, pending.merge(res.branch)))
}

// Convenience function. Tries to successfully derive a type from the reader,
// returning the updated reader.
pub fn optional<T>(reader: Reader) -> (Option<T::Derivation>, Reader)
where
    T: ParseTree,
{
    let (pending, checked_out) = reader.checkout();
    if let Ok(res) = T::derive(checked_out) {
        (Some(res.derived), pending.merge(res.branch))
    } else {
        (None, pending.discard())
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GHeaderFileItem {
    HeaderInclude(GHeaderInclude),
    HeaderPragma(GHeaderPragma),
    Comment(GCommentBlock),
    FunctionDecl(GFunctionDecl),
    StructDecl(GStructDecl),
    Marker(GMarker),
    Newline,
    Eof,
    Unrecognized(String),
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GComma;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GSemicolon;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GOpenBracket;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GCloseBracket;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GDoubleQuote;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GHeaderInclude(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GHeaderPragma(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GCommentLine(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GCommentBlock {
    lines: Vec<GCommentLine>,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GFuncName(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GNewline;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GAnyLine(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GEndOfLine;

#[derive(Debug, Clone)]
pub struct DerivationResult<'a, T> {
    pub derived: T,
    pub branch: ReaderBranch<'a>,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum EitherOr<T, D> {
    Either(T),
    Or(D),
}

pub type GNonAlphanumeric = Continuum<GNonAlphanumericItem>;

// TODO: Rename?
#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GType {
    Mutable(GTypeCategory),
    Const(GTypeCategory),
}

// TODO: Rename
// TODO: Should this wrap the item?
#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GNonAlphanumericItem;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GTypeCategory {
    Scalar(GPrimitive),
    Struct(GStruct),
    Enum(GEnum),
    Pointer(Box<GTypeCategory>),
    // TODO: Add some sort of `GAnyKeyword` that can parse valid keywords.
    Unknown(String),
}

// TODO: Not complete (eg. "unsigned char", etc...)
#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GPrimitive {
    // TODO: Not a primitive, handle somewhere else.
    Void,
    Bool,
    Char,
    ShortInt,
    Int,
    UnsignedInt,
    LongInt,
    Float,
    Double,
    LongDouble,
    UInt32T,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GStruct(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GStructDecl {
    g_struct: GStruct,
    markers: Vec<GMarker>,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GEnum(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GEof;

pub type GSeparator = Continuum<GSeparatorItem>;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GSeparatorItem {
    Space,
    Newline,
    Tab,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GReturnValue {
    pub ty: GType,
    pub markers: Vec<GMarker>,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GParamItem {
    pub ty: GType,
    pub markers: Vec<GMarker>,
    pub name: GParamName,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GParamName(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GMarker {
    TWVisibilityDefault,
    TwExportClass,
    TwExportStruct,
    //TwExportEnum,
    TwExportFunc,
    TwExportMethod,
    TwExportProperty,
    TwExportStaticMethod,
    TwExportStaticProperty,
    TwMethodDiscardableResult,
    //TwData,
    TwAssumeNonNullBegin,
    TwAssumeNonNullEnd,
    //TwDeprecated,
    //TwDeprecatedFor,
    Nullable,
    NonNull,
    //NullUnspecified,
    TWExternCBegin,
    TWExternCEnd,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GFunctionDecl {
    pub name: GFuncName,
    pub params: Vec<GParamItem>,
    pub return_value: GReturnValue,
    pub markers: Vec<GMarker>,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum Continuum<T> {
    Thing(T),
    Next(ContinuumNext<T>),
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct ContinuumNext<T> {
    thing: T,
    next: Box<Continuum<T>>,
}

// *** DERIVE IMPLEMENTATIONS ***

impl From<String> for GParamName {
    fn from(string: String) -> Self {
        GParamName(string)
    }
}

impl From<String> for GFuncName {
    fn from(string: String) -> Self {
        GFuncName(string)
    }
}

impl From<String> for GStruct {
    fn from(string: String) -> Self {
        GStruct(string)
    }
}

impl<T: ParseTree, D: ParseTree> ParseTree for EitherOr<T, D> {
    type Derivation = EitherOr<T::Derivation, D::Derivation>;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (pending, checked_out) = reader.checkout();
        if let Ok(res) = T::derive(checked_out) {
            return Ok(DerivationResult {
                derived: EitherOr::Either(res.derived),
                branch: res.branch,
            });
        }

        let reader = pending.discard();

        if let Ok(res) = D::derive(reader) {
            return Ok(DerivationResult {
                derived: EitherOr::Or(res.derived),
                branch: res.branch,
            });
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GEof {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (slice, handle) = reader.read_amt(1)?;

        if slice.is_some() {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GEof,
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GPrimitive {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Read data until the separator is reached, then return the sub-slice
        // leading up to it.
        let (slice, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;

        let derived = match slice.as_str() {
            "void" => GPrimitive::Void,
            "bool" => GPrimitive::Bool,
            "char" => GPrimitive::Char,
            "short" => GPrimitive::ShortInt,
            "int" => GPrimitive::Int,
            "signed" => GPrimitive::Int,
            "unsigned" => GPrimitive::UnsignedInt,
            "long" => GPrimitive::LongInt,
            "float" => GPrimitive::Float,
            "double" => GPrimitive::Double,
            "long double" => GPrimitive::LongDouble,
            "uint32_t" => GPrimitive::UInt32T,
            _ => {
                return Err(Error::Todo);
            }
        };

        Ok(DerivationResult {
            derived,
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GStruct {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for "struct" prefix.
        let (string, handle) = reader.read_until::<GSeparator>()?;
        if string != "struct" {
            return Err(Error::Todo);
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Derive struct name
        // TODO:
        let (name, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;
        if name.is_empty()
            || name
                .chars()
                .any(|c| (!c.is_alphanumeric() && (c != '_' || c != '-')))
        {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GStruct(name),
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GEnum {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for "enum" prefix.
        let (string, handle) = reader.read_until::<GSeparator>()?;
        if string != "enum" {
            return Err(Error::Todo);
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Derive struct name
        // TODO:
        let (name, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;
        if name.is_empty()
            || name
                .chars()
                .any(|c| (!c.is_alphanumeric() && (c != '_' || c != '-')))
        {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GEnum(name),
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GStructDecl {
    type Derivation = Self;

    fn derive<'a>(mut p_reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Derive (optional) markers.
        let mut markers = vec![];
        loop {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(p_reader);

            let (pending, checked_out) = reader.checkout();
            if let Ok(marker_res) = GMarker::derive(checked_out) {
                markers.push(marker_res.derived);
                p_reader = pending.merge(marker_res.branch);
            } else {
                p_reader = pending.discard();
                break;
            }
        }

        let (pending, checked_out) = p_reader.checkout();
        let res = GStruct::derive(checked_out)?;

        // Check for required semicolon.
        let (_, reader) = ensure::<GSemicolon>(pending.merge(res.branch))?;

        Ok(DerivationResult {
            derived: GStructDecl {
                g_struct: res.derived,
                markers,
            },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GSeparatorItem {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // TODO: Make use of `GEof` here.
        let (slice, handle) = reader.read_amt(1)?;
        let slice = match slice {
            Some(string) => string,
            None => {
                return Err(Error::Todo);
            }
        };

        let derived = match slice.as_str() {
            " " => GSeparatorItem::Space,
            "\n" => GSeparatorItem::Newline,
            "\t" => GSeparatorItem::Tab,
            _ => return Err(Error::Todo),
        };

        Ok(DerivationResult {
            derived,
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GReturnValue {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Derive parameter type.
        let (ty_derived, mut p_reader) = ensure::<GType>(reader)?;

        // Derive (optional) markers.
        let mut markers = vec![];
        loop {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(p_reader);

            let (pending, checked_out) = reader.checkout();
            if let Ok(marker_res) = GMarker::derive(checked_out) {
                markers.push(marker_res.derived);
                p_reader = pending.merge(marker_res.branch);
            } else {
                p_reader = pending.discard();
                break;
            }
        }

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GReturnValue {
                ty: ty_derived,
                markers,
            },
            branch: p_reader.into_branch(),
        })
    }
}

impl ParseTree for GParamItem {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Derive parameter type.
        let (ty_derived, mut p_reader) = ensure::<GType>(reader)?;

        // Derive (optional) markers.
        let mut markers = vec![];
        loop {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(p_reader);

            let (pending, checked_out) = reader.checkout();
            if let Ok(marker_res) = GMarker::derive(checked_out) {
                markers.push(marker_res.derived);
                p_reader = pending.merge(marker_res.branch);
            } else {
                p_reader = pending.discard();
                break;
            }
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(p_reader);

        // Derive parameter name.
        let (name_derived, reader) = ensure::<GParamName>(reader)?;

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItem {
                ty: ty_derived,
                name: name_derived,
                markers,
            },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GParamName {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (string, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;

        if string.is_empty() || string.chars().any(|c| (!c.is_alphanumeric() && c != '_')) {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GParamName(string),
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GMarker {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (string, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;

        let der = match string.as_str() {
            "TW_VISIBILITY_DEFAULT" => GMarker::TWVisibilityDefault,
            "TW_EXPORT_CLASS" => GMarker::TwExportClass,
            "TW_EXPORT_STRUCT" => GMarker::TwExportStruct,
            // TODO: Needs special handling.
            //"TW_EXPORT_ENUM" => GMarker::TwExportStruct,
            "TW_EXPORT_FUNC" => GMarker::TwExportFunc,
            "TW_EXPORT_METHOD" => GMarker::TwExportMethod,
            "TW_EXPORT_PROPERTY" => GMarker::TwExportProperty,
            "TW_EXPORT_STATIC_METHOD" => GMarker::TwExportStaticMethod,
            "TW_EXPORT_STATIC_PROPERTY" => GMarker::TwExportStaticProperty,
            "TW_METHOD_DISCARDABLE_RESULT" => GMarker::TwMethodDiscardableResult,
            "TWData" => GMarker::TwMethodDiscardableResult,
            "TW_ASSUME_NONNULL_BEGIN" => GMarker::TwAssumeNonNullBegin,
            "TW_ASSUME_NONNULL_END" => GMarker::TwAssumeNonNullEnd,
            // TODO: Needs special handling.
            //"TW_DEPRECATED" => GMarker::TwDeprecated,
            //"TW_DEPRECATED_FOR" => GMarker::TwDeprecatedFor,
            "_Nullable" => GMarker::Nullable,
            "_Nonnull" => GMarker::NonNull,
            "TW_EXTERN_C_BEGIN" => GMarker::TWExternCBegin,
            "TW_EXTERN_C_END" => GMarker::TWExternCEnd,
            _ => return Err(Error::Todo),
        };

        Ok(DerivationResult {
            derived: der,
            branch: handle.commit().into_branch(),
        })
    }
}

impl<T> Continuum<T> {
    fn add(self, new_thing: T) -> Self {
        match self {
            Continuum::Thing(thing) => Continuum::Next(ContinuumNext {
                thing,
                next: Box::new(Continuum::Thing(new_thing)),
            }),
            Continuum::Next(next) => Continuum::Next(ContinuumNext {
                thing: next.thing,
                next: Box::new(next.next.add(new_thing)),
            }),
        }
    }
}

impl<T: ParseTree<Derivation = T> + std::fmt::Debug> ParseTree for Continuum<T> {
    type Derivation = Continuum<T>;

    fn derive<'a>(mut reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let mut sep_items: Option<Continuum<T::Derivation>> = None;
        loop {
            let (pending, checked_out) = reader.checkout();
            if let Ok(res) = T::derive(checked_out) {
                reader = pending.merge(res.branch);

                if let Some(sep) = sep_items {
                    sep_items = Some(sep.add(res.derived));
                } else {
                    sep_items = Some(Continuum::Thing(res.derived));
                }
            } else if let Some(items) = sep_items {
                return Ok(DerivationResult {
                    derived: items,
                    branch: pending.discard().into_branch(),
                });
            } else {
                return Err(Error::Todo);
            }
        }
    }
}

impl ParseTree for GNonAlphanumericItem {
    type Derivation = Self;

    fn derive(mut reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (pending, checked_out) = reader.checkout();
        if GEof::derive(checked_out).is_ok() {
            //return Ok(DerivationResult { derived: GNonAlphanumericItem, branch: res.branch })
            return Err(Error::Todo);
        } else {
            reader = pending.discard();
        }

        let (slice, handle) = reader.read_amt(1).map(|(s, h)| (s.unwrap(), h))?;
        if slice
            .chars()
            .any(|c| c.is_alphanumeric() || c == '_' || c == '-')
        {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GNonAlphanumericItem,
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GComma {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (slice, handle) = reader.read_amt(1)?;

        if let Some(symbol) = slice {
            if symbol == "," {
                return Ok(DerivationResult {
                    derived: GComma,
                    branch: handle.commit().into_branch(),
                });
            }
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GSemicolon {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (slice, handle) = reader.read_amt(1)?;

        if let Some(symbol) = slice {
            if symbol == ";" {
                return Ok(DerivationResult {
                    derived: GSemicolon,
                    branch: handle.commit().into_branch(),
                });
            }
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GOpenBracket {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (slice, handle) = reader.read_amt(1)?;

        if let Some(symbol) = slice {
            if symbol == "(" {
                return Ok(DerivationResult {
                    derived: GOpenBracket,
                    branch: handle.commit().into_branch(),
                });
            }
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GDoubleQuote {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (slice, handle) = reader.read_amt(1)?;

        if let Some(symbol) = slice {
            if symbol == "\"" {
                return Ok(DerivationResult {
                    derived: GDoubleQuote,
                    branch: handle.commit().into_branch(),
                });
            }
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GNewline {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (slice, handle) = reader.read_amt(1)?;

        if let Some(symbol) = slice {
            if symbol == "\n" {
                return Ok(DerivationResult {
                    derived: GNewline,
                    branch: handle.commit().into_branch(),
                });
            }
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GCloseBracket {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (slice, handle) = reader.read_amt(1)?;

        if let Some(symbol) = slice {
            if symbol == ")" {
                return Ok(DerivationResult {
                    derived: GCloseBracket,
                    branch: handle.commit().into_branch(),
                });
            }
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GEndOfLine {
    type Derivation = Self;

    fn derive(mut p_reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        loop {
            let (item_der, reader) = optional::<GSeparatorItem>(p_reader);
            if let Some(item) = item_der {
                if let GSeparatorItem::Newline = item {
                    return Ok(DerivationResult {
                        derived: GEndOfLine,
                        branch: reader.into_branch(),
                    });
                }
            } else {
                break;
            }

            p_reader = reader;
        }

        Err(Error::Todo)
    }
}

impl ParseTree for GFuncName {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (string, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;

        if string.is_empty() || string.chars().any(|c| (!c.is_alphanumeric() && c != '_')) {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GFuncName(string),
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GHeaderInclude {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (string, handle) = reader.read_until::<GSeparator>()?;

        if string != "#include" {
            return Err(Error::Todo);
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Check for opening quote `"`.
        let (_, reader) = ensure::<GDoubleQuote>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Read the fine path to include
        let (file_path, handle) = reader.read_until::<GDoubleQuote>()?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Check for closing quote `"`.
        let (_, reader) = ensure::<GDoubleQuote>(reader)?;

        Ok(DerivationResult {
            derived: GHeaderInclude(file_path.trim().to_string()),
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GHeaderPragma {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (string, handle) = reader.read_until::<GSeparator>()?;

        if string != "#pragma" {
            return Err(Error::Todo);
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Read string until newline
        let (string, handle) = reader.read_until::<GNewline>()?;

        // Consume newline character itself.
        let (_, reader) = ensure::<GNewline>(handle.commit())?;

        Ok(DerivationResult {
            derived: GHeaderPragma(string.trim().to_string()),
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GCommentLine {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Check for comment prefix
        let (string, handle) = reader.read_until::<GSeparator>()?;
        if string != "//" && string != "///" {
            return Err(Error::Todo);
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Read the comment.
        let (comment, handle) = reader.read_until::<GNewline>()?;

        // Consume newline character itself..
        let (_, reader) = ensure::<GNewline>(handle.commit())?;

        Ok(DerivationResult {
            derived: GCommentLine(comment),
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GCommentBlock {
    type Derivation = Self;

    fn derive(mut reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let mut lines = vec![];

        loop {
            let (pending, checked_out) = reader.checkout();
            if let Ok(res) = GCommentLine::derive(checked_out) {
                let (derived, branch) = (res.derived, res.branch);

                lines.push(derived);
                reader = pending.merge(branch);
            } else {
                reader = pending.discard();
                break;
            }
        }

        if lines.is_empty() {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GCommentBlock { lines },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GAnyLine {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (string, handle) = reader.read_until::<GNewline>()?;

        // Consume newline character itself..
        let (_, reader) = ensure::<GNewline>(handle.commit())?;

        Ok(DerivationResult {
            derived: GAnyLine(string),
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GFunctionDecl {
    type Derivation = Self;

    fn derive(mut p_reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let mut markers = vec![];
        let mut params = vec![];

        // Derive (optional) marker.
        let (pending, checked_out) = p_reader.checkout();
        if let Ok(marker_res) = GMarker::derive(checked_out) {
            markers.push(marker_res.derived);
            p_reader = pending.merge(marker_res.branch);
        } else {
            p_reader = pending.discard();
        }

        // Ignore leading separators.
        (_, p_reader) = wipe::<GSeparator>(p_reader);

        // Derive return value.
        let (return_der, reader) = ensure::<GReturnValue>(p_reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Derive function name.
        let (name_der, reader) = ensure::<GFuncName>(reader)?;

        // Check for opening bracket.
        (_, p_reader) = ensure::<GOpenBracket>(reader)?;

        loop {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(p_reader);

            // Derive and track parameter.
            let (derived, reader) = ensure::<GParamItem>(reader)?;
            params.push(derived);

            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(reader);

            // Check for potential comma, indicating next paremeter declaration.
            let (comma, reader) = wipe::<GComma>(reader);
            p_reader = reader;

            if comma.is_none() {
                break;
            }
        }

        // Ignore leading separators.
        (_, p_reader) = wipe::<GSeparator>(p_reader);

        // Check for closing bracket.
        (_, p_reader) = ensure::<GCloseBracket>(p_reader)?;

        // Ignore leading separators.
        let (_, mut p_reader) = wipe::<GSeparator>(p_reader);

        // Derive (optional) marker.
        let (pending, checked_out) = p_reader.checkout();
        if let Ok(marker_res) = GMarker::derive(checked_out) {
            markers.push(marker_res.derived);
            p_reader = pending.merge(marker_res.branch);
        } else {
            p_reader = pending.discard();
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(p_reader);

        // Check for required semicolon.
        let (_, reader) = ensure::<GSemicolon>(reader)?;

        Ok(DerivationResult {
            derived: GFunctionDecl {
                name: name_der,
                params,
                return_value: return_der,
                markers,
            },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GHeaderFileItem {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Check for newline.
        let (res, reader) = optional::<GNewline>(reader);
        if res.is_some() {
            return Ok(DerivationResult {
                derived: GHeaderFileItem::Newline,
                branch: reader.into_branch(),
            });
        }

        // Check for EOF.
        let (res, reader) = optional::<GEof>(reader);
        if res.is_some() {
            return Ok(DerivationResult {
                derived: GHeaderFileItem::Eof,
                branch: reader.into_branch(),
            });
        }

        // Check for comment block.
        let (res, reader) = optional::<GCommentBlock>(reader);
        if let Some(item) = res {
            return Ok(DerivationResult {
                derived: GHeaderFileItem::Comment(item),
                branch: reader.into_branch(),
            });
        }

        // Check for pragma header.
        let (res, reader) = optional::<GHeaderPragma>(reader);
        if let Some(item) = res {
            let (_, reader) = wipe::<GEndOfLine>(reader);

            return Ok(DerivationResult {
                derived: GHeaderFileItem::HeaderPragma(item),
                branch: reader.into_branch(),
            });
        }

        // Check for include header.
        let (res, reader) = optional::<GHeaderInclude>(reader);
        if let Some(item) = res {
            let (_, reader) = wipe::<GEndOfLine>(reader);
            return Ok(DerivationResult {
                derived: GHeaderFileItem::HeaderInclude(item),
                branch: reader.into_branch(),
            });
        }

        // Check for struct decleration.
        let (res, reader) = optional::<GStructDecl>(reader);
        if let Some(item) = res {
            let (_, reader) = wipe::<GEndOfLine>(reader);
            return Ok(DerivationResult {
                derived: GHeaderFileItem::StructDecl(item),
                branch: reader.into_branch(),
            });
        }

        // Check for function decleration.
        let (res, reader) = optional::<GFunctionDecl>(reader);
        if let Some(item) = res {
            let (_, reader) = wipe::<GEndOfLine>(reader);
            return Ok(DerivationResult {
                derived: GHeaderFileItem::FunctionDecl(item),
                branch: reader.into_branch(),
            });
        }

        // Check for markers.
        let (res, reader) = optional::<GMarker>(reader);
        if let Some(item) = res {
            let (_, reader) = wipe::<GEndOfLine>(reader);
            return Ok(DerivationResult {
                derived: GHeaderFileItem::Marker(item),
                branch: reader.into_branch(),
            });
        }

        // For all other, set value as `Unrecongized`.
        // TODO: Should this just return Err?
        let (string, handle) = reader.read_until::<GNewline>()?;
        let (_, reader) = wipe::<GEndOfLine>(handle.commit());
        return Ok(DerivationResult {
            derived: GHeaderFileItem::Unrecognized(string),
            branch: reader.into_branch(),
        });
    }
}

impl ParseTree for GType {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (string, handle) = reader.read_until::<GSeparator>()?;
        if string == "const" {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(handle.commit());

            let res = GTypeCategory::derive(reader)?;

            Ok(DerivationResult {
                derived: GType::Const(res.derived),
                branch: res.branch,
            })
        } else {
            let res = GTypeCategory::derive(handle.reset())?;

            Ok(DerivationResult {
                derived: GType::Mutable(res.derived),
                branch: res.branch,
            })
        }
    }
}

impl ParseTree for GTypeCategory {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        fn check_for_pointers(
            mut derived: GTypeCategory,
            mut p_reader: Reader,
        ) -> Result<(GTypeCategory, Reader)> {
            loop {
                // Ignore leading separators.
                let (_, reader) = wipe::<GSeparator>(p_reader);

                // Try pointer.
                // TODO: Implement `GAsterisk`?
                let (slice, handle) = reader.read_amt(1)?;
                if let Some(slice) = slice {
                    if slice == "*" {
                        p_reader = handle.commit();
                        derived = GTypeCategory::Pointer(Box::new(derived));
                        continue;
                    }
                }

                p_reader = handle.reset();
                break;
            }

            Ok((derived, p_reader))
        }

        // Handle scalar.
        let (pending, checked_out) = reader.checkout();
        if let Ok(ty_res) = GPrimitive::derive(checked_out) {
            let p_reader = pending.merge(ty_res.branch);

            // Prepare scala type, might get wrapped (multiple times) in pointer.
            let derived = GTypeCategory::Scalar(ty_res.derived);
            let (derived, reader) = check_for_pointers(derived, p_reader)?;

            return Ok(DerivationResult {
                derived,
                branch: reader.into_branch(),
            });
        }

        // Reset buffer.
        let reader = pending.discard();

        // Handle struct
        let (pending, checked_out) = reader.checkout();
        if let Ok(res) = GStruct::derive(checked_out) {
            let reader = pending.merge(res.branch);

            // Prepare scala type, might get wrapped (multiple times) in pointer.
            let derived = GTypeCategory::Struct(res.derived);
            let (derived, reader) = check_for_pointers(derived, reader)?;

            return Ok(DerivationResult {
                derived,
                branch: reader.into_branch(),
            });
        }

        // Reset buffer.
        let reader = pending.discard();

        // Handle enum
        let (pending, checked_out) = reader.checkout();
        if let Ok(res) = GEnum::derive(checked_out) {
            let reader = pending.merge(res.branch);

            // Prepare scala type, might get wrapped (multiple times) in pointer.
            let derived = GTypeCategory::Enum(res.derived);
            let (derived, reader) = check_for_pointers(derived, reader)?;

            return Ok(DerivationResult {
                derived,
                branch: reader.into_branch(),
            });
        }

        // Reset buffer.
        let reader = pending.discard();

        // Handle aggregate types.
        let (string, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;

        if string.is_empty() || string.chars().any(|c| (!c.is_alphanumeric() && c != '_')) {
            return Err(Error::Todo);
        }

        let mut derived = GTypeCategory::Unknown(string);

        // TODO: Unify with scalar, use single function.
        let mut p_reader = handle.commit();
        loop {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(p_reader);

            // Try pointer.
            // TODO: Implement `GAsterisk`?
            let (slice, handle) = reader.read_amt(1)?;
            if let Some(slice) = slice {
                if slice == "*" {
                    p_reader = handle.commit();
                    derived = GTypeCategory::Pointer(Box::new(derived));
                    continue;
                }
            }

            p_reader = handle.reset();
            break;
        }

        Ok(DerivationResult {
            derived,
            branch: p_reader.into_branch(),
        })
    }
}
