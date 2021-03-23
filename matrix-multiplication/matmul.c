#include<stdio.h>
#include<stdlib.h>

typedef struct Matrix {
	int matrix[1001][1001];
} Matrix;


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
        printf("\n");
		for (int j = 0; j < c; j++) 
        {
            printf("%d ",R->matrix[i][j]);
		}
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

int main()
{
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

    // print_all_matrix(all_matrix,D,n);

    long long int m[n+1][n+1];
    long long int s[n+1][n+1];
    make_sm(n,s,m, D);


    // printf("\nm array : \n");
    // for(int i=0;i<n+1;i++)
    // {
    //     for(int j=0;j<n+1;j++)
    //     {
    //         printf("%lld ",m[i][j]);
    //     }
    //     printf("\n");
    // }

    // printf("\ns array : \n");
    // for(int i=0;i<n+1;i++)
    // {
    //     for(int j=0;j<n+1;j++)
    //     {
    //         printf("%lld ",s[i][j]);
    //     }
    //     printf("\n");
    // }

    

    return 0;
}