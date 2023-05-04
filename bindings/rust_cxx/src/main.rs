#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("rust_cxx/cppdemo/demo.hpp");

        type Demo;
        fn new_demo() -> UniquePtr<Demo>;
        fn demo_do(self: &Demo);
    }
}

fn main() {
    let demo = ffi::new_demo();
    demo.as_ref().unwrap().demo_do();
}
