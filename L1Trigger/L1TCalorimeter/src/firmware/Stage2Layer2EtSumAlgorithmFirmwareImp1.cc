///
/// \class l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1
///
/// \author: Jim Brooke
///
/// Description: first iteration of stage 2 jet algo

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "L1Trigger/L1TCalorimeter/interface/Stage2Layer2EtSumAlgorithmFirmware.h"

#include "L1Trigger/L1TCalorimeter/interface/CaloTools.h"

l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::Stage2Layer2EtSumAlgorithmFirmwareImp1(CaloParams* params) :
   params_(params)
{
  etSumEtThresholdHwEt_ = floor(params_->etSumEtThreshold(1)/params_->towerLsbSum());
  etSumEtThresholdHwMet_ = floor(params_->etSumEtThreshold(3)/params_->towerLsbSum());
  
  etSumEtaMinEt_ = params_->etSumEtaMin(1);
  etSumEtaMaxEt_ = params_->etSumEtaMax(1);
  
  etSumEtaMinMet_ = params_->etSumEtaMin(3);
  etSumEtaMaxMet_ = params_->etSumEtaMax(3);
}


l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::~Stage2Layer2EtSumAlgorithmFirmwareImp1() {


}

void l1t::Stage2Layer2EtSumAlgorithmFirmwareImp1::processEvent(const std::vector<l1t::CaloTower> & towers,
      std::vector<l1t::EtSum> & etsums) {

  double pi = std::atan(1.d) * 4.0d;

  int ietaMax=28, ietaMin=-28, iphiMax=72, iphiMin=1;
  
  int32_t ex(0), ey(0), et(0);

  for (int ieta=ietaMin; ieta<ietaMax; ieta++) {

    int32_t ringEx(0), ringEy(0), ringEt(0);

    for (int iphi=iphiMin; iphi<iphiMax; iphi++) {
      
      l1t::CaloTower tower = l1t::CaloTools::getTower(towers, ieta, iphi);
      double towPhi = l1t::CaloTools::towerPhi(ieta, iphi);

      int32_t towEx = (tower.hwPt() * (int32_t) (511.*std::cos(towPhi))) >> 9;
      int32_t towEy = (tower.hwPt() * (int32_t) (511.*std::sin(towPhi))) >> 9;
      int32_t towEt = tower.hwPt();

      ringEx += towEx;
      ringEy += towEy;
      ringEt += towEt;
      
    }

    ringEx >>= 2;
    ringEy >>= 2;
    ringEt >>= 1;

    ex += ringEx;
    ey += ringEy;
    et += ringEt;
    
  }
 
  // final MET calculation
  int32_t met = (int32_t) std::sqrt(std::pow( (double) ex,2) + std::pow( (double) ey,2));

  double metPhiRadians = std::atan2( (double) ey, (double) ex ) + pi;

  int32_t metPhi = (int32_t) metPhiRadians / (2 * pi);

 
  // apply output bitwidth constraints
  met    &= 0xfff;
  metPhi &= 0xfff;
  et     &= 0xfff;
 
  // push output
  l1t::EtSum::EtSumType typeTotalEt = l1t::EtSum::EtSumType::kTotalEt;
  l1t::EtSum::EtSumType typeMissingEt = l1t::EtSum::EtSumType::kMissingEt;
  
  math::XYZTLorentzVector p4;

  l1t::EtSum etSumTotalEt(p4,typeTotalEt,et,0,0,0);
  l1t::EtSum etSumMissingEt(p4,typeMissingEt,met,0,metPhi,0);
  
  etsums.push_back(etSumTotalEt);
  etsums.push_back(etSumMissingEt);
  
}

