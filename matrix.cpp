#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>
#include<string.h>

#define upper 5
#define lower 0

void generateValues(int**,int,int);
void print(int**,int,int);
void *multiplicate(void*);
void proof(int);

//int **matrixans;
pthread_mutex_t lock;
int r1,c1,r2,c2;
int step=0;
int **matrix1,**matrix2,**matrixans;

int main(){

	int hilos;

	printf("Ingrese numero de filas y columnas para matriz 1:");
	scanf("%d %d",&r1,&c1);
	printf("Ingrese numero de filas y columnas para matriz 2:");
	scanf("%d %d",&r2,&c2);
	printf("Ingrese el numero de hilos a ejecutarse:");
	scanf("%d",&hilos);


	if(c1!=r2){
		fprintf(stderr,"El numero de columnas de la matriz 1 no es igual al numero de filas de la matriz 2\n");
		return EXIT_FAILURE;
	}


	int*tmp;
	matrix1=(int**)malloc(r1*sizeof(int*));
	tmp=(int*)malloc(r1*c1*sizeof(int*));
	for(int i=0;i<r1;i++){
		matrix1[i]=tmp;
		tmp+=c1;
	}

	matrix2=(int **)malloc(r2*sizeof(int*));
	tmp=(int*)malloc(r2*c2*sizeof(int*));
	for(int i=0;i<r2;i++){
		matrix2[i]=tmp;
		tmp+=c2;
	}

	matrixans=(int **)malloc(r1*sizeof(int*));
	tmp=(int*)malloc(r1*c2*sizeof(int*));
	for(int i=0;i<r1;i++){
		matrixans[i]=tmp;
		tmp+=c2;
	}


	generateValues(matrix1,r1,c1);
	generateValues(matrix2,r2,c2);
	proof(hilos);

  /*
	printf("-----------------------------------------\n");
	printf("-----------------------------------------\n");
	printf("Matrix 1\n");
	print(matrix1,r1,c1);
	printf("Matrix 2\n");
	print(matrix2,r2,c2);
	printf("Matrix Answer\n");
	print(matrixans,r1,c2);
  */

	return EXIT_SUCCESS;
}
void proof(int hilos){
	struct timeval start;
	struct timeval finish;
	long compTime;
	double time;
	int i;

	pthread_t workers[hilos];
	gettimeofday(&start,0);
	for(i=0;i<hilos;i++){
		if(pthread_create(&workers[i],NULL,multiplicate,(void*)&hilos)){
			fprintf(stderr,"Hilo num %d tuvo error al ejecutarse\n",&i);
			return;
		}
	}
	for(i=0;i<hilos;i++)
		pthread_join(workers[i],NULL);
	gettimeofday(&finish,0);
	compTime=(finish.tv_sec-start.tv_sec)*1000000;
	compTime+=finish.tv_usec-start.tv_usec;
	time=(double)compTime;
	time/=1000000.0;
	printf("The time of %d threads is %f seconds\n",hilos,time);
}

void generateValues(int **matrix,int r,int c){
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			matrix[i][j]=(rand()%(upper-lower+1))+lower;
}
void print(int **matrix,int r,int c){
	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++)
			printf("%d ",matrix[i][j]);
		printf("\n");
	}
}

void *multiplicate(void*param){
	int *hilos=(int*)param;
	int count=step++;
	int l=count*r1/(*hilos);
	int r=(count+1)*r1/(*hilos);

	for(int i=l;i<r;i++)
		for(int j=0;j<c2;j++)
			for(int k=0;k<c1;k++){
				matrixans[i][j]+=matrix1[i][k]*matrix2[k][j]; 
			}

	pthread_exit(0);
}
