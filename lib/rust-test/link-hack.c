/* The Rust code in this module only exports symbols in the "commands"
 * link section. GCC ld filters out the archive entirely unless we make it look
 * more "live" with this hack. The alternative to this would be to add
 * --whole-archive linker option to EXTRA_OBJS */
__attribute__((constructor))
static void forcelink() {
  extern void rust_test_forcelink();
  rust_test_forcelink();
}
