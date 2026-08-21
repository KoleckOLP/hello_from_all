#[unsafe(no_mangle)]
pub extern "C" fn hello_from_rs() {
    println!("hello from Rust");
}
