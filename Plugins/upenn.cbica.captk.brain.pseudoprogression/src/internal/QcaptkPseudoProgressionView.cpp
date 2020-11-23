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

#include "QcaptkPseudoProgressionView.h"

// blueberry
#include <berryConstants.h>
#include <berryIWorkbenchPage.h>

// mitk
#include "mitkApplicationCursor.h"
#include "mitkStatusBar.h"
//#include "mitkPlanePositionManager.h"
#include "captkPseudoProgressionPluginActivator.h"

// Qmitk
//#include "QmitkRenderWindow.h"

// us
#include <usGetModuleContext.h>
#include <usModule.h>
#include <usModuleContext.h>
#include <usModuleResource.h>
#include <usModuleResourceStream.h>

// Qt
#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

// CaPTk
//#include "CaPTkSurvival.h"

const std::string QcaptkPseudoProgressionView::VIEW_ID = "upenn.cbica.captk.views.pseudoprogression";

QcaptkPseudoProgressionView::QcaptkPseudoProgressionView()
  : m_Parent(nullptr)
{
  //m_CaPTkSurvival = new captk::CaPTkSurvival(this);
}

QcaptkPseudoProgressionView::~QcaptkPseudoProgressionView()
{

}

void QcaptkPseudoProgressionView::CreateQtPartControl(QWidget *parent)
{
  // setup the basic GUI of this view
  m_Parent = parent;

  m_Controls.setupUi(parent);


  /**** Connect signals & slots ****/

  connect(m_Controls.m_cbUsageSelector, SIGNAL(currentTextChanged(QString)),
          this, SLOT(OnUsageComboBoxCurrentTextChanged(QString)));

  connect(m_Controls.m_cbModelSourceSelector, SIGNAL(currentTextChanged(QString)),
          this, SLOT(OnModelSourceComboBoxCurrentTextChanged(QString)));

  connect(m_Controls.pushButton_CustomModelDirBrowse, SIGNAL(clicked()),
          this, SLOT(OnCustomModelDirectoryButtonClicked()));

  connect(m_Controls.pushButton_OutputDirBrowse, SIGNAL(clicked()),
          this, SLOT(OnOutputDirectoryButtonClicked()));

  connect(m_Controls.pushButton_SubjectDirBrowse, SIGNAL(clicked()),
          this, SLOT(OnSubjectDirectoryButtonClicked()));

  connect(m_Controls.pushButtonRun, SIGNAL(clicked()),
          this, SLOT(OnRunButtonPressed()));

  //connect(m_CaPTkSurvival, SIGNAL(done()),
  //        this, SLOT(OnModuleDone()));

  /**** Initialize widgets ****/

  // Initialize the usage type combo box
  m_Controls.m_cbUsageSelector->addItems(QStringList() << "Use Existing Model" << "Train New Model");
  // Set combo box to the last user selected value
  m_Controls.m_cbUsageSelector->setCurrentText(
    this->GetPreferences()->Get("SurvivalUsageComboBox", "Use Existing Model")
  );

  // Initialize model source combo box
  m_Controls.m_cbModelSourceSelector->addItems(
    QStringList() << "CBICA CaPTk Model" << "Custom"
  );
  // Set combo box to the last user selected value
  m_Controls.m_cbModelSourceSelector->setCurrentText(
              this->GetPreferences()->Get("SurvivalModelSourceComboBox", "CBICA CaPTk Model")
  );

  // Set model information
  m_Controls.label_PaperInformation->setWordWrap(true);
  QString modelInfo = ("This is a model trained on de novo glioblastoma cases. \
	  Please note that this model was created following certain assumptions \
	  (described in the paper below). \
	  It can be used for research purposes only.<br><br>"
	  "H.Akbari, et al. Quantitative radiomics and machine learning to distinguish \
	   true progression from pseudoprogression in patients with GBM, \
	  ASNR 56th Annual Meeting, 2018.");
  m_Controls.label_PaperInformation->setText(modelInfo);
  m_Controls.label_PaperInformation->setTextFormat(Qt::RichText);
  m_Controls.label_PaperInformation->setTextInteractionFlags(Qt::TextBrowserInteraction);
  m_Controls.label_PaperInformation->setOpenExternalLinks(true);
}

