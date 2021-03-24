#define _POSIX_C_SOURCE 199309L
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define DEFAULT_ROW 1001
#define DEFAULT_COLUMN 1001


typedef struct Matrix {
	int matrix[DEFAULT_ROW][DEFAULT_COLUMN];
} Matrix;


void get_matrix(Matrix * mat, int r, int c );
void make_sm(int n,long long int s[n+1][n+1],long long int m[n+1][n+1], int D[n+1]);
void print_matrix(Matrix * R, int r, int c );
void print_all_matrix(Matrix *restrict all_matrix[],int D[],int n);
Matrix * matrix_chain_multiplication(int si,int ei,int n,Matrix *restrict all_matrix[],int D[n+1],long long int s[n+1][n+1]);
Matrix * multiply_matrix(Matrix * a, Matrix * b,int p,int q, int r);


int main()
{
    struct timespec start, end, mid1, mid2;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int n;
    scanf("%d",&n);

    int D[n+1];
    Matrix * restrict all_matrix[n];

    for(int i=1;i<=n;i++)
    {
        int r,c;
        scanf("%d %d",&r,&c);
        D[i-1]=r;
        D[i]=c;

        all_matrix[i-1] = malloc(sizeof(Matrix));
        get_matrix(all_matrix[i-1],r,c);
    }

    clock_gettime(CLOCK_MONOTONIC, &mid1);
    double time_taken1 = (mid1.tv_sec - start.tv_sec) + 1e-9*(mid1.tv_nsec - start.tv_nsec);
    printf("Time taken while getting input: %lf\n",time_taken1);


    long long int m[n+1][n+1];
    long long int s[n+1][n+1];
    make_sm(n,s,m, D);


    clock_gettime(CLOCK_MONOTONIC, &mid2);
    double time_taken2 = (mid2.tv_sec - mid1.tv_sec) + 1e-9*(mid2.tv_nsec - mid1.tv_nsec);
    printf("Time taken for function make_sm: %lf\n",time_taken2);


    Matrix * answer = matrix_chain_multiplication(1,n,n,all_matrix,D,s);

    // printf("%d %d\n",D[0],D[n]);
    // print_matrix(answer,D[0],D[n]);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken3 = (end.tv_sec - mid2.tv_sec) + 1e-9*(end.tv_nsec - mid2.tv_nsec);
    printf("Time taken by the matrix-chain-multiplication: %lf\n",time_taken3);
    printf("Total time: %lf\n",time_taken1+time_taken2+time_taken3);
    

    return 0;
}


void get_matrix(Matrix * mat, int r, int c )
{
	for (int i = 0; i < r; i++) 
    {
		for (int j = 0; j < c; j++) 
        {
            int x;
            scanf("%d",&x);
            mat->matrix[i][j] = x;
		}
	}
}


void make_sm(int n,long long int s[n+1][n+1],long long int m[n+1][n+1], int D[n+1])
{
    for(int i=0;i<n+1;i++)
    {
        for(int j=0;j<n+1;j++)
        {
            m[i][j]=0;
            s[i][j]=0;
        }
    }

    for(int d=1;d<n;d++)
    {
        for (int i=1;i<n-d+1;i++)
        {
            int j = i+d;
            long long int min = 1000000000000000;
            for(int k=i;k<=j-1;k++)
            {
                long long int temp = m[i][k] + m[k+1][j] + D[i-1]*D[k]*D[j];
                if (min > temp)
                {
                    min = temp;
                    s[i][j] = k;
                }
            }
            m[i][j] = min;
        }
    }
}


void print_matrix(Matrix * R, int r, int c )
{
	for (int i = 0; i < r; i++) 
    {
		for (int j = 0; j < c; j++) 
        {
            printf("%d ",R->matrix[i][j]);
		}
        printf("\n");
	}
}


void print_all_matrix(Matrix *restrict all_matrix[],int D[],int n)
{
    printf("Now Printing all matrix:\n\n");
    for(int i=1;i<=n;i++)
    {
        printf("\nr= %d c=%d",D[i-1],D[i]);
        print_matrix(all_matrix[i-1],D[i-1],D[i]);
        printf("\n");
    }
}


