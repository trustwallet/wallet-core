use std::io::{BufRead, BufReader, Read};

#[cfg(test)]
mod tests;

type Result<T, R> = std::result::Result<T, Error<R>>;

trait ParseTree {
    type Derivation;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R>;
}

struct DerivationResult<T, R> {
    derived: T,
    driver: Driver<R>,
}

impl<T: std::fmt::Debug, R> std::fmt::Debug for DerivationResult<T, R> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("DerivationResult")
            .field("derived", &self.derived)
            .finish()
    }
}

struct Error<R> {
    ty: ErrorType,
    driver: Driver<R>,
}

impl<R> std::fmt::Debug for Error<R> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("Error").field("type", &self.ty).finish()
    }
}

impl<R: Read> Error<R> {
    fn new(ty: ErrorType, driver: Driver<R>) -> Self {
        Error { ty, driver }
    }
}

#[derive(Debug)]
enum ErrorType {
    Todo,
}

impl<'a> From<&'a [u8]> for Driver<&'a [u8]> {
    fn from(buffer: &'a [u8]) -> Self {
        Driver {
            reader: BufReader::new(buffer),
        }
    }
}

impl<'a> From<&'a str> for Driver<&'a [u8]> {
    fn from(buffer: &'a str) -> Self {
        Driver {
            reader: BufReader::new(buffer.as_bytes()),
        }
    }
}

struct Driver<R> {
    reader: BufReader<R>,
}

impl<R: Read> Driver<R> {
    fn read_until<P>(mut self) -> Result<(String, DriverUsed<R>), R>
    where
        P: ParseTree,
    {
        let buffer = self.reader.fill_buf().unwrap();
        let string = std::str::from_utf8(buffer).unwrap();

        for counter in 0..=buffer.len() {
            let slice = &string[counter..];

            dbg!(counter, slice);

            let driver = Driver::from(slice.as_bytes());
            if P::derive(driver).is_ok() {
                let target = string[..counter].to_string();
                dbg!(&target);

                return Ok((
                    target,
                    DriverUsed {
                        reader: self.reader,
                        amt_read: counter,
                    },
                ));
            }
        }

        Err(Error::new(ErrorType::Todo, self))
    }
    fn read_amt(mut self, amt: usize) -> Result<(Option<String>, DriverUsed<R>), R> {
        let buffer = &self.reader.fill_buf().unwrap();
        if buffer.len() < amt {
            return Ok((
                None,
                DriverUsed {
                    reader: self.reader,
                    amt_read: 0,
                },
            ));
        }
        let buffer = &buffer[..amt];
        let slice = std::str::from_utf8(buffer).unwrap();
        dbg!(slice);

        if slice.is_empty() {
            return Ok((
                None,
                DriverUsed {
                    reader: self.reader,
                    amt_read: 0,
                },
            ));
        }

        Ok((
            Some(slice.to_string()),
            DriverUsed {
                reader: self.reader,
                amt_read: amt,
            },
        ))
    }
}

// TODO: Rename?
trait DriverExtractor<R> {
    fn ignore_result(self) -> Driver<R>;
}

impl<T, R> DriverExtractor<R> for Result<DerivationResult<T, R>, R> {
    fn ignore_result(self) -> Driver<R> {
        match self {
            Ok(res) => res.driver,
            Err(err) => err.driver,
        }
    }
}

struct DriverUsed<R> {
    reader: BufReader<R>,
    amt_read: usize,
}

