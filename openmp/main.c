#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define VARIAS 6
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

void fill(double *a, int size, double value)
{  
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}
void parallel_dotprod(double* a,double* b,double* sum,int nthreads,int worksize)
{
	omp_set_num_threads(nthreads);
	int size = nthreads * worksize;
	double soma_parcial = 0;
	int i = 0;
	#pragma omp parallel shared(a,b,sum,worksize) private(i,soma_parcial)
	{
		soma_parcial = 0;
		#pragma omp for schedule(static,worksize) nowait
		for(i = 0; i < size; i++)
		{
			soma_parcial += (a[i]*b[i]);
		}
		#pragma omp critical
		{
			*(sum) += soma_parcial;
		}
	}
}

int main() 
{
	int reps,size,nthreads;
	long start_time,end_time;
	FILE* output = fopen("openmp_results.csv","w");
	for (size = 1000000; size <=2000000; size*=2)
	{
		for (reps = 2000;reps <= 8000;reps*=2)
		{
			double* a = (double*)malloc(size*sizeof(double));
			double* b = (double*)malloc(size*sizeof(double));
			fill(a,size,0.01);
			fill(b,size,1);
			double c = 0;
			double single_thread_time = 0;
			double acceleration = 1;
			for (nthreads = 1; nthreads <= 8; nthreads*=2)
			{
				int worksize = size/nthreads;
				long totaltime = 0;
				for(int j = 0; j < VARIAS;j++) // Faz o teste varias vezes e calcula a média dos tempos
				{
					start_time = wtime();
					for(int i = 0; i< reps;i++)
					{
						c = 0;
						parallel_dotprod(a,b,&c,nthreads,worksize);
					}
					end_time   = wtime();
					totaltime  += (long) (end_time - start_time);
				}
				totaltime/=VARIAS;
				
				if (nthreads == 1)
					single_thread_time = (double) totaltime;
				else
					acceleration = single_thread_time / ((double)totaltime);
				
				fprintf(output,"OpenMP,%d,%d,%d,%ld,%.3f\n",nthreads,worksize,reps,totaltime,acceleration);
				printf("OpenMP,%d,%d,%d,%ld,%.3f\n",nthreads,worksize,reps,totaltime,acceleration);
				printf("Result = %f\n",c);
				printf("---\n");
			}
			printf("/////////////////////////////////\n");
			free(a);
			free(b);
		}
	}
	fclose(output);
	return 0;
}
