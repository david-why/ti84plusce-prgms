# hangman
This is a port of the Hangman game to the TI-84+ CE.

## Usage
Just send the `HANGMAN.8xp` and `HNGMNWRD.8xv` to your calculator. You might want to put the 8xv in the Archive because it is pretty big.

## Change words
To use other words than the pre-built 8xv, go to the folder `kwdsgen/src`. You should see a `mkkeywords.py`. Run it. On the first line, enter the pack name. This has to be LESS THAN 9 characters long! Then input all the words you need, one word per line. Each word MUST be 11 characters or less. After you finish typing, enter your operating system's `EOF` character (on Windows this should be <kbd>Ctrl</kbd>+<kbd>Z</kbd> then <kbd>Enter</kbd>, on Linux <kbd>Ctrl</kbd>+<kbd>D</kbd>). Then, run `make` in the `kwdsgen` folder. Send the `HNGMANKW.8xp` in `bin` to your calc, then run it. This *should* print out `Finished!` after some time. Press any key, and (if you wish) delete this program. (It is pretty big!) Now you may run `prgmHANGMAN` with your new words!
