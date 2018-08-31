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

    // SusyNtAna::selectObject fills the baseline and signal objects
    // for the given AnalysisType
    // m_preX    = objects before any selection (as they are in susyNt)
    // m_baseX   = objects with the Analysis' baseline selection AND overlap removal applied
    // m_signalX = objects with the Analysis' signal selection applied (and baseline AND overlap removal)
    SusyNtAna::selectObjects();

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

    // Fill output trees for each configuration setup
    for (auto conf : m_conf_vec) {

        // TODO: Use config options to dynamically define baseline leps and OR

        conf->n_den_leps += nBaselineLepsBeforeOR() * m_mc_weight; 
        conf->n_den_leps_pass_or += m_baseLeptons.size() * m_mc_weight;
        conf->n_num_leps += m_signalLeptons.size() * m_mc_weight;
    } 
    
    return kTRUE;
}
//////////////////////////////////////////////////////////////////////////////
int SusyStop2lIsoEff::nBaselineLepsBeforeOR() {
    int nBaseEl = m_nttools.getBaselineElectrons(m_preElectrons).size();
    int nBaseMu = m_nttools.getBaselineMuons(m_preMuons).size();
    
    return nBaseEl + nBaseMu;
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
