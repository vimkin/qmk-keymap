#!/usr/bin/env python3
"""QMK Raw HID listener with pluggable input source adapter support.

Listens for layer-change reports sent by QMK firmware over Raw HID and
delegates input source switching to an external *adapter* command.
The adapter receives a single positional argument — a language token
(``ru`` or ``en``) — and is responsible for performing the actual
OS-specific input source switch.

Adapter contract
----------------
* **argv**: ``<adapter-path> <lang>``  where *lang* is ``ru`` or ``en``.
* **stdin / stdout / stderr**: unused by the listener; the adapter may
  write diagnostics to its own stderr.
* **exit 0**: success.
* **exit ≠ 0 / timeout**: treated as a transient failure — logged and
  ignored so the listener keeps running.

The listener only invokes the adapter when the computed language
*changes* (switch-on-change), so rapid repeated reports for the same
layer do not spawn redundant processes.

Examples
--------
With the bundled test adapter::

    python scripts/hid_listener.py \\
        --vid 0x3297 --pid 0x4974 \\
        --input-source-adapter scripts/input_source_adapter_test.py

Dry-run mode (no subprocess, just prints what *would* happen)::

    python scripts/hid_listener.py \\
        --vid 0x3297 --pid 0x4974 \\
        --input-source-adapter scripts/input_source_adapter_test.py \\
        --dry-run
"""


import argparse
import shlex
import subprocess
import sys
from typing import Optional

import hid

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

# QMK Raw HID interface identifiers.
USAGE_PAGE = 0xFF60
USAGE = 0x61

# Hardcoded timeout (seconds) for adapter subprocess calls.
_ADAPTER_TIMEOUT_S = 5


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------


def _parse_int(value: str) -> int:
    """Parse a CLI integer that may be given in hex (``0x…``) or decimal."""
    return int(value, 0)


def _layer_to_lang(layer: int) -> str:
    """Map a QMK layer index to a language token.

    Layer 1 is treated as the Russian input source; every other layer
    maps to English.
    """
    return "ru" if layer == 1 else "en"


# ---------------------------------------------------------------------------
# Adapter dispatcher
# ---------------------------------------------------------------------------


def dispatch_input_source_change(
    lang: str,
    *,
    adapter: str,
    dry_run: bool = False,
    verbose: bool = False,
) -> None:
    """Call the external input source adapter with the given language token.

    Parameters
    ----------
    lang:
        Language token (``ru`` or ``en``).
    adapter:
        Path to the adapter executable.
    dry_run:
        If ``True``, print the intended invocation without running it.
    verbose:
        If ``True``, print adapter invocation details and exit status.
    """
    cmd = [*shlex.split(adapter), lang]

    if dry_run:
        print(f"[dry-run] would call: {cmd}")
        return

    if verbose:
        print(f"[adapter] calling: {cmd}")

    try:
        result = subprocess.run(
            cmd,
            timeout=_ADAPTER_TIMEOUT_S,
        )
        if result.returncode != 0:
            print(
                f"[adapter] non-zero exit ({result.returncode}) for: {cmd}",
                file=sys.stderr,
            )
        elif verbose:
            print(f"[adapter] ok (exit 0)")
    except subprocess.TimeoutExpired:
        print(
            f"[adapter] timed out after {_ADAPTER_TIMEOUT_S}s: {cmd}",
            file=sys.stderr,
        )
    except OSError as exc:
        print(f"[adapter] could not execute {cmd}: {exc}", file=sys.stderr)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Listen to QMK Raw HID reports and optionally switch "
        "the host input source via an external adapter command.",
    )
    parser.add_argument(
        "--vid",
        type=_parse_int,
        required=True,
        help="USB Vendor ID (hex or decimal).",
    )
    parser.add_argument(
        "--pid",
        type=_parse_int,
        required=True,
        help="USB Product ID (hex or decimal).",
    )
    parser.add_argument(
        "--read-size",
        type=int,
        default=64,
        help="HID read buffer size in bytes (default: 64).",
    )
    parser.add_argument(
        "--input-source-adapter",
        metavar="CMD",
        help="Path to an external command that performs the actual "
        "input source switch. It will be called as `CMD <lang>` where "
        "<lang> is 'ru' or 'en'. If omitted, no switching is performed.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print adapter invocations without executing them.",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Print extra diagnostics about adapter calls.",
    )
    args = parser.parse_args()

    # -- discover HID interfaces ------------------------------------------

    device_interfaces = hid.enumerate(args.vid, args.pid)
    raw_hid_interfaces = [
        i
        for i in device_interfaces
        if i["usage_page"] == USAGE_PAGE and i["usage"] == USAGE
    ]

    if not raw_hid_interfaces:
        print(f"No HID devices found for {args.vid:#06x}:{args.pid:#06x}")
        return 1

    # -- open device and read loop ----------------------------------------

    last_lang: Optional[str] = None

    try:
        with hid.Device(path=raw_hid_interfaces[0]["path"]) as interface:
            print("Started. Waiting for HID report...")
            try:
                while True:
                    data = interface.read(args.read_size, timeout=0)
                    if not data:
                        continue

                    layer = data[0]
                    lang = _layer_to_lang(layer)

                    if args.verbose:
                        print(
                            f"[hid] raw={bytes(data).hex(' ')}  "
                            f"layer={layer}  lang={lang}"
                        )

                    # Only dispatch when the computed language changes.
                    if lang == last_lang:
                        continue

                    last_lang = lang

                    if args.input_source_adapter:
                        dispatch_input_source_change(
                            lang,
                            adapter=args.input_source_adapter,
                            dry_run=args.dry_run,
                            verbose=args.verbose,
                        )
                    elif args.verbose:
                        print(
                            f"[hid] input source -> {lang} " "(no adapter configured)"
                        )

            except KeyboardInterrupt:
                print("\nStopped.")
                return 0
    except hid.HIDException as err:
        print(f"HID error: {err}")

    print(
        f"Could not open any interface for {args.vid:#06x}:{args.pid:#06x} "
        "(permission/privilege issue)."
    )
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
