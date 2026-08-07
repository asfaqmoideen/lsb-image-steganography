# LSB Image Steganography

A command-line tool written in **C** that hides (encodes) any secret file inside a `.bmp` image and later extracts (decodes) it back, using the **Least Significant Bit (LSB)** technique. The change made to each pixel byte is at most 1, so the resulting "stego" image looks visually identical to the original.

## How it works

Every byte of the source BMP's pixel data has its least significant bit replaced with one bit of the secret data. Because 24-bit BMP images store 3 bytes (R, G, B) per pixel, each pixel can carry 3 bits of hidden data — enough that the visual distortion is imperceptible to the human eye.

To make decoding reliable, the encoder writes the following sequence into the image, in order, before the actual secret data:

| Order | Data                     | Size                        |
|-------|---------------------------|------------------------------|
| 1     | 54-byte BMP header         | copied as-is (not touched)  |
| 2     | Magic string `#*`          | 2 bytes → used to confirm the image contains hidden data |
| 3     | Secret file extension size | 4 bytes (int)                |
| 4     | Secret file extension      | e.g. `.txt`                  |
| 5     | Secret file size           | 4 bytes (int)                |
| 6     | Secret file data           | variable                     |
| 7     | Remaining original image data | copied as-is             |

During decoding, the magic string is checked first to confirm the image is actually a stego image, after which the extension, size, and data are read back out in the same order.

## Project structure

```
lsb-image-steganography/
├── common.h        # Shared magic string definition ("#*")
├── types.h         # Shared enums/typedefs (Status, OperationType, uint, etc.)
├── encode.h        # Function declarations + EncodeInfo struct
├── encode.c        # Encoding logic (hide a file inside a BMP)
├── decode.h        # Function declarations + DecodeInfo struct
├── decode.c        # Decoding logic (extract the hidden file from a BMP)
├── test_encode.c   # main() — CLI entry point, wires encode/decode together
├── beautiful.bmp   # Sample cover/source image
├── secret.txt       # Sample secret file to hide
├── stego.bmp        # Sample output produced by encoding
├── output.txt        # Sample output produced by decoding
├── ourpi.png          # Reference/sample image
└── .gitignore
```

## Requirements

- A C compiler (GCC or Clang)
- Linux/macOS/WSL terminal (or any environment with `gcc` on the `PATH`)
- A `.bmp` (bitmap) image to use as the cover image — JPEG/PNG are **not** supported as the source, since LSB requires uncompressed pixel data

## Building

Compile all the `.c` files together into a single executable:

```bash
gcc test_encode.c encode.c decode.c -o lsb_steg
```

This produces an executable named `lsb_steg` (you can name it anything).

## Usage

### Encoding — hide a file inside an image

```bash
./lsb_steg -e <source_image.bmp> <secret_file> [output_image.bmp]
```

- `source_image.bmp` — the cover BMP image
- `secret_file` — the file whose contents you want to hide (e.g. `secret.txt`)
- `output_image.bmp` — *optional*. Defaults to `stego.bmp` if not provided

**Example:**

```bash
./lsb_steg -e beautiful.bmp secret.txt stego.bmp
```

### Decoding — extract the hidden file from an image

```bash
./lsb_steg -d <stego_image.bmp> [output_file]
```

- `stego_image.bmp` — the image previously produced by encoding
- `output_file` — *optional*. Defaults to `output.txt` if not provided (the correct extension is restored automatically based on what was encoded)

**Example:**

```bash
./lsb_steg -d stego.bmp output.txt
```

### Sample run

```bash
$ gcc test_encode.c encode.c decode.c -o lsb_steg
$ ./lsb_steg -e beautiful.bmp secret.txt stego.bmp
...
Completed encoding successfully

$ ./lsb_steg -d stego.bmp output.txt
...
Completed decoding successfully

$ diff secret.txt output.txt   # should show no differences
```

## Capacity check

Before encoding, the tool checks that the cover image is large enough to hold the magic string, extension, size fields, and the secret data itself:

```
image_capacity > (2 + 4 + 4 + 4 + secret_file_size) * 8
```

If the image is too small for the secret file, encoding fails with an error instead of silently corrupting the image.

## Notes & limitations

- Only **uncompressed 24-bit BMP** images are supported as the source/cover image.
- The secret file can be of any type (text, image, binary, etc.) — its extension is preserved automatically.
- This is a straightforward LSB replacement scheme intended for learning purposes; it is **not** resistant to steganalysis and offers no encryption of the hidden data. Do not rely on it to protect sensitive information.

