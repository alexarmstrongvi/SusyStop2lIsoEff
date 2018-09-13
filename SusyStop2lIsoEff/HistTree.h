#include <TTree.h>
#include <vector>

using std::vector;

class HistTree : public TTree
{
public:
  // Constructor
  HistTree(const char* name, const char* title)
    : TTree(name, title) {}

  //////////////////////////////////////////////////////////////////////////////
  // Define variables for branches
  unsigned int runNumber = 0; ///< Run number for the current event
  unsigned long long eventNumber = 0; ///< Event number
  float eventweight = 1; ///< MC Event Weight

  int nDenJets = -1;
  int nDenEle = -1;
  int nDenMu = -1;
  int nDenLep = -1;
  int nDenRealEl = -1;
  int nDenFakeEl = -1;
  int nDenRealMu = -1;
  int nDenFakeMu = -1;

  int nDenPassORJets = -1;
  int nDenPassOREle = -1;
  int nDenPassORMu = -1;
  int nDenPassORLep = -1;
  int nDenPassORRealEl = -1;
  int nDenPassORFakeEl = -1;
  int nDenPassORRealMu = -1;
  int nDenPassORFakeMu = -1;

  /// Jet 4-momentum variables
  vector<float> *JetEta = nullptr;
  vector<float> *JetPhi = nullptr;
  vector<float> *JetPt = nullptr;
  vector<float> *JetE = nullptr;
  vector<bool> *JetPassOREle = nullptr;
  vector<bool> *JetPassORPtRatio = nullptr;
  vector<bool> *JetPassORDeltaRy = nullptr;
  vector<bool> *JetPassORGhostMatched = nullptr;
  vector<bool> *JetPassOR = nullptr;
  vector<bool> *JetPassSignal = nullptr;
  vector<bool> *JetBTagged = nullptr;
  vector<int> *JetTruth = nullptr;

  /// Lepton 4-momentum variables
  vector<float> *LepEta = nullptr;
  vector<float> *LepPhi = nullptr;
  vector<float> *LepPt = nullptr;
  vector<float> *LepE = nullptr;
  vector<bool> *LepIsEle = nullptr;
  vector<bool> *LepPassEMuOR = nullptr;
  vector<bool> *LepPassOR = nullptr;
  vector<bool> *LepPassSignal = nullptr;
  vector<int> *LepTruthClass = nullptr;

  /// dR
  vector<float> *dR_LepClosestJet = nullptr;
  vector<float> *dR_LepClosestJetPassingOR = nullptr;
  vector<int> *dR_ClosestJetIdx = nullptr;
  vector<float> *dR_JetClosestLep = nullptr;
  vector<int> *dR_ClosestLepIdx = nullptr;


  //////////////////////////////////////////////////////////////////////////////
  // Assignment operator (if needed)
  HistTree& operator=(const HistTree& rhs) {
    this->runNumber = rhs.runNumber;
    this->eventNumber = rhs.eventNumber;
    this->eventweight = rhs.eventweight;
    this->nDenJets = rhs.nDenJets;
    this->nDenEle = rhs.nDenEle;
    this->nDenMu = rhs.nDenMu;
    this->nDenLep = rhs.nDenLep;
    this->nDenTruthLep = rhs.nDenTruthLep;
    this->nDenPassORJets = rhs.nDenPassORJets;
    this->nDenPassOREle = rhs.nDenPassOREle;
    this->nDenPassORMu = rhs.nDenPassORMu;
    this->nDenPassORLep = rhs.nDenPassORLep;
    this->JetEta = rhs.JetEta;
    this->JetPhi = rhs.JetPhi;
    this->JetPt = rhs.JetPt;
    this->JetE = rhs.JetE;
    this->JetPassOREle = rhs.JetPassOREle;
    this->JetPassORPtRatio = rhs.JetPassORPtRatio;
    this->JetPassORDeltaRy = rhs.JetPassORDeltaRy;
    this->JetPassORGhostMatched = rhs.JetPassORGhostMatched;
    this->JetPassOR = rhs.JetPassOR;
    this->JetPassSignal = rhs.JetPassSignal;
    this->JetBTagged = rhs.JetBTagged;
    this->JetTruth = rhs.JetTruth;
    this->LepEta = rhs.LepEta;
    this->LepPhi = rhs.LepPhi;
    this->LepPt = rhs.LepPt;
    this->LepE = rhs.LepE;
    this->LepIsEle = rhs.LepIsEle;
    this->LepPassEMuOR = rhs.LepPassEMuOR;
    this->LepPassOR = rhs.LepPassOR;
    this->LepPassSignal = rhs.LepPassSignal;
    this->LepTruthClass = rhs.LepTruthClass;
    this->dR_LepClosestJet = rhs.dR_LepClosestJet;
    this->dR_LepClosestJetPassingOR = rhs.dR_LepClosestJetPassingOR;
    this->dR_ClosestJetIdx = rhs.dR_ClosestJetIdx;
    this->dR_JetClosestLep = rhs.dR_JetClosestLep;
    this->dR_ClosestLepIdx = rhs.dR_ClosestLepIdx;
    return *this;
  }

