use std::fmt::{write, Display};

use crate::ordinals::OrdinalsInscription;
use crate::{Error, Recipient, Result};
use bitcoin::PublicKey;

pub enum ImageType {
    Png,
    Jpeg,
    Gif,
    Webp,
}

impl Display for ImageType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let str = match self {
            ImageType::Png => "png",
            ImageType::Jpeg => "jpeg",
            ImageType::Gif => "gif",
            ImageType::Webp => "webp",
        };

        write!(f, "image/{}", str)
    }
}

pub struct NftInscription(OrdinalsInscription);

impl NftInscription {
    pub fn new_image(data: &[u8], ty: ImageType, recipient: Recipient<PublicKey>) -> Result<Self> {
        OrdinalsInscription::new(ty.to_string().as_bytes(), data, recipient).map(NftInscription)
    }
}
