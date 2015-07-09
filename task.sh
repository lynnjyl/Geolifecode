#! /bin/sh

for (( i=0; i<=9; i++ ))
do	
	./StayPoints 00$i 150 900 ../StayPoints7
done

for (( i = 10; i < 100; i++))
do
	./StayPoints 0$i 150 900 ../StayPoints7
done

for (( i = 100; i < 182; i++))
do
	./StayPoints $i 150 900 ../StayPoints7
done
echo "end task"
