#define _POSIX_C_SOURCE 199309L
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int INFINITY =  2147483647;

int main()
{
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int V,E;
    scanf("%d %d",&V,&E);

    int * matrix = (int *)malloc( V* V * sizeof(int));
    for(int i=0;i<V;++i)
    {
        for(int j=0;j<V;++j)
        {
            *(matrix + i*V + j)=INFINITY;
            if (i==j) *(matrix + i*V + j)=0;
        }
    }

    int x,y,w;
    for(int i=0;i<E;++i)
    {
        scanf("%d %d %d",&x,&y,&w);
        if (*(matrix + (x-1)*V + (y-1))>w) *(matrix + (x-1)*V + (y-1))=w;
    }

    for(int k=0;k<V;k++)
    {
        for(int i=0;i<V;i++)
        {
            for(int j=0;j<V;j++)
            {
                if((k==j) || (k==i)) continue;
                if( ( (*(matrix + i*V + k) ) == INFINITY ) || ( (*(matrix + k*V + j)) == INFINITY ) ) continue;
                if( (*(matrix + i*V + j)) > ( (*(matrix + i*V + k) ) + ( *(matrix + k*V + j) ) ) ) 
                    (*(matrix + i*V + j)) = ( (*(matrix + i*V + k) ) + ( *(matrix + k*V + j) ) );
            }
        }
    }

    for(int i=0;i<V;++i)
    {
        for(int j=0;j<V;++j)
        {
            printf("%d ",(*(matrix + i*V + j)));
        }
        printf("\n");
    }

    free(matrix);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + 1e-9*(end.tv_nsec - start.tv_nsec);
    printf("Total time: %lf\n",time_taken);
    return 0;
}