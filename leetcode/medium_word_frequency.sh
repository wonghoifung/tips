#cat words.txt | tr -cs "[a-z]" "\n" | sort | uniq -c | sort -r | sed 's/^[ \t]*//g'|awk '{print $2 " " $1}'
cat words.txt | tr -cs "[a-z]" "\n" | sort | uniq -c | sort -r |awk '{print $2 " " $1}'
