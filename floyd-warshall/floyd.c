#define _POSIX_C_SOURCE 199309L
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int INFINITY =  1000000000;

int fun_min(int x, int y);
// void floyd_v1(int * matrix, int V);
// void floyd_v2(int * matrix, int V);
void floyd_v3(int * matrix, int V);


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

    floyd_v3(matrix,V);

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

int fun_min(int x, int y)
{
    return (x>y)?y:x;
}


// void floyd_v1(int * matrix, int V)
// {
//     for(int k=0;k<V;++k)
//     {
//         for(int i=0;i<V;++i)
//         {
//             for(int j=0;j<V;++j)
//             {
//                 if(k==j) continue;
//                 if(k==i) continue;
//                 if( (*(matrix + k*V + j)) == INFINITY ) continue;
//                 if( (*(matrix + i*V + k)) == INFINITY ) continue;
//                 if( (*(matrix + i*V + j)) > ( (*(matrix + i*V + k) ) + ( *(matrix + k*V + j) ) ) ) 
//                     (*(matrix + i*V + j)) = ( (*(matrix + i*V + k) ) + ( *(matrix + k*V + j) ) );
//             }
//         }
//     }
// }


// void floyd_v2(int * matrix, int V)
// {
//     register int i,j,k,kmj,imk, v;
//     register int * km;
//     register int * im;
//     v = V;
//     for(k=0;k<v;++k)
//     {
//         km = (matrix + k*v);
//         for(i=0;i<v;++i)
//         {
//             im = (matrix + i*v);
//             for(j=0;j<v;++j)
//             {
//                 kmj = (*(km + j));
//                 imk = (*(im + k));
//                 if( (*(im + j)) > ( kmj + imk ) ) 
//                     (*(im + j)) = ( kmj + imk );
//             }
//         }
//     }
// }


void FW_BLOCK(int * matrix,register int I,register int J,register int K,register int BLOCK,register int V)
{
    register int ii, jj, kk;
    int GOTILL = fun_min(V,K+BLOCK);

    int FH = fun_min(V, I+BLOCK)-I+1;
    int FW = fun_min(V, J+BLOCK)-J+1;
    
    int AH = fun_min(V, I+BLOCK)-I+1;
    int AW = fun_min(V, K+BLOCK)-K+1;
    
    int BH = fun_min(V, K+BLOCK)-K+1;
    int BW = fun_min(V, J+BLOCK)-J+1;
    
    for(kk=K;kk<GOTILL;++kk)
    {
        for(ii=0;ii<FH;++ii)
        {
            for(jj=0;jj<FW;++jj)
            {
                if(kk==jj) continue;
                if(kk==ii) continue;
                register int aik = (*(matrix + ( V*(I+ii) ) + ( kk )));
                if( aik == INFINITY) continue;
                register int bkj = (*(matrix + ( V*( kk ) ) + ( J + jj )));
                if( bkj == INFINITY) continue;
                if( aik+bkj < (*(matrix + ( V*( I + ii ) ) + ( J + jj ))) )
                    (*(matrix + ( V*( I + ii ) ) + ( J + jj )))= aik + bkj;
            }
        }
    }
}

void floyd_v3(int * matrix, int V)
{
    register int BLOCK=16;

    for(int K=0;K<V;K+=BLOCK)
    {
        // Diagonal update
        printf("\n\nDiagonal K=%d\n\n",K);
        FW_BLOCK(matrix,K,K,K,BLOCK,V);
        

        // Panel Row Update
        printf("\n\nPanel Row K=%d\n\n",K);
        for(int J=0;J<V;J+=BLOCK)
        {
            if(J!=K) 
            {
                FW_BLOCK(matrix,K,J,K,BLOCK,V);
            }
        }

        // Panel Column Update
        printf("\n\nPanel Column K=%d\n\n",K);
        for(int I=0;I<V;I+=BLOCK)
        {
            if(I!=K) 
            {
                FW_BLOCK(matrix,I,K,K,BLOCK,V);
            }
        }

        // MinPlus Outer Product
        printf("\n\nMinPlus Outer K=%d\n\n",K);
        for(int I=0;I<V;I+=BLOCK)
        {
            if (I==K) continue;
            for(int J=0;J<V;J+=BLOCK)
            {
                if(J!=K) 
                {
                    FW_BLOCK(matrix,I,J,K,BLOCK,V);
                }
            }
        }
    }
}