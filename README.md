# SumatraPDF Linux + OCR

Native Linux build of [SumatraPDF](https://github.com/sumatrapdfreader/sumatrapdf), based on upstream commit `b1a8e22577c9e35064315d279fc33ba6b67c5bef`.

## Download

The Fedora x86-64 build is produced by GitHub Actions. Open the [latest successful build](https://github.com/zak3123/sumatrapdf-linux/actions/runs/44190500228) and download the **SumatraPDF-linux-x86_64** artifact. It contains the application archive and SHA-256 checksums.

## Linux changes

- Native Linux desktop binary built from the upstream `src/ng` port.
- OCR fallback for scanned PDFs and image-only documents.
- Drag normally across text in an image or scan, then press **Ctrl+C**.
- Uses Tesseract and MuPDF already linked by the native build.
- Searches common Fedora, Ubuntu, Arch, Flatpak, and local tessdata locations.

## Verified

- Fedora 45 x86-64 release build completed.
- OCR test image recognized exactly as `SUMATRA LINUX OCR 123`.
- Application launch and desktop integration verified.
- 104,076 unit tests passed; two unrelated animated-GIF frame tests remain failing.

## OCR data

Install English Tesseract data using your distribution package, or place `eng.traineddata` in one of:

- `tessdata/` beside the executable
- `~/.local/share/sumatrapdf/tessdata/`
- your distribution's system tessdata directory

## License

SumatraPDF is licensed under GPLv3. See the upstream project for full license and attribution information.
