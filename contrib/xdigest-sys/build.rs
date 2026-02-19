use std::env;
use std::path::PathBuf;

#[allow(dead_code)]
fn main() {
    let target = env::var("TARGET").unwrap();

    let mut cmake = cmake::Config::new("../..");
    cmake.define("BUILD_SHARED_LIBS", "OFF");

    if target == "i686-pc-windows-msvc" {
        cmake.define("CMAKE_GENERATOR_PLATFORM", "Win32");
    }

    cmake.define("CMAKE_INSTALL_LIBDIR", "lib");

    let install_dir = cmake.build();

    let includedir = install_dir.join("include");
    let libdir = install_dir.join("lib");
    println!(
        "cargo:rustc-link-search=native={}",
        libdir.to_str().unwrap()
    );
    let libname = "xdigest";
    println!(
        "cargo:rustc-link-lib=static={}",
        libname,
    );
    println!("cargo:root={}", install_dir.to_str().unwrap());
    println!("cargo:include={}", includedir.to_str().unwrap());
    
    let bindings = bindgen::Builder::default() 
        .header(includedir.join("xdigest/xdigest.h").to_str().unwrap())
        .header(includedir.join("xdigest/xdigest_sha1.h").to_str().unwrap())
        .header(includedir.join("xdigest/xdigest_sha2.h").to_str().unwrap())
        .header(includedir.join("xdigest/xdigest_md5.h").to_str().unwrap())
        .generate()
        .expect("Failed to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Failed to write bindings");
}
