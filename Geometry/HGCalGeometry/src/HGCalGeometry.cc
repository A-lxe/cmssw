/* for High Granularity Calorimeter
 * This geometry is essentially driven by topology, 
 * which is thus encapsulated in this class. 
 * This makes this geometry not suitable to be loaded
 * by regular CaloGeometryLoader<T>
 */
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGenericDetId.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <cmath>

#include <Math/Transform3D.h>
#include <Math/EulerAngles.h>

typedef CaloCellGeometry::Tr3D Tr3D;
typedef std::vector<float> ParmVec;

//#define EDM_ML_DEBUG

HGCalGeometry::HGCalGeometry( const HGCalTopology& topology_ )
  : m_topology( topology_ ),
    m_cellVec( topology_.totalGeomModules()),
    m_validGeomIds( topology_.totalGeomModules()),
    m_halfType( topology_.detectorType()),
    m_subdet( topology_.subDetector()),
    twoBysqrt3_(2.0/std::sqrt(3.0)) {
  
  m_validIds.reserve( topology().totalModules());
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "Expected total # of Geometry Modules " 
				<< topology().totalGeomModules();
#endif
}

HGCalGeometry::~HGCalGeometry() { }

void HGCalGeometry::fillNamedParams (DDFilteredView fv) {}

void HGCalGeometry::initializeParms() {
}

void HGCalGeometry::localCorners(Pt3DVec&        lc,
				 const CCGFloat* pv,
				 unsigned int    i,
				 Pt3D&           ref) {
  FlatHexagon::localCorners( lc, pv, ref ) ;
}

void HGCalGeometry::newCell( const GlobalPoint& f1 ,
			     const GlobalPoint& f2 ,
			     const GlobalPoint& f3 ,
			     const CCGFloat*    parm ,
			     const DetId&       detId ) {

  DetId geomId;
  int   cells;
  HGCalTopology::DecodedDetId id = topology().decode(detId);
  if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) || 
      (topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
    geomId = (DetId)(HGCalDetId(detId).geometryCell());
    cells = topology().dddConstants().numberCellsHexagon(id.iSec);
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "NewCell " << HGCalDetId(detId) 
				  << " GEOM " << HGCalDetId(geomId);
#endif
  } else {
    return;
  }
  const uint32_t cellIndex (topology().detId2denseGeomId(detId));

  m_cellVec.at( cellIndex ) = FlatHexagon( cornersMgr(), f1, f2, f3, parm ) ;
  m_validGeomIds.at( cellIndex ) = geomId ;

#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "Store for DetId " << std::hex 
				<< detId.rawId() << " GeomId " 
				<< geomId.rawId() << std::dec << " Index " 
				<< cellIndex << " cells " << cells;
  unsigned int nOld = m_validIds.size();
#endif
  for (int cell = 0; cell < cells; ++cell) {
    id.iCell  = cell;
    DetId idc = topology().encode(id);
    if (topology().valid(idc)) {
      m_validIds.emplace_back(idc);
      if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Square) &&
	  (!m_halfType)) {
	id.iSubSec = -id.iSubSec;
	m_validIds.emplace_back( topology().encode(id));
	id.iSubSec = -id.iSubSec;
      }
    }
  }

#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "HGCalGeometry::newCell-> [" << cellIndex 
				<< "]" << " front:" << f1.x() << '/' << f1.y()
				<< '/' << f1.z() << " back:" <<  f2.x() << '/'
				<< f2.y() << '/' << f2.z() << " eta|phi " 
				<< m_cellVec[cellIndex].etaPos() << ":"
				<< m_cellVec[cellIndex].phiPos();
  unsigned int nNew = m_validIds.size();
  if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) || 
      (topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
    edm::LogVerbatim("HGCalGeom") << "ID: " << HGCalDetId(detId) 
				  << " with valid DetId from " << nOld 
				  << " to " << nNew;
  } else {
    edm::LogVerbatim("HGCalGeom") << "ID: " << std::hex << detId.rawId() 
				  << std::dec << " with valid DetId from " 
				  << nOld << " to " << nNew;
  }
  edm::LogVerbatim("HGCalGeom") << "Cell[" << cellIndex << "] " << std::hex 
				<< geomId.rawId() << ":"  
				<< m_validGeomIds[cellIndex].rawId() 
				<< std::dec;
#endif
}

