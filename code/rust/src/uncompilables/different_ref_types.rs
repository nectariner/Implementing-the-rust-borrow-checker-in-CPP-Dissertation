pub fn main() {
    let mut x = 10;
    let mutable_ref = &mut x;
    let const_ref = &x;

    *mutable_ref += 1;
    // mutable_ref lives until here
    // meaning that on line 4 there is a mutable reference and an immutable reference (illegal)
}
