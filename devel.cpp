/*
 * FIR filter class, by Mike Perkins
 * 
 * a simple C++ class for linear phase FIR filtering
 *
 * For background, see the post http://www.cardinalpeak.com/blog?p=1841
 *
 * Copyright (c) 2013, Cardinal Peak, LLC.  http://www.cardinalpeak.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2) Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 
 * 3) Neither the name of Cardinal Peak nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * CARDINAL PEAK, LLC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "filt.h"

  

using namespace std;


int main(int argc, char *argv[])
{
	FILE *fd_in, *fd_out;
	Filter *my_filter;
	short samp_dat;
	double out_val;
	int num_read;
	char outfile1[32] = "taps.txt";
	char outfile2[32] = "freqres.txt";

	//my_filter = new Filter(LPF, 51, 44.1, 2.4);
	//my_filter = new Filter(HPF, 51, 44.1, 3.0);
	my_filter = new Filter(BPF, 51, 44.1, 2.3, 2.7);

	fprintf(stderr, "error_flag = %d\n", my_filter->get_error_flag() );
	if( my_filter->get_error_flag() < 0 ) exit(1);
	my_filter->write_taps_to_file( outfile1 );
	my_filter->write_freqres_to_file( outfile2 );


        FILE *fd_test;
        fd_test = fopen("test2.txt", "w");

        //call terminal to tranfer wav to raw data
        system("sox /home/wei/Dropbox/FIR-filter-class/newdata/1499561956588_262_279.wav --bits 16 --channels 1 --encoding signed-integer --endian little cut.raw");
	fd_in = fopen("cut.raw", "r");
	fd_out = fopen("filtered.raw", "w");


	int *start_vec, *end_vec, *event_vec;
	start_vec = (int*)malloc(1000*sizeof(int));
	end_vec = (int*)malloc(1000*sizeof(int));
        event_vec = (int*)malloc(1000*sizeof(int));

	int data_pos = 0, vec_pos = 0, peak_start = 0, peak_end = 0, cont_flag = 0, last_peak_start = 0, clusterNum = 0, eventNum = 0, event_start =0, event_end =0;
	while(1){
		num_read = fread(&samp_dat, sizeof(short), 1, fd_in);
		if(num_read != 1) break;
                //fprintf(fd_test, "%d\n", samp_dat);
		out_val = my_filter->do_sample( (double) samp_dat );
		samp_dat = (short) out_val;
                //if there are only 2 clusters that make samp_dat>300,read the first and last index
                //if there are more than 2 clusters that make..
                //if two instances with |samp_dat| >300 && index gap < 4000, then then belong to one cluster 
		if (samp_dat > 250 || samp_dat < -250) {
			if (!cont_flag)	{
				peak_start = data_pos;
				cont_flag = 1;
			}
			peak_end = data_pos;
		} else {
			if (cont_flag) {
				start_vec[vec_pos] = peak_start;
				end_vec[vec_pos] = peak_end;
				//printf("%d, %d\n", peak_start, peak_end);
				++vec_pos;
				cont_flag = 0;
			}
		}

	if((peak_start - last_peak_start) > 4000 && (peak_start - last_peak_start) <20000){
            printf("%d, %d\n", peak_start, clusterNum);
            if(clusterNum == 0) {
            event_start = peak_start;
            event_vec[eventNum] = peak_start;
            }
            clusterNum++;
            
        } if (peak_start> 0 && (peak_start - last_peak_start) >20000 || (peak_start - last_peak_start) < -20000){

            eventNum++;
            event_end = peak_end;
            //printf("%d, %d\n", eventNum, clusterNum);
            //printf("%d, %d\n", event_start, event_end);
            
            clusterNum = 0;
            event_start = peak_start;
        }
        fprintf(fd_test, "%d\n", samp_dat);
		fwrite(&samp_dat, sizeof(short), 1, fd_out);
		++data_pos;

        last_peak_start = peak_start;
	}

        //system("cat hdr.dat filtered.raw > filtered.wav")
	free(start_vec);
	free(end_vec);
	fclose(fd_in);
	fclose(fd_out);
	delete my_filter;
}	
