#!/usr/bin/gnuplot -e

set term post eps color solid enh "Times-Roman" 16
#set colorsequence classic
#set parameters
#set output opt


set output "compare2.eps"


data1 = './test2.txt'
#data2 = './1499640655471_left_0_60_filtered.dat'
#data3 = './1499640674588_0_60_filtered.dat'

set lmargin 12
set bmargin 6
set rmargin 4
set tmargin 2

shift = 0
x = 0.01
y = 0.0
z = 0.025 


lwi = 1

set key top right
set key spacing 3
#set key font "Times-Roman, 40"


set xlabel 'time' font "Times-Roman,40"
set xlabel offset -1.3,-1.3
set xtics nomirror font "Times-Roman,25"
set xtics offset -0.4,-0.4


#set xrange [2:5]
set ylabel 'speed' font "Times-Roman,40"
set ylabel offset -1.5,-1
set ytics nomirror font "Times-Roman,15"
set ytics offset 0,-0.4

#set yrange [-0.1:0.1]

set size 2,1

set pointsize 1
set border 3
#set grid

set boxwidth 0.3
set style fill solid
#set xtics ("1" 3, "120" 1.08, "150" 2.08, "250" 3.08,"500" 4.08,"1000" 5.08 )


#plot data using ($4):($3) smooth unique t 'on' with linespoints ls 2

#plot data using ($6):($2) smooth unique t 'distance' with linespoints ls 2,\
#data using ($6):($5) smooth unique t 'std' with linespoints ls 4

#plot data using ($6):($3) smooth unique t 'time' with linespoints ls 3

#plot data1 using ($1/1000-20.75) t 'data 1' with linespoints ls 3,\
#data2 using ($1/1000-17.5):($2) t 'data 2' with linespoints ls 2,\
#data3 using ($1/1000):($2) t 'data 3' with linespoints ls 4,\

plot data1 using ($1) t 'data 1' with linespoints ls 3,\

#data2 using ($1/1000):($2) t 'data 2' with linespoints ls 2,\

#plot data2 using ($1/1000):($2) t 'data 2' with linespoints ls 2,\

#plot data3 using ($1/1000):($2) t 'data 3' with linespoints ls 4,\
