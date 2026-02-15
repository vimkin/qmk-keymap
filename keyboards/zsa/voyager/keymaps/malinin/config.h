/* SPDX-License-Identifier: GPL-2.0-or-later */

#define CHORDAL_HOLD // Prevents accidental same-hand modifier activation in rolls. Read more: https://docs.qmk.fm/tap_hold#chordal-hold
#define PERMISSIVE_HOLD // If permissive hold option is enabled, the dual-role key will perform its hold action even within the tapping term. Read more: https://docs.qmk.fm/tap_hold#permissive-hold
#define TAPPING_TERM 200
#define FLOW_TAP_TERM 110 // Flow Tap modifies mod-tap MT and layer-tap LT keys such that when pressed within a short timeout of the preceding key, the tapping behavior is triggered.
#define ONESHOT_TIMEOUT 1200 // How long to wait before the oneshot times out
#define QUICK_TAP_TERM 0 // If set to 0, it removes the auto-repeat ability and activates the hold function instead.

#define USB_SUSPEND_WAKEUP_DELAY 0
#define LAYER_STATE_8BIT // Limits the number of layers that the firmware uses to 8.

#define PALETTEFX_FLOW_ENABLE
#define PALETTEFX_POLARIZED_ENABLE
