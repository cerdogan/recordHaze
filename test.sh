unalias grep
i="1"
while [ $i -lt 20000 ]
do

# Start recording
audio-recorder -c start; 
audio-recorder -c hide; 
sleep 1200; 
audio-recorder -c stop;
i=$((i+1))	

# Get the last recording
cp ~/Audio/`ls -tp ~/Audio | grep -v /$ | head -1` "curr$i.mp3"

# Process the recording
mpg123 -w "curr$i.wav" "curr$i.mp3"
./a.out "curr$i"
 
# Place to dropbox
cp "curr$i_"* ~/Documents/Dropbox/Haze

done
