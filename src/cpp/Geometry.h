#ifndef __Shadow_Volumes__Geometry__
#define __Shadow_Volumes__Geometry__
#include <vector>

/**
 * The triangle value determines where in the indexes array the triangle
 * can be found edgeA and edgeB are the indexes of the vertex found
 * in the indexes array. adjacent is the index of the remaining vertex
 * in the triangle and adjacentTriangle is the index of the triangle.
 * 
 * */
class Adjacent{
    public:
        int triangle;
        int edgeA,edgeB;
        int adjacent;
        int adjacentTriangle; 
};
void findAdjacencies(int* indexes, std::vector<Adjacent>& target);
#endif
