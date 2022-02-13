#[cfg(test)]
#[cfg(doctest)]
mod tests {
    #[test]
    /// idk walt this is kinda sus
    fn it_fails() {
        let t = trybuild::TestCases::new();
        t.compile_fail("src/uncompilables/*.rs");
    }
}
