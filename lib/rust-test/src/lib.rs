#![no_std]

// Explicitly depend on lk_sys crate for panic handler
extern crate lk_sys;
extern crate alloc;

use alloc::string::String;
use core::ffi::{c_int, CStr};
use core::fmt::Write;
use lk_sys::sys::{CMD_AVAIL_NORMAL, console_cmd_args};

#[no_mangle]
pub unsafe extern "C" fn rust_test_forcelink() { /* see link-hack.c */ }

#[link_section = "commands"]
#[export_name = "_cmd_block_rust_test"]
#[no_mangle]
pub static mut COMMANDS: lk_sys::sys::_cmd_block = lk_sys::sys::_cmd_block {
    name: c"rust-test".as_ptr(),
    count: 2,
    list: [
        lk_sys::sys::console_cmd {
            availability_mask: CMD_AVAIL_NORMAL as u8,
            cmd_str: c"rust-test".as_ptr(),
            help_str: c"I can't believe it's not C!".as_ptr(),
            cmd_callback: Some(rust_test_cmd),
        },
        lk_sys::sys::console_cmd {
            availability_mask: CMD_AVAIL_NORMAL as u8,
            cmd_str: c"rust-panic".as_ptr(),
            help_str: c"Wait, are you sure this isn't C?".as_ptr(),
            cmd_callback: Some(rust_panic_cmd),
        }
    ].as_ptr(),
};

unsafe extern "C" fn rust_test_cmd(argc: c_int, argv: *const console_cmd_args) -> c_int {
    let mut str = String::new();
    let args = core::slice::from_raw_parts(argv, argc as usize);
    for arg in & args[1..] {
        str += CStr::from_ptr(arg.str_).to_str().unwrap();
    }
    let str = str.chars().rev().collect::<String>();
    writeln!(lk_sys::uart0(), "Very useful Rust code: {}", str).unwrap();
    return 0;
}

extern "C" fn rust_panic_cmd(_: c_int, _: *const console_cmd_args) -> c_int {
    panic!("onoes!");
}
