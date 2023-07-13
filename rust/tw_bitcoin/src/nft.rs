use crate::ordinals::OrdinalsInscription;
use crate::{Recipient, Result};
use bitcoin::PublicKey;

pub struct OrdinalNftInscription(OrdinalsInscription);

impl OrdinalNftInscription {
    // Constructs an [Ordinal inscription] with a given MIME type. Common MIME
    // types are:
    // * "application/json",
    // * "application/pdf",
    // * "image/gif",
    // * "image/jpeg",
    // * "image/png",
    // * "text/plain;charset=utf-8"
    // * ...
    //
    // [Ordinal inscription]: https://docs.ordinals.com/inscriptions.html
    pub fn new(mime_type: &[u8], data: &[u8], recipient: Recipient<PublicKey>) -> Result<Self> {
        OrdinalsInscription::new(mime_type, data, recipient).map(OrdinalNftInscription)
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
