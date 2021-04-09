categories = ""
delimiter = "#"
file = "../resources/analysis_results/test_output.tsv"
stats file using (categories = categories." ".delimiter.strcol(2).delimiter)

unique_categories = ""

do for [cat in categories] {
	if(strstrt(unique_categories, cat) == 0) {
		unique_categories = unique_categories." ".cat
	}
}

set terminal png size 707,500
set output './convergence.png'
set title 'Convergence'
set xlabel '% deviation from stable point'
set ylabel 'Iterations'

set xrange [0.5:1.5]

do for[cat in unique_categories] {
	plot for [cat in unique_categories] file using ($4):(delimiter.strcol(2).delimiter eq cat && $6 == $6 && $5 != 200 ? $5 :NaN) title cat[2:strlen(cat) -1] with lines

}
