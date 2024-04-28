/*
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 */

#pragma once

#include <platform/iomap.h>
#include <platform/irqs.h>

#define GICBASE(n)  (APPS_SS_BASE)
#define GICD_OFFSET (0x0000)
#define GICC_OFFSET (0x2000)
