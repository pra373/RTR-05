#pragma once

#include <cmath>
#include <vector>
using namespace std;

class mesh {
private:
  vector<unsigned short> elements; // vector for elements

  // vectors for basic data

  vector<float> vertices;
  vector<float> normals;
  vector<float> texcoords;

  int numElements = 0;
  int maxElements = 0;
  int numVertices = 0;

  GLuint vboPosition = 0;
  GLuint vboNormal = 0;
  GLuint vboTexture = 0;

  GLuint vboIndex = 0;
  GLuint vao = 0;

public:
  // member functions

  mesh();

  void allocate(int numIndices);

  // Add 3 vertices, 3 normal and 2 texcoords i.e. one triangle to the geometry.
  // This searches the current list for identical vertices (exactly or nearly)
  // and if one is found, it is added to the index array. if not, it is added to
  // both the index array and the vertex array.

  void addTriangle(float **, float **, float **);
  void prepareToDraw(void);
  void draw(void);
  int getIndexCount(void);
  int getVertexCount(void);
  void normalizeVector(float[]);
  bool isFoundIdentical(float, float, float);
  void cleanupMeshData(void);

  void deallocate(void);
};

void makeSphere(mesh &sphereMesh, float fRadius, int iSlices, int iStacks);
