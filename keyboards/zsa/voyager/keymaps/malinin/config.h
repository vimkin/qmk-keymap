#define CHORDAL_HOLD
#undef ONESHOT_TIMEOUT
#define ONESHOT_TIMEOUT 2000

#define QUICK_TAP_TERM 0

#define USB_SUSPEND_WAKEUP_DELAY 0
#define SERIAL_NUMBER "BWvJA/NoYydB"
#define LAYER_STATE_8BIT
#define HCS(report)                                         \
    host_consumer_send(record->event.pressed ? report : 0); \
    return false

#define RGB_MATRIX_STARTUP_SPD 60
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_PALETTEFX_FLOW

#define PALETTEFX_FLOW_ENABLE
#define PALETTEFX_POLARIZED_ENABLE
