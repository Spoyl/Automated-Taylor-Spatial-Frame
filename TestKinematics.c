/* ---------------------------------------------
 *                OLIVER STOCKS
 *  MENG MECHANICAL WITH BIOMEDICAL ENGINEERING
 *      UNIVERSITY OF SOUTHAMPTON 2018/19
 * ---------------------------------------------
 * 
 * This script is written for GDP 47 - fracture 
 * fixation device. Direct code queries to 
 * ojs1g14@soton.ac.uk.
 * 
*/

#include <stdio.h>
#include <math.h>

// Clinical measurements
    float ux;
    float uy;
    float uz;
    float dx;
    float dy;
    float dz;
    float vx=20;
    float vy=20;
    float vz=20;
    int i;
    int j;
    int k;
    int x;
//----------------------

// Mathematical Structures and Constants
    float sum=0;
    float rad;
    float D[6];
    float L[6];     // L=P-D
    float P[6][3] = {{-7.49,93.5,0},{7.49,93.5,0},{84.72,-40.26,0},{77.23,-53.24,0},{-77.23,-53.24,0},{-84.72,-40.26,0}}; //Matrix of xyz vectors
    float d[6][3] = {{-7.49,93.5,0},{7.49,93.5,0},{84.72,-40.26,0},{77.23,-53.24,0},{-77.23,-53.24,0},{-84.72,-40.26,0}}; //Matrix of xyz vectors
    float Tp[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Td[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Tv[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float T[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    float Rx[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Ry[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Rz[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Result[][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    float tmpResult[][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
// ------------------------------------

// Function Prototypes
    void assignTp(float ux,float uy,float uz);
    void restoreTp();
    void assignTd(float dx,float dy,float dz);
    void restoreTd();
    void assignTv(float vx,float vy,float vz);
    void restoreTv();
    void assignRx(float thetax);
    void restoreRx();
    void assignRy(float thetay);
    void restoreRy();
    void assignRz(float thetaz);
    void restoreRz();
    void dotProduct(float A[][4], float B[][4], float resultMat[][4]);
    void restoreResult();
    void restoreTmpResult();
    void restoreT();
    void calctransv(float thetax,float thetay,float thetaz,float vx,float vy,float vz);
// --------------------

// USEFUL TEST MATRICES
float A[4][4]={{1,2,3,4},{2,3,4,5},{3,4,5,6},{4,5,6,7}};
float B[4][4]={{1,2,3,4},{2,3,4,5},{3,4,5,6},{4,5,6,7}};
// --------------------


int main(void){

    calctransv(0,0,0,0,0,0);   // Tm is now assigned to global 'Result' variable
    assignTp(0,0,0);
    assignTd(0,0,0);

    dotProduct(Td,Result,tmpResult);  // postmultiply
    dotProduct(tmpResult, Tp, T);

    for (i=0;i<4;i++){
        for(j=0;j<4;j++){
            printf("%f ", T[j][i]);
            }
        printf("\n");
    }   
    
    restoreTp();
    restoreTd();
    restoreResult();
    restoreTmpResult();

return 1;
}


void calctransv(float thetax,float thetay,float thetaz,float vx,float vy,float vz){
    
    assignRx(thetax);
    assignRy(thetay);
    assignRz(thetaz);
    assignTv(vx,vy,vz);

    dotProduct(Rx,Ry,Result);           // Result=Rx.Ry
    dotProduct(Result,Rz,tmpResult);    // tmpResult=Result.Rz
    restoreResult();                    // tidy up

    dotProduct(tmpResult,Tv,Result);    // Result=Transv
    restoreTmpResult();
    restoreRx();
    restoreRy();
    restoreRz();
    restoreTv();
}


void assignTp(float Ux,float Uy,float Uz){
    Tp[0][3]=Ux;
    Tp[1][3]=Uy;
    Tp[2][3]=Uz;
}


void restoreTp(){
    Tp[0][3]=0;
    Tp[1][3]=0;
    Tp[2][3]=0;
}


void assignTd(float Dx,float Dy,float Dz){
    Td[0][3]=Dx;
    Td[1][3]=Dy;
    Td[2][3]=Dz;
}


void restoreTd(float Tarray[][4]){
    Td[0][3]=0;
    Td[1][3]=0;
    Td[2][3]=0;
}


void assignTv(float Vx,float Vy,float Vz){
    Tv[0][3]=Vx;
    Tv[1][3]=Vy;
    Tv[2][3]=Vz;
}


void restoreTv(){
    Tv[0][3]=0;
    Tv[1][3]=0;
    Tv[2][3]=0;
}


void assignRx(float thetax){
    /*
     * thetax must be provided in degrees!
     * 
    */
    rad=(M_PI/180.0)*thetax;

    Rx[1][1]=cos(rad);
    Rx[1][2]=-sin(rad);
    Rx[2][1]=sin(rad);
    Rx[2][2]=cos(rad);

}

void restoreRx(){

    Rx[1][1]=1;
    Rx[1][2]=0;
    Rx[2][1]=0;
    Rx[2][2]=1;
}


void assignRy(float thetay){
    /*
     * thetay must be provided in degrees!
     * 
    */
    rad=(M_PI/180.0)*thetay;

    Ry[0][0]=cos(rad);
    Ry[0][2]=sin(rad);
    Ry[2][0]=sin(rad);
    Ry[2][2]=cos(rad);
}


void restoreRy(){

    Ry[0][0]=1;
    Ry[0][2]=0;
    Ry[2][0]=0;
    Ry[2][2]=1;
}


void assignRz(float thetaz){
    /*
     * thetax must be provided in degrees!
     * 
    */
    rad=(M_PI/180.0)*thetaz;

    Rz[0][0]=cos(rad);
    Rz[0][1]=-sin(rad);
    Rz[1][0]=sin(rad);
    Rz[1][1]=cos(rad);
}


void restoreRz(float Tarray[][4]){

    Rz[0][0]=1;
    Rz[0][1]=0;
    Rz[1][0]=0;
    Rz[1][1]=1;
}


void dotProduct(float A[4][4], float B[4][4], float resultMat[4][4]){
/*
 * This function is misleadingly named - it 
 * might be more accurately named 
 * 'MatrixMultiplication' or similar. The dot 
 * product is a commutative vector operation
 * whereas matrix multiplication extends the 
 * dot product to multiply two matrices - This
 * is non-commutative.
 * This function calculates the transformation matrix 
 * Tv. Having 'Result' as an argument is just
 * done to aid readability - the function could
 * just act on the global variable without it 
 * being passed to the function.
 * 
*/

    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            resultMat[i][j]=0;
            for (k=0;k<4;k++){
                resultMat[i][j]+=(A[i][k]*B[k][j]);
            }
        }
    }
}



void restoreResult(){

    for (i=0;i<4;i++){
        for(x=0;x<4;x++){
            Result[i][x]=0;
        }
    }
}


void restoreTmpResult(){

    for (i=0;i<4;i++){
        for(x=0;x<4;x++){
            tmpResult[i][x]=0;
        }
    }
}

void restoreT(){

    for (i=0;i<4;i++){
        for(x=0;x<4;x++){
            T[i][x]=0;
        }
    }
}
