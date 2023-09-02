if [[ -f student-record && -x student-record ]];
then
    if [ -f record.txt ]; 
    then
        records=$(cat record.txt)
        records=$(echo $records | tr '\n' ' ')
        bool=0
        for i in $records
        do
            if [ $bool == 0 ]
            then
                ./student-record -f database -c $i
                bool=$(($bool+1))
            else
                ./student-record -f database -a $i
            fi
        done
        ./student-record -f database -l
        xxd database
        for j in $records
        do
            nif=$(echo $j | cut -d ':' -f2)
            ./student-record -f database -q -n $nif
        done
    else
        echo "Missing txt file"
    fi
else
    echo "Missing file"
fi  