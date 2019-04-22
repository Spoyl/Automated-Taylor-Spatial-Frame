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
    float ux,uy,uz;
    float dx,dy,dz;
    float vx=20;
    float vy=20;
    float vz = 20;
    int i;
    int x;
//----------------------

// Mathematical Structures and Constants
    float rad;
    float P[6];
    float D[6];
    float L[6];     // L=P-B
    float Tp[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Td[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Tv[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Rx[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Ry[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    float Rz[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
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
// --------------------


int main(void){

    printf("Before assignment:\n");
    for (i=0;i<4;i++){
            for(x=0;x<4;x++){
                printf("%f\n", Tv[x][i]);
            }
        }

    printf("After:\n");

        assignTv(vx,vy,vz);

        for (i=0;i<4;i++){
            for(x=0;x<4;x++){
                printf("%f\n", Tv[x][i]);
            }
        }

        printf("Restore:\n");

        restoreTv();

        for (i=0;i<4;i++){
            for(x=0;x<4;x++){
                printf("%f\n", Tv[x][i]);
            }
        }

    return 1;
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