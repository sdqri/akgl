#include "glad.h"
#include <cglm/cam.h>
#include <cglm/cglm.h>
#include <cglm/vec2.h>
/* #include "shapes.h" */
#include"shader/shader.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include"mesh/mesh.h"

// Error callback function
static void errorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

// Window resize callback function
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /* const float cameraSpeed = 0.05f; // adjust accordingly */
    /* if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) */
    /*     glms_k */
    /*     cameraPos += cameraSpeed * cameraFront; */
    /* if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) */
    /*     cameraPos -= cameraSpeed * cameraFront; */
    /* if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) */
    /*     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; */
    /* if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) */
    /*     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; */
}

void printMat4(mat4 matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%f ", matrix[j][i]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW initialization failed\n");
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(800, 600, "hello", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "unable to create GLFWwindow");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "unable to initialize glad");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    
    glViewport(0, 0, 800, 600);

    // changes Viewport data on resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    const char* VERTEX_SHADER_PATH = "/home/sadiq/src/tmpgl/vertex_shader.glsl";
    const char* FRAGMENT_SHADER_PATH = "/home/sadiq/src/tmpgl/fragment_shader.glsl";

    ShaderResult shaderResult = NewShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    if (shaderResult.Err != NULL){
        printf("shader_result has error. err : %s", shaderResult.Err);
        return EXIT_FAILURE;
    }
    unsigned int shader_program = shaderResult.Ok;
    

    Mesh* sphereMesh = NewSphere(50, 10);
    /* PrintMesh(sphereMesh); */
    InitMesh(sphereMesh);

    vec3 cameraPosition = {-10.0f, -10.0f, 0.0f};

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(0.5f, 0.9f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        
        mat4 model;
        glm_mat4_identity(model);
        glm_scale(model, (vec3){1.0f, 1.5f, 1.0f});
        /* glm_rotate(model, 3.14f/4.0f, (vec3){0.0f, 0.0f, 1.0f}); */
        glm_translate(model, (vec3){0.0f, 0.0f, 0.0f});
        printMat4(model);

        const float radius = 10.0f;
        float eyeX = sin(glfwGetTime()) * radius;
        float eyeY = cos(glfwGetTime()) * radius;

        mat4 view;
        glm_lookat((vec3){eyeX, eyeY, 0.0f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f}, view);
        
        mat4 projection;
        glm_perspective_default(800.f/600.0f, projection);

        int modelLoc = glGetUniformLocation(shader_program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const float*)model[0]);
        
        int viewLoc = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float*)view[0]);
        
        int projectionLoc = glGetUniformLocation(shader_program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const float*)projection[0]);

        glBindVertexArray(sphereMesh->VAO);
        glDrawElements(GL_TRIANGLES, 3 * sphereMesh->IndicesCount, GL_UNSIGNED_INT, 0);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
