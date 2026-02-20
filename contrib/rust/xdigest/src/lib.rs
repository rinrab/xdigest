use std::{mem::MaybeUninit};

use xdigest_sys::*;

pub trait XDigest<const DIGEST_SIZE: usize> {
    fn new() -> Self;

    fn update(&mut self, data: &[u8]);

    fn finalise(&mut self) -> [u8; DIGEST_SIZE];

    fn digest(data: &[u8]) -> [u8; DIGEST_SIZE];
}

macro_rules! define_algorithm {
    (
        $struct:ident,
        $ctx_c:ident,
        $ctx_size_c:ident,
        $ctx_init_c:ident,
        $ctx_update_c:ident,
        $ctx_final_c:ident,
        $digest_c:ident,
        $digest_size:expr
    ) => {
        pub struct $struct {
            ptr: *mut $ctx_c,
            _storage: Box<[MaybeUninit<u8>]>,
        }

        impl XDigest<$digest_size> for $struct {
            fn new() -> Self {
                let ctx_size = unsafe { $ctx_size_c() };
                let storage = vec![MaybeUninit::uninit(); ctx_size].into_boxed_slice();
                let ptr = storage.as_ptr() as *mut $ctx_c;
                unsafe { $ctx_init_c(ptr) };
                return Self { ptr, _storage: storage }
            }

            fn update(&mut self, data: &[u8]) {
                unsafe {
                    $ctx_update_c(
                        self.ptr,
                        data.as_ptr() as *const std::os::raw::c_void,
                        data.len()
                    );
                };
            }

            fn finalise(&mut self) -> [u8; $digest_size] {
                let mut result = [0u8; $digest_size];
                unsafe {
                    $ctx_final_c(
                        self.ptr,
                        result.as_mut_ptr() as *mut std::os::raw::c_uchar,
                    );
                };
                return result;
            }

            fn digest(data: &[u8]) -> [u8; $digest_size] {
                let mut result = [0u8; $digest_size];
                unsafe {
                    $digest_c(
                        result.as_mut_ptr() as *mut std::os::raw::c_uchar,
                        data.as_ptr() as *const std::os::raw::c_void,
                        data.len(),
                    );
                };
                return result;
            }
        }
    };
}

pub const SHA256_DIGEST_LENGTH: usize = XDIG_SHA256_DIGEST_LENGTH as usize;

define_algorithm!(
    SHA256,
    xdig_sha256_ctx_t,
    xdig_sha256_ctx_size,
    xdig_sha256_ctx_init,
    xdig_sha256_ctx_update,
    xdig_sha256_ctx_final,
    xdig_sha256,
    SHA256_DIGEST_LENGTH
);

