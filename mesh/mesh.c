#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"

float EPSILON = 0.0001;

Mesh* NewSphere(int slices, int stacks) {
    Mesh* sphere = (Mesh*)malloc(sizeof(Mesh));

    if (sphere == NULL) {
        free(sphere);
        return NULL;
    }

    // Calculate the number of vertices and allocate memory
    //
    
    sphere->VerticesCount = ((stacks - 1) * slices) + 2;
    sphere->Vertices = (float*)malloc(3 * sphere->VerticesCount * sizeof(float));
    printf("VerticesCount:%d \n",sphere->VerticesCount);

    // Calculate the number of indices and allocate memory
    sphere ->IndicesCount = (slices * 2) + ((stacks - 2) * slices * 2); // first part because top stack and bottom slack are on triagle per part
    sphere->Indices = (unsigned int*)malloc(3 * sphere->IndicesCount * sizeof(unsigned int));
    printf("IndicesCount:%d \n",sphere->IndicesCount);

    if (sphere->Vertices == NULL || sphere->Indices == NULL) {
        // Handle memory allocation failure
        free(sphere->Vertices);
        free(sphere->Indices);
        free(sphere);
        return NULL;
    }

    // Generate parametric sphere vertices and indices
    int index = 0;
    for (int i = 0; i <= stacks; ++i) {
        float phi = ((float)i / stacks) * M_PI;            // Polar angle
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);
        for (int j = 0; j < slices; ++j) {
            printf("index=%d, i=%d, j=%d, sinPhi=%f\n", index, i, j, sinPhi);
            printf("islower=%b\n", fabsf(sinPhi)<EPSILON);
            if (fabsf(sinPhi)<EPSILON) {
                printf("ok\n");
                float x = 0.0f;
                float y = 0.0f;
                float z = cosPhi;
                sphere->Vertices[3 * index] = x;
                sphere->Vertices[3 * index + 1] = y;
                sphere->Vertices[3 * index + 2] = z;
                index++;
                break;
            }
            float theta = ((float)j / slices) * 2.0f * M_PI;  // Azimuthal angle
            // Spherical coordinates to Cartesian coordinates conversion
            float x = sinPhi * cos(theta);
            float y = sinPhi * sin(theta);
            float z = cosPhi;
            sphere->Vertices[3 * index] = x;
            sphere->Vertices[3 * index + 1] = y;
            sphere->Vertices[3 * index + 2] = z;
            index++;
        }       
    
    }

    // Generate indices for triangles
    index = 0;
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int k1 = (i==0)? 0 : ((i-1) * slices) + (j + 1); 
            int k2 = k1 + slices;
            if (i == 0){
                k2 = j + 1;
            } else if (i == stacks - 1){
                k2 = (i * slices) + 1; 
            }
            printf("i=%d, j=%d, k1=%d, k2=%d\n", i, j, k1, k2);
            if (i!=0){
                sphere->Indices[index++] = k1;
                sphere->Indices[index++] = k2;
                sphere->Indices[index++] = (j != slices - 1) ? (k1 + 1) : k1 + 1 - (j + 1);
            }

            if (i != stacks-1){
                sphere->Indices[index++] = k1;
                sphere->Indices[index++] = k2;
                sphere->Indices[index++] = (j != slices - 1) ? (k2 + 1) : k2 + 1 - (j + 1);;
            }
            printf("index = %d\n", index);
        }
    }
    return sphere;
}

void InitMesh(Mesh* sphereMesh) {
    glGenVertexArrays(1, &sphereMesh->VAO);
    glGenBuffers(1, &sphereMesh->VBO);
    glGenBuffers(1, &sphereMesh->EBO);

    
    glBindVertexArray(sphereMesh->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, sphereMesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sphereMesh->VerticesCount * 3 * sizeof(float), sphereMesh->Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereMesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereMesh->IndicesCount * 3 * sizeof(unsigned int), sphereMesh->Indices, GL_STATIC_DRAW);
    
    // Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PrintMesh(Mesh* mesh) {
    printf("Vertices:\n");
    for (int i = 0; i < mesh->VerticesCount; ++i) {
        printf("V%d | x:%+1.2f, y:%+1.2f, z:%+1.2f\n", i, mesh->Vertices[i * 3], mesh->Vertices[i * 3 + 1], mesh->Vertices[i * 3 + 2]);
    }
    printf("\nIndices:\n");
    for (int j = 0; j < mesh->IndicesCount; ++j) {
        printf("I%d | V%d, V%d, V%d\n", j, mesh->Indices[j * 3], mesh->Indices[j * 3 + 1], mesh->Indices[j * 3 + 2]);
    }
}

void DestroyMesh(Mesh* mesh) {
    if (mesh != NULL) {
        free(mesh->Vertices);
        free(mesh->Indices);
        free(mesh);
    }
}


