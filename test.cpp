#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sndfile.h>
#include <vector>

int main(int argc, char* argv[]) {

	// Create file names
	if(argc < 2) {
		printf("No input file!\n");
		return -1;
	}
	char f1 [256];
	sprintf(f1, "%s.wav", argv[1]);
	printf("Opening '%s.wav'\n", argv[1]); 

	// Open the WAV file. 
	SF_INFO info;
	info.format = 0;
	SNDFILE *sf = sf_open(f1,SFM_READ,&info);
	if (sf == NULL) {
		printf("Failed to open the file.\n");
		exit(-1);
	}

	// Allocate space for the data to be read, then read it.
	int num_items = info.frames * info.channels;
	int* buf = (int *) malloc(num_items*sizeof(int));
	int num = sf_read_int(sf,buf,num_items);
	if(num < 1) {
		printf("Failed to read the file.\n");
		exit(-1);
	}
	sf_close(sf);

	// Write the data to filedata.out. 
	// FILE* out = fopen("filedata.out","w");
	// for(int i = 0; i < num; i += info.channels) {
	// 	for (int j = 0; j < info.channels; ++j)
	// 		fprintf(out,"%d ",buf[i+j]);
	// 	fprintf(out,"\n");
	// }
	// fclose(out);

	// Traverse the data: If a time point is higher than the threshold
	// set it as t0 and ti. Then, if any other point is close to ti within
	// a small period, then adapt ti. Keep If not after the same 
	int lastStartTime = -1;
	int lastEndTime = -1;
	int INTENSITY_THRESHOLD = 50000000;
	int TIME_THRESHOLD = 500000;
	int POINTS_THRESHOLD = 20000;
	int pointsInPeriod = 0;
	int timeSinceLastPointInPeriod= -1;
	std::vector <std::pair<int,int> > startTimes;
	for(int i = 0; i < num; i += info.channels) {

		// Check if reached the end with an ongoing period
		if((i >= (num - info.channels)) && timeSinceLastPointInPeriod > -1) {
			if(lastStartTime > 0 && (pointsInPeriod > POINTS_THRESHOLD)) 
				startTimes.push_back(std::make_pair(lastStartTime,lastEndTime));
			timeSinceLastPointInPeriod = -1;
			lastStartTime = -1;
			pointsInPeriod = 0;
		}

		// Update the time since last start
		if(timeSinceLastPointInPeriod > -1) timeSinceLastPointInPeriod += 1;

		// Check if the last start point should be forgotten
		if(timeSinceLastPointInPeriod > TIME_THRESHOLD) {
			if(lastStartTime > 0 && (pointsInPeriod > POINTS_THRESHOLD)) 
				startTimes.push_back(std::make_pair(lastStartTime,lastEndTime));
			timeSinceLastPointInPeriod = -1;
			lastStartTime = -1;
			pointsInPeriod = 0;
		}

		// Check if this point is relevant
		if(abs(buf[i]) < INTENSITY_THRESHOLD) continue;

		// Check if this point should be regarded as a new point
		if(timeSinceLastPointInPeriod < 0) {
			if(lastStartTime > 0 && (pointsInPeriod > POINTS_THRESHOLD)) 
				startTimes.push_back(std::make_pair(lastStartTime,lastEndTime));
			lastStartTime = i/2;
			timeSinceLastPointInPeriod = 0;
			pointsInPeriod = 0;
		}

		// Otherwise, consider it a part of the current period
		else {
			timeSinceLastPointInPeriod = 0;
			lastEndTime = i/2;
			pointsInPeriod++;
		}
	}

	printf("# periods: %lu\n", startTimes.size());

	// Print for matlab visualization
	FILE* out2 = fopen("keyTimes", "w");
	for(int i = 0; i < startTimes.size(); i++) {
		fprintf(out2, "%d\n%d\n", startTimes[i].first, startTimes[i].second);
	}
	fclose(out2);

	// Cut the file
	char cmd [256];
	for(int i = 0; i < startTimes.size(); i++) {
		int t0 = floor(((double) startTimes[i].first) / 44100);
		int tf = ceil(((double) startTimes[i].second) / 44100);
		sprintf(cmd, "mp3splt %d.%d %d.%d %s.mp3\n", t0/60, t0%60, tf/60, tf%60, argv[1]);
		system(cmd);
	}

	







	return 0;
}
