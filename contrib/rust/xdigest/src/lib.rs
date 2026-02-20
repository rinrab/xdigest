use std::{mem::MaybeUninit};

use xdigest_sys::*;

pub trait XDigest<const DIGEST_SIZE: usize> {
    fn new() -> Self;

    fn update(&mut self, data: &[u8]);

    fn finalise(&mut self) -> [u8; DIGEST_SIZE];

    fn digest(data: &[u8]) -> [u8; DIGEST_SIZE];
}

pub struct SHA256 {
    ptr: *mut xdig_sha256_ctx_t,
    _storage: Box<[MaybeUninit<u8>]>,
}

const SHA256_SIZE: usize = XDIG_SHA256_DIGEST_LENGTH as usize;
impl XDigest<SHA256_SIZE> for SHA256 {
    fn new() -> Self {
        let ctx_size = unsafe {
            xdig_sha256_ctx_size()
        };
        let storage = vec![MaybeUninit::uninit(); ctx_size].into_boxed_slice();
        let ptr = storage.as_ptr() as *mut xdig_sha256_ctx_t;
        unsafe {
            xdig_sha256_ctx_init(ptr);
        };
        return Self { ptr, _storage: storage }
    }

    fn update(&mut self, data: &[u8]) {
        unsafe {
            xdig_sha256_ctx_update(
                self.ptr,
                data.as_ptr() as *const std::os::raw::c_void,
                data.len()
            );
        };
    }

    fn finalise(&mut self) -> [u8; SHA256_SIZE] {
        let mut result = [0u8; SHA256_SIZE];
        unsafe {
            xdig_sha256_ctx_final(
                self.ptr,
                result.as_mut_ptr() as *mut std::os::raw::c_uchar,
            );
        };
        return result;
    }

    fn digest(data: &[u8]) -> [u8; SHA256_SIZE] {
        let mut result = [0u8; SHA256_SIZE];
        unsafe {
            xdig_sha256(
                result.as_mut_ptr() as *mut std::os::raw::c_uchar,
                data.as_ptr() as *const std::os::raw::c_void,
                data.len(),
            );
        };
        return result;
    }
}
