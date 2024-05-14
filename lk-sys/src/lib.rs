#![no_std]

pub mod alloc;
pub mod panic;
pub mod uart;

pub fn uart0() -> uart::Uart {
    uart::Uart { port: 0 }
}

pub mod sys {
    #![allow(non_upper_case_globals)]
    #![allow(non_camel_case_types)]
    #![allow(non_snake_case)]
    use core::concat;
    use core::env;
    use core::include;

    include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
}
