#![no_std]

// Explicitly depend on lk_sys crate for panic handler
extern crate lk_sys;

use core::fmt::Write;
use lk_sys::sys::{app_descriptor, lk_time_t};

#[no_mangle]
pub unsafe extern "C" fn rust_app_forcelink() { /* see link-hack.c */ }

#[link_section = "apps"]
#[no_mangle]
pub static mut APP: lk_sys::sys::app_descriptor = lk_sys::sys::app_descriptor {
    name: c"rust-example".as_ptr(),
    init: Some(init),
    entry: Some(entry),
    stack_size: 0,
    flags: lk_sys::sys::APP_FLAG_NO_AUTOSTART,
};

unsafe extern "C" fn init(_: *const app_descriptor) {
    writeln!(lk_sys::uart0(), "Rust app initialized.").unwrap();
}

unsafe extern "C" fn entry(_: *const app_descriptor, _: *mut ::core::ffi::c_void) {
    writeln!(lk_sys::uart0(), "Doing very complicated Rust-y things.").unwrap();
    lk_sys::sys::thread_sleep(1000 as lk_time_t);
    writeln!(lk_sys::uart0(), "Ok done.").unwrap();
}
