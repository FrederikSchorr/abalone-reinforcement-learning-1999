set terminal emtex
clear
set format x "$%g$"
set xlabel "$\xi$"


set format y "$%g$"
set ylabel "$\sigma(\xi)$"
set ytic -1,1,1
set key 3,-0.7   
set output "sigmoid.tex" 
plot [-4:4][-1.2:1.2] (sgn(x)+1)/2 title "treshold" with line 3,\
1/(1+exp(-x)) title "logistic" with line 1,\
tanh(x) title "hyperbolic tangent" with line 4
