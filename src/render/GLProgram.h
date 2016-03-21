#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "render/IProgram.h"

namespace render
{
class GLProgram : public IProgram
{
public:
    GLProgram(uint32_t programObject);
    virtual ~GLProgram();

    uint32_t GetProgramObject();

private:
    uint32_t m_programObject;
};
}

#endif