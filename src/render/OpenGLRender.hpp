#ifndef OPENGLRENDER_HPP
#define OPENGLRENDER_HPP
#include "GLXW/glxw.h"

namespace render
{
namespace gl
{
    uint32_t CreateShaderFromString(uint32_t type, const char* source)
    {
        return glCreateShaderProgramv(type, 1, (const char**)&source);
    }

    uint32_t CreateProgramPipeline(uint32_t vert_program = 0,
                                   uint32_t frag_program = 0,
                                   uint32_t geom_program = 0)
    {
        uint32_t ppo;
        glCreateProgramPipelines(1, &ppo);

        if (vert_program)
            glUseProgramStages(ppo, GL_VERTEX_SHADER_BIT, vert_program);
        if (frag_program)
            glUseProgramStages(ppo, GL_FRAGMENT_SHADER_BIT, frag_program);
        if (geom_program)
            glUseProgramStages(ppo, GL_GEOMETRY_SHADER_BIT, geom_program);

        return ppo;
    }
}
}

#endif