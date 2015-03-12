/******************************************************************************
*
* Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#ifndef _RTC_H_
#define _RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * RTC Base Address
 */
#define RTC_BASEADDR      ((u32)0XFFA60000U)

/**
 * Register: RTC_SET_TIME_WRITE
 */
#define RTC_SET_TIME_WRITE    ( ( RTC_BASEADDR ) + ((u32)0X00000000U) )

#define RTC_SET_TIME_WRITE_VALUE_SHIFT   0
#define RTC_SET_TIME_WRITE_VALUE_WIDTH   32
#define RTC_SET_TIME_WRITE_VALUE_MASK    ((u32)0XFFFFFFFFU)

/**
 * Register: RTC_SET_TIME_READ
 */
#define RTC_SET_TIME_READ    ( ( RTC_BASEADDR ) + ((u32)0X00000004U) )

#define RTC_SET_TIME_READ_VALUE_SHIFT   0
#define RTC_SET_TIME_READ_VALUE_WIDTH   32
#define RTC_SET_TIME_READ_VALUE_MASK    ((u32)0XFFFFFFFFU)

/**
 * Register: RTC_CALIB_WRITE
 */
#define RTC_CALIB_WRITE    ( ( RTC_BASEADDR ) + ((u32)0X00000008U) )

#define RTC_CALIB_WRITE_FRACTION_EN_SHIFT   20
#define RTC_CALIB_WRITE_FRACTION_EN_WIDTH   1
#define RTC_CALIB_WRITE_FRACTION_EN_MASK    ((u32)0X00100000U)

#define RTC_CALIB_WRITE_FRACTION_DATA_SHIFT   16
#define RTC_CALIB_WRITE_FRACTION_DATA_WIDTH   4
#define RTC_CALIB_WRITE_FRACTION_DATA_MASK    ((u32)0X000F0000U)

#define RTC_CALIB_WRITE_MAX_TICK_SHIFT   0
#define RTC_CALIB_WRITE_MAX_TICK_WIDTH   16
#define RTC_CALIB_WRITE_MAX_TICK_MASK    ((u32)0X0000FFFFU)

/**
 * Register: RTC_CALIB_READ
 */
#define RTC_CALIB_READ    ( ( RTC_BASEADDR ) + ((u32)0X0000000CU) )

#define RTC_CALIB_READ_FRACTION_EN_SHIFT   20
#define RTC_CALIB_READ_FRACTION_EN_WIDTH   1
#define RTC_CALIB_READ_FRACTION_EN_MASK    ((u32)0X00100000U)

#define RTC_CALIB_READ_FRACTION_DATA_SHIFT   16
#define RTC_CALIB_READ_FRACTION_DATA_WIDTH   4
#define RTC_CALIB_READ_FRACTION_DATA_MASK    ((u32)0X000F0000U)

#define RTC_CALIB_READ_MAX_TICK_SHIFT   0
#define RTC_CALIB_READ_MAX_TICK_WIDTH   16
#define RTC_CALIB_READ_MAX_TICK_MASK    ((u32)0X0000FFFFU)

/**
 * Register: RTC_CURRENT_TIME
 */
#define RTC_CURRENT_TIME    ( ( RTC_BASEADDR ) + ((u32)0X00000010U) )

#define RTC_CURRENT_TIME_VALUE_SHIFT   0
#define RTC_CURRENT_TIME_VALUE_WIDTH   32
#define RTC_CURRENT_TIME_VALUE_MASK    ((u32)0XFFFFFFFFU)

/**
 * Register: RTC_CURRENT_TICK
 */
#define RTC_CURRENT_TICK    ( ( RTC_BASEADDR ) + ((u32)0X00000014U) )

#define RTC_CURRENT_TICK_VALUE_SHIFT   0
#define RTC_CURRENT_TICK_VALUE_WIDTH   16
#define RTC_CURRENT_TICK_VALUE_MASK    ((u32)0X0000FFFFU)

/**
 * Register: RTC_ALARM
 */
#define RTC_ALARM    ( ( RTC_BASEADDR ) + ((u32)0X00000018U) )

#define RTC_ALARM_VALUE_SHIFT   0
#define RTC_ALARM_VALUE_WIDTH   32
#define RTC_ALARM_VALUE_MASK    ((u32)0XFFFFFFFFU)

/**
 * Register: RTC_RTC_INT_STATUS
 */
#define RTC_RTC_INT_STATUS    ( ( RTC_BASEADDR ) + ((u32)0X00000020U) )

#define RTC_RTC_INT_STATUS_ALARM_SHIFT   1
#define RTC_RTC_INT_STATUS_ALARM_WIDTH   1
#define RTC_RTC_INT_STATUS_ALARM_MASK    ((u32)0X00000002U)

#define RTC_RTC_INT_STATUS_SECONDS_SHIFT   0
#define RTC_RTC_INT_STATUS_SECONDS_WIDTH   1
#define RTC_RTC_INT_STATUS_SECONDS_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_RTC_INT_MASK
 */
#define RTC_RTC_INT_MASK    ( ( RTC_BASEADDR ) + ((u32)0X00000024U) )