  //////////////////////////////////////////////////////////////////////////////
  // Attach variables to branches
  void initialize() {
    this->Branch ("runNumber", &runNumber);
    this->Branch ("eventNumber", &eventNumber);
    this->Branch ("eventweight", &eventweight);
    this->Branch ("nDenJets", &nDenJets);
    this->Branch ("nDenEle", &nDenEle);
    this->Branch ("nDenMu", &nDenMu);
    this->Branch ("nDenLep", &nDenLep);
    this->Branch ("nDenTruthLep", &nDenTruthLep);
    this->Branch ("nDenPassORJets", &nDenPassORJets);
    this->Branch ("nDenPassOREle", &nDenPassOREle);
    this->Branch ("nDenPassORMu", &nDenPassORMu);
    this->Branch ("nDenPassORLep", &nDenPassORLep);

    JetEta = new vector<float>();
    this->Branch ("JetEta", &JetEta);
    JetPhi = new vector<float>();
    this->Branch ("JetPhi", &JetPhi);
    JetPt = new vector<float>();
    this->Branch ("JetPt", &JetPt);
    JetE = new vector<float>();
    this->Branch ("JetE", &JetE);
    JetPassOREle = new vector<bool>();
    this->Branch ("JetPassOREle", &JetPassOREle);
    JetPassORPtRatio = new vector<bool>();
    this->Branch ("JetPassORPtRatio", &JetPassORPtRatio);
    JetPassORDeltaRy = new vector<bool>();
    this->Branch ("JetPassORDeltaRy", &JetPassORDeltaRy);
    JetPassORGhostMatched = new vector<bool>();
    this->Branch ("JetPassORGhostMatched", &JetPassORGhostMatched);
    JetPassOR = new vector<bool>();
    this->Branch ("JetPassOR", &JetPassOR);
    JetPassSignal = new vector<bool>();
    this->Branch ("JetPassSignal", &JetPassSignal);
    JetBTagged = new vector<bool>();
    this->Branch ("JetBTagged", &JetBTagged);
    JetTruth = new vector<int>();
    this->Branch ("JetTruth", &JetTruth);
    LepEta = new vector<float>();
    this->Branch ("LepEta", &LepEta);
    LepPhi = new vector<float>();
    this->Branch ("LepPhi", &LepPhi);
    LepPt = new vector<float>();
    this->Branch ("LepPt", &LepPt);
    LepE = new vector<float>();
    this->Branch ("LepE", &LepE);
    LepIsEle = new vector<bool>();
    this->Branch ("LepIsEle", &LepIsEle);
    LepPassEMuOR = new vector<bool>();
    this->Branch ("LepPassEMuOR", &LepPassEMuOR);
    LepPassOR = new vector<bool>();
    this->Branch ("LepPassOR", &LepPassOR);
    LepPassSignal = new vector<bool>();
    this->Branch ("LepPassSignal", &LepPassSignal);
    LepTruthClass = new vector<int>();
    this->Branch ("LepTruthClass", &LepTruthClass);
    dR_LepClosestJet = new vector<float>();
    this->Branch ("dR_LepClosestJet", &dR_LepClosestJet);
    dR_LepClosestJetPassingOR = new vector<float>();
    this->Branch ("dR_LepClosestJetPassingOR", &dR_LepClosestJetPassingOR);
    dR_ClosestJetIdx = new vector<int>();
    this->Branch ("dR_ClosestJetIdx", &dR_ClosestJetIdx);
    dR_JetClosestLep = new vector<float>();
    this->Branch ("dR_JetClosestLep", &dR_JetClosestLep);
    dR_ClosestLepIdx = new vector<int>();
    this->Branch ("dR_ClosestLepIdx", &dR_ClosestLepIdx);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Clear
  void clear() {
    JetEta->clear();
    JetPhi->clear();
    JetPt->clear();
    JetE->clear();
    JetPassOREle->clear();
    JetPassORPtRatio->clear();
    JetPassORDeltaRy->clear();
    JetPassORGhostMatched->clear();
    JetPassOR->clear();
    JetPassSignal->clear();
    JetBTagged->clear();
    JetTruth->clear();
    LepEta->clear();
    LepPhi->clear();
    LepPt->clear();
    LepE->clear();
    LepIsEle->clear();
    LepPassEMuOR->clear();
    LepPassOR->clear();
    LepPassSignal->clear();
    LepTruthClass->clear();
    dR_LepClosestJet->clear();
    dR_LepClosestJetPassingOR->clear();
    dR_ClosestJetIdx->clear();
    dR_JetClosestLep->clear();
    dR_JetClosestLep->clear();
    dR_ClosestLepIdx->clear();
  }

  //////////////////////////////////////////////////////////////////////////////
  // Destructor
  ~HistTree() {
    delete JetEta;
    delete JetPhi;
    delete JetPt;
    delete JetE;
    delete JetPassOREle;
    delete JetPassORPtRatio;
    delete JetPassORDeltaRy;
    delete JetPassORGhostMatched;
    delete JetPassOR;
    delete JetPassSignal;
    delete JetBTagged;
    delete JetTruth;
    delete LepEta;
    delete LepPhi;
    delete LepPt;
    delete LepE;
    delete LepIsEle;
    delete LepPassEMuOR;
    delete LepPassOR;
    delete LepPassSignal;
    delete LepTruthClass;
    delete dR_LepClosestJet;
    delete dR_LepClosestJetPassingOR;
    delete dR_ClosestJetIdx;
    delete dR_JetClosestLep;
    delete dR_ClosestLepIdx;
  }

};
