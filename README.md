# SumatraPDF Linux + OCR

Native Linux build of [SumatraPDF](https://github.com/sumatrapdfreader/sumatrapdf), based on upstream commit `b1a8e22577c9e35064315d279fc33ba6b67c5bef`.

## Download

Open [GitHub Actions](https://github.com/zak3123/sumatrapdf-linux/actions/workflows/build-linux.yml), choose the newest successful **Build SumatraPDF Linux** run, and download the **SumatraPDF-linux-x86_64** artifact.

## Linux changes

- Native Linux desktop binary built from the official upstream `src/ng` port.
- Scanned PDF pages receive a local Tesseract text layer when no embedded text exists.\n- English and Japanese OCR models are bundled; the app automatically uses `jpn+eng` when both are available.
- OCR character coordinates feed SumatraPDF's original text-selection model.
- Drag across scanned text to highlight individual letters/words, then right-click for Copy, Search, Translate, annotations, and other selection actions.
- `Ctrl+C` retains a rectangular OCR fallback for image-only selections.
- Searches common Fedora, Ubuntu, Arch, Flatpak, executable-local, and user-local tessdata locations.

## Verified

- Fedora x86-64 build completed.
- Image-only PDF recognized exactly as `SUMATRA LINUX OCR 123`.
- OCR output contains a bounding box for every recognized character, enabling mouse selection and context-menu actions.
- All 104,704 unit tests passed.
- Installed binary was stripped to 32 MB and re-tested after stripping.

## OCR data

Install English Tesseract data using your distribution package, or place `eng.traineddata` in one of:

- `tessdata/` beside the executable
- `~/.local/share/sumatrapdf/tessdata/`\n\nFor Japanese text, place `jpn.traineddata` in the same directory as `eng.traineddata`.
- your distribution's system tessdata directory

## License

SumatraPDF is licensed under GPLv3. See the upstream project for full license and attribution information.
