/**
\file  TrainingModule.h

\brief The header file containing the TrainingModule class, used to build machine learning models
Author: Saima Rathore
Library Dependecies: ITK 4.7+ <br>

https://www.med.upenn.edu/sbia/software/ <br>
software@cbica.upenn.edu

Copyright (c) 2016 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.med.upenn.edu/sbia/software-agreement.html

*/
#ifndef CaPTkTrainingModuleAlgorithm_h
#define CaPTkTrainingModuleAlgorithm_h

// The following header file is generated by CMake and thus it's located in
// the build directory. It provides an export macro for classes and functions
// that you want to be part of the public interface of your module.
#include <MitkCaPTkTrainingModuleExports.h>

#include "itkExtractImageFilter.h"
#include "itkCSVArray2DFileReader.h"
#include <itkConstantBoundaryCondition.h>
#include <itkVariableSizeMatrix.h>
#include <itkVariableLengthVector.h>
#include <itkVector.h>

#include <string>

namespace captk
{
class MITKCAPTKTRAININGMODULE_EXPORT TrainingModuleAlgorithm
{
public:
  /**** Types ****/

  using VectorDouble = std::vector < double >;
  using VariableSizeMatrixType = itk::VariableSizeMatrix< double >;
  using VariableLengthVectorType = itk::VariableLengthVector< double >;
  typedef itk::Image< float, 3 > ImageType;
  typedef std::tuple<VectorDouble, VectorDouble, VariableSizeMatrixType, VectorDouble, 
                     VectorDouble, VariableSizeMatrixType, VectorDouble> FoldTupleType;
  typedef std::map<int, FoldTupleType> MapType;

  typedef std::tuple<VectorDouble, VectorDouble, VariableSizeMatrixType> TrainingDataTuple;
  typedef std::map<int, TrainingDataTuple> TrainingMapType;

  typedef std::tuple<VectorDouble, VectorDouble, VariableSizeMatrixType, VectorDouble> TestingDataTuple;
  typedef std::map<int, TestingDataTuple> TestingMapType;

  typedef itk::CSVArray2DFileReader<double> CSVFileReaderType;
  typedef vnl_matrix<double> MatrixType;

  /**** Public methods ****/

  TrainingModuleAlgorithm() {};
  ~TrainingModuleAlgorithm() {};

  VectorDouble TestData(const VariableSizeMatrixType inputFeatures, const std::string modelfolder, 
    const int classifiertype, const std::string outputfolder);
  
  VectorDouble TrainData(const VariableSizeMatrixType inputFeatures, const VariableLengthVectorType inputLabels,
    const std::string outputfolder, const int classifiertype);

  /** \brief Runs the algorithm
   * 
   * @return first field is true when execution went fine, std::string is the potential error message
   */
  std::tuple<bool, std::string> 
  Run(const std::string inputFeaturesFile, const std::string inputLabelsFile, 
    const std::string outputdirectory,const int classifierType, const int foldtype, 
    const int conftype, const std::string modeldirectory);

  VectorDouble CalculatePerformanceMeasures(VariableLengthVectorType predictedLabels, std::vector<double> GivenLabels);

  bool CheckPerformanceStatus(double ist, double second, double third, double fourth, double fifth, 
    double sixth, double seventh, double eighth, double ninth, double tenth);
  VectorDouble CalculatePerformanceMeasures(VectorDouble predictedLabels, VectorDouble GivenLabels);

  VectorDouble CrossValidation(const VariableSizeMatrixType inputFeatures, 
    const VariableLengthVectorType inputLabels, const std::string outputfolder,
    const int classifiertype, const int foldtype);

  VectorDouble InternalCrossValidation(VariableSizeMatrixType inputFeatures, std::vector<double> inputLabels, 
    double cValue, double gValue,int kerneltype);

  VectorDouble SplitTrainTest(const VariableSizeMatrixType inputFeatures, const VariableLengthVectorType inputLabels, 
    const std::string outputfolder, const int classifiertype, const int training_size);

  VectorDouble trainOpenCVSVM(const VariableSizeMatrixType &trainingDataAndLabels, const std::string &outputModelName, 
    bool considerWeights, int ApplicationCallingSVM, double bestc, double bestg);

  // VectorDouble testOpenCVSVM(const VariableSizeMatrixType &testingData, const std::string &inputModelName);

  VectorDouble CombineEstimates(const VariableLengthVectorType &estimates1, const VariableLengthVectorType &estimates2);

  VectorDouble EffectSizeFeatureSelection(const VariableSizeMatrixType training_features, std::vector<double> target);

  std::string CheckDataQuality(const VariableSizeMatrixType & FeaturesOfAllSubjects, 
    const VariableLengthVectorType & LabelsOfAllSubjects);

private:
  VectorDouble InternalCrossValidationSplitTrainTest(VariableSizeMatrixType inputFeatures, 
    std::vector<double> inputLabels, double cValue, double gValue, int kerneltype, 
    int counter, std::string outputfolder);

  template <typename T>
  std::vector<size_t> sort_indexes(const std::vector<T> &v);

  std::string mEighteenTrainedFile, mSixTrainedFile;
};
}

#endif // ! CaPTkTrainingModuleAlgorithm_h