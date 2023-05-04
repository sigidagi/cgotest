fn main() {
    cxx_build::bridge("src/main.rs") // returns a cc::Build
        .file("cppdemo/demo.cpp")
        .flag_if_supported("-std=c++14")
        .compile("cppdemo-bridge");

    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=cppdemo/demo.cpp");
    println!("cargo:rerun-if-changed=cppdemo/demo.hpp");
}
