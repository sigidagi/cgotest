[#](#) 

### Build C static library


```
cd build
./configure
make

```

It will create static lib: `libqrparser.a` in `build` directory

### Build an Run golang client

Follow this [link](https://go.dev/doc/install) to install `golang` compiler

```
cd bindings/go
go get ./...                            // fetch all dependencies
make                                    // compile
./build/qrparser code justadummystring  // run

```

### Build and Run Rust client

You have to have rust tools already installed. You can use use [rustup](https://rustup.rs/) to install rust.
Rustup is the officially recommended way to install and manage your rust toolchain(s) and associated tools like cargo.

```
cd binding/rust 
cargo run justadummystring
```

#### Alternative example of rust <-> c++ bindings using cxx create

You can use the rust cxx crate to create direct bindings between c++ and rust without intermediate C wrappers

```
cd bindings/rust_cxx
cargo run
```

### Build and Run Zig client

Install [Zig](https://ziglang.org/) compiler first

```
cd bindings/zig
zig build
./zig-out/bin/parser_cli
```
