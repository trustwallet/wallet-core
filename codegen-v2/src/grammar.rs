use super::reader::{Reader, ReaderBranch, ReaderPending, ReaderStaged};
use super::{Error, Result};
use std::any::Any;

pub trait ParseTree {
    type Derivation;

    fn derive(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>>;
}

fn wipe<'a, T>(reader: Reader<'a>) -> Reader<'a>
where
    T: ParseTree,
{
    let (pending, checked_out) = reader.checkout();
    if let Ok(res) = T::derive(checked_out) {
        pending.merge(res.branch)
    } else {
        pending.discard()
    }
}

fn ensure<'a, T>(reader: Reader<'a>) -> Result<(T::Derivation, Reader<'a>)>
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

// TODO: Rename
// TODO: Should this wrap the item?
#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GNonAlphanumericItem;

impl ParseTree for GNonAlphanumericItem {
    type Derivation = Self;

    fn derive(mut reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let (pending, checked_out) = reader.checkout();
        if let Ok(res) = GEof::derive(checked_out) {
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

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GType {
    Bool,
    Char,
    Int,
    Struct(GStruct),
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GStruct;

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GEof;

pub type GSeparator = Continuum<GSeparatorItem>;

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum GSeparatorItem {
    Space,
    Newline,
    Tab,
}

pub enum GParamItem {}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GParamItemWithMarker {
    pub ty: GType,
    pub marker: GMarker,
    pub name: GParamName,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GParamItemWithoutMarker {
    pub ty: GType,
    pub name: GParamName,
}

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GParamName(String);

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GMarker(String);

pub struct GComma;

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

#[derive(Debug, Clone, Eq, PartialEq)]
pub struct GFuncParams {
    //params: Vec<EitherOr<GParamItemWithMarker, GParamItemWithoutMarker>>,
    params: Vec<GParamItemWithoutMarker>,
}

impl ParseTree for GFuncParams {
    type Derivation = Self;

    fn derive(mut reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        let mut params = vec![];

        loop {
            let (pending, checked_out) = reader.checkout();
            //let res = EitherOr::<GParamItemWithMarker, GParamItemWithoutMarker>::derive(checked_out)?;
            let res = GParamItemWithoutMarker::derive(checked_out)?;
            reader = pending.merge(res.branch);

            params.push(res.derived);

            let (pending, checked_out) = reader.checkout();
            if let Ok(res) = GComma::derive(checked_out) {
                reader = pending.merge(res.branch);
            } else {
                reader = pending.discard();
                break;
            }
        }

        Ok(DerivationResult {
            derived: GFuncParams { params },
            branch: reader.into_branch(),
        })
    }
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

impl From<String> for GMarker {
    fn from(string: String) -> Self {
        GMarker(string)
    }
}

impl From<String> for GParamName {
    fn from(string: String) -> Self {
        GParamName(string)
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

impl ParseTree for GType {
    type Derivation = Self;

    fn derive<'a>(mut reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Read data until the separator is reached, then return the sub-slice
        // leading up to it.
        let (slice, handle) = reader.read_until::<EitherOr<GNonAlphanumeric, GEof>>()?;
        dbg!(&slice);

        let derived = match slice.as_str() {
            "bool" => GType::Bool,
            "char" => GType::Char,
            "int" => GType::Int,
            _ => {
                // Rollback reader, retry with the next derivation attempt.
                let reader = handle.reset();

                if let Ok(res) = GStruct::derive(reader) {
                    return Ok(DerivationResult {
                        derived: GType::Struct(res.derived),
                        branch: res.branch,
                    });
                }

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

    fn derive<'a>(_driver: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        todo!()
    }
}

impl ParseTree for GSeparatorItem {
    type Derivation = Self;

    fn derive<'a>(reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // TODO: Make use of `GEof` here.
        let (slice, handle) = reader.read_amt(1)?;
        dbg!(&slice, &handle);
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
        let (ty_derived, mut reader) = ensure::<GType>(reader)?;

        // Ignore leading separators.
        reader = wipe::<EitherOr<GSeparator, GEof>>(reader);

        // Derive marker, ignore leading separators.
        let (marker_derived, mut reader) = ensure::<GMarker>(reader)?;

        // Ignore leading separators.
        reader = wipe::<EitherOr<GSeparator, GEof>>(reader);

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

    fn derive<'a>(mut reader: Reader<'_>) -> Result<DerivationResult<'_, Self::Derivation>> {
        // Derive parameter type.
        let (pending, checked_out) = reader.checkout();
        let ty_res = GType::derive(checked_out)?;
        reader = pending.merge(ty_res.branch);

        // Ignore leading separators.
        let (pending, checked_out) = reader.checkout();
        if let Ok(res) = EitherOr::<GSeparator, GEof>::derive(checked_out) {
            reader = pending.merge(res.branch);
        } else {
            reader = pending.discard();
        }

        // Derive parameter name.
        let name_res = GParamName::derive(reader)?;

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItemWithoutMarker {
                ty: ty_res.derived,
                name: name_res.derived,
            },
            branch: name_res.branch,
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
        let (string, handle) = reader.read_until::<GNonAlphanumeric>()?;

        if string.is_empty() || string.chars().any(|c| (!c.is_alphanumeric() && c != '_')) {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GMarker(string),
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
            dbg!(&reader);
            let (pending, checked_out) = reader.checkout();
            if let Ok(res) = T::derive(checked_out) {
                dbg!(&res.branch);
                reader = pending.merge(res.branch);
                dbg!(&reader);

                if let Some(sep) = sep_items {
                    sep_items = Some(sep.add(res.derived));
                } else {
                    sep_items = Some(Continuum::Thing(res.derived));
                }
            } else {
                dbg!(&sep_items);
                if let Some(items) = sep_items {
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
}
