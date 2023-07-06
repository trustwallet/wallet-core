use crate::ordinals::OrdinalsInscription;
use crate::{Recipient, Result};
use bitcoin::PublicKey;
use std::fmt::Display;

// Available inscription types, as specified in the `ord` repository.
#[repr(C)]
#[derive(Clone, Debug, Copy, Eq, PartialEq)]
pub enum ImageType {
    ApplicationJson,
    ApplicationPdf,
    ApplicationPgpSignature,
    ApplicationYaml,
    AudioFlac,
    AudioMpeg,
    AudioWav,
    ImageApng,
    ImageAvif,
    ImageGif,
    ImageJpeg,
    ImagePng,
    ImageSvgXml,
    ImageWebp,
    ModelGltfBinary,
    ModelStl,
    TextCss,
    TextHtml,
    TextJavascript,
    TextPlain,
    TextMarkdown,
    VideoMp4,
    VideoWebm,
}

impl Display for ImageType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        use ImageType::*;

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
    pub fn new(ty: ImageType, data: &[u8], recipient: Recipient<PublicKey>) -> Result<Self> {
        OrdinalsInscription::new(ty.to_string().as_bytes(), data, recipient).map(NftInscription)
    }
    pub fn inscription(&self) -> &OrdinalsInscription {
        &self.0
    }
}
