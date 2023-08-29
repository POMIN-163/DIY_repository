/**
 * @author: Pomin
 * @date: 2022-12-11 11:11:26
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-11 21:37:27
 * @description:
 **/
#ifndef _KEYBORD_h_
#define _KEYBORD_h_

#include <CH552.H>
#include <intrins.h>

#define uchar                      unsigned char
#define uint                       unsigned int

// HID Keyboard/Keypad Usage IDs (subset of the codes available in the USB HID
// Usage Tables spec)
#define HID_KEYBOARD_RESERVED      0   // 0x00 - No event inidicated
#define HID_KEYBOARD_A             4   // 0x04 - Keyboard a and A
#define HID_KEYBOARD_B             5   // 0x05 - Keyboard b and B
#define HID_KEYBOARD_C             6   // 0x06 - Keyboard c and C
#define HID_KEYBOARD_D             7   // 0x07 - Keyboard d and D
#define HID_KEYBOARD_E             8   // 0x08 - Keyboard e and E
#define HID_KEYBOARD_F             9   // 0x09 - Keyboard f and F
#define HID_KEYBOARD_G             10  // 0x0A - Keyboard g and G
#define HID_KEYBOARD_H             11  // 0x0B - Keyboard h and H
#define HID_KEYBOARD_I             12  // 0x0C - Keyboard i and I
#define HID_KEYBOARD_J             13  // 0x0D - Keyboard j and J
#define HID_KEYBOARD_K             14  // 0x0E - Keyboard k and K
#define HID_KEYBOARD_L             15  // 0x0F - Keyboard l and L
#define HID_KEYBOARD_M             16  // 0x10 - Keyboard m and M
#define HID_KEYBOARD_N             17  // 0x11 - Keyboard n and N
#define HID_KEYBOARD_O             18  // 0x12 - Keyboard o and O
#define HID_KEYBOARD_P             19  // 0x13 - Keyboard p and p
#define HID_KEYBOARD_Q             20  // 0x14 - Keyboard q and Q
#define HID_KEYBOARD_R             21  // 0x15 - Keyboard r and R
#define HID_KEYBOARD_S             22  // 0x16 - Keyboard s and S
#define HID_KEYBOARD_T             23  // 0x17 - Keyboard t and T
#define HID_KEYBOARD_U             24  // 0x18 - Keyboard u and U
#define HID_KEYBOARD_V             25  // 0x19 - Keyboard v and V
#define HID_KEYBOARD_W             26  // 0x1A - Keyboard w and W
#define HID_KEYBOARD_X             27  // 0x1B - Keyboard x and X
#define HID_KEYBOARD_Y             28  // 0x1C - Keyboard y and Y
#define HID_KEYBOARD_Z             29  // 0x1D - Keyboard z and Z
#define HID_KEYBOARD_1             30  // 0x1E - Keyboard 1 and !
#define HID_KEYBOARD_2             31  // 0x1F - Keyboard 2 and @
#define HID_KEYBOARD_3             32  // 0x20 - Keyboard 3 and #
#define HID_KEYBOARD_4             33  // 0x21 - Keyboard 4 and %
#define HID_KEYBOARD_5             34  // 0x22 - Keyboard 5 and %
#define HID_KEYBOARD_6             35  // 0x23 - Keyboard 6 and ^
#define HID_KEYBOARD_7             36  // 0x24 - Keyboard 7 and &
#define HID_KEYBOARD_8             37  // 0x25 - Keyboard 8 and *
#define HID_KEYBOARD_9             38  // 0x26 - Keyboard 9 and (
#define HID_KEYBOARD_0             39  // 0x27 - Keyboard 0 and )
#define HID_KEYBOARD_RETURN        40  // 0x28 - Keyboard Return (ENTER)
#define HID_KEYBOARD_ESCAPE        41  // 0x29 - Keyboard ESCAPE
#define HID_KEYBOARD_DELETE        42  // 0x2A - Keyboard DELETE (Backspace)
#define HID_KEYBOARD_TAB           43  // 0x2B - Keyboard Tab
#define HID_KEYBOARD_SPACEBAR      44  // 0x2C - Keyboard Spacebar
#define HID_KEYBOARD_MINUS         45  // 0x2D - Keyboard - and (underscore)
#define HID_KEYBOARD_EQUAL         46  // 0x2E - Keyboard = and +
#define HID_KEYBOARD_LEFT_BRKT     47  // 0x2F - Keyboard [ and {
#define HID_KEYBOARD_RIGHT_BRKT    48  // 0x30 - Keyboard ] and }
#define HID_KEYBOARD_BACK_SLASH    49  // 0x31 - Keyboard \ and |
#define HID_KEYBOARD_SEMI_COLON    51  // 0x33 - Keyboard ; and :
#define HID_KEYBOARD_SGL_QUOTE     52  // 0x34 - Keyboard ' and "
#define HID_KEYBOARD_GRV_ACCENT    53  // 0x35 - Keyboard Grave Accent and Tilde
#define HID_KEYBOARD_COMMA         54  // 0x36 - Keyboard , and <
#define HID_KEYBOARD_DOT           55  // 0x37 - Keyboard . and >
#define HID_KEYBOARD_FWD_SLASH     56  // 0x38 - Keyboard / and ?
#define HID_KEYBOARD_CAPS_LOCK     57  // 0x39 - Keyboard Caps Lock
#define HID_KEYBOARD_F1            58  // 0x3A - Keyboard F1
#define HID_KEYBOARD_F2            59  // 0x3B - Keyboard F2
#define HID_KEYBOARD_F3            60  // 0x3C - Keyboard F3
#define HID_KEYBOARD_F4            61  // 0x3D - Keyboard F4
#define HID_KEYBOARD_F5            62  // 0x3E - Keyboard F5
#define HID_KEYBOARD_F6            63  // 0x3F - Keyboard F6
#define HID_KEYBOARD_F7            64  // 0x40 - Keyboard F7
#define HID_KEYBOARD_F8            65  // 0x41 - Keyboard F8
#define HID_KEYBOARD_F9            66  // 0x42 - Keyboard F9
#define HID_KEYBOARD_F10           67  // 0x43 - Keyboard F10
#define HID_KEYBOARD_F11           68  // 0x44 - Keyboard F11
#define HID_KEYBOARD_F12           69  // 0x45 - Keyboard F12
#define HID_KEYBOARD_PRNT_SCREEN   70  // 0x46 - Keyboard Print Screen
#define HID_KEYBOARD_SCROLL_LOCK   71  // 0x47 - Keyboard Scroll Lock
#define HID_KEYBOARD_PAUSE         72  // 0x48 - Keyboard Pause
#define HID_KEYBOARD_INSERT        73  // 0x49 - Keyboard Insert
#define HID_KEYBOARD_HOME          74  // 0x4A - Keyboard Home
#define HID_KEYBOARD_PAGE_UP       75  // 0x4B - Keyboard PageUp
#define HID_KEYBOARD_DELETE_FWD    76  // 0x4C - Keyboard Delete Forward
#define HID_KEYBOARD_END           77  // 0x4D - Keyboard End
#define HID_KEYBOARD_PAGE_DOWN     78  // 0x4E - Keyboard PageDown
#define HID_KEYBOARD_RIGHT_ARROW   79  // 0x4F - Keyboard RightArrow
#define HID_KEYBOARD_LEFT_ARROW    80  // 0x50 - Keyboard LeftArrow
#define HID_KEYBOARD_DOWN_ARROW    81  // 0x51 - Keyboard DownArrow
#define HID_KEYBOARD_UP_ARROW      82  // 0x52 - Keyboard UpArrow
#define HID_KEYBPAD_NUM_LOCK       83  // 0x53 - Keypad Num Lock and Clear
#define HID_KEYBPAD_DIVIDE         84  // 0x54 - Keypad /
#define HID_KEYBPAD_MULTIPLY       85  // 0x55 - Keypad *
#define HID_KEYBPAD_SUBTRACT       86  // 0x56 - Keypad -
#define HID_KEYBPAD_ADD            87  // 0x57 - Keypad +
#define HID_KEYBPAD_ENTER          88  // 0x58 - Keypad ENTER
#define HID_KEYBPAD_1              89  // 0x59 - Keypad 1 and End
#define HID_KEYBPAD_2              90  // 0x5A - Keypad 2 and Down Arrow
#define HID_KEYBPAD_3              91  // 0x5B - Keypad 3 and PageDn
#define HID_KEYBPAD_4              92  // 0x5C - Keypad 4 and Lfet Arrow
#define HID_KEYBPAD_5              93  // 0x5D - Keypad 5
#define HID_KEYBPAD_6              94  // 0x5E - Keypad 6 and Right Arrow
#define HID_KEYBPAD_7              95  // 0x5F - Keypad 7 and Home
#define HID_KEYBPAD_8              96  // 0x60 - Keypad 8 and Up Arrow
#define HID_KEYBPAD_9              97  // 0x61 - Keypad 9 and PageUp
#define HID_KEYBPAD_0              98  // 0x62 - Keypad 0 and Insert
#define HID_KEYBPAD_DOT            99  // 0x63 - Keypad . and Delete
#define HID_KEYBOARD_MENU          101  // 0x65 - Keyboard Application

