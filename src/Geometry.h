#pragma once
#include <glm/glm.hpp>

glm::vec3 ExtractCameraPos_NoScale(const glm::mat4 & a_modelView)
{
  glm::mat3 rotMat(a_modelView);
  glm::vec3 d(a_modelView[3]);

  glm::vec3 retVec = -d * rotMat;
  return retVec;
}
