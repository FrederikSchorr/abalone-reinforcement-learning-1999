set terminal emtex
set xlabel "Lookahead depth $n$" 0,-1
set format x "$%g$"
set xtic 1,1,5
set ytic


set output "e_a.tex"
set ylabel "$e_a$" -2,0
set logscale y
set format y "$%g$"
set key 4.5,2000  
plot [1:5][25:5e05] "caesar.tab" using 1:3 title "Minimax" with linesp 1 10, \
"charles.tab" using 1:3 title "$\alpha$--$\beta$" with linesp 1 4, \
"david.tab" using 1:3 title "Killer" with linesp 1 7, \
"emil.tab" using 1:3 title "Hash" with linesp 1 6, \
exp(log(58)*x) title "$M^n$" with line 6, \
"best.tab" using 1:2 title "Perfect $\alpha$--$\beta$" with line 4, \
"emil.tab" using 1:10 notitle with line 1  



set output "t.tex"
set ylabel "$t$(sec)" 0,0
set logscale y
set format y "$%g$"
set key 4.5,0.01
plot "caesar.tab" using 1:8 title "Minimax" with linesp 1 10, \
"charles.tab" using 1:8 title "$\alpha$--$\beta$" with linesp 1 4, \
"david.tab" using 1:8 title "Killer" with linesp 1 7, \
"emil.tab" using 1:8 title "Hash" with linesp 1 6



set output "b_e.tex"
set ylabel "$b_e$" 0,0
set nologscale y
set format y "$%g$"
set key 4.5,50
plot [][0:70] 58 title "$M$" with line 6, \
"caesar.tab" using 1:5 title "Minimax" with linesp 1 10, \
"charles.tab" using 1:5 title "$\alpha$--$\beta$" with linesp 1 4, \
"david.tab" using 1:5 title "Killer" with linesp 1 7, \
"emil.tab" using 1:5 title "Hash" with linesp 1 6



set output "b_t.tex"
set ylabel "$b_t$" 0,0
set nologscale y
set format y "$%g$"
set key 4.5,50 
plot [][0:70] 58 title "$M$" with line 6, \
"caesar.tab" using 1:6 title "Minimax" with linesp 1 10, \
"charles.tab" using 1:6 title "$\alpha$--$\beta$" with linesp 1 4, \
"david.tab" using 1:6 title "Killer" with linesp 1 7, \
"emil.tab" using 1:6 title "Hash" with linesp 1 6, \
19.21 title "$R_{\alpha\mbox{\scriptsize--}\beta}$" with line 2, \
"best.tab" using 1:3 title "Perfect $\alpha$--$\beta$" with line 4, \
"emil.tab" using 1:6 notitle with line 1



set output "h.tex"
set ylabel "$h$ (in \%)" 0,0
set nologscale y
set format y "$%g$"
set ytic 0,10,100
set key 4.5,90
plot [1:5][0:100] "emil.tab" using 1:9 title "Hash" with linesp 1 6


set output "reinWon.tex"
set ylabel "Games\\won\\ (in \%)" -2,0
set nologscale y
set format y "$%g$"
set ytic 0,10,100
set key 4.5,20
plot 67 notitle with line 4,\
  "gunilla.tab" using 1:5 title "Hash" with linesp 1 6, \
  "gunilla.tab" using 1:4 title "Reinforcement" with linesp 1 5


set output "reinTime.tex"
set ylabel "$t$(sec)" 0,0
set logscale y
set ytic
set format y "$%g$"
set key 4.5,0.01
plot 0.237 notitle with line 4, \
  "gunilla.tab" using 1:3 title "Hash" with linesp 1 6, \
  "gunilla.tab" using 1:2 title "Reinforcement" with linesp 1 5


set output "killer.tex"
set xlabel "Number of killer moves"
set xtic 0,1
set ylabel "$e_a$" 0,0
set nologscale y
set format y "$%g$"
set ytic 3000,1000
set key 8,7000
plot [][3500:7500] "killer.tab" title "3-ply Killer" with linesp 1 7


