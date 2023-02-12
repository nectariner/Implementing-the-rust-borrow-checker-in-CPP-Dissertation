#[cfg(test)]
mod tests {
    #[test]
    fn it_fails() {
        let t = trybuild::TestCases::new();
        t.compile_fail("src/uncompilables/*.rs");
    }
}
