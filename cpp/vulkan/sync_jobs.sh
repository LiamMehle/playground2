# sync with jobs
for pid in `jobs -p`
do
    wait $pid
done
