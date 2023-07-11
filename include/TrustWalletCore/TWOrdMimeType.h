// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// The Bitcoin inscription MIME type.
TW_EXPORT_ENUM()
enum TWOrdMimeType {
    TWOrdMimeTypeApplicationJson = 1,
    TWOrdMimeTypeApplicationPdf = 2,
    TWOrdMimeTypeApplicationPgpSignature = 3,
    TWOrdMimeTypeApplicationYaml = 4,
    TWOrdMimeTypeAudioFlac = 5,
    TWOrdMimeTypeAudioMpeg = 6,
    TWOrdMimeTypeAudioWav = 7,
    TWOrdMimeTypeImageApng = 8,
    TWOrdMimeTypeImageAvif = 9,
    TWOrdMimeTypeImageGif = 10,
    TWOrdMimeTypeImageJpeg = 11,
    TWOrdMimeTypeImagePng = 12,
    TWOrdMimeTypeImageSvgXml = 13,
    TWOrdMimeTypeImageWebp = 14,
    TWOrdMimeTypeModelGltfBinary = 15,
    TWOrdMimeTypeModelStl = 16,
    TWOrdMimeTypeTextCss = 17,
    TWOrdMimeTypeTextHtml = 18,
    TWOrdMimeTypeTextJavascript = 19,
    TWOrdMimeTypeTextPlain = 20,
    TWOrdMimeTypeTextMarkdown = 21,
    TWOrdMimeTypeVideoMp4 = 22,
    TWOrdMimeTypeVideoWebm = 23
};

TW_EXTERN_C_END
