# ESP32-SurfaceDial

#### 介绍
    ESP32蓝牙模拟SurfaceDial
    Fork From https://github.com/dieki-n/OpenDial

#### 开发环境
Vscode
Platformio

#### 引脚说明

| EC11 | ESP32 |
| ---- | ----- |
| CLK  | D16   |
| DT   | D4    |
| SW   | D26   |
| +    | 3V3   |
| GND  | GND   |

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


### 原项目README

This project is ESP32 firmware for a bluetooth rotary dial device. The main purpose of this device is for use with creativity and art programs, like photoshop.

![Image of the device](https://i.redd.it/zjgpwo4b77751.jpg)

It implements the surface dial protocol over BLE HID and appears as a Surface Dial to Windows. This means it works with all the apps that implement support for the surface dial, like Photoshop or Sketchable.

Related helpful resources:
https://docs.microsoft.com/en-us/windows-hardware/design/component-guidelines/radial-implementation-guide

This code also uses the BLEKeyboard library from https://github.com/T-vK/ESP32-BLE-Keyboard
