#define BILLION 1000000000
#define calclock(timevalue, total_time, total_count) do { \
	        unsigned long long timedelay, temp, temp_n; \
	        struct timespec *myclock = (struct timespec*)timevalue; \
	        if(myclock[1].tv_nsec >= myclock[0].tv_nsec){ \
				                temp = myclock[1].tv_sec - myclock[0].tv_sec; \
				                temp_n = myclock[1].tv_nsec - myclock[0].tv_nsec; \
				                timedelay = BILLION * temp + temp_n; \
				        } else { \
							                temp = myclock[1].tv_sec - myclock[0].tv_sec - 1; \
							                temp_n = BILLION + myclock[1].tv_nsec - myclock[0].tv_nsec; \
							                timedelay = BILLION * temp + temp_n; \
							        } \
	        __sync_fetch_and_add(total_time, timedelay); \
	        __sync_fetch_and_add(total_count, 1); \
} while(0)