void QcaptkPseudoProgressionView::Activated()
{

}

void QcaptkPseudoProgressionView::Deactivated()
{
  // Not yet implemented
}

void QcaptkPseudoProgressionView::Visible()
{
  // Not yet implemented
}

void QcaptkPseudoProgressionView::Hidden()
{
  // Not yet implemented
}

int QcaptkPseudoProgressionView::GetSizeFlags(bool width)
{
  if (!width)
  {
    return berry::Constants::MIN | berry::Constants::MAX | berry::Constants::FILL;
  }
  else
  {
    return 0;
  }
}

int QcaptkPseudoProgressionView::ComputePreferredSize(bool width,
                                                          int /*availableParallel*/,
                                                          int /*availablePerpendicular*/,
                                                          int preferredResult)
{
  if (width == false)
  {
    return 100;
  }
  else
  {
    return preferredResult;
  }
}

/************************************************************************/
/* protected slots                                                      */
/************************************************************************/
void QcaptkPseudoProgressionView::OnUsageComboBoxCurrentTextChanged(const QString& text)
{
  // set preference to remember usage choice
  this->GetPreferences()->Put("SurvivalUsageComboBox", text);
  this->GetPreferences()->Flush();

  if (text == "Use Existing Model") {
      m_Controls.m_cbModelSourceSelector->setVisible(true);
      m_Controls.label_Source->setVisible(true);
      if (m_Controls.m_cbModelSourceSelector->currentText() == "Custom") {
          m_Controls.lineEdit_CustomModelDir->setVisible(true);
          m_Controls.label_CustomModelDir->setVisible(true);
          m_Controls.pushButton_CustomModelDirBrowse->setVisible(true);
      }
      else if (m_Controls.m_cbModelSourceSelector->currentText() == "CBICA CaPTk Model") {
          m_Controls.groupBox_PaperInfo->setVisible(true);
      }

  }
  else if (text == "Train New Model") {
      m_Controls.m_cbModelSourceSelector->setVisible(false);
      m_Controls.label_Source->setVisible(false);
      m_Controls.lineEdit_CustomModelDir->setVisible(false);
      m_Controls.label_CustomModelDir->setVisible(false);
      m_Controls.pushButton_CustomModelDirBrowse->setVisible(false);
      m_Controls.groupBox_PaperInfo->setVisible(false);

  }
}

void QcaptkPseudoProgressionView::OnModelSourceComboBoxCurrentTextChanged(const QString& text)
{
  // Set preference to remember model source choice
  this->GetPreferences()->Put("SurvivalSourceComboBox", text);
  this->GetPreferences()->Flush();

  // Show/Hide views below it
  if (text == "CBICA CaPTk Model")
  {
    m_Controls.lineEdit_CustomModelDir->setVisible(false);
    m_Controls.label_CustomModelDir->setVisible(false);
    m_Controls.pushButton_CustomModelDirBrowse->setVisible(false);
    m_Controls.groupBox_PaperInfo->setVisible(true);
    m_Controls.label_PaperInformation->setVisible(true);
  }
  else if (text == "Custom")
  {
    m_Controls.lineEdit_CustomModelDir->setVisible(true);
    m_Controls.pushButton_CustomModelDirBrowse->setVisible(true);
    m_Controls.label_CustomModelDir->setVisible(true);
    m_Controls.groupBox_PaperInfo->setVisible(false);
    m_Controls.label_PaperInformation->setVisible(false);
  }

}


void QcaptkPseudoProgressionView::OnSubjectDirectoryButtonClicked()
{
  auto dirName = QFileDialog::getExistingDirectory(m_Parent,
    tr("Select subject directory"), this->GetLastFileOpenPath());

  if(dirName.isEmpty() || dirName.isNull()) { return; }

  // Find and save file information
  QFileInfo file(dirName);
  if (!file.isDir()) { return; }
  this->SetLastFileOpenPath(dirName);



  // Set the path to the QLineEdit
  m_Controls.lineEdit_SubjectDir->setText(dirName);
}

