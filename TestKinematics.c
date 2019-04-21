#include <stdio.h>
#include <math.h>

float Ux,Uy,Uz;
int i;
int x;

float P[6];
float D[6];
float L[6]; // l=p-b
float Tp[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

void assignTp(float Ux,float Uy,float Uz,float Tarray[][4]);
void restoreTp(float Tarray[][4]);


int main(void){

    Ux=200;
    Uy=300;
    Uz=400;
    
    for(i=0;i<4;i++){
        for (x=0;x<4;x++){
            printf("%f\n", Tp[x][i]);
        }
    }

    printf("Now insert new values:\n");

    assignTp(Ux,Uy,Uz,Tp);

    for(i=0;i<4;i++){
        for (x=0;x<4;x++){
            printf("%f\n", Tp[x][i]);
        }
    }
    
    printf("Restore array:\n");

    restoreTp(Tp);

    for(i=0;i<4;i++){
        for (x=0;x<4;x++){
            printf("%f\n", Tp[x][i]);
        }
    }

    return 1;
}

void assignTp(float Ux,float Uy,float Uz,float Tarray[][4]){
    Tarray[0][3]=Ux;
    Tarray[1][3]=Uy;
    Tarray[2][3]=Uz;
}


void restoreTp(float Tarray[][4]){
    Tarray[0][3]=0;
    Tarray[1][3]=0;
    Tarray[2][3]=0;
}