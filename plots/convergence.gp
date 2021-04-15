categories = ""
delimiter = "#"
file = "../resources/analysis_results/parameter_convergence/result1.tsv"
stats file using (categories = categories." ".delimiter.strcol(2).delimiter)

unique_categories = ""

do for [cat in categories] {
	if(strstrt(unique_categories, cat) == 0) {
		unique_categories = unique_categories." ".cat
	}
}

print unique_categories

set terminal pngcairo size 707,500
set output './img/convergence.png'
set title 'Convergence'
set xlabel 'Initial deviation from stable point'
set ylabel 'Iterations'

set xrange [-0.5:0.5]

plot for [cat in unique_categories] file using ($4 - 1):(delimiter.strcol(2).delimiter eq cat && $6 == $6 && $5 < 200 ? $5 :NaN) title cat[2:strlen(cat) -1] with lines

set output './img/convergence_solution_drift.png'
set title 'Convergence of stable parameters'
set xlabel 'Initial deviation from stable point'
set ylabel 'Final deviation from stable point'
#set yrange [1e-9:10]
unset yrange
set logscale y

#cats_to_print = "#Tt_1# #Tt_5# #P_6# #v_7# #P_7# #P_8# #v_10# #P_10#"
plot for [cat in unique_categories] file using ($4 - 1):(delimiter.strcol(2).delimiter eq cat && $6 == $6 && $5 < 200 ? $7 :NaN) title cat[2:strlen(cat) -1] with lines

set output './img/convergence_log_stable.png'
set title 'Convergence of stable parameters'
set xlabel 'Initial deviation from stable point (log scale)'
set ylabel 'Iterations'
unset xrange
unset logscale

set yrange [20:34]
set key top left
set logscale x
set xrange [1e-3:0.5]

cats_to_print = "#Tt_1# #Tt_5# #P_6# #v_7# #P_7# #P_8# #v_10# #P_10#"
plot for [cat in cats_to_print] file using (abs($4-1)):(delimiter.strcol(2).delimiter eq cat && $6 == $6 && $5 < 200 ? $5 :NaN) title cat[2:strlen(cat) -1] with lines

set output './img/convergence_log_unstable.png'
set title 'Convergence of unstable parameters'

set yrange [5:100]
set key top left

cats_to_print = "#v_6# #T_6# #T_7# #w_8# #v_9# #P_9# #T_9# #T_10#"
plot for [cat in cats_to_print] file using (abs($4-1)):(delimiter.strcol(2).delimiter eq cat && $6 == $6 && $5 < 200 ? $5 :NaN) title cat[2:strlen(cat) -1] with lines