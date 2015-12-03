#awk 'BEGIN{r=0} {r=r+1; if(r==10)print $0;}' file.txt
awk 'NR==10' file.txt
