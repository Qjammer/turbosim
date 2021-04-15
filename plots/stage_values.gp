file = "../resources/setups/single_spool_setup_initial_params.tsv"

set terminal pngcairo size 1414,1000 font ",17"
set output './img/stage_plot.png'
set title 'Fluid state at each stage' offset 0,-0.5

set margins screen 0.07, screen 0.82, screen 0.05, screen 0.95
set xlabel 'Stage'
set ylabel 'Temperature [K]' offset 1,0 textcolor rgb "red"
set y2label 'Pressure [Pa]' offset -1,0 textcolor rgb "blue"
set key off
set ytics nomirror tc "red"
set y2tics 90000,10000 tc "blue"
set y2range [90000:170000]

array Data[17]
stats file using (Data[int($0+1)] = $2) prefix "B"

array Stagelabels[6] = ["Inlet tank", "Compressor inlet", "Compressor outlet", "Turbine inlet", "Turbine outlet", "Outlet tank"]
array Tvars[6] = [1, 6, 9, 14, 17, 3]
array Pvars[6] = [0,5,8,13,16,0]
array Vvars[6] = [0,4,7,12,15,0]

print Tvars
array Tdata[6]
array Pdata[6]
array vdata[6]

do for[i=1:6] {
	Tdata[i] = Data[Tvars[i]]
}
Pdata[1] = 101300
vdata[1] = 0
do for[i=2:5] {
	Pdata[i] = Data[Pvars[i]]
	vdata[i] = Data[Vvars[i]]
}
Pdata[6] = 101300
vdata[6] = 0

print Tdata
print Pdata
print vdata

set multiplot

plot Tdata using 1:2:xtic(Stagelabels[$1]) axis x1y1 with lines lc "red",\
	Pdata using 1:2:xtic(Stagelabels[$1]) axis x1y2 with lines lc "blue"


unset xlabel
unset ylabel
unset y2label
unset ytics
unset xtics
unset title

set border 4
set y2range[0:200]
set yrange[0:200]
set xrange[1:6]
plot vdata using 1:2:xtic(Stagelabels[$1]) axis x1y2 with lines lc "sea-green"

set rmargin at screen 0.93
set border 8
set y2label 'Speed [m/s]' offset  -1,0 textcolor rgb "sea-green"
set y2tics 0,20 tc "sea-green"
plot NaN

unset multiplot