#define RTC_RTC_INT_MASK_ALARM_SHIFT   1
#define RTC_RTC_INT_MASK_ALARM_WIDTH   1
#define RTC_RTC_INT_MASK_ALARM_MASK    ((u32)0X00000002U)

#define RTC_RTC_INT_MASK_SECONDS_SHIFT   0
#define RTC_RTC_INT_MASK_SECONDS_WIDTH   1
#define RTC_RTC_INT_MASK_SECONDS_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_RTC_INT_EN
 */
#define RTC_RTC_INT_EN    ( ( RTC_BASEADDR ) + ((u32)0X00000028U) )

#define RTC_RTC_INT_EN_ALARM_SHIFT   1
#define RTC_RTC_INT_EN_ALARM_WIDTH   1
#define RTC_RTC_INT_EN_ALARM_MASK    ((u32)0X00000002U)

#define RTC_RTC_INT_EN_SECONDS_SHIFT   0
#define RTC_RTC_INT_EN_SECONDS_WIDTH   1
#define RTC_RTC_INT_EN_SECONDS_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_RTC_INT_DIS
 */
#define RTC_RTC_INT_DIS    ( ( RTC_BASEADDR ) + ((u32)0X0000002CU) )

#define RTC_RTC_INT_DIS_ALARM_SHIFT   1
#define RTC_RTC_INT_DIS_ALARM_WIDTH   1
#define RTC_RTC_INT_DIS_ALARM_MASK    ((u32)0X00000002U)

#define RTC_RTC_INT_DIS_SECONDS_SHIFT   0
#define RTC_RTC_INT_DIS_SECONDS_WIDTH   1
#define RTC_RTC_INT_DIS_SECONDS_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_ADDR_ERROR
 */
#define RTC_ADDR_ERROR    ( ( RTC_BASEADDR ) + ((u32)0X00000030U) )

#define RTC_ADDR_ERROR_STATUS_SHIFT   0
#define RTC_ADDR_ERROR_STATUS_WIDTH   1
#define RTC_ADDR_ERROR_STATUS_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_ADDR_ERROR_INT_MASK
 */
#define RTC_ADDR_ERROR_INT_MASK    ( ( RTC_BASEADDR ) + ((u32)0X00000034U) )

#define RTC_ADDR_ERROR_INT_MASK_MASK_SHIFT   0
#define RTC_ADDR_ERROR_INT_MASK_MASK_WIDTH   1
#define RTC_ADDR_ERROR_INT_MASK_MASK_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_ADDR_ERROR_INT_EN
 */
#define RTC_ADDR_ERROR_INT_EN    ( ( RTC_BASEADDR ) + ((u32)0X00000038U) )

#define RTC_ADDR_ERROR_INT_EN_MASK_SHIFT   0
#define RTC_ADDR_ERROR_INT_EN_MASK_WIDTH   1
#define RTC_ADDR_ERROR_INT_EN_MASK_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_ADDR_ERROR_INT_DIS
 */
#define RTC_ADDR_ERROR_INT_DIS    ( ( RTC_BASEADDR ) + ((u32)0X0000003CU) )

#define RTC_ADDR_ERROR_INT_DIS_MASK_SHIFT   0
#define RTC_ADDR_ERROR_INT_DIS_MASK_WIDTH   1
#define RTC_ADDR_ERROR_INT_DIS_MASK_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_CONTROL
 */
#define RTC_CONTROL    ( ( RTC_BASEADDR ) + ((u32)0X00000040U) )

#define RTC_CONTROL_BATTERY_DISABLE_SHIFT   31
#define RTC_CONTROL_BATTERY_DISABLE_WIDTH   1
#define RTC_CONTROL_BATTERY_DISABLE_MASK    ((u32)0X80000000U)

#define RTC_CONTROL_OSC_CNTRL_SHIFT   24
#define RTC_CONTROL_OSC_CNTRL_WIDTH   4
#define RTC_CONTROL_OSC_CNTRL_MASK    ((u32)0X0F000000U)

#define RTC_CONTROL_SLVERR_ENABLE_SHIFT   0
#define RTC_CONTROL_SLVERR_ENABLE_WIDTH   1
#define RTC_CONTROL_SLVERR_ENABLE_MASK    ((u32)0X00000001U)

/**
 * Register: RTC_SAFETY_CHK
 */
#define RTC_SAFETY_CHK    ( ( RTC_BASEADDR ) + ((u32)0X00000050U) )

#define RTC_SAFETY_CHK_REG_SHIFT   0
#define RTC_SAFETY_CHK_REG_WIDTH   32
#define RTC_SAFETY_CHK_REG_MASK    ((u32)0XFFFFFFFFU)

/**
 * Register: RTC_ECO
 */
#define RTC_ECO    ( ( RTC_BASEADDR ) + ((u32)0X00000060U) )

#define RTC_ECO_REG_SHIFT   0
#define RTC_ECO_REG_WIDTH   32
#define RTC_ECO_REG_MASK    ((u32)0XFFFFFFFFU)

#ifdef __cplusplus
}
#endif


#endif /* _RTC_H_ */