void QcaptkPseudoProgressionView::OnCustomModelDirectoryButtonClicked()
{
    auto dirName = QFileDialog::getExistingDirectory(m_Parent,
                                                     tr("Select model directory"),
                                                     this->GetLastFileOpenPath());

    if(dirName.isEmpty() || dirName.isNull()) { return; }

    // Find and save file information
    QFileInfo file(dirName);
    if (!file.isDir()) { return; }
    this->SetLastFileOpenPath(dirName);


    // Set the path to the QLineEdit
    m_Controls.lineEdit_CustomModelDir->setText(dirName);
}

void QcaptkPseudoProgressionView::OnOutputDirectoryButtonClicked()
{
  auto dirName = QFileDialog::getExistingDirectory(m_Parent, 
    tr("Select output directory"), this->GetLastFileOpenPath());

  if(dirName.isEmpty() || dirName.isNull()) { return; }

  // Find and save file information
  QFileInfo file(dirName);
  if (!file.isDir()) { return; }
  this->SetLastFileOpenPath(dirName);

  // Set the path to the QLineEdit
  m_Controls.lineEdit_OutputDir->setText(dirName);
}

void QcaptkPseudoProgressionView::OnRunButtonPressed()
{
  QString modelDirPath = m_Controls.lineEdit_CustomModelDir->text();
  QString subjectDirPath = m_Controls.lineEdit_SubjectDir->text();
  QString outputDirPath = m_Controls.lineEdit_OutputDir->text();
  bool trainNewModel = false; // true if training, false if using an existing model
  bool useCustomModel = false; // true if using a custom model, false if using CBICA's CaPTk model

  if (m_Controls.m_cbUsageSelector->currentText() == "Train New Model") {
      trainNewModel = true;
      useCustomModel = false;
  }
  else if (m_Controls.m_cbUsageSelector->currentText() == "Use Existing Model") {
      trainNewModel = false;

      if  (m_Controls.m_cbModelSourceSelector->currentText() == "CBICA CaPTk Model") {
          useCustomModel = false;
      }
      else if (m_Controls.m_cbModelSourceSelector->currentText() == "Custom") {
          useCustomModel = true;
      }

  }
  m_Controls.pushButtonRun->setDisabled(true);
  m_Controls.pushButtonRun->setText("Running Survival Prediction...");
//  m_CaPTkSurvival->Run(
//   modelDirPath,
//   subjectDirPath,
//   outputDirPath,
//   trainNewModel,
//   useCustomModel
//   );
}

//void QcaptkPseudoProgressionView::OnModuleDone()
//{
//    m_Controls.pushButtonRun->setEnabled(true);
//    m_Controls.pushButtonRun->setText("Run Survival Prediction");
//}

/************************************************************************/
/* protected                                                            */
/************************************************************************/
void QcaptkPseudoProgressionView::OnSelectionChanged(berry::IWorkbenchPart::Pointer, const QList<mitk::DataNode::Pointer> &)
{

}

void QcaptkPseudoProgressionView::OnPreferencesChanged(const berry::IBerryPreferences*)
{

}

void QcaptkPseudoProgressionView::NodeAdded(const mitk::DataNode *)
{

}

void QcaptkPseudoProgressionView::NodeRemoved(const mitk::DataNode *)
{

}

void QcaptkPseudoProgressionView::SetFocus()
{
}

void QcaptkPseudoProgressionView::UpdateControls()
{
  this->RequestRenderWindowUpdate(mitk::RenderingManager::REQUEST_UPDATE_ALL);
}

void QcaptkPseudoProgressionView::InitializeListeners()
{

}

QString QcaptkPseudoProgressionView::GetLastFileOpenPath()
{
  return this->GetPreferences()->Get("LastFileOpenPath", "");
}

void QcaptkPseudoProgressionView::SetLastFileOpenPath(const QString &path)
{
  this->GetPreferences()->Put("LastFileOpenPath", path);
  this->GetPreferences()->Flush();
}