std::shared_ptr<const CaloCellGeometry> HGCalGeometry::getGeometry(const DetId& id) const {
  if (id == DetId()) return nullptr; // nothing to get
  DetId geoId;
  if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) || 
      (topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
    geoId = (DetId)(HGCalDetId(id).geometryCell());
  } else {
    return nullptr;
  }
  const uint32_t cellIndex (topology().detId2denseGeomId(geoId));
  const GlobalPoint pos = (id != geoId) ? getPosition(id) : GlobalPoint();
  return cellGeomPtr (cellIndex, pos);

}

bool HGCalGeometry::present(const DetId& id) const {
  if (id == DetId()) return false;
  DetId geoId;
  if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) || 
      (topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
    geoId = (DetId)(HGCalDetId(id).geometryCell());
  } else {
    return false;
  }
  const uint32_t index (topology().detId2denseGeomId(geoId));
  return (nullptr != getGeometryRawPtr(index)) ;
}

GlobalPoint HGCalGeometry::getPosition(const DetId& id) const {

  unsigned int cellIndex =  indexFor(id);
  GlobalPoint glob;
  if (cellIndex <  m_cellVec.size()) {
    HGCalTopology::DecodedDetId id_ = topology().decode(id);
    std::pair<float,float> xy;
    if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) ||
	(topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
      xy = topology().dddConstants().locateCellHex(id_.iCell,id_.iSec,true);
    } else {
      xy = std::pair<float,float>(0,0);
    }
    const HepGeom::Point3D<float> lcoord(xy.first,xy.second,0);
    glob = m_cellVec[cellIndex].getPosition(lcoord);
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "getPosition:: index " << cellIndex 
				  << " Local " << lcoord.x() << ":" 
				  << lcoord.y() << " ID " << id_.iCell << ":"
				  << id_.iLay << " Global " << glob;
#endif
  } 
  return glob;
}

HGCalGeometry::CornersVec HGCalGeometry::getCorners(const DetId& id) const {

  HGCalGeometry::CornersVec co (FlatHexagon::ncorner_, GlobalPoint(0,0,0));
  unsigned int cellIndex =  indexFor(id);
  if (cellIndex <  m_cellVec.size()) {
    HGCalTopology::DecodedDetId id_ = topology().decode(id);
    std::pair<float,float> xy;
    if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) ||
	(topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
      xy = topology().dddConstants().locateCellHex(id_.iCell,id_.iSec,true);
    } else {
      xy = std::pair<float,float>(0,0);
    }
    float dz = m_cellVec[cellIndex].param()[0];
    float dx = m_cellVec[cellIndex].param()[1];
    float dy = m_cellVec[cellIndex].param()[2];
    static const int signx[] = {0,-1,-1,0,1,1,0,-1,-1,0,1,1};
    static const int signy[] = {-2,-1,1,2,1,-1,-2,-1,1,2,1,-1};
    static const int signz[] = {-1,-1,-1,-1,-1,-1,1,1,1,1,1,1};
    /*
    float dx = 0.5*m_cellVec[cellIndex].param()[11];
    float dy(dx);
    static const int signx[] = {-1,-1,1,1,-1,-1,1,1};
    static const int signy[] = {-1,1,1,-1,-1,1,1,-1};
    static const int signz[] = {-1,-1,-1,-1,1,1,1,1};
    */
    for (unsigned int i = 0; i != FlatHexagon::ncorner_; ++i) {
      const HepGeom::Point3D<float> lcoord(xy.first+signx[i]*dx,xy.second+signy[i]*dy,signz[i]*dz);
      co[i] = m_cellVec[cellIndex].getPosition(lcoord);
    }
  }
  return co;
}

DetId HGCalGeometry::getClosestCell(const GlobalPoint& r) const {
  unsigned int cellIndex = getClosestCellIndex(r);
  if (cellIndex < m_cellVec.size()) {
    HGCalTopology::DecodedDetId id_ = topology().decode(m_validGeomIds[cellIndex]);
    HepGeom::Point3D<float> local;
    if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) ||
	(topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
      if (r.z() > 0) local = HepGeom::Point3D<float>(r.x(),r.y(),0);
      else           local = HepGeom::Point3D<float>(-r.x(),r.y(),0);
      std::pair<int,int> kxy = 
	topology().dddConstants().assignCell(local.x(),local.y(),id_.iLay,
					     id_.iSubSec,true);
      id_.iCell   = kxy.second;
      id_.iSec    = kxy.first;
      id_.iSubSec = topology().dddConstants().waferTypeT(kxy.first);
      if (id_.iSubSec != 1) id_.iSubSec = -1;
#ifdef EDM_ML_DEBUG
      edm::LogVerbatim("HGCalGeom") << "getClosestCell: local " << local 
				    << " Id " << id_.zside << ":" << id_.iLay 
				    << ":" << id_.iSec << ":" << id_.iSubSec
				    << ":" << id_.iCell;
#endif

      //check if returned cell is valid
      if (id_.iCell>=0) return topology().encode(id_);
    }
  }

  //if not valid or out of bounds return a null DetId
  return DetId();
}

