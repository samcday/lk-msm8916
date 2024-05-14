use core::fmt::Write;

pub struct Uart {
    pub port: u32,
}

impl Write for Uart {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        for c in s.chars() {
            if unsafe { crate::sys::uart_putc(self.port as _, c as _) } < 0 {
                return Err(core::fmt::Error);
            }
        }
        Ok(())
    }
}
