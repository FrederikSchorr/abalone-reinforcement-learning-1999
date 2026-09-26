# Thesis-LaTeX

LaTeX source of the diploma thesis *Reinforcement Learning Abalone* (TU Wien, February 1999). The finished PDF is in the repository root.

- `Main.tex` – main document (LaTeX2e, `report` class, Latin-1 encoding); includes the chapters `Title.tex`, `Intro.tex`, `Rules.tex`, `Search.tex`, `Neuro.tex`, `Reinforc.tex`, `Design.tex`, `Conclus.tex`.
- `MainUser.tex`, `Userguid.tex` – the user guide of the Malin program as a separate document.
- `Malin.bib`, `MlGame.bib` – bibliographies.
- `Experim/` – tables and text of the experiments (tournament results of the players).
- `Graphics/`, `RulesEps/`, `UserGuid/` – figures (`.EPS`, `.pic`, CorelDraw `.cdr` originals).
- `Ps2Text/` – text extracted from selected pages of the PostScript output.
- `CORRECT.ENG` – word list for the spell checker; `Q.TEX` – scratch file.

Build (as originally, DVI/PostScript): `latex Main`, `bibtex Main`, `makeindex`, `latex Main` (twice). Generated files (`.aux`, `.dvi`, `.ps`, `.log`, `.toc`, ...) are not included.
