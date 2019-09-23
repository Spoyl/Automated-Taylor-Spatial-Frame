/* ---------------------------------------------
 *                OLIVER STOCKS
 *  MENG MECHANICAL WITH BIOMEDICAL ENGINEERING
 *      UNIVERSITY OF SOUTHAMPTON 2018/19
 * ---------------------------------------------
 * 
 * This script is written for GDP 47 - fracture 
 * fixation device. Direct code queries to 
 * Spoyl (github account).
 * 
*/

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
//----------------------

// Mathematical Structures and Constants
    float sum=0;
    float rad;
    float lengths[6]={0,0,0,0,0,0};
    float D[6][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    float L[6][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};     // L=P-D
    float P[6][4] = {{-7.49,93.5,0,1},{7.49,93.5,0,1},{84.72,-40.26,0,1},{77.23,-53.24,0,1},{-77.23,-53.24,0,1},{-84.72,-40.26,0,1}}; //Matrix of xyz1
    float d[6][4] = {{-7.49,93.5,0,1},{7.49,93.5,0,1},{84.72,-40.26,0,1},{77.23,-53.24,0,1},{-77.23,-53.24,0,1},{-84.72,-40.26,0,1}}; //Matrix of xyz1
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
//    void assignTp(float ux,float uy,float uz);
//    void restoreTp();
//    void assignTd(float dx,float dy,float dz);
//    void restoreTd();
//    void assignTv(float vx,float vy,float vz);
//    void restoreTv();
//    void assignRx(float thetax);
//    void restoreRx();
//    void assignRy(float thetay);
//    void restoreRy();
//    void assignRz(float thetaz);
//    void restoreRz();
//    void dotProduct(float A[][4], float B[][4], float resultMat[][4]);
//    void dotProductN();
//    void restoreResult();
//    void restoreTmpResult();
//    void restoreT();
//    void calctransv(float thetax,float thetay,float thetaz,float vx,float vy,float vz);
//    void defineD();
//    void findDifference(float distal[6][4],float ref[6][4], float struts[6][4]);
//    void findLengths(float link[6][4], float outputVec[6]);
// --------------------

void setup(){

  delay(100);
}

void loop(){
    calctransv(0,10,0,0,0,10);   // Tm is now assigned to global 'Result' variable
    assignTp(0,0,200);
    assignTd(0,0,300);

    dotProduct(Td,Result,tmpResult);    // postmultiply
    dotProduct(tmpResult, Tp, T);       // T == global transformation matrix

    for (i=0;i<4;i++){
        for(j=0;j<4;j++){
            Serial.print(T[j][i]);
            }
        Serial.print("\n");
    }   
    
    restoreTp();
    restoreTd();
    restoreResult();
    restoreTmpResult();

    dotProductN(d,T,D);

Serial.print("\nd=\n");
for (i=0;i<6;i++){ 
        Serial.print("\n");
        for (j=0;j<4;j++) { 
            Serial.print(d[i][j]);
        }
    } 

Serial.print("\nT=\n");
for (i=0;i<4;i++){ 
        Serial.print("\n");
        for (j=0;j<4;j++) { 
            Serial.print(T[i][j]);
        }
    } 

Serial.print("\nD= \n");
for (i=0;i<6;i++){ 
    Serial.print("\n");
    for (j=0;j<4;j++) { 
        Serial.print(D[i][j]);
    }
}

findDifference(D,P,L);

Serial.print("\nL= \n");

for (i=0;i<6;i++){ 
    Serial.print("\n");
    for (j=0;j<4;j++) { 
        Serial.print(L[i][j]);
    }
}

Serial.println("\nLength vector:\n");
findLengths(L,lengths);

for (i=0;i<6;i++){
    Serial.println(lengths[i]);
}

restoreT();
restoreTmpResult();
restoreResult();
restoreRz();
restoreRy();
restoreRx();
restoreTv();

float lengths[6]={0,0,0,0,0,0};
float D[6][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
float L[6][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};     // L=P-D
float P[6][4] = {{-7.49,93.5,0,1},{7.49,93.5,0,1},{84.72,-40.26,0,1},{77.23,-53.24,0,1},{-77.23,-53.24,0,1},{-84.72,-40.26,0,1}};
float d[6][4] = {{-7.49,93.5,0,1},{7.49,93.5,0,1},{84.72,-40.26,0,1},{77.23,-53.24,0,1},{-77.23,-53.24,0,1},{-84.72,-40.26,0,1}};

delay(10000);

}



void findLengths(float link[6][4], float outputVec[6]){
    for (i=0;i<6;i++){ 
        sum=0;
        for (j=0;j<4;j++) {
            sum+=link[i][j]*link[i][j];
        }
    outputVec[i]=sqrt(sum);
    }  
}


void findDifference(float distal[6][4],float ref[6][4], float struts[6][4]){
    for (i=0;i<6;i++){ 
        for (j=0;j<4;j++) {
            struts[i][j]+=distal[i][j]-ref[i][j];
        }
    }  
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
    Tp[3][0]=Ux;
    Tp[3][1]=Uy;
    Tp[3][2]=Uz;
}


void restoreTp(){
    Tp[3][0]=0;
    Tp[3][1]=0;
    Tp[3][2]=0;
}


void assignTd(float Dx,float Dy,float Dz){
    Td[3][0]=Dx;
    Td[3][1]=Dy;
    Td[3][2]=Dz;
}


void restoreTd(){
    Td[3][0]=0;
    Td[3][1]=0;
    Td[3][2]=0;
}


void assignTv(float Vx,float Vy,float Vz){
    Tv[3][0]=Vx;
    Tv[3][1]=Vy;
    Tv[3][2]=Vz;
}


void restoreTv(){
    Tv[3][0]=0;
    Tv[3][1]=0;
    Tv[3][2]=0;
}


void assignRx(float thetax){
    /*
     * thetax must be provided in degrees!
     * 
    */
    rad=(M_PI/180.0)*thetax;

    Rx[1][1]=cos(rad);
    Rx[2][1]=-sin(rad);
    Rx[1][2]=sin(rad);
    Rx[2][2]=cos(rad);

}

void restoreRx(){

    Rx[1][1]=1;
    Rx[2][1]=0;
    Rx[1][2]=0;
    Rx[2][2]=1;
}


void assignRy(float thetay){
    /*
     * thetay must be provided in degrees!
     * 
    */
    rad=(M_PI/180.0)*thetay;

    Ry[0][0]=cos(rad);
    Ry[0][2]=-sin(rad);
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
     * thetaz must be provided in degrees!
     * 
    */
    rad=(M_PI/180.0)*thetaz;

    Rz[0][0]=cos(rad);
    Rz[0][1]=sin(rad);
    Rz[1][0]=-sin(rad);
    Rz[1][1]=cos(rad);
}


void restoreRz(){

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
 * dot product to the multiplication of two 
 * matrices - This is non-commutative.
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
                resultMat[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
}


void dotProductN(float A[6][4], float B[4][4], float resultMat[6][4]){ 

    for (i=0;i<6;i++){ 
        for (j=0;j<4;j++) { 
            for (k=0;k<4;k++) 
                resultMat[i][j]+=A[i][k]*B[k][j]; 
        } 
    } 
} 


void restoreResult(){

    for (i=0;i<4;i++){
        for(j=0;j<4;j++){
            Result[i][j]=0;
        }
    }
}


void restoreTmpResult(){

    for (i=0;i<4;i++){
        for(j=0;j<4;j++){
            tmpResult[i][j]=0;
        }
    }
}

void restoreT(){

    for (i=0;i<4;i++){
        for(j=0;j<4;j++){
            T[i][j]=0;
        }
    }
}

