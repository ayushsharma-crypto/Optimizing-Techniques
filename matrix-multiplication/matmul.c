#include<stdio.h>
#include<stdlib.h>

typedef struct Matrix {
	int matrix[1001][1001];
} Matrix;


void get_matrix(Matrix * R, int r, int c )
{
	for (int i = 0; i < r; i++) 
    {
		for (int j = 0; j < c; j++) 
        {
            int x;
            scanf("%d",&x);
            R->matrix[i][j] = x;
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

int main()
{
    int n;
    scanf("%d",&n);

    int D[n+1];
    Matrix * restrict all_matix[n];

    for(int i=1;i<=n;i++)
    {
        int r,c;
        scanf("%d %d",&r,&c);
        D[i-1]=r;
        D[i]=c;

        all_matix[i-1] = malloc(sizeof(Matrix));
        get_matrix(all_matix[i-1],r,c);
    }

    // printf("Now Printing all matrix:\n\n");
    // for(int i=1;i<=n;i++)
    // {
    //     printf("\nr= %d c=%d",D[i-1],D[i]);
    //     print_matrix(all_matix[i-1],D[i-1],D[i]);
    //     printf("\n");
    // }

    
    
    return 0;
}