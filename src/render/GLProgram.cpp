#include "GLProgram.h"

namespace render
{
GLProgram::GLProgram(uint32_t programObject) : m_programObject(programObject)
{
}

GLProgram::~GLProgram()
{
}

uint32_t GLProgram::GetProgramObject()
{
    return m_programObject;
}
}