impl<R: Read> DriverUsed<R> {
    fn commit(mut self) -> Driver<R> {
        self.reader.consume(self.amt_read);
        Driver {
            reader: self.reader,
        }
    }
    fn rollback(self) -> Driver<R> {
        // Do nothing with `amt_read`.
        Driver {
            reader: self.reader,
        }
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
enum GType {
    Bool,
    Char,
    Int,
    Struct(GStruct),
}

impl ParseTree for GType {
    type Derivation = Self;

    fn derive<R: Read>(mut driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        // Read data until the separator is reached, then return the sub-slice
        // leading up to it.
        let (slice, handle) = driver.read_until::<GSeparator>()?;
        dbg!(&slice);

        let derived = match slice.as_str() {
            "bool" => GType::Bool,
            "char" => GType::Char,
            "int" => GType::Int,
            _ => {
                // Rollback driver, retry with the next derivation attempt.
                let mut driver = handle.rollback();

                match GStruct::derive(driver) {
                    Ok(der) => {
                        return Ok(DerivationResult {
                            derived: GType::Struct(der.derived),
                            driver: der.driver,
                        })
                    }
                    Err(err) => driver = err.driver,
                }

                return Err(Error::new(ErrorType::Todo, driver));
            }
        };

        // Commit driver.
        driver = handle.commit();

        Ok(DerivationResult { derived, driver })
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GStruct;

impl ParseTree for GStruct {
    type Derivation = Self;

    fn derive<R: Read>(_driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        todo!()
    }
}

#[derive(Debug)]
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

    fn derive<R: Read>(mut driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        let mut sep_items: Option<GSeparator> = None;
        let mut is_eof = false;
        loop {
            match GSeparatorItem::derive(driver) {
                Ok(der_result) => {
                    driver = der_result.driver;

                    if let GSeparatorItem::Eof = der_result.derived {
                        is_eof = true;
                    }

                    if let Some(sep) = sep_items {
                        sep_items = Some(sep.add(der_result.derived));
                    } else {
                        sep_items = Some(GSeparator::Item(der_result.derived));
                    }

                    if is_eof {
                        return Ok(DerivationResult {
                            derived: sep_items.unwrap(),
                            driver,
                        });
                    }
                }
                Err(err) => {
                    if let Some(items) = sep_items {
                        return Ok(DerivationResult {
                            derived: items,
                            driver: err.driver,
                        });
                    } else {
                        return Err(Error::new(err.ty, err.driver));
                    }
                }
            }
        }
    }
}

#[derive(Debug)]
enum GSeparatorItem {
    Space,
    Newline,
    Tab,
    Eof,
}

impl ParseTree for GSeparatorItem {
    type Derivation = Self;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        let (slice, handle) = driver.read_amt(1)?;
        dbg!(&slice);
        let slice = match slice {
            Some(string) => string,
            None => {
                return Ok(DerivationResult {
                    derived: GSeparatorItem::Eof,
                    driver: handle.commit(),
                });
            }
        };

        let derived = match slice.as_str() {
            " " => GSeparatorItem::Space,
            "\n" => GSeparatorItem::Newline,
            "\t" => GSeparatorItem::Tab,
            _ => return Err(Error::new(ErrorType::Todo, handle.rollback())),
        };

        let driver = handle.commit();

        Ok(DerivationResult { derived, driver })
    }
}

#[derive(Debug)]
struct GSeparatorContinued {
    item: GSeparatorItem,
    next: Box<GSeparator>,
}

enum GParam {
    Item(GParamItem),
    Continued(GParamContinued),
}

enum GParamItem {}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GParamItemWithMarker {
    ty: GType,
    marker: GMarker,
    name: GParamName,
}

impl ParseTree for GParamItemWithMarker {
    type Derivation = Self;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        // Derive parameter type, ignore leading separator.
        let ty_der = GType::derive(driver)?;
        dbg!(&ty_der);
        let driver = GSeparator::derive(ty_der.driver).ignore_result();

        // Derive marker, ignore leading separator.
        let marker_der = GMarker::derive(driver)?;
        dbg!(&marker_der);
        let driver = GSeparator::derive(marker_der.driver).ignore_result();

        // Derive parameter name, ignore leading separator.
        let name_der = GParamName::derive(driver)?;
        dbg!(&name_der);

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItemWithMarker {
                ty: ty_der.derived,
                marker: marker_der.derived,
                name: name_der.derived,
            },
            driver: name_der.driver,
        })
    }
}

struct GParamItemWithoutMarker {
    ty: GType,
    name: GParamName,
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GParamName(String);

impl ParseTree for GParamName {
    type Derivation = Self;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        let (string, handle) = driver.read_until::<GSeparator>()?;

        Ok(DerivationResult {
            derived: GParamName(string),
            driver: handle.commit(),
        })
    }
}

struct GParamContinued {
    item: GParamItem,
    next: Box<GParam>,
}

impl ParseTree for GParam {
    type Derivation = Self;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        todo!()
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GMarker(String);

impl ParseTree for GMarker {
    type Derivation = Self;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        let (string, handle) = driver.read_until::<GSeparator>()?;

        Ok(DerivationResult {
            derived: GMarker(string),
            driver: handle.commit(),
        })
    }
}
