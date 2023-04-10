use super::reader::{Reader, ReaderBranch, ReaderPending, ReaderStaged};
use super::{Error, Result};
use std::any::Any;

pub trait ParseTree {
    type Derivation;

    fn derive(reader: Reader) -> Result<DerivationResult<Self::Derivation>>;
}

// Convenience function. Removes a derived type from the reader, returning the
// updated reader.
fn wipe<T>(reader: Reader) -> (Option<T::Derivation>, Reader)
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
fn ensure<T>(reader: Reader) -> Result<(T::Derivation, Reader)>
where
    T: ParseTree,
{
    let (pending, checked_out) = reader.checkout();
    let res = T::derive(checked_out)?;
    Ok((res.derived, pending.merge(res.branch)))
}

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
    Pointer(Box<GTypeCategory>),
    // TODO: Add some sort of `GAnyKeyword` that can parse valid keywords.
    Unknown(String),
}

impl ParseTree for GTypeCategory {
    type Derivation = Self;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Handle scalar.
        let (pending, checked_out) = reader.checkout();
        if let Ok(ty_res) = GPrimitive::derive(checked_out) {
            let mut p_reader = pending.merge(ty_res.branch);

            // Prepare scala type, might get wrapped (multiple times) in pointer.
            let mut derived = GTypeCategory::Scalar(ty_res.derived);

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

            return Ok(DerivationResult {
                derived,
                branch: p_reader.into_branch(),
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

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GPrimitive {
    Void,
    Bool,
    Char,
    Int,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GStruct(String);

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
pub struct GParamItemWithMarker {
    pub ty: GPrimitive,
    pub marker: GMarker,
    pub name: GParamName,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GParamItemWithoutMarker {
    pub ty: GPrimitive,
    pub name: GParamName,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GParamName(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GMarker {
    TWVisibilityDefault,
    TwExportClass,
    TwExportStruct,
    TwExportEnum,
    TwExportFunc,
    TwExportMethod,
    TwExportProperty,
    TwExportStaticMethod,
    TwExportStaticProperty,
    TwMethodDiscardableResult,
    TwData,
    TwAssumeNonNullBegin,
    TwAssumeNonNullEnd,
    TwDeprecated,
    TwDeprecatedFor,
    Nullable,
    NonNull,
    NullUnspecified,
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
pub struct GFuncName(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GHeaderInclude(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GFunctionDecl {
    pub name: GFuncName,
    //params: Vec<EitherOr<GParamItemWithMarker, GParamItemWithoutMarker>>,
    pub params: Vec<GParamItemWithoutMarker>,
    pub return_ty: GPrimitive,
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
        dbg!(&slice);

        let derived = match slice.as_str() {
            "void" => GPrimitive::Void,
            "bool" => GPrimitive::Bool,
            "char" => GPrimitive::Char,
            "int" => GPrimitive::Int,
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
        dbg!("GOT HERE");

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Check for "struct" prefix.
        let (string, handle) = reader.read_until::<GSeparator>()?;
        dbg!(&string);
        if string != "struct" {
            return Err(Error::Todo);
        }

        dbg!("GOT HERE, TOO");

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(handle.commit());

        // Derive struct name
        let (name, handle) = reader.read_until::<EitherOr<GSeparator, GEof>>()?;
        dbg!(&name);
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

impl ParseTree for GParamItemWithMarker {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Derive parameter type.
        let (ty_derived, mut reader) = ensure::<GPrimitive>(reader)?;

        // Ignore leading separators.
        (_, reader) = wipe::<GSeparator>(reader);

        // Derive marker, ignore leading separators.
        let (marker_derived, reader) = ensure::<GMarker>(reader)?;

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Derive parameter name.
        let (name_derived, reader) = ensure::<GParamName>(reader)?;

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItemWithMarker {
                ty: ty_derived,
                marker: marker_derived,
                name: name_derived,
            },
            branch: reader.into_branch(),
        })
    }
}

impl ParseTree for GParamItemWithoutMarker {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Derive parameter type.
        let (ty_derived, mut reader) = ensure::<GPrimitive>(reader)?;

        // Ignore leading separators.
        (_, reader) = wipe::<GSeparator>(reader);

        // Derive parameter name.
        let (name_derived, reader) = ensure::<GParamName>(reader)?;

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItemWithoutMarker {
                ty: ty_derived,
                name: name_derived,
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
        let (return_der, reader) = ensure::<GPrimitive>(p_reader)?;

        dbg!("GOT HERE");

        // Ignore leading separators.
        let (_, reader) = wipe::<GSeparator>(reader);

        // Derive return value.
        let (name_der, reader) = ensure::<GFuncName>(reader)?;

        // Check for opening bracket.
        (_, p_reader) = ensure::<GOpenBracket>(reader)?;

        loop {
            // Ignore leading separators.
            let (_, reader) = wipe::<GSeparator>(p_reader);

            // Derive and track parameter.
            let (derived, reader) = ensure::<GParamItemWithoutMarker>(reader)?;
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

        Ok(DerivationResult {
            derived: GFunctionDecl {
                name: name_der,
                params,
                return_ty: return_der,
                markers,
            },
            branch: p_reader.into_branch(),
        })
    }
}