HGCalGeometry::DetIdSet HGCalGeometry::getCells(const GlobalPoint& r, double dR)  const {
   HGCalGeometry::DetIdSet dss;
   return dss;
}

std::string HGCalGeometry::cellElement() const {
  if      (m_subdet == HGCEE)  return "HGCalEE";
  else if (m_subdet == HGCHEF) return "HGCalHEFront";
  else if (m_subdet == HGCHEB) return "HGCalHEBack";
  else                         return "Unknown";
}

unsigned int HGCalGeometry::indexFor(const DetId& id) const {
  unsigned int cellIndex =  m_cellVec.size();
  if (id != DetId()) {
    DetId geoId;
    if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) ||
	(topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
      geoId = (DetId)(HGCalDetId(id).geometryCell());
      cellIndex = topology().detId2denseGeomId(geoId);
#ifdef EDM_ML_DEBUG
      edm::LogVerbatim("HGCalGeom") << "indexFor " << std::hex << id.rawId() 
				    << ":" << geoId.rawId() << std::dec 
				    << " index " << cellIndex;
#endif
    }
  }
  return cellIndex;
}

unsigned int HGCalGeometry::sizeForDenseIndex() const {
  return topology().totalGeomModules();
}

const CaloCellGeometry* HGCalGeometry::getGeometryRawPtr(uint32_t index) const {
  // Modify the RawPtr class
  const CaloCellGeometry* cell(&m_cellVec[index]);
  return (m_cellVec.size() < index ||
	  nullptr == cell->param() ? nullptr : cell);
}

std::shared_ptr<const CaloCellGeometry> HGCalGeometry::cellGeomPtr(uint32_t index) const {
  if ((index >= m_cellVec.size()) || (m_validGeomIds[index].rawId() == 0)) 
    return nullptr;
  static const auto do_not_delete = [](const void*){};
  auto cell = std::shared_ptr<const CaloCellGeometry>(&m_cellVec[index],do_not_delete);
  if (nullptr == cell->param()) return nullptr;
  return cell;
}

std::shared_ptr<const CaloCellGeometry> HGCalGeometry::cellGeomPtr(uint32_t index, const GlobalPoint& pos) const {
  if ((index >= m_cellVec.size()) || (m_validGeomIds[index].rawId() == 0)) 
    return nullptr;
  if (pos == GlobalPoint()) return cellGeomPtr(index);
  auto cell = std::make_shared<FlatHexagon>(m_cellVec[index]);
  cell->setPosition(pos);
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "cellGeomPtr " << index << ":" << cell;
#endif
  if (nullptr == cell->param()) return nullptr;
  return cell;
}

void HGCalGeometry::addValidID(const DetId& id) {
  edm::LogError("HGCalGeom") << "HGCalGeometry::addValidID is not implemented";
}


unsigned int HGCalGeometry::getClosestCellIndex (const GlobalPoint& r) const {

  float phip = r.phi();
  float zp   = r.z();
  unsigned int cellIndex =  m_cellVec.size();
  float dzmin(9999), dphimin(9999), dphi10(0.175);
  for (unsigned int k=0; k<m_cellVec.size(); ++k) {
    float dphi = phip-m_cellVec[k].phiPos();
    while (dphi >   M_PI) dphi -= 2*M_PI;
    while (dphi <= -M_PI) dphi += 2*M_PI;
    if (fabs(dphi) < dphi10) {
      float dz = fabs(zp - m_cellVec[k].getPosition().z());
      if (dz < (dzmin+0.001)) {
	dzmin     = dz;
	if (fabs(dphi) < (dphimin+0.01)) {
	  cellIndex = k;
	  dphimin   = fabs(dphi);
	} else {
	  if (cellIndex >= m_cellVec.size()) cellIndex = k;
	}
      }
    }
  }
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "getClosestCellIndex::Input " << zp << ":" 
				<< phip << " Index " << cellIndex;
  if (cellIndex < m_cellVec.size()) 
    edm::LogVerbatim("HGCalGeom") << " Cell z " 
				  << m_cellVec[cellIndex].getPosition().z() 
				  << ":" << dzmin << " phi " 
				  << m_cellVec[cellIndex].phiPos() << ":"
				  << dphimin;
#endif
  return cellIndex;
}

