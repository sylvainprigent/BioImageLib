#include <blCore/blMatrix.h>
#include <blCore/blData.h>
#include <iostream>

int main(int argc, char* argv[])
{
    // test the IO
    blMatrix matrix(2, 2);
    matrix(0,0) = 1;
    matrix(0,1) = 2;
    matrix(1,0) = 3;
    matrix(1,1) = 4;
    if (matrix(0,0) != 1 || matrix(0,1) != 2 || matrix(1,0) != 3 || matrix(1,1) != 4){
        return 1;
    }

    // test scalar sum
    blMatrix matrixSum = matrix + 2;
    if (matrixSum(0,0) != 3 || matrixSum(0,1) != 4 || matrixSum(1,0) != 5 || matrixSum(1,1) != 6){
        return 1;
    }

    // test scalar diff
    blMatrix matrixDiff = matrix - 2;
    if (matrixDiff(0,0) != -1 || matrixDiff(0,1) != 0 || matrixDiff(1,0) != 1 || matrixDiff(1,1) != 2){
        return 1;
    }

    // test scalar prod
    blMatrix matrixsp = matrix * 2;
    if (matrixsp(0,0) != 2 || matrixsp(0,1) != 4 || matrixsp(1,0) != 6 || matrixsp(1,1) != 8){
        return 1;
    }

    // test scalar div
    blMatrix matrixsd = matrix / 2;
    if (matrixsd(0,0) != 0.5 || matrixsd(0,1) != 1 || matrixsd(1,0) != 1.5 || matrixsd(1,1) != 2){
        return 1;
    }


    blMatrix matrix2(2,2);
    matrix2(0,0) = 4;
    matrix2(0,1) = 3;
    matrix2(1,0) = 2;
    matrix2(1,1) = 1;

    // test sum
    blMatrix sum = matrix+matrix2;
    if (sum(0,0) != 5 || sum(0,1) != 5 || sum(1,0) != 5 || sum(1,1) != 5){
        return 1;
    }

    // test diff
    blMatrix diff = matrix-matrix2;
    if (diff(0,0) != -3 || diff(0,1) != -1 || diff(1,0) != 1 || diff(1,1) != 3){
        return 1;
    }

    // product
    blMatrix prod = matrix*matrix2;
    if (prod(0,0) != 8 || prod(0,1) != 5 || prod(1,0) != 20 || prod(1,1) != 13){
        return 1;
    }

    // test equal
    if (matrix == matrix2){
        return 1;
    }

    // test none equal
    blMatrix matrix1c = blMatrix(matrix);
    if (matrix != matrix1c){
        return 1;
    }

    return 0;
}
