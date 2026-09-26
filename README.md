# Abalone Reinforcement Learning (1999)

Source code, players and documentation from my diploma thesis *Reinforcement Learning Abalone* (TU Wien, February 1999). **Malin** is a Windows program that plays the board game Abalone. Its players are neural networks trained by reinforcement learning, mostly through self-play.

## Thesis

- Library record: [TU Wien catalogplus](https://catalogplus.tuwien.at/permalink/f/8j3js/UTW_alma2150385700003336)
- PDF in this repo: [1999-02 - Reinforcement Learning Abalone - Schorr.pdf](<1999-02 - Reinforcement Learning Abalone - Schorr.pdf>)

## Repository layout

| Path | Content |
|------|---------|
| `Kernel/` | Game engine, players, training and tournament code (C++) |
| `Windows/` | Windows GUI (Visual C++ 6) |
| `Text/` | Console front end |
| `Release/` | Released build: `Malin.exe`, setup files, and the finished players (`.ap`) used as opponents in the thesis: Gunilla1–5, Aba4, Boub4, Caesar4, Charles4, David4, Emil5p, Random, Human |
| `Game/` | Game definition files (`.ag`) |
| `*.as` | Training and self-play scripts |
| `UserGuide/` | HTML user guide |
| `Predecessors/` | Earlier DOS versions of Abalone (1988–1998) |
| `Thesis-LaTeX/` | LaTeX source of the thesis |
| `Web/` | Web pages of the project |

## Notes

- Intermediate training checkpoints (about 4,000 `.ap` files) are not part of this repo.
- The project was written for Windows and Visual C++ 6 (`Malin.dsw`).
