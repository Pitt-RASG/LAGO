#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>



void run_cpu_for_one_second() {
	time_t start, end;
	double elapsed;
	start = time(NULL);
	int terminate = 1;
	while (terminate) {
		end = time(NULL);
		elapsed = difftime(end, start);
		if (elapsed >= 1.0 /* seconds */) {
			terminate = 0;
		} else {
			int agg = 0;
			int i;
			for (i=0;i<100000;i++) {
				agg += i*i+i-i;
			}
		}
	}
}


void run_mem_bound_for_one_second() {
	long* arr = (long*) malloc(sizeof(long)*1000*4096);
	time_t start, end;
	double elapsed;
	start = time(NULL);
	int terminate = 1;
	while (terminate) {
		end = time(NULL);
		elapsed = difftime(end, start);
		if (elapsed >= 1.0 /* seconds */) {
			terminate = 0;
		} else {
			long agg = 0;
			int i;
			for (i = 0; i < 100; i++) {
				agg += arr[i*4096];
			}
		}
	}
}

int main(void) {
	int i;
	for (i=0;i<60;i++) {
		run_cpu_for_one_second();
		run_mem_bound_for_one_second();
	}
}

