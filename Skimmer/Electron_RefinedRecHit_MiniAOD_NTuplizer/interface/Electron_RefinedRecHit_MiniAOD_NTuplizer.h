#ifndef Electron_RefinedRecHit_MiniAOD_NTuplizer_h
#define Electron_RefinedRecHit_MiniAOD_NTuplizer_h

// -*- C++ -*-
//
// Package:    Electron_GNN_Regression/Electron_RefinedRecHit_MiniAOD_NTuplizer
// Class:      Electron_RefinedRecHit_MiniAOD_NTuplizer
//
/**\class Electron_RefinedRecHit_MiniAOD_NTuplizer Electron_RefinedRecHit_NTuplizer.cc Electron_GNN_Regression/Electron_RefinedRecHit_NTuplizer/plugins/Electron_RefinedRecHit_NTuplizer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Rajdeep Mohan Chatterjee
// Modified by    :  Bhargav Madhusudan Joshi     
//         Created:  Tue, 20 Sep 2022 11:54:35 GMT
//
//

// system include files
#include <memory>
#include <iostream>
#include "TTree.h"


// utilities
#include "Math/VectorUtil.h"
#include "TFile.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
// #include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/Records/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"

#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/VIDCutFlowResult.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"



//
//
// class declaration
//

using namespace std;
using namespace edm;
using namespace pat;
using namespace reco;

class Electron_RefinedRecHit_MiniAOD_NTuplizer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit Electron_RefinedRecHit_MiniAOD_NTuplizer(const edm::ParameterSet&);
      ~Electron_RefinedRecHit_MiniAOD_NTuplizer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

      std::vector<float> Hit_ES_Eta[2];
      std::vector<float> Hit_ES_Phi[2];
      std::vector<float> Hit_ES_X[2];
      std::vector<float> Hit_ES_Y[2];
      std::vector<float> Hit_ES_Z[2];
      std::vector<float> ES_RecHitEn[2];


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      bool DEBUG = false;

      //   cluster tools
      EcalClusterLazyTools *clustertools;
      noZS::EcalClusterLazyTools *clustertools_NoZS;
      edm::ESHandle<EcalPedestals> _ped;

      //   Identify if the SC lies in EB OR EE based on its seed
      bool isEB = 0;
      bool isEE = 0; // !isEB not sufficient since later will try to include the preshower as well


      //     bool GetGenMatchType(const reco::Eleton& Eleton, const reco::GenParticle& GenColl, int pdgId, double dRThresh);
      // Get the hits from the ES
      //     std::vector<GlobalPoint> GetESPlaneRecHits(const reco::SuperCluster& sc, unsigned int planeIndex) const;
      void GetESPlaneRecHits(const reco::SuperCluster& sc, const CaloGeometry& theGeometry, unsigned int elenum, unsigned int planeIndex);

      //   clear the vectors 
      void ClearTreeVectors();

      // ----------member data ---------------------------
      bool isMC_, miniAODRun_, refinedCluster_;
      double egammaDeltaRMatch_;

      TTree* T;

      // Variables for Run info.
      int run;
      int event;
      int lumi;

      bool isRefinedSC;

      // Electron variables
      int nElectrons_;
      Float_t rho;
      std::vector<float> iEta[2];
      std::vector<float> iPhi[2];
      std::vector<float> Hit_Eta[2];
      std::vector<float> Hit_Phi[2];
      std::vector<float> Hit_X[2];
      std::vector<float> Hit_Y[2];
      std::vector<float> Hit_Z[2];


      std::vector<float> RecHitFrac[2];
      std::vector<float> RecHitEn[2];
      std::vector<int>   RecHitGain[2];
      std::vector<bool>  RecHitQuality[2];
      std::vector<float> HitNoise[2];

      // individual flags
      std::vector<bool> RecHitFlag_kGood[2];                   // channel ok, the energy and time measurement are reliable
      std::vector<bool> RecHitFlag_kPoorReco[2];                 // the energy is available from the UncalibRecHit, but approximate (bad shape, large chi2)
      std::vector<bool> RecHitFlag_kOutOfTime[2];                // the energy is available from the UncalibRecHit (sync reco), but the event is out of time
      std::vector<bool> RecHitFlag_kFaultyHardware[2];           // The energy is available from the UncalibRecHit, channel is faulty at some hardware level (e.g. noisy)
      std::vector<bool> RecHitFlag_kNoisy[2];                    // the channel is very noisy
      std::vector<bool> RecHitFlag_kPoorCalib[2];                // the energy is available from the UncalibRecHit, but the calibration of the channel is poor
      std::vector<bool> RecHitFlag_kSaturated[2];                // saturated channel (recovery not tried)
      std::vector<bool> RecHitFlag_kLeadingEdgeRecovered[2];     // saturated channel: energy estimated from the leading edge before saturation
      std::vector<bool> RecHitFlag_kNeighboursRecovered[2];      // saturated/isolated dead: energy estimated from neighbours
      std::vector<bool> RecHitFlag_kTowerRecovered[2];           // channel in TT with no data link, info retrieved from Trigger Primitive
      std::vector<bool> RecHitFlag_kDead[2];                     // channel is dead and any recovery fails
      std::vector<bool> RecHitFlag_kKilled[2];                   // MC only flag: the channel is killed in the real detector
      std::vector<bool> RecHitFlag_kTPSaturated[2];              // the channel is in a region with saturated TP
      std::vector<bool> RecHitFlag_kL1SpikeFlag[2];              // the channel is in a region with TP with sFGVB = 0
      std::vector<bool> RecHitFlag_kWeird[2];                    // the signal is believed to originate from an anomalous deposit (spike) 
      std::vector<bool> RecHitFlag_kDiWeird[2];                  // the signal is anomalous, and neighbors another anomalous signal  
      std::vector<bool> RecHitFlag_kHasSwitchToGain6[2];         // at least one data frame is in G6
      std::vector<bool> RecHitFlag_kHasSwitchToGain1[2];         // at least one data frame is in G1

      // individual ES flags
      std::vector<bool> RecHitFlag_kESGood[2];
      std::vector<bool> RecHitFlag_kESDead[2];
      std::vector<bool> RecHitFlag_kESHot[2];
      std::vector<bool> RecHitFlag_kESPassBX[2];
      std::vector<bool> RecHitFlag_kESTwoGoodRatios[2];
      std::vector<bool> RecHitFlag_kESBadRatioFor12[2];
      std::vector<bool> RecHitFlag_kESBadRatioFor23Upper[2];
      std::vector<bool> RecHitFlag_kESBadRatioFor23Lower[2];
      std::vector<bool> RecHitFlag_kESTS1Largest[2];
      std::vector<bool> RecHitFlag_kESTS3Largest[2];
      std::vector<bool> RecHitFlag_kESTS3Negative[2];
      std::vector<bool> RecHitFlag_kESSaturated[2];
      std::vector<bool> RecHitFlag_kESTS2Saturated[2];
      std::vector<bool> RecHitFlag_kESTS3Saturated[2];
      std::vector<bool> RecHitFlag_kESTS13Sigmas[2];
      std::vector<bool> RecHitFlag_kESTS15Sigmas[2];

      std::vector<bool>* RecHitFlag_container[18] = {
         RecHitFlag_kGood,
         RecHitFlag_kPoorReco,
         RecHitFlag_kOutOfTime,
         RecHitFlag_kFaultyHardware,
         RecHitFlag_kNoisy,
         RecHitFlag_kPoorCalib,
         RecHitFlag_kSaturated,
         RecHitFlag_kLeadingEdgeRecovered,
         RecHitFlag_kNeighboursRecovered,
         RecHitFlag_kTowerRecovered,
         RecHitFlag_kDead,
         RecHitFlag_kKilled,
         RecHitFlag_kTPSaturated,
         RecHitFlag_kL1SpikeFlag,
         RecHitFlag_kWeird,
         RecHitFlag_kDiWeird,
         RecHitFlag_kHasSwitchToGain6,
         RecHitFlag_kHasSwitchToGain1
      };

      std::vector<bool>* RecHitESFlag_container[16] = {
         RecHitFlag_kESGood,
         RecHitFlag_kESDead,
         RecHitFlag_kESHot,
         RecHitFlag_kESPassBX,
         RecHitFlag_kESTwoGoodRatios,
         RecHitFlag_kESBadRatioFor12,
         RecHitFlag_kESBadRatioFor23Upper,
         RecHitFlag_kESBadRatioFor23Lower,
         RecHitFlag_kESTS1Largest,
         RecHitFlag_kESTS3Largest,
         RecHitFlag_kESTS3Negative,
         RecHitFlag_kESSaturated,
         RecHitFlag_kESTS2Saturated,
         RecHitFlag_kESTS3Saturated,
         RecHitFlag_kESTS13Sigmas,
         RecHitFlag_kESTS15Sigmas
      };

      std::vector<float> Ele_pt_;
      std::vector<float> Ele_eta_;
      std::vector<float> Ele_phi_;
      std::vector<float> Ele_energy_;
      std::vector<float> Ele_correctedEcalEnergy_;
      std::vector<float> Ele_correctedEcalEnergy_error_;
      std::vector<float> Ele_ecal_mustache_energy_;
      std::vector<float> Ele_energy_ECAL_pho;
      std::vector<float> Ele_energyUncertainty_ECAL_pho;

      std::vector<float> Ele_R9;
      std::vector<float> Ele_S4;
      std::vector<float> Ele_SigIEIE;
      std::vector<float> Ele_SigIPhiIPhi;
      std::vector<float> Ele_SCEtaW;
      std::vector<float> Ele_SCPhiW;
      std::vector<float> Ele_CovIEtaIEta;
      std::vector<float> Ele_CovIEtaIPhi;
      std::vector<float> Ele_ESSigRR;
      std::vector<float> Ele_SCRawE;
      std::vector<float> Ele_SC_ESEnByRawE;
      std::vector<float> Ele_HadOverEm;

      // Electron Isolation Variables
      vector<float> Ele_sumChargedHadronPt;
      vector<float> Ele_sumChargedParticlePt;
      vector<float> Ele_sumEcalClusterEt;
      vector<float> Ele_sumHcalClusterEt;
      vector<float> Ele_sumNeutralHadronEt;
      vector<float> Ele_sumPhotonEt;
      vector<float> Ele_sumPUPt;
      vector<float>  Ele_EcalPFClusterIso;
      vector<float>  Ele_HcalPFClusterIso;

      std::vector<float> Ele_Gen_Pt;
      std::vector<float> Ele_Gen_Eta;
      std::vector<float> Ele_Gen_Phi;
      std::vector<float> Ele_Gen_E;

      std::vector<int> passLooseId_;
      std::vector<int> passMediumId_;
      std::vector<int> passTightId_;
      std::vector<int> passMVAMediumId_;

      std::vector<int> isTrue_;

      // -----------------Handles--------------------------
      edm::Handle<double> rhoHandle;
      edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > EBRechitsHandle;
      edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > EERechitsHandle;
      edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > ESRechitsHandle;
      //edm::Handle<EcalRecHitCollection> EBRechitsHandle;
      //edm::Handle<EcalRecHitCollection> EERechitsHandle;
      //edm::Handle<EcalRecHitCollection> ESRechitsHandle;
      edm::Handle<std::vector<pat::Electron> > electrons;
      edm::Handle<std::vector<pat::Photon> > photons;
      edm::Handle<std::vector<pat::PackedGenParticle> > genParticles;
      edm::Handle<edm::ValueMap<bool> > loose_id_decisions;
      edm::Handle<edm::ValueMap<bool> > medium_id_decisions;
      edm::Handle<edm::ValueMap<bool> > tight_id_decisions;
      //---------------- Input Tags-----------------------
      edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > recHitCollectionEBToken_;
      edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > recHitCollectionEEToken_;
      edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > recHitCollectionESToken_;
      edm::EDGetTokenT<double> rhoToken_;
      //edm::EDGetTokenT<EcalRecHitCollection> recHitCollectionEBToken_;
      //edm::EDGetTokenT<EcalRecHitCollection> recHitCollectionEEToken_;
      //edm::EDGetTokenT<EcalRecHitCollection> recHitCollectionESToken_;
      edm::EDGetToken electronsToken_;
      edm::EDGetToken photonsToken_;
      edm::EDGetTokenT<std::vector<pat::PackedGenParticle> > genParticlesToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> > eleLooseIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> > eleMediumIdMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> > eleTightIdMapToken_;
      edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeomToken_;
      edm::ESGetToken<EcalPedestals, EcalPedestalsRcd> pedestalsToken_;

};

#endif