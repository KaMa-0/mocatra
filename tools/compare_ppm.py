#!/usr/bin/env python3
"""Compare two binary PPM (P6) images.

Exit codes: 0 identical (or within --tolerance), 1 differ, 2 usage/format error.

    compare_ppm.py baseline.ppm new.ppm
    compare_ppm.py baseline.ppm new.ppm --tolerance 1   # allow +-1 per channel
"""
import argparse
import sys


def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()

    # header: magic, width, height, maxval; '#' comments allowed
    tokens, pos = [], 0
    while len(tokens) < 4:
        while pos < len(data) and data[pos:pos + 1].isspace():
            pos += 1
        if data[pos:pos + 1] == b"#":
            while pos < len(data) and data[pos:pos + 1] != b"\n":
                pos += 1
            continue
        start = pos
        while pos < len(data) and not data[pos:pos + 1].isspace():
            pos += 1
        tokens.append(data[start:pos])
    pos += 1  # single whitespace after maxval

    if tokens[0] != b"P6":
        raise ValueError(f"{path}: not a P6 file")
    width, height, maxval = (int(t) for t in tokens[1:])
    pixels = data[pos:]
    if maxval != 255 or len(pixels) != width * height * 3:
        raise ValueError(f"{path}: unexpected maxval or truncated pixel data")
    return width, height, pixels


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawTextHelpFormatter)
    ap.add_argument("baseline")
    ap.add_argument("new")
    ap.add_argument("--tolerance", type=int, default=0,
                    help="max allowed per-channel difference (default 0)")
    args = ap.parse_args()

    try:
        wa, ha, a = read_ppm(args.baseline)
        wb, hb, b = read_ppm(args.new)
    except (OSError, ValueError) as e:
        print(f"error: {e}", file=sys.stderr)
        return 2

    if (wa, ha) != (wb, hb):
        print(f"FAIL: size differs: {wa}x{ha} vs {wb}x{hb}")
        return 1

    if a == b:
        print(f"OK: identical ({wa}x{ha})")
        return 0

    n_diff_ch, max_diff, first = 0, 0, None
    for i, (x, y) in enumerate(zip(a, b)):
        d = abs(x - y)
        if d > args.tolerance:
            n_diff_ch += 1
            if first is None:
                first = i
        max_diff = max(max_diff, d)

    if n_diff_ch == 0:
        print(f"OK: within tolerance {args.tolerance} (max diff {max_diff})")
        return 0

    px = first // 3
    print(f"FAIL: {n_diff_ch} of {len(a)} channel values differ "
          f"(max diff {max_diff}, tolerance {args.tolerance})")
    print(f"      first difference at pixel x={px % wa}, y={px // wa}, "
          f"channel {'RGB'[first % 3]}: {a[first]} vs {b[first]}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
