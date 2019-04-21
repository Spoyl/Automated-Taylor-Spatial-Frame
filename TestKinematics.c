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
    void assignTp(float ux,float uy,float uz,float Tarray[][4]);
    void restoreTp(float Tarray[][4]);
    void assignTd(float dx,float dy,float dz,float Tarray[][4]);
    void restoreTd(float Tarray[][4]);
    void assignRx(float thetax, float Tarray[][4]);
    void restoreRx(float Tarray[][4]);
    // void assignRy(float thetay, float Tarray[][4]);
    // void restoreRy(float Tarray[][4]);
    // void assignRz(float thetaz, float Tarray[][4]);
    // void restoreRz(float Tarray[][4]);
// --------------------


int main(void){

printf("Before assignment:\n");
for (i=0;i<4;i++){
        for(x=0;x<4;x++){
            printf("%f\n", Rx[i][x]);
        }
    }

printf("After:\n");

    assignRx(90, Rx);

    for (i=0;i<4;i++){
        for(x=0;x<4;x++){
            printf("%f\n", Rx[i][x]);
        }
    }

    printf("Restore:\n");

    restoreRx(Rx);

    for (i=0;i<4;i++){
        for(x=0;x<4;x++){
            printf("%f\n", Rx[i][x]);
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


void assignTd(float dx,float dy,float dz,float Tarray[][4]){
    Tarray[0][3]=dx;
    Tarray[1][3]=dy;
    Tarray[2][3]=dz;
}


void restoreTd(float Tarray[][4]){
    Tarray[0][3]=0;
    Tarray[1][3]=0;
    Tarray[2][3]=0;
}

void assignRx(float thetax, float Tarray[][4]){
    /*
     * thetax must be provided in degrees!
     * This function handles the conversion to Radians
     * 
    */
    rad=(M_PI/180.0)*thetax;

    Tarray[1][1]=cos(rad);
    Tarray[1][2]=-sin(rad);
    Tarray[2][1]=sin(rad);
    Tarray[2][2]=cos(rad);

}

void restoreRx(float Tarray[][4]){

    Tarray[1][1]=1;
    Tarray[1][2]=0;
    Tarray[2][1]=0;
    Tarray[2][2]=1;
}
