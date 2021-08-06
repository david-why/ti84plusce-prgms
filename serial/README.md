# serial
This is a test of the `srldrvce` CE C Library.

This library is still in development, you need to compile it from the toolchain.

## Compilation
1. Clone the `srldrvce-rewrite` branch of the toolchain, found [here](https://github.com/CE-Programming/toolchain/tree/srldrvce-rewrite).
2. Run `make` and `make install`. This will install the toolchain in your home directory, `~/CEdev`.
3. Run `make release-libs`. This will create a `clibs.8xg` file in the toolchain directory.
4. Set the `CEDEV` environment variable to the install directory (e.g. `CEDEV=~/CEdev`).
5. Run `make` in this program directory. This should create the `SRLTEST.8xp` in the `bin` directory. Alternatively, use the compiled one already in the directory.
6. Send the `clibs.8xg` in the toolchain directory and `SRLTEST.8xp` in `bin` to your calculator.
7. Connect your calculator to a LINUX computer. For some reason the serial library does NOT work with Windows.
8. Run `python3 main.py` on the Linux computer. This requires [Python 3](https://www.python.org) to be installed.
9. Run `prgmSRLTEST` on your calculator.
10. I hope the program is self-explanatory :-)
Note: Sometimes the ModemManager service will send `AT` commands to the calculator, like `AT^SQPORT?`. Just ignore them- do not disable the ModemManager service.

In the program, you may use the `[alpha]` key to input letters. When you press it once, the input is locked to lowercase. When you press it again, the input is locked to UPPERCASE. When you press it one more time, the input reverts to normal.

You may also use the `[2nd]` key. When you press it, the input is locked to the blue text on your calc. When you press it again, the input reverts back to normal.

`[2nd]` has precedence over `[alpha]`.

When you press `[2nd]` and then `[alpha]`, you go back one screen.
