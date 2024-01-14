extern crate bindgen;

use std::process::Command;
use std::env;
use std::path::{PathBuf};

fn main() {
    let out_dir = env::var("OUT_DIR").unwrap();

    // note that there are a number of downsides to this approach, the comments
    // below detail how to improve the portability of these commands.
    let mut cc_args = vec!["tga/tgaimage.cpp", "-shared", "-rdynamic", "-fPIC", "-o"];

    if env::var("PROFILE").unwrap_or(String::from("debug")) == "debug" {
        cc_args.insert(0, "-g");
    }

    Command::new("g++").args(cc_args.as_slice())
                       .arg(&format!("{}/libtgaimage.so", out_dir))
                       .status().unwrap();


    println!("cargo:rustc-link-search=native={}", out_dir);
    println!("cargo:rustc-link-lib=tgaimage");

    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    let bindings = bindgen::Builder::default()
        // The input header we would like to generate
        // bindings for.
        .header("tga/tgaimage.hpp")
        .allowlist_type("TGAImage")
        .allowlist_type("TGAColor")
        //.conservative_inline_namespaces() // see issue #789
        // Finish the builder and generate the bindings.
        .generate()
        // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
