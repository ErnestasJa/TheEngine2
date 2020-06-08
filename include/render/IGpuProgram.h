#ifndef IGPUPROGRAM_H
#define IGPUPROGRAM_H

#include "IGpuProgramUniform.h"

namespace render {
class IGpuProgram
{
  public:
  virtual ~IGpuProgram()
  {
  }

  virtual void Bind()                                                            = 0;
  virtual const core::Vector<core::UniquePtr<IGpuProgramUniform>>& GetUniforms() = 0;
  virtual IGpuProgramUniform* GetUniform(const core::String& name)               = 0;
};
} // namespace render

#endif