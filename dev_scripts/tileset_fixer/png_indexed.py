"""
Minimal reader/writer for indexed (palette, colortype 3) PNGs, non-interlaced,
bit depth 4 or 8 — exactly what pokeemerald tileset tiles.png files are.

No third-party dependencies (Pillow isn't installed in this environment),
so this implements just enough of the PNG spec to round-trip these files:
chunk parsing, zlib inflate/deflate, and the 5 standard scanline filters.

Ancillary chunks (PLTE, tRNS, etc.) are preserved byte-for-byte and replayed
verbatim on write, since we only ever need to read/write raw pixel indices.
"""
import struct
import zlib
from binascii import crc32

PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"


class IndexedPng:
    def __init__(self, width, height, bit_depth, pixels, chunks):
        self.width = width
        self.height = height
        self.bit_depth = bit_depth
        self.pixels = pixels  # flat bytearray, 1 byte per pixel (0-255), row-major
        self.chunks = chunks  # list of (type_bytes, data_bytes), IHDR/IDAT/IEND excluded

    def get(self, x, y):
        return self.pixels[y * self.width + x]


def _read_chunks(data):
    pos = len(PNG_SIGNATURE)
    chunks = []
    while pos < len(data):
        length = struct.unpack(">I", data[pos:pos + 4])[0]
        ctype = data[pos + 4:pos + 8]
        cdata = data[pos + 8:pos + 8 + length]
        chunks.append((ctype, cdata))
        pos += 12 + length
        if ctype == b"IEND":
            break
    return chunks


def _paeth(a, b, c):
    p = a + b - c
    pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
    if pa <= pb and pa <= pc:
        return a
    if pb <= pc:
        return b
    return c


def _unfilter(raw, width, height, bit_depth):
    row_bytes = (width * bit_depth + 7) // 8
    bpp = 1  # colortype 3 (indexed) always has 1 channel; bpp = ceil(bitdepth/8) capped at 1 per spec
    out = bytearray(row_bytes * height)
    prev = bytearray(row_bytes)
    pos = 0
    for y in range(height):
        ftype = raw[pos]
        pos += 1
        line = bytearray(raw[pos:pos + row_bytes])
        pos += row_bytes
        for i in range(row_bytes):
            a = line[i - bpp] if i >= bpp else 0
            b = prev[i]
            c = prev[i - bpp] if i >= bpp else 0
            if ftype == 0:
                pass
            elif ftype == 1:
                line[i] = (line[i] + a) & 0xFF
            elif ftype == 2:
                line[i] = (line[i] + b) & 0xFF
            elif ftype == 3:
                line[i] = (line[i] + (a + b) // 2) & 0xFF
            elif ftype == 4:
                line[i] = (line[i] + _paeth(a, b, c)) & 0xFF
            else:
                raise ValueError(f"Unknown PNG filter type {ftype}")
        out[y * row_bytes:(y + 1) * row_bytes] = line
        prev = line
    return out


def _unpack_pixels(row_data, width, height, bit_depth):
    pixels = bytearray(width * height)
    if bit_depth == 8:
        for i in range(width * height):
            pixels[i] = row_data[i]
        return pixels
    row_bytes = (width * bit_depth + 7) // 8
    ppb = 8 // bit_depth  # pixels per byte
    mask = (1 << bit_depth) - 1
    for y in range(height):
        base = y * row_bytes
        for x in range(width):
            byte = row_data[base + x // ppb]
            shift = 8 - bit_depth - (x % ppb) * bit_depth
            pixels[y * width + x] = (byte >> shift) & mask
    return pixels


def read_indexed_png(path):
    data = open(path, "rb").read()
    if data[:8] != PNG_SIGNATURE:
        raise ValueError(f"{path} is not a PNG")
    chunks = _read_chunks(data)
    ihdr = None
    idat = bytearray()
    other_chunks = []
    for ctype, cdata in chunks:
        if ctype == b"IHDR":
            ihdr = cdata
        elif ctype == b"IDAT":
            idat += cdata
        elif ctype == b"IEND":
            pass
        else:
            other_chunks.append((ctype, cdata))
    width, height, bit_depth, color_type, comp, filt, interlace = struct.unpack(">IIBBBBB", ihdr)
    if color_type != 3:
        raise ValueError(f"{path}: only indexed (colortype 3) PNGs are supported, got {color_type}")
    if interlace != 0:
        raise ValueError(f"{path}: interlaced PNGs are not supported")
    if bit_depth not in (4, 8):
        raise ValueError(f"{path}: only bit depth 4 or 8 supported, got {bit_depth}")
    raw = zlib.decompress(bytes(idat))
    row_data = _unfilter(raw, width, height, bit_depth)
    pixels = _unpack_pixels(row_data, width, height, bit_depth)
    return IndexedPng(width, height, bit_depth, pixels, other_chunks)


def _pack_pixels(pixels, width, height, bit_depth):
    if bit_depth == 8:
        return bytes(pixels)
    row_bytes = (width * bit_depth + 7) // 8
    ppb = 8 // bit_depth
    out = bytearray(row_bytes * height)
    for y in range(height):
        base = y * row_bytes
        for x in range(width):
            shift = 8 - bit_depth - (x % ppb) * bit_depth
            out[base + x // ppb] |= (pixels[y * width + x] & ((1 << bit_depth) - 1)) << shift
    return bytes(out)


def _write_chunk(f, ctype, data):
    f.write(struct.pack(">I", len(data)))
    f.write(ctype)
    f.write(data)
    f.write(struct.pack(">I", crc32(ctype + data) & 0xFFFFFFFF))


def write_indexed_png(path, png: IndexedPng):
    row_data = _pack_pixels(png.pixels, png.width, png.height, png.bit_depth)
    row_bytes = len(row_data) // png.height
    filtered = bytearray()
    for y in range(png.height):
        filtered.append(0)  # filter type 0 (None) for every scanline
        filtered += row_data[y * row_bytes:(y + 1) * row_bytes]
    compressed = zlib.compress(bytes(filtered), 9)

    ihdr = struct.pack(">IIBBBBB", png.width, png.height, png.bit_depth, 3, 0, 0, 0)
    with open(path, "wb") as f:
        f.write(PNG_SIGNATURE)
        _write_chunk(f, b"IHDR", ihdr)
        for ctype, cdata in png.chunks:
            _write_chunk(f, ctype, cdata)
        _write_chunk(f, b"IDAT", compressed)
        _write_chunk(f, b"IEND", b"")
