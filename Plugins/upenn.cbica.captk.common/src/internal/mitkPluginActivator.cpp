/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/
#include "mitkPluginActivator.h"

#include "QmitkCaPTkSplitLabelsToImagesAction.h"

using namespace mitk;

ctkPluginContext* PluginActivator::m_context = nullptr;
PluginActivator* PluginActivator::m_Instance = nullptr;

PluginActivator::PluginActivator()
{
  m_Instance = this;
}

PluginActivator::~PluginActivator()
{
  m_Instance = nullptr;
}

void PluginActivator::start(ctkPluginContext *context)
{
  BERRY_REGISTER_EXTENSION_CLASS(QmitkCaPTkSplitLabelsToImagesAction, context)

  this->m_context = context;
}

void PluginActivator::stop(ctkPluginContext *)
{
  this->m_context = nullptr;
}

PluginActivator* PluginActivator::getDefault()
{
  return m_Instance;
}

ctkPluginContext*PluginActivator::getContext()
{
  return m_context;
}