pub fn main() {
    let mut x = 10;
    let y = &mut x;
    let z = &mut x;

    *y += 1;
}
