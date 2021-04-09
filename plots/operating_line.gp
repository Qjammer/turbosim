file = "../resources/analysis_results/test4.tsv"

col = 1
row = 1
initial_vals = ""

N = 19
array first_row_values[N]
do for [i=1:N] {
	stats file every ::row::row using i nooutput
	first_row_values[i] = STATS_max
}

print first_row_values
set terminal png size 1414,1000
set output './operating_line.png'
set title 'Convergence'
set xlabel 'Mass flow [kg/s]'
set ylabel 'Value with respect to initial'

set key bottom right

plot for [i=3:N] '< (head -n 1 '.file.' && tail -n +2 '.file.' | sort -nk4)' using ($4):($1 != 250  && $2 == $2 ? strcol(i)/first_row_values[i] : NaN) with lines title columnhead(i)