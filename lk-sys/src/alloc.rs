use core::alloc::{GlobalAlloc, Layout};

pub struct Heap;

#[cfg_attr(not(test), global_allocator)]
pub static ALLOCATOR: Heap = Heap;

unsafe impl GlobalAlloc for Heap {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        crate::sys::memalign(layout.align() as _, layout.size() as _) as _
    }

    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        crate::sys::free(ptr as _);
    }

    unsafe fn realloc(&self, ptr: *mut u8, _layout: Layout, new_size: usize) -> *mut u8 {
        crate::sys::realloc(ptr as _, new_size as _) as _
    }
}
