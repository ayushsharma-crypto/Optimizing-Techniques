#define _POSIX_C_SOURCE 199309L
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define DEFAULT_ROW 1001
#define DEFAULT_COLUMN 1001


typedef struct Matrix {
	long long int matrix[DEFAULT_ROW][DEFAULT_COLUMN];
} Matrix;


void get_matrix(Matrix * mat, int r, int c );
void transpose_matrix(Matrix * mat);
void make_sm(int n,long long int s[n+1][n+1],long long int m[n+1][n+1], int D[n+1]);
void print_matrix(Matrix * R, int r, int c );
void print_all_matrix(Matrix *restrict all_matrix[],int D[],int n);
int fun_min(int x, int y);
Matrix * matrix_chain_multiplication(int si,int ei,int n,Matrix *restrict all_matrix[],int D[n+1],long long int s[n+1][n+1]);
Matrix * multiply_matrix_v1(Matrix * a, Matrix * b,int p,int q, int r);
Matrix * multiply_matrix_v2(Matrix * a, Matrix * b,int p,int q, int r);
Matrix * multiply_matrix_v3(Matrix * a, Matrix * b,int p,int q, int r);
Matrix * multiply_matrix_v4(Matrix * a, Matrix * b,int p,int q, int r);


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
    // printf("Time taken while getting input: %lf\n",time_taken1);


    long long int m[n+1][n+1];
    long long int s[n+1][n+1];
    make_sm(n,s,m, D);


    clock_gettime(CLOCK_MONOTONIC, &mid2);
    double time_taken2 = (mid2.tv_sec - mid1.tv_sec) + 1e-9*(mid2.tv_nsec - mid1.tv_nsec);
    // printf("Time taken for function make_sm: %lf\n",time_taken2);


    Matrix * answer = matrix_chain_multiplication(1,n,n,all_matrix,D,s);

    printf("%d %d\n",D[0],D[n]);
    print_matrix(answer,D[0],D[n]);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken3 = (end.tv_sec - mid2.tv_sec) + 1e-9*(end.tv_nsec - mid2.tv_nsec);
    // printf("Time taken by the matrix-chain-multiplication: %lf\n",time_taken3);
    printf("Total time: %lf\n",time_taken1+time_taken2+time_taken3);
    

    return 0;
}

int fun_min(int x, int y)
{
    return (x>y)?y:x;
}


void get_matrix(Matrix * mat, int r, int c )
{
	for (int i = 0; i < DEFAULT_ROW; i++) 
    {
		for (int j = 0; j < DEFAULT_COLUMN; j++) 
        {
            mat->matrix[i][j] = 0;
		}
	}
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

void transpose_matrix(Matrix * mat)
{
    register long long int i,j,swap;
    for(i = 0; i < DEFAULT_ROW; i++){
        for(j = 0; j < i; j++){
            swap = mat->matrix[i][j];
            mat->matrix[i][j] = mat->matrix[j][i];
            mat->matrix[j][i] = swap;
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
            printf("%lld ",R->matrix[i][j]);
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
    // printf("si = %d and ei = %d\n",si,ei);

    if (si==ei)
        return all_matrix[si-1];

    int ret_matrix_row = D[si-1];
    int ret_matrix_col = D[ei];

    int pi = s[si][ei];

    Matrix * ret =  multiply_matrix_v4(matrix_chain_multiplication(si,pi,n,all_matrix,D,s),matrix_chain_multiplication(pi+1,ei,n,all_matrix,D,s),ret_matrix_row,D[si],ret_matrix_col);

    struct timespec et;
    clock_gettime(CLOCK_MONOTONIC, &et);

    double time_taken = (et.tv_sec - st.tv_sec) + 1e-9*(et.tv_nsec - st.tv_nsec);
    // printf("Time taken by the matrix-multiplication-chain: %lf, si= %d,ei= %d\n",time_taken,si,ei);
    return ret;
}


Matrix * multiply_matrix_v1(Matrix * a, Matrix * b,int p,int q, int r)
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
        for(int j = 0; j < r; j++) 
        {
            for  (int k = 0; k < q; k++) 
                result->matrix[i][j] += (a->matrix[i][k] * b->matrix[k][j]);
        }
    }
    struct timespec et;
    clock_gettime(CLOCK_MONOTONIC, &et);

    double time_taken = (et.tv_sec - st.tv_sec) + 1e-9*(et.tv_nsec - st.tv_nsec);
    // printf("Time taken by the matrix-multiplication: %lf, p= %d,q= %d,r= %d\n",time_taken,p,q,r);
    return result;
}


Matrix * multiply_matrix_v2(Matrix * a, Matrix * b,int p,int q, int r)
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

    struct timespec et;
    clock_gettime(CLOCK_MONOTONIC, &et);

    double time_taken = (et.tv_sec - st.tv_sec) + 1e-9*(et.tv_nsec - st.tv_nsec);
    // printf("Time taken by the matrix-multiplication: %lf, p= %d,q= %d,r= %d\n",time_taken,p,q,r);
    return result;
}

