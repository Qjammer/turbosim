file = "../resources/analysis_results/operating_line/result1.tsv"

row = 315
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
set title 'Operating line'
set xlabel 'Fuel mass flow [kg/s]'
set ylabel 'Variation with respect to initial'
set xrange [0.0008: 0.0018]
set key bottom right

plot for [i=3:N] '< (head -n 1 '.file.' && tail -n +2 '.file.' | sort -nk4)' using ($4):($1 != 250  && $2 == $2 ? strcol(i)/first_row_values[i] : NaN) with lines title columnhead(i)