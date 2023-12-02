#include <stdio.h>
#include <stdlib.h>
#include "../glad.h" 
#include "shader.h"

// Read shader file by filePath
char* readShaderFile(const char* filePath) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file content
    char* shaderSource = (char*)malloc(fileSize + 1);
    if (!shaderSource) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return NULL;
    }

    // Read the file content
    fread(shaderSource, 1, fileSize, file);
    shaderSource[fileSize] = '\0'; // Null-terminate the string

    fclose(file);
    return shaderSource;
}

unsigned int compileShader(GLenum shaderType, const char *shaderSource) {
    // Create shader
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check for shader compile errors
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", info_log);
    }

    return shader;
}

unsigned int linkShaders(unsigned int vertexShader,
                         unsigned int fragmentShader) {
    // Create shader program
    unsigned int shaderProgram = glCreateProgram();

    // Attach shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link the program
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    // Delete the shaders as they are now linked into the program and no longer
    // necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

ShaderResult NewShader(const char* vertexShaderPath, const char* fragmentShaderPath){
    ShaderResult shaderResult;

    // Load shaders from files
    char* vertexShaderSource = readShaderFile(vertexShaderPath);
    char* fragmentShaderSource = readShaderFile(fragmentShaderPath);

    if (!vertexShaderSource || !fragmentShaderSource) {
        // Handle error, shader loading failed
        free(vertexShaderSource);
        free(fragmentShaderSource);
        shaderResult.Err = "unable to load vertex or fragment shader";
        return shaderResult;
    }

    unsigned int vertexShader =
        compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = linkShaders(vertexShader, fragmentShader);

    free(vertexShaderSource);
    free(fragmentShaderSource);

    shaderResult.Ok = shaderProgram;
    return shaderResult; 
}

