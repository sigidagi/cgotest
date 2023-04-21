#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
use std::env;
use std::ffi::{c_void, CString};

extern "C" fn callback(setup: *mut QSetup, context: *mut c_void) {
    println!("Setup in callback was {:?}", unsafe { setup.as_ref() });
    if !context.is_null() {
        let context: &mut Context = unsafe { &mut *(context as *mut Context) };
        println!("Context content in callback was {:?}", context);
        context.info = CString::new("New info").unwrap();
    }
}

#[repr(C)]
#[derive(Debug)]
struct Context {
    info: CString,
}

fn parse_qr_code(code: &str) {
    let mut setup: QSetup = QSetup::default();

    //Create raw pointer to mutable setup var. Create from mutable reference (&mut)
    let setup_ptr: *mut QSetup = &mut setup;

    let c_str_code = CString::new(code).unwrap();
    let mut context = Context {
        info: CString::new("Info").unwrap(),
    };

    // The C functions are unsafe so we have to call them in an unsafe block
    unsafe {
        RegisterCallback(Some(callback), &mut context as *mut _ as *mut c_void);
        let res = QRParse(c_str_code.as_ptr(), setup_ptr);
        if res != 0 {
            println!("Error: {}", res);
        }
    }
    println!("Context after callback {:?}", context);
}

fn main() {
    println!("Hello, from rust");
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("You must provide exactly 1 QR Code string as first argument");
        return;
    }
    parse_qr_code(args.get(1).unwrap());
}
