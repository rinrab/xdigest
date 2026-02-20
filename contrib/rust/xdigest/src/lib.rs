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

/* SHA-1 */
pub const SHA1_DIGEST_LENGTH: usize = XDIG_SHA1_DIGEST_LENGTH as usize;

define_algorithm!(
    SHA1,
    xdig_sha1_ctx_t,
    xdig_sha1_ctx_size,
    xdig_sha1_ctx_init,
    xdig_sha1_ctx_update,
    xdig_sha1_ctx_final,
    xdig_sha1,
    SHA1_DIGEST_LENGTH
);

/* SHA-2 (256) */
pub const SHA256_DIGEST_LENGTH: usize = XDIG_SHA256_DIGEST_LENGTH as usize;
pub const SHA224_DIGEST_LENGTH: usize = XDIG_SHA224_DIGEST_LENGTH as usize;

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

define_algorithm!(
    SHA224,
    xdig_sha224_ctx_t,
    xdig_sha224_ctx_size,
    xdig_sha224_ctx_init,
    xdig_sha224_ctx_update,
    xdig_sha224_ctx_final,
    xdig_sha224,
    SHA224_DIGEST_LENGTH
);

/* SHA-2 (512) */
pub const SHA512_DIGEST_LENGTH: usize = XDIG_SHA512_DIGEST_LENGTH as usize;
pub const SHA384_DIGEST_LENGTH: usize = XDIG_SHA384_DIGEST_LENGTH as usize;

define_algorithm!(
    SHA512,
    xdig_sha512_ctx_t,
    xdig_sha512_ctx_size,
    xdig_sha512_ctx_init,
    xdig_sha512_ctx_update,
    xdig_sha512_ctx_final,
    xdig_sha512,
    SHA512_DIGEST_LENGTH
);


define_algorithm!(
    SHA384,
    xdig_sha384_ctx_t,
    xdig_sha384_ctx_size,
    xdig_sha384_ctx_init,
    xdig_sha384_ctx_update,
    xdig_sha384_ctx_final,
    xdig_sha384,
    SHA384_DIGEST_LENGTH
);

/* MD5 */
pub const MD5_DIGEST_LENGTH: usize = XDIG_MD5_DIGEST_LENGTH as usize;

define_algorithm!(
    MD5,
    xdig_md5_ctx_t,
    xdig_md5_ctx_size,
    xdig_md5_ctx_init,
    xdig_md5_ctx_update,
    xdig_md5_ctx_final,
    xdig_md5,
    MD5_DIGEST_LENGTH
);

pub const MD4_DIGEST_LENGTH: usize = XDIG_MD4_DIGEST_LENGTH as usize;

define_algorithm!(
    MD4,
    xdig_md4_ctx_t,
    xdig_md4_ctx_size,
    xdig_md4_ctx_init,
    xdig_md4_ctx_update,
    xdig_md4_ctx_final,
    xdig_md4,
    MD4_DIGEST_LENGTH
);

pub const MD2_DIGEST_LENGTH: usize = XDIG_MD2_DIGEST_LENGTH as usize;

define_algorithm!(
    MD2,
    xdig_md2_ctx_t,
    xdig_md2_ctx_size,
    xdig_md2_ctx_init,
    xdig_md2_ctx_update,
    xdig_md2_ctx_final,
    xdig_md2,
    MD2_DIGEST_LENGTH
);
