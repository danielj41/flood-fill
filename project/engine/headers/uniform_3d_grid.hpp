#ifndef UNIFORM_3D_GRID
#define UNIFORM_3D_GRID

#include <exception>
#include <cstring>

#include "pointer_macros.h"
DEF_PTR_TMPL(Uniform3DGrid, Uniform3DGridPtr, template<typename T>, T);

template<typename T>
class Uniform3DGrid{

public:
    Uniform3DGrid();

    Uniform3DGrid(unsigned int numberOfCubesInX,
                  unsigned int numberOfCubesInY,
                  unsigned int numberOfCubesInZ,
                  float _minX, float _maxX,
                  float _minY, float _maxY,
                  float _minZ, float _maxZ);

    Uniform3DGrid(const Uniform3DGrid<T>& uGrid);

    virtual ~Uniform3DGrid();

    void initialize(const T& value);

    T& operator() (unsigned int x, unsigned int y, unsigned int z);
    T& operator() (unsigned int x, unsigned int y, unsigned int z) const;

    void setValue(float x, float y, float z, const T& value);
    T& getValue(float x, float y, float z) const;

    Uniform3DGrid& operator= (const Uniform3DGrid& uGrid);

    unsigned int getSizeX();
    unsigned int getSizeY();
    unsigned int getSizeZ();
    unsigned int size();

    float getEdgeSizeX();
    float getEdgeSizeY();
    float getEdgeSizeZ();

    float getMinX();
    float getMaxX();
    float getMinY();
    float getMaxY();
    float getMinZ();
    float getMaxZ();

    float getRoundX(float x);
    float getRoundY(float y);
    float getRoundZ(float z);
    bool inGrid(float x, float y, float z);

private:
    unsigned int sizeX, sizeY, sizeZ;

    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;
    float edgeSizeX, edgeSizeY, edgeSizeZ;

    T * grid;
};




/**
*    Uniform 3D Grid Template Class Implementation
*/



/**
* Exceptions
*/

class Uniform3DGridOutOfRangeException : public std::exception{
    virtual const char* what() const throw()
    {
        return "Index Out of Range";
    }
};



/**
*   Uniform 3D Grid Implementation
*/

template<typename T>
Uniform3DGrid<T>::Uniform3DGrid() : sizeX(0), sizeY(0), sizeZ(0){
    grid = new T[sizeX * sizeY * sizeZ];
}

template<typename T>
Uniform3DGrid<T>::Uniform3DGrid(unsigned int numberOfCubesInX,
                                unsigned int numberOfCubesInY,
                                unsigned int numberOfCubesInZ,
                                float _minX, float _maxX,
                                float _minY, float _maxY,
                                float _minZ, float _maxZ)
: sizeX(numberOfCubesInX), sizeY(numberOfCubesInY), sizeZ(numberOfCubesInZ),
  minX(_minX), maxX(_maxX), minY(_minY), maxY(_maxY), minZ(_minZ), maxZ(_maxZ){

//TODO: throw exceptions if the max and min are equal or if the number of cubes is 0

    if(_minX > _maxX){
        minX = _maxX;
        maxX = _minX;
    }
    if(_minY > _maxY){
        minY = _maxY;
        maxY = _minY;
    }
    if(_minZ > _maxZ){
        minZ = _maxZ;
        maxZ = _minZ;
    }


    if(numberOfCubesInX > 0) edgeSizeX = (maxX - minX)/numberOfCubesInX;
    else edgeSizeX = 0;
    if(numberOfCubesInY > 0) edgeSizeY = (maxY - minY)/numberOfCubesInY;
    else edgeSizeY = 0;
    if(numberOfCubesInZ > 0) edgeSizeZ = (maxZ - minZ)/numberOfCubesInZ;
    else edgeSizeZ = 0;

    grid = new T[sizeX * sizeY * sizeZ];
}


template<typename T>
Uniform3DGrid<T>::Uniform3DGrid(const Uniform3DGrid<T>& uGrid){
    sizeX = uGrid.sizeX;
    sizeY = uGrid.sizeY;
    sizeZ = uGrid.sizeZ;

    minX = uGrid.minX; maxX = uGrid.maxX;
    minY = uGrid.minY; maxY = uGrid.maxY;
    minZ = uGrid.minZ; maxZ = uGrid.maxZ;

    edgeSizeX = uGrid.edgeSizeX;
    edgeSizeY = uGrid.edgeSizeY;
    edgeSizeZ = uGrid.edgeSizeZ;

    grid = new T[sizeX * sizeY * sizeZ];
    memcpy(grid, uGrid.grid, sizeof(T)*sizeX*sizeY*sizeZ);
}

template<typename T>
Uniform3DGrid<T>::~Uniform3DGrid(){
    delete[] grid;
}

template<typename T>
void Uniform3DGrid<T>::initialize(const T& value){
    for ( int i = 0; i < sizeX*sizeY*sizeZ; i++) {
        grid[i] = value;
    }
}

template<typename T>
T& Uniform3DGrid<T>::operator() (unsigned int x, unsigned int y, unsigned int z){
    if(x >= sizeX || y >= sizeY || z >= sizeZ)
        throw Uniform3DGridOutOfRangeException();
    return grid[x + sizeX * y + sizeX * sizeY * z];
}

