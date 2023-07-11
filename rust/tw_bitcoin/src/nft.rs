use crate::ordinals::OrdinalsInscription;
use crate::{Recipient, Result};
use bitcoin::PublicKey;
use std::fmt::Display;

// Available inscription types, as specified in the `ord` repository.
#[repr(C)]
#[derive(Clone, Debug, Copy, Eq, PartialEq)]
pub enum MimeType {
    ApplicationJson = 1,
    ApplicationPdf = 2,
    ApplicationPgpSignature = 3,
    ApplicationYaml = 4,
    AudioFlac = 5,
    AudioMpeg = 6,
    AudioWav = 7,
    ImageApng = 8,
    ImageAvif = 9,
    ImageGif = 10,
    ImageJpeg = 11,
    ImagePng = 12,
    ImageSvgXml = 13,
    ImageWebp = 14,
    ModelGltfBinary = 15,
    ModelStl = 16,
    TextCss = 17,
    TextHtml = 18,
    TextJavascript = 19,
    TextPlain = 20,
    TextMarkdown = 21,
    VideoMp4 = 22,
    VideoWebm = 23,
}

impl Display for MimeType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        use MimeType::*;

        let str = match self {
            ApplicationJson => "application/json",
            ApplicationPdf => "application/pdf",
            ApplicationPgpSignature => "application/pgp-signature",
            ApplicationYaml => "application/yaml",
            AudioFlac => "audio/flac",
            AudioMpeg => "audio/mpeg",
            AudioWav => "audio/wav",
            ImageApng => "image/apng",
            ImageAvif => "image/avif",
            ImageGif => "image/gif",
            ImageJpeg => "image/jpeg",
            ImagePng => "image/png",
            ImageSvgXml => "image/svg+xml",
            ImageWebp => "image/webp",
            ModelGltfBinary => "model/gltf-binary",
            ModelStl => "model/stl",
            TextCss => "text/css",
            TextHtml => "text/html;charset=utf-8",
            TextJavascript => "text/javascript",
            TextPlain => "text/plain;charset=utf-8",
            TextMarkdown => "text/markdown;charset=utf-8",
            VideoMp4 => "video/mp4",
            VideoWebm => "video/webm",
        };

        write!(f, "{}", str)
    }
}

pub struct NftInscription(OrdinalsInscription);

impl NftInscription {
    pub fn new(ty: MimeType, data: &[u8], recipient: Recipient<PublicKey>) -> Result<Self> {
        OrdinalsInscription::new(ty.to_string().as_bytes(), data, recipient).map(NftInscription)
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
