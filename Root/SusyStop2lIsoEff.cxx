#include "SusyStop2lIsoEff/SusyStop2lIsoEff.h"

// SusyNtuple
#include "SusyNtuple/KinematicTools.h"
#include "SusyNtuple/SusyDefs.h"
using namespace Susy; // everything in SusyNtuple is in this namespace

//ROOT

// std/stl
#include <iomanip> // setw
#include <iostream>
#include <string>
#include <sstream> // stringstream, ostringstream
using namespace std;

//////////////////////////////////////////////////////////////////////////////
SusyStop2lIsoEff::SusyStop2lIsoEff() :
    m_dbg(0),
    m_input_chain(nullptr),
    m_mc_weight(1.0)
{
}
//////////////////////////////////////////////////////////////////////////////
void SusyStop2lIsoEff::Begin(TTree* /*tree*/)
{
    // call base class' Begin method
    SusyNtAna::Begin(0);
    if(dbg()) cout << "SusyStop2lIsoEff::Begin" << endl;

    
    // Create list of configuration setups
    unsigned int config_counter = 0;
    for (auto el_ID_WP_op : el_ID_WP_ops) {
    for (auto el_pt_min_op : el_pt_min_ops) {
    for (auto mu_ID_WP_op : mu_ID_WP_ops) {
    for (auto mu_pt_min_op : mu_pt_min_ops) {
    for (auto j_e_bjet_or_op : j_e_bjet_or_ops) {
    for (auto j_m_bjet_or_op : j_m_bjet_or_ops) {
        config_counter++;  
        for (auto el_iso_WP_op : el_iso_WP_ops) {
        for (auto mu_iso_WP_op : mu_iso_WP_ops) {
            // Initialize
            string conf_name = "name";
            OutputTree* conf = new OutputTree(conf_name.c_str(), "title");
            conf->initialize();
            // Baseline config options
            conf->config_id = config_counter;
            conf->el_ID_WP = el_ID_WP_op;
            conf->el_pt_min = el_pt_min_op;
            conf->mu_ID_WP = mu_ID_WP_op;
            conf->mu_pt_min = mu_pt_min_op;
            // Overlap removal options
            conf->j_e_bjet_or = j_e_bjet_or_op;
            conf->j_m_bjet_or = j_m_bjet_or_op;
            // Isolation WPs
            conf->el_iso_WP = el_iso_WP_op;
            conf->mu_iso_WP = mu_iso_WP_op;
            // Store configuration setup 
            m_conf_vec.push_back(conf);
        }}
    }}}}}}
    return;
}
//////////////////////////////////////////////////////////////////////////////
Bool_t SusyStop2lIsoEff::Process(Long64_t entry)
{

    // calling "GetEntry" loads into memory the susyNt class objects for this event
    GetEntry(entry);
    SusyNtAna::clearObjects(); // clear the previous event's objects

    // increment the chain entry (c.f. SusyNtuple/SusyNtAna.h)
    m_chainEntry++;

    // evt() provides pointer to the SusyNt::Event class object for this event
    int run_number = nt.evt()->run;
    int event_number = nt.evt()->eventNumber;

    if(dbg() || m_chainEntry%1000==0) {
        cout << "SusyStop2lIsoEff::Process    **** Processing entry " << setw(6) << m_chainEntry
                << "  run " << run_number << "  event " << event_number << " **** " << endl;
    }

    // Fill output trees for each configuration setup
    for (auto conf : m_conf_vec) {
        //SusyNtAna::selectObjects();
        SusyNtAna::clearObjects();
        Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM;
        m_nttools.getPreObjects(&nt, sys, m_preElectrons, m_preMuons, m_preJets, m_preTaus, m_prePhotons);
        
        //m_nttools.getBaselineObjects(m_preElectrons, m_preMuons, m_preJets, m_preTaus, m_prePhotons,
        //                             m_baseElectrons, m_baseMuons, m_baseJets, m_baseTaus, m_basePhotons);
        m_baseElectrons = getBaselineElectrons(m_preElectrons, *conf);
        m_baseMuons     = getBaselineMuons(m_preMuons, *conf);
        m_baseJets      = m_nttools.getBaselineJets(m_preJets);
        m_baseTaus      = m_nttools.getBaselineTaus(m_preTaus);
        m_basePhotons   = m_nttools.getBaselinePhotons(m_prePhotons);
        int n_baseLep_beforeOR = m_baseMuons.size() + m_baseElectrons.size();
        
        m_nttools.overlapTool().performOverlap(m_baseElectrons, m_baseMuons, m_baseJets, m_baseTaus, m_basePhotons);
        int n_baseLep_afterOR = m_baseMuons.size() + m_baseElectrons.size();
        
        //m_nttools.getSignalObjects(m_baseElectrons, m_baseMuons, m_baseJets, m_baseTaus, m_basePhotons,
        //                           m_signalElectrons, m_signalMuons, m_signalJets, m_signalTaus, m_signalPhotons);
        m_signalElectrons = getSignalElectrons(m_baseElectrons, *conf);
        m_signalMuons     = getSignalMuons(m_baseMuons, *conf);
        m_signalJets      = m_nttools.getSignalJets(m_baseJets);
        m_signalTaus      = m_nttools.getSignalTaus(m_baseTaus);
        m_signalPhotons   = m_nttools.getSignalPhotons(m_basePhotons);
        //m_nttools.buildLeptons(m_preLeptons, m_preElectrons, m_preMuons);
        //m_nttools.buildLeptons(m_baseLeptons, m_baseElectrons, m_baseMuons);
        //m_nttools.buildLeptons(m_signalLeptons, m_signalElectrons, m_signalMuons);
        int n_signalLep = m_signalElectrons.size() + m_signalMuons.size();
        //
        //SusyNtSys metSys = sys;
        //m_met = m_nttools.getMet(&nt, metSys);
        //m_trackMet = m_nttools.getTrackMet(&nt, metSys);

        // get the MC weight using the inherited MCWeighter object
        // (c.f. SusyNtuple/MCWeighter.h)
        if(nt.evt()->isMC) {
            float lumi = 100000; // normalize the MC to 100 fb-1
            m_mc_weight = SusyNtAna::mcWeighter().getMCWeight(nt.evt(), lumi, NtSys::NOM);
        }
        else {
            m_mc_weight = 1.; // don't re-weight data
        }
        // check that the event passes the standard ATLAS event cleaning cuts
        if(!passEventCleaning(m_preMuons, m_baseMuons, m_baseJets)) return false;

        conf->n_den_leps += n_baseLep_beforeOR * m_mc_weight; 
        conf->n_den_leps_pass_or += n_baseLep_afterOR * m_mc_weight;
        conf->n_num_leps += n_signalLep * m_mc_weight;
    } 
    
    return kTRUE;
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------*/
ElectronVector SusyStop2lIsoEff::getSignalElectrons(const ElectronVector& baseElecs, const OutputTree& conf)
{
    ElectronVector sigElecs;
    for (uint ie = 0; ie < baseElecs.size(); ++ie) {
        Electron* e = baseElecs.at(ie);
        bool pass_iso = false;
        if (conf.el_iso_WP == isoFixedCutTightTrackOnly) pass_iso = e->isoFixedCutTightTrackOnly;
        else if (conf.el_iso_WP == isoLooseTrackOnly) pass_iso = e->isoLooseTrackOnly;
        else if (conf.el_iso_WP == isoLoose) pass_iso = e->isoLoose;
        else if (conf.el_iso_WP == isoGradientLoose) pass_iso = e->isoGradientLoose;
        else if (conf.el_iso_WP == isoGradient) pass_iso = e->isoGradient;
        
        bool pass_signal = pass_iso;
        if (pass_signal){
            sigElecs.push_back(e);
        }
    }
    // sort by pT
    std::sort(sigElecs.begin(), sigElecs.end(), comparePt);

    return sigElecs;
}
MuonVector SusyStop2lIsoEff::getSignalMuons(const MuonVector& baseMuons, const OutputTree& conf)
{
    MuonVector sigMuons;
    for (uint im = 0; im < baseMuons.size(); ++im) {
        Muon* mu = baseMuons.at(im);
        bool pass_iso = false;
        if (conf.mu_iso_WP == isoFixedCutTightTrackOnly) pass_iso = mu->isoFixedCutTightTrackOnly;
        else if (conf.mu_iso_WP == isoLooseTrackOnly) pass_iso = mu->isoLooseTrackOnly;
        else if (conf.mu_iso_WP == isoLoose) pass_iso = mu->isoLoose;
        else if (conf.mu_iso_WP == isoGradientLoose) pass_iso = mu->isoGradientLoose;
        else if (conf.mu_iso_WP == isoGradient) pass_iso = mu->isoGradient;

        bool pass_signal = pass_iso;
        if (pass_signal) sigMuons.push_back(mu);
    }
    // sort by pT
    std::sort(sigMuons.begin(), sigMuons.end(), comparePt);
    
    return sigMuons;
}
/*--------------------------------------------------------------------------------*/
ElectronVector SusyStop2lIsoEff::getBaselineElectrons(const ElectronVector& preElecs, const OutputTree& conf)
{
    ElectronVector elecs;
    for (uint ie = 0; ie < preElecs.size(); ++ie) {
        Electron* e = preElecs.at(ie);
        bool pass_OQ = e->passOQBadClusElectron;
        bool pass_pt = (e->Pt() > conf.el_pt_min);
        bool pass_eta = (fabs(e->clusEta) <  2.47);
        bool pass_ID = false;
        if (conf.el_ID_WP == looseLLH) pass_ID = e->looseLLH;
        else if (conf.el_ID_WP == looseLLHBLayer) pass_ID = e->looseLLHBLayer;
        else if (conf.el_ID_WP == mediumLLH) pass_ID = e->mediumLLH;
        else if (conf.el_ID_WP == tightLLH) pass_ID = e->tightLLH;
        
        bool pass_base = pass_OQ && pass_pt && pass_eta && pass_ID;
        if(pass_base) elecs.push_back(e);
    } // ie
    // sort by pT
    std::sort(elecs.begin(), elecs.end(), comparePt);

    return elecs;
}
MuonVector SusyStop2lIsoEff::getBaselineMuons(const MuonVector& preMuons, const OutputTree& conf)
{
    MuonVector baseMuons;
    for (uint im = 0; im < preMuons.size(); ++im) {
        Muon* mu = preMuons.at(im);
        bool pass_pt = (mu->Pt() > conf.mu_pt_min);
        bool pass_eta = (fabs(mu->Eta()) <  2.4);
        bool pass_ID = false;
        if (conf.mu_ID_WP == veryLoose) pass_ID = (mu->veryLoose);
        else if (conf.mu_ID_WP == loose) pass_ID = (mu->loose);
        else if (conf.mu_ID_WP == medium) pass_ID = (mu->medium);
        else if (conf.mu_ID_WP == tight) pass_ID = (mu->tight);
        
        bool pass_base = pass_pt && pass_eta && pass_ID;
        if(pass_base) baseMuons.push_back(mu);
    } // im
    // sort by pT
    std::sort(baseMuons.begin(), baseMuons.end(), comparePt);

    return baseMuons;
}

bool SusyStop2lIsoEff::passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
            const JetVector& baseJets)
{
    int flags = nt.evt()->cutFlags[NtSys::NOM];

    if(!nttools().passGRL(flags))           return false;

    if(!nttools().passLarErr(flags))        return false;

    if(!nttools().passTileErr(flags))       return false;

    if(!nttools().passTTC(flags))           return false;

    if(!nttools().passSCTErr(flags))        return false;

    if(!nttools().passGoodVtx(flags))       return false;


    ///////////////////////////////////////////////////////
    // for bad muon, cosmic moun, and jet cleaning the
    // cuts depend on the baseline object defintion
    // (and in thec ase of the cosmic muon cut, it also
    // depends on the analysis' overlap removal
    // procedure) -- so we do not use the cutFlags but
    // rather use the objects that have passed the various
    // analysis selections to do the checks
    ///////////////////////////////////////////////////////
    if(!nttools().passBadMuon(preMuons))    return false;

    if(!nttools().passCosmicMuon(baseMuons)) return false;

    if(!nttools().passJetCleaning(baseJets)) return false;

    return true;
}
//////////////////////////////////////////////////////////////////////////////
void SusyStop2lIsoEff::Terminate()
{
    m_ofile = new TFile("isoEff.root","RECREATE");
    m_ofile->cd();
    m_otree = new OutputTree("outputTree","Title");
    m_otree->initialize();
    for (auto conf : m_conf_vec) {
        conf->calculateResults();
        //conf->Print();
        *m_otree = *conf;
        m_otree->Print();
        m_otree->Fill();
        //delete conf;
    }
    m_ofile->Write();
    m_conf_vec.clear();
    m_ofile->Close();
    //delete m_otree;
    //delete m_ofile;

    // close SusyNtAna and print timers
    SusyNtAna::Terminate();

    return;
}
//////////////////////////////////////////////////////////////////////////////