template<typename T>
T& Uniform3DGrid<T>::operator() (unsigned int x, unsigned int y, unsigned int z) const{
    if(x >= sizeX || y >= sizeY || z >= sizeZ)
        throw Uniform3DGridOutOfRangeException();
    return grid[x + sizeX * y + sizeX * sizeY * z];
}

template<typename T>
void Uniform3DGrid<T>::setValue(float x, float y, float z, const T& value){
    if(x < minX || x > maxX ||
       y < minY || y > maxY ||
       z < minZ || z > maxZ)
        throw Uniform3DGridOutOfRangeException();
    unsigned int xIndex = ((unsigned int) ((x - minX)/edgeSizeX));
    unsigned int yIndex = ((unsigned int) ((y - maxY)/-edgeSizeY));
    unsigned int zIndex = ((unsigned int) ((z - minZ)/edgeSizeZ));

    if(xIndex == sizeX) xIndex--;
    if(yIndex == sizeY) yIndex--;
    if(zIndex == sizeZ) zIndex--;

    grid[xIndex + sizeX * yIndex + sizeX * sizeY * zIndex] = value;
}

template<typename T>
T& Uniform3DGrid<T>::getValue(float x, float y, float z) const{
    if(x < minX || x > maxX ||
       y < minY || y > maxY ||
       z < minZ || z > maxZ)
        throw Uniform3DGridOutOfRangeException();
    unsigned int xIndex = ((unsigned int) ((x - minX)/edgeSizeX));
    unsigned int yIndex = ((unsigned int) ((y - maxY)/-edgeSizeY));
    unsigned int zIndex = ((unsigned int) ((z - minZ)/edgeSizeZ));

    if(xIndex == sizeX) xIndex--;
    if(yIndex == sizeY) yIndex--;
    if(zIndex == sizeZ) zIndex--;

    return grid[xIndex + sizeX * yIndex + sizeX * sizeY * zIndex];
}

template<typename T>
Uniform3DGrid<T>& Uniform3DGrid<T>::operator= (const Uniform3DGrid& uGrid){
    sizeX = uGrid.sizeX;
    sizeY = uGrid.sizeY;
    sizeZ = uGrid.sizeZ;

    minX = uGrid.minX; maxX = uGrid.maxX;
    minY = uGrid.minY; maxY = uGrid.maxY;
    minZ = uGrid.minZ; maxZ = uGrid.maxZ;

    edgeSizeX = uGrid.edgeSizeX;
    edgeSizeY = uGrid.edgeSizeY;
    edgeSizeZ = uGrid.edgeSizeZ;

    grid = new T[sizeX * sizeY * sizeZ];
    memcpy(grid, uGrid.grid, sizeof(T)*sizeX*sizeY*sizeZ);

    return *this;
}

template<typename T>
unsigned int Uniform3DGrid<T>::getSizeX() { return sizeX; }

template<typename T>
unsigned int Uniform3DGrid<T>::getSizeY() { return sizeY; }

template<typename T>
unsigned int Uniform3DGrid<T>::getSizeZ() { return sizeZ; }

template<typename T>
unsigned int Uniform3DGrid<T>::size() { return sizeX * sizeY * sizeZ; }


template<typename T>
float Uniform3DGrid<T>::getEdgeSizeX(){ return edgeSizeX; }

template<typename T>
float Uniform3DGrid<T>::getEdgeSizeY(){ return edgeSizeY; }

template<typename T>
float Uniform3DGrid<T>::getEdgeSizeZ(){ return edgeSizeZ; }

template<typename T>
float Uniform3DGrid<T>::getMinX(){ return minX; }

template<typename T>
float Uniform3DGrid<T>::getMaxX(){ return maxX; }

template<typename T>
float Uniform3DGrid<T>::getMinY(){ return minY; }

template<typename T>
float Uniform3DGrid<T>::getMaxY(){ return maxY; }

template<typename T>
float Uniform3DGrid<T>::getMinZ(){ return minZ; }

template<typename T>
float Uniform3DGrid<T>::getMaxZ(){ return maxZ; }



template<typename T>
float Uniform3DGrid<T>::getRoundX(float x) {
    unsigned int xIndex = ((unsigned int) ((x - minX)/edgeSizeX));
    return xIndex * edgeSizeX + minX + edgeSizeX/2.0;
}

template<typename T>
float Uniform3DGrid<T>::getRoundY(float y) {
    unsigned int yIndex = ((unsigned int) ((y - minY)/edgeSizeY));
    return yIndex * edgeSizeY + minY + edgeSizeY/2.0;
}

template<typename T>
float Uniform3DGrid<T>::getRoundZ(float z) {
    unsigned int zIndex = ((unsigned int) ((z - minZ)/edgeSizeZ));
    return zIndex * edgeSizeZ + minZ + edgeSizeZ/2.0;
}

template<typename T>
bool Uniform3DGrid<T>::inGrid(float x, float y, float z) {
    return !(x < minX || x > maxX ||
             y < minY || y > maxY ||
             z < minZ || z > maxZ);
}

#endif
