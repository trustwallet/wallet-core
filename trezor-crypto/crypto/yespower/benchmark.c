/*-
 * Copyright 2013-2018 Alexander Peslyak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h> /* for atoi() */
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sched.h>

#include "yespower.h"

#ifdef _OPENMP
#include <omp.h>

#define NSAVE 1000

static uint64_t time_us(void)
{
	struct timespec t;
#ifdef CLOCK_MONOTONIC_RAW
	if (clock_gettime(CLOCK_MONOTONIC_RAW, &t))
		return 0;
#else
	if (clock_gettime(CLOCK_MONOTONIC, &t))
		return 0;
#endif
	return 1 + (uint64_t)t.tv_sec * 1000000 + t.tv_nsec / 1000;
}
#endif

int main(int argc, const char * const *argv)
{
	yespower_params_t params = {
		.version = YESPOWER_0_5,
		.N = 2048,
		.r = 8,
		.pers = (const uint8_t *)"Client Key",
		.perslen = 10
	};

	if (argc > 1)
		params.version = atoi(argv[1]);
	if (argc > 2)
		params.N = atoi(argv[2]);
	if (argc > 3)
		params.r = atoi(argv[3]);

	printf("version=%.1f N=%u r=%u\n",
	    params.version * 0.1, params.N, params.r);

	printf("Will use %.2f KiB RAM\n", 0.125 * params.N * params.r);

	static __thread union {
		uint8_t u8[80];
		uint32_t u32[20];
	} src;
	yespower_binary_t dst;
	unsigned int i;

	for (i = 0; i < sizeof(src); i++)
		src.u8[i] = i * 3;

	if (yespower_tls(src.u8, sizeof(src), &params, &dst)) {
		puts("FAILED");
		return 1;
	}

	for (i = 0; i < sizeof(dst); i++)
		printf("%02x%c", dst.uc[i], i < sizeof(dst) - 1 ? ' ' : '\n');

	puts("Benchmarking 1 thread ...");

	clock_t clk_tck = sysconf(_SC_CLK_TCK);
	struct tms start_tms, end_tms;
	clock_t start = times(&start_tms), end;
	unsigned int n;
	unsigned long long count;
#ifdef _OPENMP
	yespower_binary_t save[NSAVE];
	unsigned int nsave = 0;
#endif
	uint32_t seed = start * 1812433253U;

	n = 1;
	count = 0;
	do {
		for (i = 0; i < n; i++) {
			yespower_binary_t *p = &dst;
#ifdef _OPENMP
			if (nsave < NSAVE)
				p = &save[nsave++];
#endif
			src.u32[19] = seed + (count + i);
			if (yespower_tls(src.u8, sizeof(src), &params, p)) {
				puts("FAILED");
				return 1;
			}
		}
		count += n;

		end = times(&end_tms);
		n <<= 1;
	} while (end - start < clk_tck * 2);

	clock_t start_v = start_tms.tms_utime + start_tms.tms_stime +
	    start_tms.tms_cutime + start_tms.tms_cstime;
	clock_t end_v = end_tms.tms_utime + end_tms.tms_stime +
	    end_tms.tms_cutime + end_tms.tms_cstime;

	printf("%llu H/s real, %llu H/s virtual "
	    "(%llu hashes in %.2f seconds)\n",
	    count * clk_tck / (end - start),
	    count * clk_tck / (end_v - start_v),
	    count, (double)(end - start) / clk_tck);

	for (i = 0; i < nsave; i++) {
		unsigned int j;
		for (j = i + 1; j < nsave; j++) {
			unsigned int k = 8;
			if (!memcmp(&save[i], &save[j], k)) {
				printf("%u-byte collision(s) detected\n", k);
				i = nsave; break;
			}
		}
	}

#ifdef _OPENMP
	unsigned int nt = omp_get_max_threads();

	printf("Benchmarking %u thread%s ...\n",
	    nt, nt == 1 ? "" : "s");

	typedef struct {
		uint64_t min, max, total;
	} thread_data_s;
	union {
		thread_data_s s;
		uint8_t cachelines[2][64]; /* avoid false sharing */
	} thread_data[nt]; /* tricky to align this when on stack */

	unsigned int t;
	for (t = 0; t < nt; t++) {
		thread_data_s *td = &thread_data[t].s;
		td->min = ~(uint64_t)0; td->max = 0; td->total = 0;
	}

	unsigned long long count1 = count, count_restart = 0;

	if (!geteuid()) {
		puts("Running as root, so trying to set SCHED_RR");
#pragma omp parallel
		{
			struct sched_param param = { .sched_priority = 1 };
			if (sched_setscheduler(getpid(), SCHED_RR, &param))
				perror("sched_setscheduler");
		}
	}

	start = times(&start_tms);

	n = count * omp_get_max_threads();
	count = 0;
	do {
#pragma omp parallel for default(none) copyin(src) private(i, dst) shared(n, thread_data, params, seed, count, save, nsave)
		for (i = 0; i < n; i++) {
			unsigned int j = count + i;

			src.u32[19] = seed + j;

			uint64_t start1 = time_us();

			if (yespower_tls(src.u8, sizeof(src), &params, &dst)) {
#pragma omp critical
				puts("FAILED");
			}

			uint64_t end1 = time_us();
			if (end1 < start1)
				end1 = start1;
			uint64_t diff1 = end1 - start1;

			thread_data_s *td = &thread_data[omp_get_thread_num()].s;
			td->total += diff1;
			if (diff1 < td->min)
				td->min = diff1;
			if (diff1 > td->max)
				td->max = diff1;

#ifdef _OPENMP
			if (j < nsave && memcmp(&save[j], &dst, sizeof(dst))) {
#pragma omp critical
				printf("Mismatch at %u\n", j);
			}
#endif
		}

		count += n;
		if ((count - n) < count1 && count >= count1) {
/* Disregard our repeat of single thread's results (could be partially cached
 * by same core, but OTOH other cores not yet warmed up to full clock rate). */
			start = times(&start_tms);
			count_restart = count;
			for (t = 0; t < nt; t++) {
				thread_data_s *td = &thread_data[t].s;
				td->min = ~(uint64_t)0; td->max = 0; td->total = 0;
			}
		} else {
			n <<= 1;
		}

		end = times(&end_tms);
	} while (end - start < clk_tck);

	if (!count_restart)
		puts("Didn't reach single-thread's hash count");
	count -= count_restart;

	start_v = start_tms.tms_utime + start_tms.tms_stime +
	    start_tms.tms_cutime + start_tms.tms_cstime;
	end_v = end_tms.tms_utime + end_tms.tms_stime +
	    end_tms.tms_cutime + end_tms.tms_cstime;

	printf("%llu H/s real, %llu H/s virtual "
	    "(%llu hashes in %.2f seconds)\n",
	    count * clk_tck / (end - start),
	    count * clk_tck / (end_v - start_v),
	    count, (double)(end - start) / clk_tck);

	uint64_t min = ~(uint64_t)0, max = 0, total = 0;
	for (t = 0; t < nt; t++) {
		thread_data_s *td = &thread_data[t].s;
		total += td->total;
		if (td->min < min)
			min = td->min;
		if (td->max > max)
			max = td->max;
	}
	printf("min %.3f ms, avg %.3f ms, max %.3f ms\n",
		min / 1000.0, total / 1000.0 / count, max / 1000.0);
#endif

	return 0;
}
