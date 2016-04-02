#ifndef IGPU_PROGRAM_H
#define IGPU_PROGRAM_H

namespace render
{
class IGpuProgram
{
public:
    virtual ~IGpuProgram()
    {
    }

    virtual void Bind() = 0;
};
}

#endif