Matrix * multiply_matrix_v3(Matrix * a, Matrix * b,int p,int q, int r)
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

    transpose_matrix(b);

 
    for (int i = 0; i < p; i++)	
    {
        for(int j = 0; j < r; j++) 
        {
            register long long int temp = 0;
            for  (int k = 0; k < q; k++) 
                temp += (a->matrix[i][k] * b->matrix[j][k]);
            result->matrix[i][j] = temp;
        }
    }
    struct timespec et;
    clock_gettime(CLOCK_MONOTONIC, &et);

    double time_taken = (et.tv_sec - st.tv_sec) + 1e-9*(et.tv_nsec - st.tv_nsec);
    // printf("Time taken by the matrix-multiplication: %lf, p= %d,q= %d,r= %d\n",time_taken,p,q,r);
    return result;
}



Matrix * multiply_matrix_v4(Matrix * a, Matrix * b,int p,int q, int r)
{
    Matrix *result;
    result=malloc(sizeof(Matrix));

    register int ii, jj, kk, BLOCK=21,BLOCK_SUM=512;
    register long long int *aii;
    register long long int *bjj;

    for (ii = 0; ii < p; ii++)	
    {
        for (jj = 0; jj < r; jj++) 
            result->matrix[ii][jj] =0;
    }
    
    transpose_matrix(b);

    for(int i=0;i<p;i+=BLOCK)
    {
        for(int j=0;j<r;j+=BLOCK)
        {
            for(int k=0;k<q;k+=BLOCK_SUM)
            {
                int min_I = fun_min(p,i+BLOCK);
                for(ii=i;ii<min_I;ii++)
                {
                    aii = a->matrix[ii];
                    register long long int temp = 0;
                    int min_J = fun_min(r,j+BLOCK);
                    for(jj=j;jj<min_J;jj++)
                    {
                        temp = 0;
                        bjj = b->matrix[jj];
                        int min_K = fun_min(q, k+BLOCK_SUM);
                        for(kk=k;kk<min_K-15;kk+=16)
                        {
                            temp += ( (*(aii+kk + 0)) * (*(bjj+kk + 0)) )
                                 +  ( (*(aii+kk + 1)) * (*(bjj+kk + 1)) )
                                 +  ( (*(aii+kk + 2)) * (*(bjj+kk + 2)) )
                                 +  ( (*(aii+kk + 3)) * (*(bjj+kk + 3)) )
                                 +  ( (*(aii+kk + 4)) * (*(bjj+kk + 4)) )
                                 +  ( (*(aii+kk + 5)) * (*(bjj+kk + 5)) )
                                 +  ( (*(aii+kk + 6)) * (*(bjj+kk + 6)) )
                                 +  ( (*(aii+kk + 7)) * (*(bjj+kk + 7)) )
                                 +  ( (*(aii+kk + 8)) * (*(bjj+kk + 8)) )
                                 +  ( (*(aii+kk + 9)) * (*(bjj+kk + 9)) )
                                 +  ( (*(aii+kk + 10)) * (*(bjj+kk + 10)) )
                                 +  ( (*(aii+kk + 11)) * (*(bjj+kk + 11)) )
                                 +  ( (*(aii+kk + 12)) * (*(bjj+kk + 12)) )
                                 +  ( (*(aii+kk + 13)) * (*(bjj+kk + 13)) )
                                 +  ( (*(aii+kk + 14)) * (*(bjj+kk + 14)) )
                                 +  ( (*(aii+kk + 15)) * (*(bjj+kk + 15)) );
                        }
                        while(kk < min_K){
                            temp += aii[kk] * bjj[kk];
                            kk++;
                        }
                        result->matrix[ii][jj]+=temp;
                    }
                }
            }
        }
    }

    return result;
}