// FIXME: Change sorting algorithm if needed
namespace {
  struct rawIdSort {
    bool operator()( const DetId& a, const DetId& b ) {
      return( a.rawId() < b.rawId());
    }
  };
}

void HGCalGeometry::sortDetIds( void ) {
  m_validIds.shrink_to_fit();
  std::sort( m_validIds.begin(), m_validIds.end(), rawIdSort());
}

void HGCalGeometry::getSummary(CaloSubdetectorGeometry::TrVec&  trVector,
			       CaloSubdetectorGeometry::IVec&   iVector,
			       CaloSubdetectorGeometry::DimVec& dimVector,
			       CaloSubdetectorGeometry::IVec& dinsVector ) const {

  unsigned int numberOfCells = topology().totalGeomModules(); // total Geom Modules both sides
  unsigned int numberOfShapes = HGCalGeometry::k_NumberOfShapes;
  unsigned int numberOfParametersPerShape = HGCalGeometry::k_NumberOfParametersPerShape;

  trVector.reserve( numberOfCells * numberOfTransformParms());
  iVector.reserve( numberOfCells );
  dimVector.reserve( numberOfShapes * numberOfParametersPerShape );
  dinsVector.reserve( numberOfCells );
  
  if ((topology().dddConstants().geomMode() == HGCalGeometryMode::Hexagon) ||
      (topology().dddConstants().geomMode() == HGCalGeometryMode::HexagonFull)) {
    for (unsigned itr=0; itr<topology().dddConstants().getTrFormN(); ++itr) {
      HGCalParameters::hgtrform mytr = topology().dddConstants().getTrForm(itr);
      int layer  = mytr.lay;
      for (int wafer=0; wafer<topology().dddConstants().sectors(); ++wafer) {
        if (topology().dddConstants().waferInLayer(wafer,layer,true)) {
	  HGCalParameters::hgtrap vol = topology().dddConstants().getModule(wafer, true, true);
	  ParmVec params( HGCalGeometry::k_NumberOfParametersPerShape, 0 );
	  params[0] = vol.dz;
	  params[1] = vol.cellSize;
	  params[2] = twoBysqrt3_*params[1];
	  /*
	  params[1] = params[2] = 0;
	  params[3] = params[7] = vol.h;
	  params[4] = params[8] = vol.bl;
	  params[5] = params[9] = vol.tl;
	  params[6] = params[10]= vol.alpha;
	  params[11]= vol.cellSize;
	  */
	  dimVector.insert( dimVector.end(), params.begin(), params.end());
	}
      }
    }
  }
  
  for (unsigned int i( 0 ); i < numberOfCells; ++i) {
    DetId detId = m_validGeomIds[i];
    int layer   = HGCalDetId(detId).layer();
    dinsVector.emplace_back( topology().detId2denseGeomId( detId ));
    iVector.emplace_back( layer );
    
    Tr3D tr;
    auto ptr =  cellGeomPtr( i );
    if ( nullptr != ptr ) {
      ptr->getTransform( tr, ( Pt3DVec* ) nullptr );

      if( Tr3D() == tr ) { // there is no rotation
	const GlobalPoint& gp( ptr->getPosition()); 
	tr = HepGeom::Translate3D( gp.x(), gp.y(), gp.z());
      }

      const CLHEP::Hep3Vector tt( tr.getTranslation());
      trVector.emplace_back( tt.x());
      trVector.emplace_back( tt.y());
      trVector.emplace_back( tt.z());
      if (6 == numberOfTransformParms()) {
	const CLHEP::HepRotation rr( tr.getRotation());
	const ROOT::Math::Transform3D rtr( rr.xx(), rr.xy(), rr.xz(), tt.x(),
					   rr.yx(), rr.yy(), rr.yz(), tt.y(),
					   rr.zx(), rr.zy(), rr.zz(), tt.z());
	ROOT::Math::EulerAngles ea;
	rtr.GetRotation( ea );
	trVector.emplace_back( ea.Phi());
	trVector.emplace_back( ea.Theta());
	trVector.emplace_back( ea.Psi());
      }
    }
  }
}

#include "FWCore/Utilities/interface/typelookup.h"

TYPELOOKUP_DATA_REG(HGCalGeometry);
