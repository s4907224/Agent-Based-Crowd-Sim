#include "objhandler.h"

ObjHandler::ObjHandler() : m_shaderProps ({1.f, 1.f, 1.f},
                                          {1.f, 1.f, 1.f},
                                           1.f,
                                           1.f,
                                           0.25f,
                                           0.1f,
                                           1.f,
                                           99999),
                           m_position ({0, 0, 0})
{}

ObjHandler::~ObjHandler() = default;