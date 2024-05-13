# lk-msm8916

This is an effort to port/rebase/rewrite the downstream Little Kernel sources from [lk2nd][lk2nd] / CAF for modern upstream LK.

## Development

You'll need:
 * An `arm-none-eabi` toolchain in your PATH.
 * An msm8916 device with UART access.
   > Some Samsung msm8916 devices have a MUIC that exposes UART via a [debug cable][debug-cable].

### Building

```
make -j$(nproc)
```

### Running

Uh, I'll get back to you on that. Right now I'm chainloading from a brutally hacked-up lk2nd.

[lk2nd]: https://github.com/msm8916-mainline/lk2nd/
[debug-cable]: https://wiki.postmarketos.org/wiki/Serial_debugging/Cable_schematics#microUSB/Carkit_debug_cable
