use std::{thread, time::Duration};
use ctrlc;
// to build release version
// cargo build --release
fn main() {
    ctrlc::set_handler(move || {
        println!("Terminating application keep alive");
        std::process::exit(0);
    })
    .expect("Error setting ctrl handler");

    println!("Start keep alive sample app");
    loop {
        println!("staying alive...");
        thread::sleep(Duration::from_secs(5));
    }
}

// std::sync::mpsc::channel
// https://rust-cli.github.io/book/in-depth/signals.html
