#ifndef SHADER_H
#define SHADER_H

typedef struct{
    unsigned int Ok;
    char* Err;
} ShaderResult;

ShaderResult NewShader(const char* vertex_shader_path, const char* fragment_shader_path);

#endif
