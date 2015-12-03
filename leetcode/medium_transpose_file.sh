filetext=$(cat file.txt)
read -a fields <<< $filetext
#echo "fields: " $fields
colcount=`awk 'NR==1{print NF;}' file.txt`
#echo "colcount: " $colcount
rowcount=`awk 'BEGIN{r=0}{r=r+1;}END{print r}' file.txt`
#echo "rowcount: " $rowcount
rowlast_idx=$(($rowcount-1))
#echo $rowlast_idx
for (( i = 0; i < $colcount; i++ )); do
	for (( j = 0; j < $rowcount; j++ )); do
		k=$(($j*$colcount+$i))
		#echo "index: " $k
		if [[ $j -eq $rowlast_idx ]]; then
			echo "${fields[$k]}"
		else
			echo -e "${fields[$k]} \c"
		fi
	done
done
