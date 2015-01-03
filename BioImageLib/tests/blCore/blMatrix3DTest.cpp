#include <blCore/blMatrix3D.h>
#include <iostream>

int main(int argc, char* argv[])
{
    //std::cout << "// test the IO" << std::endl;
    // test the IO
    blMatrix3D matrix(2, 2, 2);
    matrix(0,0,0) = 1;
    matrix(0,1,0) = 1;
    matrix(1,0,0) = 1;
    matrix(1,1,0) = 1;
    matrix(0,0,1) = 2;
    matrix(0,1,1) = 2;
    matrix(1,0,1) = 2;
    matrix(1,1,1) = 2;
    if (matrix(0,0,0) != 1 || matrix(0,1,1) != 2 ){
        return 1;
    }

    //std::cout << "// test scalar sum "<< std::endl;
    // test scalar sum
    blMatrix3D matrixSum = matrix + 2;
    if (matrixSum(0,0,0) != 3 || matrixSum(0,1,1) != 4){
        return 1;
    }

    //std::cout << "// test scalar diff" << std::endl;
    // test diff
    blMatrix3D matrixDiff = matrix - 2;
    if (matrixDiff(0,0,0) != -1 || matrixDiff(0,1,1) != 0){
        return 1;
    }

    //std::cout << "// test scalar prod" << std::endl;
    // test scalar prod
    blMatrix3D matrixsp = matrix * 2;
    if (matrixsp(0,0,0) != 2 || matrixsp(0,1,1) != 4 ){
        return 1;
    }

    //std::cout << "// test scalar div" << std::endl;
    // test scalar div
    blMatrix3D matrixsd = matrix / 2;
    if (matrixsd(0,0,0) != 0.5 || matrixsd(0,1,1) != 1 ){
        return 1;
    }

    blMatrix3D matrix2(2,2, 2);
    matrix2(0,0,0) = 1;
    matrix2(0,1,0) = 2;
    matrix2(1,0,0) = 3;
    matrix2(1,1,0) = 4;
    matrix2(0,0,1) = 5;
    matrix2(0,1,1) = 6;
    matrix2(1,0,1) = 7;
    matrix2(1,1,1) = 8;

    //std::cout << "// test sum" << std::endl;
    // test sum
    blMatrix3D sum = matrix+matrix2;
    if (sum(0,0,0) != 2 || sum(0,1,1) != 8 ){
        return 1;
    }

    //std::cout << "// test diff" << std::endl;
    // test diff
    blMatrix3D diff = matrix-matrix2;
    if (diff(0,0,0) != 0 || diff(0,1,1) != -4 ){
        return 1;
    }


    //std::cout << "// test equal" << std::endl;
    // test equal
    if (matrix == matrix2){
        return 1;
    }

    //std::cout << "// test none equal" << std::endl;
    // test none equal
    blMatrix3D matrix1c = blMatrix3D(matrix);
    if (matrix != matrix1c){
        return 1;
    }

    return 0;
}
