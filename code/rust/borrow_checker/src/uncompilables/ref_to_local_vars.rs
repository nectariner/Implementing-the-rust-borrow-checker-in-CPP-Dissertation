pub fn local_var() -> &i32 {
    let x = 10;
    &x
}

fn main() {
    let y = local_var();
}
