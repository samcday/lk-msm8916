/* Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#define MSM_IOMAP_BASE              0x00000000
#define MSM_IOMAP_END               0x08000000
#define A53_SS_BASE                 0x0B000000
#define A53_SS_END                  0x0B200000

#define SYSTEM_IMEM_BASE            0x08600000
#define MSM_SHARED_IMEM_BASE        0x08600000

#define MSM_SHARED_BASE             0x86300000
#define APPS_SS_BASE                0x0B000000

#define MSM_GIC_DIST_BASE           APPS_SS_BASE
#define MSM_GIC_CPU_BASE            (APPS_SS_BASE + 0x2000)
#define APCS_ALIAS0_IPC_INTERRUPT   (APPS_SS_BASE + 0x00011008)
#define APPS_APCS_F0_QTMR_V1_BASE   (APPS_SS_BASE + 0x00021000)
#define QTMR_BASE                   APPS_APCS_F0_QTMR_V1_BASE

#define TLMM_BASE_ADDR              0x1000000
#define GPIO_CONFIG_ADDR(x)         (TLMM_BASE_ADDR + (x)*0x1000)
#define GPIO_IN_OUT_ADDR(x)         (TLMM_BASE_ADDR + 0x00000004 + (x)*0x1000)

#define PERIPH_SS_BASE              0x07800000
#define CLK_CTL_BASE                0x1800000

/* GPLL */
#define GPLL0_STATUS                (CLK_CTL_BASE + 0x2101C)
#define GPLL1_STATUS                (CLK_CTL_BASE + 0x2001C)
#define APCS_GPLL_ENA_VOTE          (CLK_CTL_BASE + 0x45000)
#define APCS_CLOCK_BRANCH_ENA_VOTE  (CLK_CTL_BASE + 0x45004)

/* UART */
#define BLSP1_UART0_BASE            (PERIPH_SS_BASE + 0x000AF000)
#define BLSP1_UART1_BASE            (PERIPH_SS_BASE + 0x000B0000)
#define BLSP1_AHB_CBCR              (CLK_CTL_BASE + 0x1008)
#define BLSP1_UART1_APPS_CBCR       (CLK_CTL_BASE + 0x203C)
#define BLSP1_UART1_APPS_CMD_RCGR   (CLK_CTL_BASE + 0x2044)
#define BLSP1_UART1_APPS_CFG_RCGR   (CLK_CTL_BASE + 0x2048)
#define BLSP1_UART1_APPS_M          (CLK_CTL_BASE + 0x204C)
#define BLSP1_UART1_APPS_N          (CLK_CTL_BASE + 0x2050)
#define BLSP1_UART1_APPS_D          (CLK_CTL_BASE + 0x2054)
#define BLSP1_UART2_APPS_CBCR       (CLK_CTL_BASE + 0x302C)
#define BLSP1_UART2_APPS_CMD_RCGR   (CLK_CTL_BASE + 0x3034)
#define BLSP1_UART2_APPS_CFG_RCGR   (CLK_CTL_BASE + 0x3038)
#define BLSP1_UART2_APPS_M          (CLK_CTL_BASE + 0x303C)
#define BLSP1_UART2_APPS_N          (CLK_CTL_BASE + 0x3040)
#define BLSP1_UART2_APPS_D          (CLK_CTL_BASE + 0x3044)
