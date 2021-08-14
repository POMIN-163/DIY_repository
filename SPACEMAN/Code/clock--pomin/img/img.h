/**
 * @file img.h
 * @author Pomin
 * @brief 图像列表
 * @date 2021-08-11
 *
 * @copyright Copyright (c) 2021
 *
**/
#ifndef __IMG_H
#define __IMG_H

#include "spaceman/spaceman.h"
#include "longmao/longmao.h"
#include "pingpang/pingpang.h"

#include "target/humidity.h"
#include "target/temperature.h"
#include "target/watch_bottom.h"
#include "target/watch_top.h"
#include <stdint.h>

/* 图像索引表 */
/* 太空人 */
const uint8_t* img_arr_spaceman[] = {
    i0, i1, i2, i3, i4, i5, i6, i7, i8, i9
};
uint16_t img_size_spaceman[] = {
    sizeof(i0), sizeof(i1), sizeof(i2), sizeof(i3), sizeof(i4),
    sizeof(i5), sizeof(i6), sizeof(i7), sizeof(i8), sizeof(i9)
};
/* 龙猫转圈 */
const uint8_t* img_arr_longmao[] = {
    img_0 , img_1 , img_2 , img_3 , img_4 , img_5 , img_6 , img_7 , img_8 , img_9 ,
    img_10, img_11, img_12, img_13, img_14, img_15, img_16, img_17, img_18, img_19,
    img_20, img_21, img_22, img_23, img_24, img_25, img_26, img_27, img_28, img_29,
    img_30, img_31, img_32, img_33, img_34, img_35, img_36, img_37, img_38, img_39,
    img_40, img_41, img_42, img_43, img_44, img_45, img_46, img_47, img_48, img_49,
    img_50, img_51, img_52, img_53, img_54, img_55, img_56, img_57, img_58, img_59,
    img_60, img_61, img_62, img_63, img_64, img_65, img_66, img_67, img_68, img_69,
    img_70, img_71, img_72, img_73, img_74, img_75, img_76, img_77, img_78, img_79,
};
uint16_t img_size_longmao[] = {
    sizeof(img_0 ), sizeof(img_1 ), sizeof(img_2 ), sizeof(img_3 ), sizeof(img_4 ),
    sizeof(img_5 ), sizeof(img_6 ), sizeof(img_7 ), sizeof(img_8 ), sizeof(img_9 ),
    sizeof(img_10), sizeof(img_11), sizeof(img_12), sizeof(img_13), sizeof(img_14),
    sizeof(img_15), sizeof(img_16), sizeof(img_17), sizeof(img_18), sizeof(img_19),
    sizeof(img_20), sizeof(img_21), sizeof(img_22), sizeof(img_23), sizeof(img_24),
    sizeof(img_25), sizeof(img_26), sizeof(img_27), sizeof(img_28), sizeof(img_29),
    sizeof(img_30), sizeof(img_31), sizeof(img_32), sizeof(img_33), sizeof(img_34),
    sizeof(img_35), sizeof(img_36), sizeof(img_37), sizeof(img_38), sizeof(img_39),
    sizeof(img_40), sizeof(img_41), sizeof(img_42), sizeof(img_43), sizeof(img_44),
    sizeof(img_45), sizeof(img_46), sizeof(img_47), sizeof(img_48), sizeof(img_49),
    sizeof(img_50), sizeof(img_51), sizeof(img_52), sizeof(img_53), sizeof(img_54),
    sizeof(img_55), sizeof(img_56), sizeof(img_57), sizeof(img_58), sizeof(img_59),
    sizeof(img_60), sizeof(img_61), sizeof(img_62), sizeof(img_63), sizeof(img_64),
    sizeof(img_65), sizeof(img_66), sizeof(img_67), sizeof(img_68), sizeof(img_69),
    sizeof(img_70), sizeof(img_71), sizeof(img_72), sizeof(img_73), sizeof(img_74),
    sizeof(img_75), sizeof(img_76), sizeof(img_77), sizeof(img_78), sizeof(img_79),
};
/* 打乒乓球 */
const uint8_t* img_arr_pingpang[] = {
    pingpang_0 , pingpang_1 , pingpang_2 , pingpang_3 , pingpang_4 ,
    pingpang_5 , pingpang_6 , pingpang_7 , pingpang_8 , pingpang_9 ,
    pingpang_10, pingpang_11, pingpang_12, pingpang_13, pingpang_14,
    pingpang_15, pingpang_16, pingpang_17, pingpang_18, pingpang_19,
    pingpang_20, pingpang_21, pingpang_22, pingpang_23, pingpang_24,
    pingpang_25, pingpang_26, pingpang_27,
};
uint16_t img_size_pingpang[] = {
    sizeof(pingpang_0 ), sizeof(pingpang_1 ), sizeof(pingpang_2 ), sizeof(pingpang_3 ), sizeof(pingpang_4 ),
    sizeof(pingpang_5 ), sizeof(pingpang_6 ), sizeof(pingpang_7 ), sizeof(pingpang_8 ), sizeof(pingpang_9 ),
    sizeof(pingpang_10), sizeof(pingpang_11), sizeof(pingpang_12), sizeof(pingpang_13), sizeof(pingpang_14),
    sizeof(pingpang_15), sizeof(pingpang_16), sizeof(pingpang_17), sizeof(pingpang_18), sizeof(pingpang_19),
    sizeof(pingpang_20), sizeof(pingpang_21), sizeof(pingpang_22), sizeof(pingpang_23), sizeof(pingpang_24),
    sizeof(pingpang_25), sizeof(pingpang_26), sizeof(pingpang_27),
};
/* 简易 "键值对" 表 */
typedef struct {
    const uint8_t** arr;
    uint16_t* size;
    uint16_t  arr_size;
} img_index;

img_index myimgs[3] = {
    { img_arr_spaceman, img_size_spaceman, sizeof(img_arr_spaceman) / sizeof(const uint8_t *) },
    { img_arr_longmao , img_size_longmao , sizeof(img_arr_longmao)  / sizeof(const uint8_t *) },
    { img_arr_pingpang, img_size_pingpang, sizeof(img_arr_spaceman) / sizeof(const uint8_t *) },
};

#define LOGO "\r\n" \
"  .----------------.  .----------------.  .----------------.  .----------------.  .-----------------. \r\n" \
" | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. | \r\n" \
" | |   ______     | || |     ____     | || | ____    ____ | || |     _____    | || | ____  _____  | | \r\n" \
" | |  |_   __ \\   | || |   .'    `.   | || ||_   \\  /   _|| || |    |_   _|   | || ||_   \\|_   _| | | \r\n" \
" | |    | |__) |  | || |  /  .--.  \\  | || |  |   \\/   |  | || |      | |     | || |  |   \\ | |   | | \r\n" \
" | |    |  ___/   | || |  | |    | |  | || |  | |\\  /| |  | || |      | |     | || |  | |\\ \\| |   | | \r\n" \
" | |   _| |_      | || |  \\  `--'  /  | || | _| |_\\/_| |_ | || |     _| |_    | || | _| |_\\   |_  | | \r\n" \
" | |  |_____|     | || |   `.____.'   | || ||_____||_____|| || |    |_____|   | || ||_____|\\____| | | \r\n" \
" | |              | || |              | || |              | || |              | || |              | | \r\n" \
" | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' | \r\n" \
"  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  \r\n"



#endif