#define HID_CONSUMER_VOLUME_UP     233  // 0xE9 - Volume Increment
#define HID_CONSUMER_VOLUME_DOWN   234  // 0xEA - Volume Decrement
#define HID_CONSUMER_BRIGH_UP      111  // 0x6F - Brightness Increment
#define HID_CONSUMER_BRIGH_DOWN    112  // 0x70 - Brightness Decrement

#define HID_CONSUMER_SCAN_NEXT_TRK 181  // 0xB5 - Scan Next Track
#define HID_CONSUMER_SCAN_PREV_TRK 182  // 0xB6 - Scan Previous Track
#define HID_CONSUMER_STOP          183  // 0xB7 - Stop
#define HID_CONSUMER_PLAY_PAUSE    205  // 0xCD - Play/Pause

#define HID_CONSUMER_CALCULATOR    146  // 0x92 - Calculator
#define HID_CONSUMER_FAST_FORWARD  179  // 0xB3 - Fast Forward
#define HID_CONSUMER_REWIND        180  // 0xB4 - Rewind
#define HID_CONSUMER_MUTE          226  // 0xE2 - Mute

#define HID_CONSUMER_CHROME        35   // 0x23 - Chrome
#define HID_CONSUMER_MULTI         131  // 0x83 - Multi

#define HID_L_WIN                  0X08
#define HID_L_ALT                  0X04
#define HID_L_SHIFT                0X02
#define HID_L_CTL                  0X01
#define HID_R_WIN                  0X80
#define HID_R_ALT                  0X40
#define HID_R_SHIFT                0X20
#define HID_R_CTL                  0X10

// 多控制键可以用 | 连接比如 Ctrl+Alt 就是 KEY_Left_Control | KEY_Left_Alt
#define KEY_Left_Control           0x01
#define KEY_Left_Shift             0x02
#define KEY_Left_Alt               0x04
#define KEY_Left_GUI               0x08
#define KEY_Right_Control          0x10
#define KEY_Right_Shift            0x20
#define KEY_Right_Alt              0x40
#define KEY_Right_GUI              0x80

#define KEY_SYS_POWEROFF 0x81  // 关机
#define KEY_SYS_REST     0x82  // 休眠
#define KEY_SYS_WAKE     0x83  // 唤醒

#endif
