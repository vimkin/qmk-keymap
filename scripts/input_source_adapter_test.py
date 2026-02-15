#!/usr/bin/env python3
"""
Test input source adapter — prints (and optionally logs) the language token it
receives from HID listener.

This script exists solely for local development and integration testing.
A production adapter should perform the actual OS-specific input source
switch (e.g. via ``InputSourceSelector`` on macOS or similar).

Contract
--------
* Called as: ``input_source_adapter_test.py <lang>``

Options
-------
``--fail``
    Exit with code 1 instead of 0. Useful for testing that the HID
    listener handles adapter failures gracefully.

Examples
--------
Normal invocation::

    python scripts/input_source_adapter_test.py en

Simulated failure::

    python scripts/input_source_adapter_test.py --fail ru
"""


import argparse
import re
import sys


def _two_letter_code(value: str) -> str:
    """Validate that *value* is exactly two lowercase ASCII letters."""
    if not re.fullmatch(r"[a-z]{2}", value):
        raise argparse.ArgumentTypeError(
            f"expected a two-letter language code (e.g. en, ru, de), got {value!r}"
        )
    return value


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Test adapter: prints the received language token.",
    )
    parser.add_argument(
        "lang",
        type=_two_letter_code,
        help="Language token sent by HID listener (two-letter code, e.g. en, ru, de).",
    )
    parser.add_argument(
        "--fail",
        action="store_true",
        help="Simulate adapter failure (exit 1).",
    )
    args = parser.parse_args()

    print(f"[test-adapter] received: {args.lang}")

    if args.fail:
        print("[test-adapter] simulating failure (exit 1)", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
