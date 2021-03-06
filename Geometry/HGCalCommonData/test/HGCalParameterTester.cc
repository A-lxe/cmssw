#include <iostream>

#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/HGCalCommonData/interface/HGCalParameters.h"

class HGCalParameterTester : public edm::one::EDAnalyzer<> {

public:
  explicit HGCalParameterTester( const edm::ParameterSet& );
  ~HGCalParameterTester() override {}
  
  void beginJob() override {}
  void analyze(edm::Event const& iEvent, edm::EventSetup const&) override;
  void endJob() override {}
private:
  template<typename T> void myPrint(std::string const& s, 
				    std::vector<T> const& obj, int n);
  void myPrint(std::string const& s, HGCalParameters::wafer_map const& obj,
	       int n);
  const std::string name_;
  const int         mode_;
};

HGCalParameterTester::HGCalParameterTester(const edm::ParameterSet& ic) :
  name_(ic.getUntrackedParameter<std::string>("Name")),
  mode_(ic.getUntrackedParameter<int>("Mode")) { }

void HGCalParameterTester::analyze(const edm::Event& iEvent, 
				   const edm::EventSetup& iSetup) {

  edm::LogVerbatim("HGCalGeomr") << "HGCalParameter::Here I am";
  auto start = std::chrono::high_resolution_clock::now();
  
  edm::ESHandle<HGCalParameters> phgp;
  iSetup.get<IdealGeometryRecord>().get(name_, phgp);

  std::cout << phgp->name_ << "\n";
  if (mode_ == 0) {
    std::cout << "WaferR_: "   << phgp->waferR_   << "\n";
    std::cout << "SlopeMin_: " << phgp->slopeMin_ << "\n";
    std::cout << "nCells_: "   << phgp->nCells_   << "\n";
    std::cout << "nSectors_: " << phgp->nSectors_ << "\n";
    std::cout << "mode_: "     << phgp->mode_     << "\n";

    myPrint("CellSize",          phgp->cellSize_,          10);
    myPrint("slopeTop",          phgp->slopeTop_,          10);  
    myPrint("zFront",            phgp->zFront_,            10);
    myPrint("rMaxFront",         phgp->rMaxFront_,         10);
    myPrint("zRanges",           phgp->zRanges_,           10);  
    myPrint("moduleBlS",         phgp->moduleBlS_,         10);  
    myPrint("moduleTlS",         phgp->moduleTlS_,         10);  
    myPrint("moduleHS",          phgp->moduleHS_,          10);
    myPrint("moduleDzS",         phgp->moduleDzS_,         10);  
    myPrint("moduleAlphaS",      phgp->moduleAlphaS_,      10);  
    myPrint("moduleCellS",       phgp->moduleCellS_,       10);  
    myPrint("moduleBlR",         phgp->moduleBlR_,         10);  
    myPrint("moduleTlR",         phgp->moduleTlR_,         10);  
    myPrint("moduleHR",          phgp->moduleHR_,          10);
    myPrint("moduleDzR",         phgp->moduleDzR_,         10);  
    myPrint("moduleAlphaR",      phgp->moduleAlphaR_,      10);  
    myPrint("moduleCellR",       phgp->moduleCellR_,       10);  
    myPrint("trformTranX",       phgp->trformTranY_,       10);
    myPrint("trformTranY",       phgp->trformTranY_,       10);
    myPrint("trformTranZ",       phgp->trformTranZ_,       10);
    myPrint("trformRotXX",       phgp->trformRotXX_,       10);  
    myPrint("trformRotYX",       phgp->trformRotYX_,       10);  
    myPrint("trformRotZX",       phgp->trformRotZX_,       10);  
    myPrint("trformRotXY",       phgp->trformRotXY_,       10);  
    myPrint("trformRotYY",       phgp->trformRotYY_,       10);  
    myPrint("trformRotZY",       phgp->trformRotZY_,       10);  
    myPrint("trformRotXZ",       phgp->trformRotXZ_,       10);  
    myPrint("trformRotYZ",       phgp->trformRotYZ_,       10);  
    myPrint("trformRotZZ",       phgp->trformRotZZ_,       10);  
    myPrint("zLayerHex",         phgp->zLayerHex_,         10);  
    myPrint("rMinLayHex",        phgp->rMinLayHex_,        10);  
    myPrint("rMaxLayHex",        phgp->rMaxLayHex_,        10);  
    myPrint("waferPosX",         phgp->waferPosX_,         10);  
    myPrint("waferPosY",         phgp->waferPosY_,         10);
    myPrint("cellFineX",         phgp->cellFineX_,         10);  
    myPrint("cellFineY",         phgp->cellFineY_,         10);  
    myPrint("cellFineHalf",      phgp->cellFineHalf_,      10);  
    myPrint("cellCoarseX",       phgp->cellCoarseX_,       10);  
    myPrint("cellCoarseY",       phgp->cellCoarseY_,       10);  
    myPrint("cellCoarseHalf",    phgp->cellCoarseHalf_,    10);
    myPrint("boundR",            phgp->boundR_,            10);  
    myPrint("moduleLayS",        phgp->moduleLayS_,        10);  
    myPrint("moduleLayR",        phgp->moduleLayR_,        10);  
    myPrint("layer",             phgp->layer_,             18);  
    myPrint("layerIndex",        phgp->layerIndex_,        18);  
    myPrint("layerGroup",        phgp->layerGroup_,        18);  
    myPrint("cellFactor",        phgp->cellFactor_,        10);  
    myPrint("depth",             phgp->depth_,             18);
    myPrint("depthIndex",        phgp->depthIndex_,        18);
    myPrint("depthLayerF",       phgp->depthLayerF_,       18);
    myPrint("waferCopy",         phgp->waferCopy_,         8);    
    myPrint("waferTypeL",        phgp->waferTypeL_,        20);
    myPrint("waferTypeT",        phgp->waferTypeT_,        20);
    myPrint("layerGroupM",       phgp->layerGroupM_,       18);  
    myPrint("layerGroupO",       phgp->layerGroupO_,       18);
    myPrint("trformIndex",       phgp->trformIndex_,       10);
    myPrint("levelTop",          phgp->levelT_,            10);

  } else if (mode_ == 1) {

    std::cout << "SlopeMin_: "   << phgp->slopeMin_ << "\n";
    std::cout << "Wafer Parameters: " << phgp->waferSize_ << ":"
	      << phgp->waferR_  << ":" << phgp->waferThick_ << ":"
	      << phgp->sensorSeparation_ << ":" << phgp->mouseBite_ << "\n";
    std::cout << "nCells_: " << phgp->nCellsFine_  << ":" 
	      << phgp->nCellsCoarse_ << "\n";
    std::cout << "nSectors_: "  << phgp->nSectors_ << "\n";
    std::cout << "FirstLayer: " << phgp->firstLayer_ << "\n";
    std::cout << "mode_: "      << phgp->mode_     << "\n";
    std::cout << "waferUVMax: " << phgp->waferUVMax_ << "\n";

    myPrint("waferUVMaxLayer",   phgp->waferUVMaxLayer_,   20);
    myPrint("CellThickness",     phgp->cellThickness_,     10);
    myPrint("radius100to200",    phgp->radius100to200_,    10);  
    myPrint("radius200to300",    phgp->radius200to300_,    10);
    std::cout << "nCornerCut " << phgp->nCornerCut_ << "  zMinForRad "
	      << phgp->zMinForRad_ << "\n";
  
    myPrint("radiusMixBoundary", phgp->radiusMixBoundary_, 10);  
    myPrint("slopeTop",          phgp->slopeTop_,          10);  
    myPrint("zFront",            phgp->zFront_,            10);
    myPrint("rMaxFront",         phgp->rMaxFront_,         10);
    myPrint("zRanges",           phgp->zRanges_,           10);  
    myPrint("moduleBlS",         phgp->moduleBlS_,         10);  
    myPrint("moduleTlS",         phgp->moduleTlS_,         10);  
    myPrint("moduleHS",          phgp->moduleHS_,          10);
    myPrint("moduleDzS",         phgp->moduleDzS_,         10);  
    myPrint("moduleAlphaS",      phgp->moduleAlphaS_,      10);  
    myPrint("moduleCellS",       phgp->moduleCellS_,       10);  
    myPrint("moduleBlR",         phgp->moduleBlR_,         10);  
    myPrint("moduleTlR",         phgp->moduleTlR_,         10);  
    myPrint("moduleHR",          phgp->moduleHR_,          10);
    myPrint("moduleDzR",         phgp->moduleDzR_,         10);  
    myPrint("moduleAlphaR",      phgp->moduleAlphaR_,      10);  
    myPrint("moduleCellR",       phgp->moduleCellR_,       10);  
    myPrint("trformTranX",       phgp->trformTranY_,       10);
    myPrint("trformTranY",       phgp->trformTranY_,       10);
    myPrint("trformTranZ",       phgp->trformTranZ_,       10);
    myPrint("trformRotXX",       phgp->trformRotXX_,       10);  
    myPrint("trformRotYX",       phgp->trformRotYX_,       10);  
    myPrint("trformRotZX",       phgp->trformRotZX_,       10);  
    myPrint("trformRotXY",       phgp->trformRotXY_,       10);  
    myPrint("trformRotYY",       phgp->trformRotYY_,       10);  
    myPrint("trformRotZY",       phgp->trformRotZY_,       10);  
    myPrint("trformRotXZ",       phgp->trformRotXZ_,       10);  
    myPrint("trformRotYZ",       phgp->trformRotYZ_,       10);  
    myPrint("trformRotZZ",       phgp->trformRotZZ_,       10);  
    myPrint("zLayerHex",         phgp->zLayerHex_,         10);  
    myPrint("rMinLayHex",        phgp->rMinLayHex_,        10);  
    myPrint("rMaxLayHex",        phgp->rMaxLayHex_,        10);  
    myPrint("waferPosX",         phgp->waferPosX_,         10);  
    myPrint("waferPosY",         phgp->waferPosY_,         10);
    myPrint("cellFineIndex",     phgp->cellFineIndex_,     10);  
    myPrint("cellFineX",         phgp->cellFineX_,         10);  
    myPrint("cellFineY",         phgp->cellFineY_,         10);  
    myPrint("cellCoarseIndex",   phgp->cellCoarseIndex_,   10);
    myPrint("cellCoarseX",       phgp->cellCoarseX_,       10);
    myPrint("cellCoarseY",       phgp->cellCoarseY_,       10);  
    myPrint("layer",             phgp->layer_,             18);  
    myPrint("layerIndex",        phgp->layerIndex_,        18);  
    myPrint("depth",             phgp->depth_,             18);
    myPrint("depthIndex",        phgp->depthIndex_,        18);
    myPrint("depthLayerF",       phgp->depthLayerF_,       18);
    myPrint("waferCopy",         phgp->waferCopy_,         8);    
    myPrint("waferTypeL",        phgp->waferTypeL_,        20);
    myPrint("trformIndex",       phgp->trformIndex_,       10);
    myPrint("levelTop",          phgp->levelT_,            10);

  } else {

    std::cout << "SlopeMin_: "  << phgp->slopeMin_   << "\n";
    std::cout << "nCells_: " << phgp->nCellsFine_  << ":" 
	      << phgp->nCellsCoarse_ << "\n";
    std::cout << "EtaMinBH: "   << phgp->etaMinBH_   << "\n";
    std::cout << "FirstLayer: " << phgp->firstLayer_ << "\n";
    std::cout << "mode_: "      << phgp->mode_       << "\n";
    std::cout << "waferUVMax: " << phgp->waferUVMax_ << "\n";
  
    myPrint("radiusMixBoundary", phgp->radiusMixBoundary_, 10);  
    myPrint("nPhiBinBH",         phgp->nPhiBinBH_,         18);  
    myPrint("dPhiEtaBH",         phgp->dPhiEtaBH_,         10);  
    myPrint("slopeTop",          phgp->slopeTop_,          10);  
    myPrint("zFront",            phgp->zFront_,            10);
    myPrint("rMaxFront",         phgp->rMaxFront_,         10);
    myPrint("zRanges",           phgp->zRanges_,           10);  
    myPrint("firstModule",       phgp->firstModule_,       10);
    myPrint("lastModule",        phgp->lastModule_,        10);  
    myPrint("iEtaMinBH",         phgp->iEtaMinBH_,         20);  
    myPrint("moduleBlS",         phgp->moduleBlS_,         10);  
    myPrint("moduleTlS",         phgp->moduleTlS_,         10);  
    myPrint("moduleHS",          phgp->moduleHS_,          10);
    myPrint("moduleDzS",         phgp->moduleDzS_,         10);  
    myPrint("moduleAlphaS",      phgp->moduleAlphaS_,      10);  
    myPrint("moduleCellS",       phgp->moduleCellS_,       10);  
    myPrint("moduleBlR",         phgp->moduleBlR_,         10);  
    myPrint("moduleTlR",         phgp->moduleTlR_,         10);  
    myPrint("moduleHR",          phgp->moduleHR_,          10);
    myPrint("moduleDzR",         phgp->moduleDzR_,         10);  
    myPrint("moduleAlphaR",      phgp->moduleAlphaR_,      10);  
    myPrint("moduleCellR",       phgp->moduleCellR_,       10);  
    myPrint("trformTranX",       phgp->trformTranY_,       10);
    myPrint("trformTranY",       phgp->trformTranY_,       10);
    myPrint("trformTranZ",       phgp->trformTranZ_,       10);
    myPrint("trformRotXX",       phgp->trformRotXX_,       10);  
    myPrint("trformRotYX",       phgp->trformRotYX_,       10);  
    myPrint("trformRotZX",       phgp->trformRotZX_,       10);  
    myPrint("trformRotXY",       phgp->trformRotXY_,       10);  
    myPrint("trformRotYY",       phgp->trformRotYY_,       10);  
    myPrint("trformRotZY",       phgp->trformRotZY_,       10);  
    myPrint("trformRotXZ",       phgp->trformRotXZ_,       10);  
    myPrint("trformRotYZ",       phgp->trformRotYZ_,       10);  
    myPrint("trformRotZZ",       phgp->trformRotZZ_,       10);  
    myPrint("zLayerHex",         phgp->zLayerHex_,         10);  
    myPrint("rMinLayHex",        phgp->rMinLayHex_,        10);  
    myPrint("rMaxLayHex",        phgp->rMaxLayHex_,        10);  
    myPrint("layer",             phgp->layer_,             18);  
    myPrint("layerIndex",        phgp->layerIndex_,        18);  
    myPrint("depth",             phgp->depth_,             18);
    myPrint("depthIndex",        phgp->depthIndex_,        18);
    myPrint("depthLayerF",       phgp->depthLayerF_,       18);
    myPrint("trformIndex",       phgp->trformIndex_,       10);
    myPrint("levelTop",          phgp->levelT_,            10);
  }

  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

template<typename T> 
void HGCalParameterTester::myPrint(std::string const& s, 
				   std::vector<T> const& obj, int n) {
  int k(0);
  std::cout << s << " with " << obj.size() << " elements\n";
  for (auto const& it : obj) {
    std::cout << it << ", "; ++k; 
    if (k == n) { std::cout << "\n"; k = 0;}
  }
  if (k > 0) std::cout << "\n";
}

void HGCalParameterTester::myPrint(std::string const& s, 
				   HGCalParameters::wafer_map const& obj,
				   int n) {
  int k(0);
  std::cout << s << " with " << obj.size() << " elements\n";
  for (auto const& it : obj) {
    std::cout << it.first << ":" << it.second << ", "; ++k; 
    if (k == n) { std::cout << "\n"; k = 0;}
  }
  if (k > 0) std::cout << "\n";
}

DEFINE_FWK_MODULE(HGCalParameterTester);
