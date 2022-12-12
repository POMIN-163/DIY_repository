/* 调试 */
/* 提供printf子程序和延时函数 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#define FREQ_SYS 24000000  // 系统主频24MHz

void CfgFsys();                       // CH554时钟选择和配置
void mDelayuS(UINT16 n);              // 以uS为单位延时
void mDelaymS(UINT16 n);              // 以mS为单位延时

#endif
