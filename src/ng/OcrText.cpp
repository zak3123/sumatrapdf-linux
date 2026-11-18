/* Copyright 2026 the SumatraPDF project authors (see AUTHORS file).
   License: GPLv3 */

#include "base/Base.h"
#include "base/File.h"

#include "OcrText.h"

static TempStr OcrDataDirIfValidTemp(Str dir) {
    if (len(dir) == 0 || !file::Exists(path::JoinTemp(dir, StrL("eng.traineddata")))) {
        return {};
    }
    return str::DupTemp(dir);
}

TempStr FindOcrDataDirTemp() {
    const char* env = getenv("TESSDATA_PREFIX");
    TempStr result;
    if (env) {
        result = OcrDataDirIfValidTemp(Str(env));
        if (result) {
            return result;
        }
        result = OcrDataDirIfValidTemp(path::JoinTemp(Str(env), StrL("tessdata")));
        if (result) {
            return result;
        }
    }

    result = OcrDataDirIfValidTemp(path::JoinTemp(GetSelfExeDirTemp(), StrL("tessdata")));
    if (result) {
        return result;
    }
    const char* home = getenv("HOME");
    if (home) {
        result = OcrDataDirIfValidTemp(path::JoinTemp(Str(home), StrL(".local/share/sumatrapdf/tessdata")));
        if (result) {
            return result;
        }
    }

    const char* systemDirs[] = {
        "/usr/share/tesseract/tessdata",
        "/usr/share/tesseract-ocr/5/tessdata",
        "/usr/share/tesseract-ocr/4.00/tessdata",
        "/usr/share/tessdata",
        "/usr/local/share/tessdata",
        "/app/share/tessdata",
    };
    for (const char* dir : systemDirs) {
        result = OcrDataDirIfValidTemp(Str(dir));
        if (result) {
            return result;
        }
    }
    return {};
}

TempStr FindOcrLanguagesTemp(Str dataDir) {
    bool hasEnglish = file::Exists(path::JoinTemp(dataDir, StrL("eng.traineddata")));
    bool hasJapanese = file::Exists(path::JoinTemp(dataDir, StrL("jpn.traineddata")));
    if (hasJapanese && hasEnglish) {
        return StrL("jpn+eng");
    }
    if (hasJapanese) {
        return StrL("jpn");
    }
    return hasEnglish ? StrL("eng") : TempStr{};
}
