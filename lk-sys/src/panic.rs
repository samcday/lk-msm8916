use core::panic::PanicInfo;
use crate::sys::{platform_halt_action_HALT_ACTION_REBOOT, platform_halt_reason_HALT_REASON_SW_PANIC};
use core::fmt::Write;

#[panic_handler]
unsafe fn panic(info: &PanicInfo) -> ! {
    writeln!(crate::uart0(), "{}", info).unwrap();
    crate::sys::platform_halt(platform_halt_action_HALT_ACTION_REBOOT,
                              platform_halt_reason_HALT_REASON_SW_PANIC);
}
