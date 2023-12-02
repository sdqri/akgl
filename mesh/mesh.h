#ifndef MESH_H
#define MESH_H

#include "../glad.h" 

typedef struct {
    float* Vertices;
    unsigned int VerticesCount;
    unsigned int* Indices;
    unsigned int IndicesCount;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
} Mesh;

Mesh* NewSphere(int slices, int stacks);
void InitMesh(Mesh*);
void PrintMesh(Mesh*);
void BindMesh(Mesh*);
void UnbindMesh();
void DestroyMesh(Mesh* mesh);

#endif
