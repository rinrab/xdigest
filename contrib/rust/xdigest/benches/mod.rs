/*
 * note: to compile this file, a nightly versin of rust compiler is required
 * because the whole bench feature is considered unstable. why? no idea. 
 */

#![feature(test)]
extern crate test;

use xdigest::XDigest;

fn create_buf(buf: &mut [u8]) {
    let mut i = 0;
    let mut seed: u32 = 67;

    while (i < buf.len()) {
        seed = (seed * 1103515245 + 12345) & 0xffffffff;
        buf[i] = (seed & 0xff) as u8;
        i += 1;
    }
}

macro_rules! bench_update {
    (
        $init:expr,
        $name:ident
    ) => {
        #[bench]
        fn $name(b: &mut test::Bencher) {
            let mut buf = [0u8; 1024 * 1024];
            let mut i = 0;
            let mut ctx = $init;

            create_buf(&mut buf);

            b.iter(|| {
                ctx.update(&buf);
                i += 1;
            });
            b.bytes = buf.len() as u64;

            let _hash = ctx.finalise();
        }
    }
}

bench_update!(
    xdigest::SHA256::new(),
    sha256
);

bench_update!(
    xdigest::SHA512::new(),
    sha512
);

bench_update!(
    xdigest::SHA1::new(),
    sha1
);

bench_update!(
    xdigest::MD5::new(),
    md5
);
