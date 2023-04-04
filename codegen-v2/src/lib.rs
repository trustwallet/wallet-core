use std::io::{BufRead, BufReader, Read};

type Result<T, R> = std::result::Result<T, Error<R>>;

trait ParseTree {
    type Derivation;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R>;
}

struct DerivationResult<T, R> {
    derived: T,
    driver: Driver<R>,
}

struct Error<R> {
    ty: ErrorType,
    driver: Driver<R>,
}

impl<R: Read> Error<R> {
    fn new(ty: ErrorType, driver: Driver<R>) -> Self {
        Error { ty, driver }
    }
}

enum ErrorType {
    Todo,
}

impl<'a> From<&'a [u8]> for Driver<&'a [u8]> {
    fn from(input: &'a [u8]) -> Self {
        Driver {
            reader: BufReader::new(input),
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

        for counter in 0..buffer.len() {
            let slice = &string[counter..];

            let driver = Driver::from(slice.as_bytes());
            if P::derive(driver).is_ok() {
                return Ok((
                    slice.to_string(),
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
        let buffer = &self.reader.fill_buf().unwrap()[..amt];
        let slice = std::str::from_utf8(buffer).unwrap();

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
    fn revert(self) -> Driver<R> {
        // Do nothing with `amt_read`.
        Driver {
            reader: self.reader,
        }
    }
}

enum GType {
    Bool,
    Char,
    Int,
    Struct(GStruct),
}

impl ParseTree for GType {
    type Derivation = Self;

    fn derive<R: Read>(mut driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        let (slice, handle) = driver.read_until::<GSeparator>()?;

        let derived = match slice.as_str() {
            "bool" => GType::Bool,
            "char" => GType::Char,
            "int" => GType::Int,
            _ => {
                driver = handle.revert();
                let der_result = GStruct::derive(driver)?;
                driver = der_result.driver;

                GType::Struct(der_result.derived)
            }
        };

        Ok(DerivationResult { derived, driver })
    }
}

struct GStruct;

impl ParseTree for GStruct {
    type Derivation = Self;

    fn derive<R: Read>(mut driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
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

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        let mut sep_items: Option<GSeparator> = None;
        loop {
            if let Ok(item) = GSeparatorItem::derive(driver) {
                if let Some(sep) = sep_items {
                    sep_items = Some(sep.add(item));
                }
            } else if let Some(items) = sep_items {
                return Ok(items);
            } else {
                return Err(Error::new(ErrorType::Todo, driver));
            }
        }
    }
}

enum GSeparatorItem {
    Space,
    Newline,
    Tab,
}

impl ParseTree for GSeparatorItem {
    type Derivation = Self;

    fn derive<R: Read>(driver: Driver<R>) -> Result<DerivationResult<Self::Derivation, R>, R> {
        let (slice, handle) = driver.read_amt(1)?;
        let slice = slice.ok_or(Error::new(ErrorType::Todo, handle.revert()))?;

        let derived = match slice.as_str() {
            " " => GSeparatorItem::Space,
            "\n" => GSeparatorItem::Newline,
            "\t" => GSeparatorItem::Tab,
            _ => return Err(Error::new(ErrorType::Todo, handle.revert())),
        };

        let driver = handle.commit();

        Ok(DerivationResult { derived, driver })
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