Matrix * matrix_chain_multiplication(int si,int ei,int n,Matrix *restrict all_matrix[],int D[n+1],long long int s[n+1][n+1])
{
    struct timespec st;
    clock_gettime(CLOCK_MONOTONIC, &st);
    printf("si = %d and ei = %d\n",si,ei);

    if (si==ei)
        return all_matrix[si-1];

    int ret_matrix_row = D[si-1];
    int ret_matrix_col = D[ei];

    int pi = s[si][ei];

    Matrix * ret =  multiply_matrix(matrix_chain_multiplication(si,pi,n,all_matrix,D,s),matrix_chain_multiplication(pi+1,ei,n,all_matrix,D,s),ret_matrix_row,D[si],ret_matrix_col);

    struct timespec et;
    clock_gettime(CLOCK_MONOTONIC, &et);

    double time_taken = (et.tv_sec - st.tv_sec) + 1e-9*(et.tv_nsec - st.tv_nsec);
    printf("Time taken by the matrix-multiplication-chain: %lf, si= %d,ei= %d\n",time_taken,si,ei);
    return ret;
}


Matrix * multiply_matrix(Matrix * a, Matrix * b,int p,int q, int r)
{    

    struct timespec st;
    clock_gettime(CLOCK_MONOTONIC, &st);

    Matrix *result;
    result=malloc(sizeof(Matrix));

    for (int i = 0; i < p; i++)	
    {
        for (int j = 0; j < r; j++) 
            result->matrix[i][j] =0;
    }

 
    for (int i = 0; i < p; i++)	
    {
        for (int k = 0; k < q; k++) 
        {
            for (int j = 0; j < r; j++) 
                result->matrix[i][j] += (a->matrix[i][k] * b->matrix[k][j]);
        }
    }
    // for (int i = 0; i < p; i++)	
    // {
    //     for   (int j = 0; j < r; j++) 
    //     {
    //         for(int k = 0; k < q; k++)
    //             result->matrix[i][j] += (a->matrix[i][k] * b->matrix[k][j]);
    //     }
    // }
    

    // #define N 1000

    // int stridei = 20, stridek = 512, stridej = 20, i, j, k, gotill;
    // register int *arow;
    // register int *brow;
    // register int ii, jj, kk, tmp;

    // for(ii = 0; ii < p; ii++){
    //     for(jj = 0; jj < r; jj++){
    //         result->matrix[ii][jj] = 0;
    //     }
    // }

    // #define min(x, y) (y^((x^y) & -(x<y)))

    // for(i = 0; i < N; i++){
    //     for(j = 0; j < i; j++){
    //         tmp = b->matrix[i][j];
    //         b->matrix[i][j] = b->matrix[j][i];
    //         b->matrix[j][i] = tmp;
    //     }
    // }

    // for(i = 0; i < p; i += stridei){
    //     for(j = 0; j < r; j += stridej){
    //         for(k = 0; k < q; k += stridek){
    //             for(ii = i; ii < min(i + stridei, p); ii++){
    //                 arow = a->matrix[ii];
    //                 for(jj = j; jj < min(j + stridej, r); jj++){
    //                     gotill = min(k + stridek, q);
    //                     tmp = 0;
    //                     brow = b->matrix[jj];
    //                     for(kk = k; kk + 15 < gotill; kk+=16){
    //                         tmp += arow[kk + 0] * brow[kk + 0]
    //                             +  arow[kk + 1] * brow[kk + 1]
    //                             +  arow[kk + 2] * brow[kk + 2]
    //                             +  arow[kk + 3] * brow[kk + 3]
    //                             +  arow[kk + 4] * brow[kk + 4]
    //                             +  arow[kk + 5] * brow[kk + 5]
    //                             +  arow[kk + 6] * brow[kk + 6]
    //                             +  arow[kk + 7] * brow[kk + 7]
    //                             +  arow[kk + 8] * brow[kk + 8]
    //                             +  arow[kk + 9] * brow[kk + 9]
    //                             +  arow[kk + 10] * brow[kk + 10]
    //                             +  arow[kk + 11] * brow[kk + 11]
    //                             +  arow[kk + 12] * brow[kk + 12]
    //                             +  arow[kk + 13] * brow[kk + 13]
    //                             +  arow[kk + 14] * brow[kk + 14]
    //                             +  arow[kk + 15] * brow[kk + 15];
    //                     }
    //                     while(kk < gotill){
    //                         tmp += arow[kk] * brow[kk];
    //                         kk++;
    //                     }
    //                     result->matrix[ii][jj] += tmp;
    //                 }
    //             }
    //         }
    //     }
    // }

    struct timespec et;
    clock_gettime(CLOCK_MONOTONIC, &et);

    double time_taken = (et.tv_sec - st.tv_sec) + 1e-9*(et.tv_nsec - st.tv_nsec);
    printf("Time taken by the matrix-multiplication: %lf, p= %d,q= %d,r= %d\n",time_taken,p,q,r);
    return result;
}