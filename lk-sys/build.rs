use std::path::PathBuf;

fn main() {
    let lk_dir = PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("../lk")
        .canonicalize()
        .unwrap();
    let lk_dir = lk_dir.to_str().unwrap();
    let bindings = bindgen::Builder::default()
        .clang_arg("-DWITH_LIB_CONSOLE")
        .clang_arg(format!("-I{}/dev/include", lk_dir))
        .clang_arg(format!("-I{}/lib/console/include", lk_dir))
        .clang_arg(format!("-I{}/lib/heap/include", lk_dir))
        .clang_arg(format!("-I{}/lib/libc/include", lk_dir))
        .clang_arg(format!("-I{}/platform/include", lk_dir))
        .clang_arg(format!("-I{}/top/include", lk_dir))
        .header("lk.h")
        .use_core()
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Failed to generate bindings");

    let out_path = std::path::PathBuf::from(std::env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Failed to write bindings");
}
