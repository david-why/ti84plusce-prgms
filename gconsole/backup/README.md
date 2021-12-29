# gconsole-backup
Backup of `prgmAACONSOL`.

## Usage
1. Send `zconsbak.8xp` to your calculator.
2. Run the program. If `ERR: MEMORY` occurs, archive some stuff and then try again.
3. This will install an application named `ZCONSBAK` in your apps menu.
4. To restore a backup of `prgmAACONSOL` and the C libraries, simply create a program with arbitrary name and put the following contents:
   ```
   OpenLib(ZCONSBAK
   ExecLib 1
   ```
   The tokens `OpenLib(` and `ExecLib` can be found at the bottom of the `prgm` menu.
5. Run the program, and voila! `prgmAACONSOL` is back, along with the C libraries it needs.