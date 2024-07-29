
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#include "sphere.h"
#include <iostream>

const float PLP_PI = 3.14159265358979323846;

//enum { AMC_ATTRIBUTE_POSITION, AMC_ATTRIBUTE_NORMAL, AMC_ATTRIBUTE_TEXTURE };

mesh::mesh() {
  vertices.clear();
  normals.clear();
  texcoords.clear();
}

void mesh::allocate(int numIndices) {
  // code

  // first cleanup, if not initially empty

  cleanupMeshData();

  maxElements = numIndices;
  numElements = 0;
  numVertices = 0;

  int iNumIndices = numIndices / 3;

  elements.resize(
      iNumIndices *
      3); // No need to multiply by 2 as we are not dealing with bytes directly

  vertices.resize(iNumIndices * 3); // 3 is x,y,z

  normals.resize(iNumIndices * 3); // 3 is x,y,z

  texcoords.resize(iNumIndices * 2); // 2 is s,t
}

void mesh::addTriangle(float **single_vertex, float **single_normal,
                       float **single_texture) {
  const float diff = 0.00001f;

  // Normalize vectors
  normalizeVector(single_normal[0]);
  normalizeVector(single_normal[1]);
  normalizeVector(single_normal[2]);

  for (int i = 0; i < 3; ++i) {
    bool found = false;

    for (int j = 0; j < numVertices; ++j) {
      if (isFoundIdentical(vertices[j * 3], single_vertex[i][0], diff) &&
          isFoundIdentical(vertices[j * 3 + 1], single_vertex[i][1], diff) &&
          isFoundIdentical(vertices[j * 3 + 2], single_vertex[i][2], diff) &&
          isFoundIdentical(normals[j * 3], single_normal[i][0], diff) &&
          isFoundIdentical(normals[j * 3 + 1], single_normal[i][1], diff) &&
          isFoundIdentical(normals[j * 3 + 2], single_normal[i][2], diff) &&
          isFoundIdentical(texcoords[j * 2], single_texture[i][0], diff) &&
          isFoundIdentical(texcoords[j * 2 + 1], single_texture[i][1], diff)) {
        elements[numElements] = j;
        ++numElements;
        found = true;
        break;
      }
    }

    if (!found && numVertices < maxElements && numElements < maxElements) {
      vertices[numVertices * 3] = single_vertex[i][0];
      vertices[numVertices * 3 + 1] = single_vertex[i][1];
      vertices[numVertices * 3 + 2] = single_vertex[i][2];

      normals[numVertices * 3] = single_normal[i][0];
      normals[numVertices * 3 + 1] = single_normal[i][1];
      normals[numVertices * 3 + 2] = single_normal[i][2];

      texcoords[numVertices * 2] = single_texture[i][0];
      texcoords[numVertices * 2 + 1] = single_texture[i][1];

      elements[numElements] = numVertices;

      numElements++;
      numVertices++;
    }
  }
}

void mesh::prepareToDraw(void) {
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  // VBO for position

  glGenBuffers(1, &vboPosition);

  // step 16: Bind with VBO of position

  glBindBuffer(GL_ARRAY_BUFFER, vboPosition);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // VBO for Normal

  glGenBuffers(1, &vboNormal);

  // step 16: Bind with VBO of TEXCOORD

  glBindBuffer(GL_ARRAY_BUFFER, vboNormal);

  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // vbo for texture

  // VBO for Normal

  glGenBuffers(1, &vboTexture);

  // step 16: Bind with VBO of TEXCOORD

  glBindBuffer(GL_ARRAY_BUFFER, vboTexture);

  glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float),
               texcoords.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(AMC_ATTRIBUTE_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, NULL);

  glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXTURE);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // element vbo
  glGenBuffers(1, &vboIndex);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               elements.size() * sizeof(unsigned short), elements.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // step 18 : unbind with VAO
  glBindVertexArray(0);

  // after sending data to GPU, now we can free our arrays
  cleanupMeshData();
}

void mesh::draw(void) {
  glBindVertexArray(vao);

  // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);

  glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

  glBindVertexArray(0);
}

int mesh::getIndexCount(void) {
  // code
  return (numElements);
}

int mesh::getVertexCount(void) {
  // code

  return (numVertices);
}

void mesh::normalizeVector(float v[]) {
  // Calculate the squared length of the vector

  float squaredVectorLength = (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);

  // Calculate the square root of the squared length

  float squareRootOfSquaredVectorLength = sqrt(squaredVectorLength);

  // scale the vector with 1/squareRootOfSquaredVectorLength

  v[0] = v[0] * 1.0 / squareRootOfSquaredVectorLength;
  v[1] = v[1] * 1.0 / squareRootOfSquaredVectorLength;
  v[2] = v[2] * 1.0 / squareRootOfSquaredVectorLength;
}

bool mesh::isFoundIdentical(float val1, float val2, float diff) {
  // code

  if (abs(val1 - val2) < diff) {
    return (true);
  }

  else {
    return (false);
  }
}

void mesh::cleanupMeshData(void) {
  elements.clear();
  vertices.clear();
  normals.clear();
  texcoords.clear();
}

