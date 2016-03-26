#ifndef IPROGRAM_H
#define IPROGRAM_H

namespace render
{
class IProgram
{
public:
    virtual ~IProgram()
    {
    }

    virtual void Bind() = 0;
};
}

#endif