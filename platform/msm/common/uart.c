/* Copyright (c) 2010-2012, 2014, The Linux Foundation. All rights reserved.
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

#include <string.h>
#include <kernel/thread.h>
#include <lk/debug.h>
#include <lk/err.h>
#include <lk/reg.h>
#include <lk/utils.h>
#include <platform/clock.h>
#include <platform/gpio.h>
#include <uart_dm.h>

/* Mostly a copy/pasta from U-Boot. Interestingly, some of the constants in
 * U-Boot make me think that it was originally based on CAF LK code. So,
 * everything comes full circle. */

#define UARTDM_DMRX             0x34 /* Max RX transfer length */
#define UARTDM_DMEN             0x3C /* DMA/data-packing mode */
#define UARTDM_NCF_TX           0x40 /* Number of chars to TX */

#define UARTDM_CSR				 0xA0

#define UARTDM_SR                0xA4 /* Status register */
#define UARTDM_SR_RX_READY       (1 << 0) /* Word is the receiver FIFO */
#define UARTDM_SR_TX_EMPTY       (1 << 3) /* Transmitter underrun */
#define UARTDM_SR_UART_OVERRUN   (1 << 4) /* Receive overrun */

#define UARTDM_CR                         0xA8 /* Command register */
#define UARTDM_CR_CMD_RESET_ERR           (3 << 4) /* Clear overrun error */
#define UARTDM_CR_CMD_RESET_STALE_INT     (8 << 4) /* Clears stale irq */
#define UARTDM_CR_CMD_RESET_TX_READY      (3 << 8) /* Clears TX Ready irq*/
#define UARTDM_CR_CMD_FORCE_STALE         (4 << 8) /* Causes stale event */

#define UARTDM_ISR                0xB4 /* Interrupt status register */
#define UARTDM_ISR_TX_READY       0x80 /* TX FIFO empty */

#define UARTDM_TF               0x100 /* UART Transmit FIFO register */
#define UARTDM_RF               0x140 /* UART Receive FIFO register */

#define UARTDM_RXFS             0x50 /* RX channel status register */
#define UARTDM_RXFS_BUF_SHIFT   0x7  /* Number of bytes in the packing buffer */
#define UARTDM_RXFS_BUF_MASK    0x7
#define UARTDM_MR1				 0x00
#define UARTDM_MR2				 0x04

#define MSM_BOOT_UART_DM_8_N_1_MODE	0x34
#define MSM_BOOT_UART_DM_CMD_RESET_RX	0x10
#define MSM_BOOT_UART_DM_CMD_RESET_TX	0x20
#define MSM_UART_MR1_RX_RDY_CTL		(1 << 7)

static uint32_t port_lookup[4];

/* Configure UART clock based on the UART block id*/
static void clock_config_uart_dm(uint8_t id)
{
    int ret;
    char iclk[64];
    char cclk[64];

    snprintf(iclk, sizeof(iclk), "uart%u_iface_clk", id);
    snprintf(cclk, sizeof(cclk), "uart%u_core_clk", id);

    ret = clk_get_set_enable(iclk, 0, 1);
    if(ret)
    {
        dprintf(CRITICAL, "failed to set %s ret = %d\n", iclk, ret);
        ASSERT(0);
    }

    ret = clk_get_set_enable(cclk, 7372800, 1);
    if(ret)
    {
        dprintf(CRITICAL, "failed to set %s ret = %d\n", cclk, ret);
        ASSERT(0);
    }
}

void uart_dm_init(uint8_t id, uint32_t base)
{
	static uint8_t port = 0;

	clock_config_uart_dm(id);
	gpio_config_uart_dm(id);

    /* CAF LK had some unnecessary DMBs here, and U-Boot still has a 5ms delay
     * because otherwise pins pick up some random noise. */

	writel(UART_DM_CLK_RX_TX_BIT_RATE, base + UARTDM_CSR);

    /* Enable RS232 flow control to support RS232 db9 connector */
    writel(MSM_UART_MR1_RX_RDY_CTL, base + UARTDM_MR1);
    /* 8-N-1 configuration: 8 data bits - No parity - 1 stop bit */
    writel(MSM_BOOT_UART_DM_8_N_1_MODE, base + UARTDM_MR2);

    writel(MSM_BOOT_UART_DM_CMD_RESET_RX, base + UARTDM_CR);
    writel(MSM_BOOT_UART_DM_CMD_RESET_TX, base + UARTDM_CR);

    /* Make sure BAM/single character mode is disabled */
    writel(0x0, base + UARTDM_DMEN);

	ASSERT(port < ARRAY_SIZE(port_lookup));
	port_lookup[port++] = base;
}

static void serial_write(uint32_t base, const char c) {
    while (!(readl(base + UARTDM_SR) & UARTDM_SR_TX_EMPTY) &&
           !(readl(base + UARTDM_ISR) & UARTDM_ISR_TX_READY)) {}

    writel(UARTDM_CR_CMD_RESET_TX_READY, base + UARTDM_CR);

    writel(1, base + UARTDM_NCF_TX);
    writel(c, base + UARTDM_TF);
}

int uart_putc(int port, char c)
{
	uint32_t base = port_lookup[port];

    if (c == '\n') {
        serial_write(base, '\r');
    }
    serial_write(base, c);

    return true;
}

static int serial_fetch(uint32_t base, unsigned int *word) {
    unsigned int sr;
    unsigned int count;

    /* Clear error in case of buffer overrun */
    if (readl(base + UARTDM_SR) & UARTDM_SR_UART_OVERRUN)
        writel(UARTDM_CR_CMD_RESET_ERR, base + UARTDM_CR);

    /* We need to fetch new character */
    sr = readl(base + UARTDM_SR);

    if (sr & UARTDM_SR_RX_READY) {
        /* There are at least 4 bytes in fifo */
        *word = readl(base + UARTDM_RF);
    } else {
        /* Check if there is anything in fifo */
        count = (readl(base + UARTDM_RXFS) >> UARTDM_RXFS_BUF_SHIFT)
                    & UARTDM_RXFS_BUF_MASK;
        if (!count) {
            return ERR_NOT_READY;
        }
        /* There is at least one character, move it to fifo */
        writel(UARTDM_CR_CMD_FORCE_STALE, base + UARTDM_CR);
        *word = readl(base + UARTDM_RF);
        writel(UARTDM_CR_CMD_RESET_STALE_INT, base + UARTDM_CR);
        writel(0x7, base + UARTDM_DMRX);
    }

    return NO_ERROR;
}

int uart_getc(int port, bool wait)
{
	int byte;
	static unsigned int word = 0;
	uint32_t base = port_lookup[port];

	if (!word) {
        while (serial_fetch(base, &word) < 0 && wait) {}
        if (word == 0) {
            return -1;
        }
	}

	byte = (int)word & 0xff;
	word = word >> 8;

	return byte;
}
