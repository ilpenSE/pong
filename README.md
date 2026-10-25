# Ping Pong Game in Raylib

- Simple ping-pong game made with [Raylib](https://raylib.com) in C++.

<img width="1006" height="740" alt="game_ss" src="https://github.com/user-attachments/assets/aa4073a2-6724-47ba-9871-1d1c5d851ef9" />

## Quick Start

- Build the project and then run the executable if you have raylib system-wide:

```console
$ make
$ ./main
```

## Game

- Press W or S to move up/down the left paddle
- Press up or down arrow keys to move up/down the right paddle
- Press SPACE to pause the game (everything'll be frozen)
- If a side couldn't bounce the ball and ball passes through screen, this counts as other side's score
- On any scoring, ball'll be resetted to the start position (center of the screen) and goes to scorer's side

## References

- Raylib: https://raylib.com
- Sounds: https://opengameart.org/content/3-ping-pong-sounds-8-bit-style