void mesh::deallocate(void) {
  if (vao) {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }

  if (vboIndex) {
    glDeleteBuffers(1, &vboIndex);
    vboIndex = 0;
  }

  if (vboTexture) {
    glDeleteBuffers(1, &vboTexture);
    vboTexture = 0;
  }

  if (vboNormal) {
    glDeleteBuffers(1, &vboNormal);
    vboNormal = 0;
  }

  if (vboPosition) {
    glDeleteBuffers(1, &vboPosition);
    vboPosition = 0;
  }
}

void makeSphere(mesh &sphereMesh, float fRadius, int iSlices, int iStacks) {
  float drho = (float)PLP_PI / (float)iStacks;
  float dtheta = (float)(2.0f * PLP_PI) / (float)iSlices;

  float ds = 1.0f / (float)iSlices;
  float dt = 1.0f / (float)iStacks;

  float t = 1.0f;
  float s = 0.0f;

  sphereMesh.allocate(iSlices * iStacks * 6);

  for (int i = 0; i < iStacks; i++) {
    float rho = (float)(i * drho);
    float srho = (float)(sin(rho));
    float crho = (float)(cos(rho));
    float srhodrho = (float)sin(rho + drho);
    float crhodrho = (float)cos(rho + drho);

    // Many sources of OpenGL sphere drawing code uses a triangle fan
    // for the caps of the sphere. This however introduces texturing
    // artifacts at the poles on some OpenGL implementations

    s = 0.0f;

    // initialization of three 2-D arrays, two are 4 x 3 and one is 4 x 2

    float vertex[4][3];
    float normal[4][3];
    float texture[4][2];

    for (int j = 0; j < iSlices; j++) {
      /*var theta = (j == iSlices) ? 0.0 : j * dtheta;
      var stheta = parseFloat(-Math.sin(theta));
      var ctheta = parseFloat(Math.cos(theta));

      var x = stheta * srho;
      var y = ctheta * srho;
      var z = crho;*/

      float theta = (j == iSlices) ? 0.0f : j * dtheta;
      float stheta = -sin(theta);
      float ctheta = cos(theta);

      float x = stheta * srho;
      float y = ctheta * srho;
      float z = crho;

      texture[0][0] = s;
      texture[0][1] = t;
      normal[0][0] = x;
      normal[0][1] = y;
      normal[0][2] = z;
      vertex[0][0] = x * fRadius;
      vertex[0][1] = y * fRadius;
      vertex[0][2] = z * fRadius;

      x = stheta * srhodrho;
      y = ctheta * srhodrho;
      z = crhodrho;

      texture[1][0] = s;
      texture[1][1] = t - dt;
      normal[1][0] = x;
      normal[1][1] = y;
      normal[1][2] = z;
      vertex[1][0] = x * fRadius;
      vertex[1][1] = y * fRadius;
      vertex[1][2] = z * fRadius;

      theta = ((j + 1) == iSlices) ? 0.0f : (j + 1) * dtheta;
      stheta = -std::sin(theta);
      ctheta = std::cos(theta);

      x = stheta * srho;
      y = ctheta * srho;
      z = crho;

      s += ds;
      texture[2][0] = s;
      texture[2][1] = t;
      normal[2][0] = x;
      normal[2][1] = y;
      normal[2][2] = z;
      vertex[2][0] = x * fRadius;
      vertex[2][1] = y * fRadius;
      vertex[2][2] = z * fRadius;

      x = stheta * srhodrho;
      y = ctheta * srhodrho;
      z = crhodrho;

      texture[3][0] = s;
      texture[3][1] = t - dt;
      normal[3][0] = x;
      normal[3][1] = y;
      normal[3][2] = z;
      vertex[3][0] = x * fRadius;
      vertex[3][1] = y * fRadius;
      vertex[3][2] = z * fRadius;

      // Convert arrays to float** for the function call

      float *vertexPtrs[4];
      float *normalPtrs[4];
      float *texturePtrs[4];

      for (int k = 0; k < 4; k++) {
        vertexPtrs[k] = vertex[k];
        normalPtrs[k] = normal[k];
        texturePtrs[k] = texture[k];
      }

      sphereMesh.addTriangle(vertexPtrs, normalPtrs, texturePtrs);

      // Rearrange for next triangle

      vertex[0][0] = vertex[1][0];
      vertex[0][1] = vertex[1][1];
      vertex[0][2] = vertex[1][2];
      normal[0][0] = normal[1][0];
      normal[0][1] = normal[1][1];
      normal[0][2] = normal[1][2];
      texture[0][0] = texture[1][0];
      texture[0][1] = texture[1][1];

      vertex[1][0] = vertex[3][0];
      vertex[1][1] = vertex[3][1];
      vertex[1][2] = vertex[3][2];
      normal[1][0] = normal[3][0];
      normal[1][1] = normal[3][1];
      normal[1][2] = normal[3][2];
      texture[1][0] = texture[3][0];
      texture[1][1] = texture[3][1];

      sphereMesh.addTriangle(vertexPtrs, normalPtrs, texturePtrs);
    }

    t -= dt;
  }

  sphereMesh.prepareToDraw();
}
