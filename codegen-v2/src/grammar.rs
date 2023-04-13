use super::reader::{Reader, ReaderBranch};
use super::{Error, Result};
use crate::define_char;

define_char!(GComma, ",");
define_char!(GSemicolon, ";");
define_char!(GSpace, " ");
define_char!(GAssignment, "=");
define_char!(GForwardSlash, "/");
define_char!(GBackwardSlash, "\\");
define_char!(GAsterisk, "*");
define_char!(GOpenBracket, "(");
define_char!(GCloseBracket, ")");
define_char!(GOpenCurlyBracket, "{");
define_char!(GCloseCurlyBracket, "}");
define_char!(GDoubleQuote, "\"");
define_char!(GNewline, "\n");

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
// TODO: Alias to `wipe()`.
// TODO: Should this be wrapped in a `Result`?
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

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_type", content = "value")]
pub enum GHeaderFileItem {
    HeaderInclude(GHeaderInclude),
    HeaderPragma(GHeaderPragma),
    Comment(GCommentBlock),
    Define(GDefine),
    StaticVar(GStaticVar),
    Typedef(GTypedef),
    FunctionDecl(GFunctionDecl),
    StructDecl(GStructDecl),
    EnumDecl(GEnumDecl),
    Marker(GMarker),
    Newline,
    Eof,
    Unrecognized(String),
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GDefine {
    pub key: GKeyword,
    pub value: Option<String>,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GStaticVar {
    pub ty: GType,
    pub name: GKeyword,
    pub value: String,
    pub markers: GMarkers,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GHeaderInclude(pub String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GHeaderPragma(pub String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GTypedef {
    pub ty: GType,
    pub name: String,
    pub markers: GMarkers,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GCommentLine(String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GCommentBlock {
    lines: Vec<GCommentLine>,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GFuncName(String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GAnyLine(String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GInlineComment(String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GEndOfLine;

#[derive(Debug, Clone)]
pub struct DerivationResult<'a, T> {
    pub derived: T,
    pub branch: ReaderBranch<'a>,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_variant", content = "value")]
pub enum EitherOr<T, D> {
    Either(T),
    Or(D),
}

pub type GNonAlphanumeric = Continuum<GNonAlphanumericItem>;

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_variant", content = "value")]
pub enum GType {
    Mutable(GTypeCategory),
    Const(GTypeCategory),
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GKeyword(pub String);

// TODO: Should this wrap the item?
#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GNonAlphanumericItem;

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_variant", content = "value")]
pub enum GTypeCategory {
    Scalar(GPrimitive),
    Struct(GStructName),
    Enum(GEnum),
    Pointer(Box<GTypeCategory>),
    Unrecognized(GKeyword),
}

// TODO: Not complete (eg. "unsigned char", etc...)
#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_variant", content = "value")]
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
    UInt32T,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GStructName(String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GStructDecl {
    #[serde(rename = "struct")]
    struct_val: GStructName,
    markers: GMarkers,
}

// TODO: Rename?
// TODO: Should take GKeyword
#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GEnum(String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GEnumDecl {
    pub name: GKeyword,
    pub variants: Vec<(GKeyword, Option<usize>)>,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GEof;

pub type GSeparator = Continuum<GSeparatorItem>;

pub type GSpaces = Continuum<GSpace>;

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_variant", content = "value")]
pub enum GSeparatorItem {
    Space,
    Newline,
    Tab,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GReturnValue {
    #[serde(rename = "type")]
    pub ty: GType,
    pub markers: GMarkers,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GParamItem {
    #[serde(rename = "type")]
    pub ty: GType,
    pub markers: GMarkers,
    pub name: GParamName,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GParamName(String);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_variant", content = "value")]
pub enum GMarker {
    TWVisibilityDefault,
    TwExportClass,
    TwExportStruct,
    TwExportEnum(String),
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

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GMarkers(pub Vec<GMarker>);

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct GFunctionDecl {
    pub name: GFuncName,
    pub params: Vec<GParamItem>,
    pub return_value: GReturnValue,
    pub markers: GMarkers,
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
#[serde(tag = "g_variant", content = "value")]
pub enum Continuum<T> {
    Thing(T),
    Next(ContinuumNext<T>),
}

#[derive(Debug, Clone, Eq, PartialEq, Serialize, Deserialize)]
pub struct ContinuumNext<T> {
    thing: T,
    next: Box<Continuum<T>>,
}

// *** DERIVE IMPLEMENTATIONS ***

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

impl ParseTree for GEnumDecl {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        let (string, handle) = reader.read_until::<GSpace>()?;

        if string != "enum" {
            return Err(Error::Todo);
        }

        // Read the enum name.
        let (enum_name, reader) = ensure::<GKeyword>(handle.commit())?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for opening curly bracket.
        let (_, reader) = ensure::<GOpenCurlyBracket>(reader)?;

        // Parse variant(s)
        let mut variants = vec![];
        let mut p_reader = reader;
        loop {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(p_reader);

            // TODO: Should be parsed
            // Wipe (possible) comment.
            let (_, reader) = wipe::<GCommentLine>(reader);

            // Read variant name.
            let (field_name, reader) = optional::<GKeyword>(reader);
            if field_name.is_none() {
                p_reader = reader;
                break;
            }

            let field_name = field_name.unwrap();

            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(reader);

            // Check for possible assignment ("=").
            let (assignment, reader) = optional::<GAssignment>(reader);
            if assignment.is_none() {
                // Track variant without value.
                variants.push((field_name, None));

                // TODO: Should be parsed
                // Wipe (possible) inline comment.
                let (_, reader) = wipe::<GInlineComment>(reader);

                // Check for comma.
                let (comma, reader) = optional::<GComma>(reader);

                // TODO: Should be parsed
                // Wipe (possible) comment.
                let (_, reader) = wipe::<GCommentLine>(reader);

                p_reader = reader;

                // If there's no comma, we assume we're at the end of the enum.
                if comma.is_none() {
                    break;
                }

                continue;
            }

            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(reader);

            // Read variant value.
            let (number, reader) = ensure::<GKeyword>(reader)?;
            let number = if number.0.starts_with("0x") {
                usize::from_str_radix(&number.0[2..], 16).map_err(|_| Error::Todo)?
            } else {
                number.0.parse::<usize>().map_err(|_| Error::Todo)?
            };

            // Track variant with value.
            variants.push((field_name, Some(number)));

            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(reader);

            // Check for comma.
            let (comma, reader) = optional::<GComma>(reader);

            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(reader);

            // TODO: Should be parsed
            // Wipe (possible) inline comment.
            let (_, reader) = wipe::<GInlineComment>(reader);

            // TODO: Should be parsed
            // Wipe (possible) comment.
            let (_, reader) = wipe::<GCommentLine>(reader);

            p_reader = reader;

            // If there's no comma, we assume we're at the end of the enum.
            if comma.is_none() {
                break;
            }
        }

        if variants.is_empty() {
            return Err(Error::Todo);
        }

        let reader = p_reader;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for closing curly bracket.
        let (_, reader) = ensure::<GCloseCurlyBracket>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for required semicolon.
        let (_, reader) = ensure::<GSemicolon>(reader)?;

        Ok(DerivationResult {
            derived: GEnumDecl {
                name: enum_name,
                variants,
            },
            branch: reader.into_branch(),
        })
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

impl ParseTree for GStructName {
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
        let (name, reader) = ensure::<GKeyword>(reader)?;

        Ok(DerivationResult {
            derived: GStructName(name.0),
            branch: reader.into_branch(),
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
        let (name, reader) = ensure::<GKeyword>(reader)?;

        Ok(DerivationResult {
            derived: GEnum(name.0),
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GStructDecl {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Derive (optional) markers.
        let (markers, reader) = ensure::<GMarkers>(reader)?;

        // Derive struct declaration.
        let (struct_res, reader) = ensure::<GStructName>(reader)?;

        // Check for required semicolon.
        let (_, reader) = ensure::<GSemicolon>(reader)?;

        Ok(DerivationResult {
            derived: GStructDecl {
                struct_val: struct_res,
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
        let (ty, reader) = ensure::<GType>(reader)?;

        // Derive (optional) markers.
        let (markers, reader) = ensure::<GMarkers>(reader)?;

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GReturnValue { ty, markers },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GParamItem {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        // Derive parameter type.
        let (ty_derived, reader) = ensure::<GType>(reader)?;

        // Derive (optional) markers.
        let (markers, reader) = ensure::<GMarkers>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

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
            "TW_EXPORT_ENUM" => {
                let (_, reader) = wipe::<GSeparator>(handle.commit());

                let (_, reader) = ensure::<GOpenBracket>(reader)?;
                let (string, handle) = reader.read_until::<GCloseBracket>()?;
                let (_, reader) = ensure::<GCloseBracket>(handle.commit())?;

                return Ok(DerivationResult {
                    derived: GMarker::TwExportEnum(string),
                    branch: reader.into_branch(),
                });
            },
            "TW_EXPORT_FUNC" => GMarker::TwExportFunc,
            "TW_EXPORT_METHOD" => GMarker::TwExportMethod,
            "TW_EXPORT_PROPERTY" => GMarker::TwExportProperty,
            "TW_EXPORT_STATIC_METHOD" => GMarker::TwExportStaticMethod,
            "TW_EXPORT_STATIC_PROPERTY" => GMarker::TwExportStaticProperty,
            "TW_METHOD_DISCARDABLE_RESULT" => GMarker::TwMethodDiscardableResult,
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

impl ParseTree for GMarkers {
    type Derivation = Self;

    fn derive(mut p_reader: Reader) -> Result<DerivationResult<Self::Derivation>> {
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

        Ok(DerivationResult {
            derived: GMarkers(markers),
            branch: p_reader.into_branch(),
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
                return Err(Error::Todo);
            }

            p_reader = reader;
        }
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

impl ParseTree for GDefine {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        let (string, handle) = reader.read_until::<GSpace>()?;

        if string != "#define" {
            return Err(Error::Todo);
        }

        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(handle.commit());

        // Retrieve the key name.
        let (key, reader) = ensure::<GKeyword>(reader)?;

        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        // Retrieve the value itself, read until newline.
        let (value, reader) = ensure::<GAnyLine>(reader)?;

        let value = value.0.trim().to_string();
        let value = if value.is_empty() { None } else { Some(value) };

        Ok(DerivationResult {
            derived: GDefine { key, value },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GStaticVar {
    type Derivation = Self;

    fn derive(reader: Reader) -> Result<DerivationResult<Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        let (string, handle) = reader.read_until::<GSeparator>()?;
        if string != "static" {
            return Err(Error::Todo);
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Retrieve the type.
        let (ty, reader) = ensure::<GType>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Retrieve markers.
        let (markers, reader) = ensure::<GMarkers>(reader)?;

        // Retrieve the name.
        let (var_name, reader) = ensure::<GKeyword>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for assignment ("=").
        let (_, reader) = ensure::<GAssignment>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check and consume for doube quote.
        let (quote, reader) = optional::<GDoubleQuote>(reader);

        // TODO: This should be determiend by checking the type, not whether
        // there's a quote or not.
        let (var_value, reader) = if quote.is_some() {
            // Read until doubleq uote.
            let (var_value, handle) = reader.read_until::<GDoubleQuote>()?;

            // Check and consume for quote.
            let (_, reader) = ensure::<GDoubleQuote>(handle.commit())?;

            (var_value, reader)
        } else {
            // Read to end .
            let (var_value, handle) = reader.read_until::<GSemicolon>()?;
            (var_value, handle.commit())
        };

        // Consume semicolon.
        let (_, reader) = ensure::<GSemicolon>(reader)?;

        Ok(DerivationResult {
            derived: GStaticVar {
                ty,
                name: var_name,
                value: var_value,
                markers,
            },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GHeaderInclude {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

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

        // Consume closing quote.
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

impl ParseTree for GTypedef {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        let (string, handle) = reader.read_until::<GSeparator>()?;

        if string != "typedef" {
            return Err(Error::Todo);
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Read type
        let (ty, reader) = ensure::<GType>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Derive (optional) markers.
        let (markers, reader) = ensure::<GMarkers>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Read typedef name.
        let (keyword, reader) = ensure::<GKeyword>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Consume semicolon.
        let (_, reader) = ensure::<GSemicolon>(reader)?;

        Ok(DerivationResult {
            // TODO: `name` should be `GKeyword`.
            derived: GTypedef {
                ty,
                name: keyword.0,
                markers,
            },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GKeyword {
    type Derivation = Self;

    fn derive(reader: Reader) -> Result<DerivationResult<Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        let (string, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;

        if string.is_empty()
            || string
                .chars()
                .any(|c| (!c.is_alphanumeric() && c != '_' && c != '-'))
        {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GKeyword(string),
            branch: handle.commit().into_branch(),
        })
    }
}

impl ParseTree for GCommentLine {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

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
        let (string, handle) = reader.read_until::<EitherOr<GNewline, GEof>>()?;

        // Consume newline character itself..
        let (_, reader) = wipe::<GNewline>(handle.commit());

        Ok(DerivationResult {
            derived: GAnyLine(string),
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GInlineComment {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Ignore leading spaces.
        let (_, reader) = wipe::<GSpaces>(reader);

        let (_, reader) = ensure::<GForwardSlash>(reader)?;
        let (_, reader) = ensure::<GAsterisk>(reader)?;

        // TODO: Should have some sort of read_until_str` method.
        // TODO: Does not handle `/* some * word */`.
        let (comment, handle) = reader.read_until::<GAsterisk>()?;
        let reader = handle.commit();

        // Consume ending characters.
        let (_, reader) = ensure::<GAsterisk>(reader)?;
        let (_, reader) = ensure::<GForwardSlash>(reader)?;

        Ok(DerivationResult {
            derived: GInlineComment(comment.trim().to_string()),
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GFunctionDecl {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let mut params = vec![];

        // Derive (optional) marker.
        let (mut markers, reader) = ensure::<GMarkers>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Derive return value.
        let (return_der, reader) = ensure::<GReturnValue>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Derive function name.
        let (name_der, reader) = ensure::<GFuncName>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for opening bracket.
        let (_, mut reader) = ensure::<GOpenBracket>(reader)?;

        // Derive parameters
        loop {
            // Ignore leading separators.
            let (_, r) = wipe::<GSeparator>(reader);

            // Derive and track parameter.
            let (derived, r) = ensure::<GParamItem>(r)?;
            params.push(derived);

            // Ignore leading separators.
            let (_, r) = wipe::<GSeparator>(r);

            // Check for potential comma, indicating next paremeter declaration.
            let (comma, r) = wipe::<GComma>(r);
            reader = r;

            if comma.is_none() {
                break;
            }
        }

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for closing bracket.
        let (_, reader) = ensure::<GCloseBracket>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Derive (optional) marker at the end of the function.
        let (markers2, reader) = ensure::<GMarkers>(reader)?;

        // Merge with first marker check.
        markers.0.extend(markers2.0);

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

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

        // Check for define statement.
        let (res, reader) = optional::<GDefine>(reader);
        if let Some(item) = res {
            return Ok(DerivationResult {
                derived: GHeaderFileItem::Define(item),
                branch: reader.into_branch(),
            });
        }

        // Check for define statement.
        let (res, reader) = optional::<GStaticVar>(reader);
        if let Some(item) = res {
            return Ok(DerivationResult {
                derived: GHeaderFileItem::StaticVar(item),
                branch: reader.into_branch(),
            });
        }

        // Check for typedef statement.
        let (res, reader) = optional::<GTypedef>(reader);
        if let Some(item) = res {
            let (_, reader) = wipe::<GEndOfLine>(reader);
            return Ok(DerivationResult {
                derived: GHeaderFileItem::Typedef(item),
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

        // Check for enum decleration.
        let (res, reader) = optional::<GEnumDecl>(reader);
        if let Some(item) = res {
            let (_, reader) = wipe::<GEndOfLine>(reader);
            return Ok(DerivationResult {
                derived: GHeaderFileItem::EnumDecl(item),
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
        if let Ok(res) = GStructName::derive(checked_out) {
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

        // All other is handled as `GTypeCategory::Unrecognized`
        let (pending, checked_out) = reader.checkout();
        if let Ok(res) = GKeyword::derive(checked_out) {
            let reader = pending.merge(res.branch);

            // Prepare scala type, might get wrapped (multiple times) in pointer.
            let derived = GTypeCategory::Unrecognized(res.derived);
            let (derived, reader) = check_for_pointers(derived, reader)?;

            return Ok(DerivationResult {
                derived,
                branch: reader.into_branch(),
            });
        }

        Err(Error::Todo)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    impl From<&str> for GKeyword {
        fn from(string: &str) -> Self {
            GKeyword(string.to_string())
        }
    }

    impl From<&str> for GInlineComment {
        fn from(string: &str) -> Self {
            GInlineComment(string.to_string())
        }
    }

    impl From<&str> for GParamName {
        fn from(string: &str) -> Self {
            GParamName(string.to_string())
        }
    }

    impl From<&str> for GFuncName {
        fn from(string: &str) -> Self {
            GFuncName(string.to_string())
        }
    }

    impl From<&str> for GStructName {
        fn from(string: &str) -> Self {
            GStructName(string.to_string())
        }
    }
}
