data peppertest;
infile '/home/u54352672/MA 3740/Data/Exam 2/problem8.csv' dlm = ',' firstobs = 2;
input shu pepper$ time$;

/* proc print data = peppertest; */

proc sort data = peppertest; by pepper;
proc means data = peppertest n mean std; var shu; by pepper;

proc sort data = peppertest; by time;
proc means data = peppertest n mean std; var shu; by time;

proc anova; class pepper time; model shu = pepper time;
means pepper /T; means time /T;

run;