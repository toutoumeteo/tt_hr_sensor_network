R_PROC=plot_tt_hr

DATE=$(date +%Y%m%d)
for file in ${HOME}/data/tt_hr_sensor_network/*${DATE}.txt ; do

   echo ${file}

   cat > job.r <<EOF
 source("${R_PROC}.r")
 ${R_PROC}(file="${file}",title="${file%*_${DATE}.txt}")
EOF
   R --no-save < job.r
   
   exit
   
done
