use std::thread;

fn main() {
    let mut x = 50;
    let t1 = thread::spawn(|| {
        x = x + 1;
    });

    let t2 = thread::spawn(|| {
        x = x + 1;
    });
}
