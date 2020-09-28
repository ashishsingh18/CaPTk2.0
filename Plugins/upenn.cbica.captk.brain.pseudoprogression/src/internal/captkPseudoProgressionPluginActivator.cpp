#include "captkPseudoProgressionPluginActivator.h"

#include "QcaptkPseudoProgressionView.h"

#include <usModuleInitialization.h>

ctkPluginContext* mitk::PluginActivator::m_Context = nullptr;

US_INITIALIZE_MODULE 

void mitk::PluginActivator::start(ctkPluginContext *context)
{
  BERRY_REGISTER_EXTENSION_CLASS(QcaptkPseudoProgressionView, context)
  // BERRY_REGISTER_EXTENSION_CLASS(QmitkMultiLabelSegmentationPreferencePage, context)
  // BERRY_REGISTER_EXTENSION_CLASS(QmitkMultiLabelSegmentationUtilitiesView, context)

  m_Context = context;
}

void mitk::PluginActivator::stop(ctkPluginContext*)
{
}

ctkPluginContext* mitk::PluginActivator::getContext()
{
  return m_Context;
}
