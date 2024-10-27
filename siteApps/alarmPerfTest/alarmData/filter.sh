cat alarm.txt | awk '{print $2, $4}' | column -t -s "," | column -t -s ")" >> test.txt
