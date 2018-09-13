#include "SusyStop2lIsoEff/SusyStop2lIsoEff.h"

// SusyNtuple
#include "SusyNtuple/KinematicTools.h"
#include "SusyNtuple/SusyDefs.h"
#include "SusyNtuple/MCTruthClassifierDefs.h"
using namespace Susy; // everything in SusyNtuple is in this namespace

//ROOT

// std/stl
#include <iomanip> // setw
#include <iostream>
#include <string>
#include <sstream> // stringstream, ostringstream
#include <map>
#include <limits.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////////
SusyStop2lIsoEff::SusyStop2lIsoEff() :
    m_dbg(0),
    m_input_chain(nullptr),
    m_mc_weight(1.0),
    m_make_hists(true)
{
}
//////////////////////////////////////////////////////////////////////////////
void SusyStop2lIsoEff::Begin(TTree* /*tree*/)
{
    // call base class' Begin method
    SusyNtAna::Begin(0);
    if(dbg()) cout << "SusyStop2lIsoEff::Begin" << endl;

    m_ofile = new TFile("isoEff.root","RECREATE");
    m_ofile->cd();
    
    // Create list of configuration setups
    unsigned int config_counter = 0;
    for (const auto el_ID_WP_op : el_ID_WP_ops) {
    for (const auto el_pt_min_op : el_pt_min_ops) {
    for (const auto mu_ID_WP_op : mu_ID_WP_ops) {
    for (const auto mu_pt_min_op : mu_pt_min_ops) {
    for (const auto e_j_sliding_cone_or_op : e_j_sliding_cone_or_ops) {
    for (const auto j_e_bjet_or_op : j_e_bjet_or_ops) {
    for (const auto j_m_bjet_or_op : j_m_bjet_or_ops) {
    for (const auto m_j_sliding_cone_or_op : m_j_sliding_cone_or_ops) {
    for (const auto el_iso_WP_op : el_iso_WP_ops) {
    for (const auto mu_iso_WP_op : mu_iso_WP_ops) {
    for (const auto truth_matching_op : truth_matching_ops) {
        config_counter++;  
        
        // Initialize
        string h_name = "histTree_conf" + to_string(config_counter);
        HistTree* histTree = new HistTree(h_name.c_str(), "HistTitle");
        histTree->initialize();
        m_hist_vec.push_back(histTree);

        string conf_name = "name";
        EffTree* conf = new EffTree(conf_name.c_str(), "title");
        conf->initialize();

        // Baseline config options
        conf->config_id = config_counter;
        conf->el_ID_WP = el_ID_WP_op;
        conf->el_pt_min = el_pt_min_op;
        conf->mu_ID_WP = mu_ID_WP_op;
        conf->mu_pt_min = mu_pt_min_op;
        // Overlap removal options
        conf->j_e_bjet_or = j_e_bjet_or_op;
        conf->e_j_sliding_cone_or = e_j_sliding_cone_or_op;
        conf->j_m_bjet_or = j_m_bjet_or_op;
        conf->m_j_sliding_cone_or = m_j_sliding_cone_or_op;
        // Isolation WPs
        conf->el_iso_WP = el_iso_WP_op;
        conf->mu_iso_WP = mu_iso_WP_op;
        // Other
        conf->truth_matching = truth_matching_op;
        
        conf->PrintConf();

        // Store configuration setup 
        m_conf_vec.push_back(conf);

        // Only fill histogram tree when running one configuration
        // This prevents the file size from becoming huge accidentally
        if (config_counter > 1) m_make_hists = false;
    }}}}}}}}}}}
    
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
    for (uint conf_id = 1; conf_id < m_conf_vec.size()+1; conf_id++) {
        SusyNtAna::clearObjects();
        Susy::NtSys::SusyNtSys sys = Susy::NtSys::NOM;
        EffTree* conf = m_conf_vec.at(conf_id-1);
        HistTree* h_tree = m_hist_vec.at(conf_id-1);
        map<const Susy::Lepton*, int> lep_idx;
        map<const Susy::Jet*, int> jet_idx;

        // Record event variables
        h_tree->runNumber = run_number;
        h_tree->eventNumber = event_number;

        // Filter for pre-baseline objects
        m_nttools.getPreObjects(&nt, sys, m_preElectrons, m_preMuons, m_preJets, m_preTaus, m_prePhotons);
       
        // Filter for baseline objects
        m_baseElectrons = getBaselineElectrons(m_preElectrons, *conf);
        m_baseMuons     = getBaselineMuons(m_preMuons, *conf);
        m_baseJets      = m_nttools.getBaselineJets(m_preJets);
        m_baseTaus      = m_nttools.getBaselineTaus(m_preTaus);
        m_basePhotons   = m_nttools.getBaselinePhotons(m_prePhotons);
        m_nttools.buildLeptons(m_baseLeptons, m_baseElectrons, m_baseMuons);
        int n_baseLep_beforeOR = m_baseLeptons.size(); 
        // Event Selection
        if (n_baseLep_beforeOR < 2) { continue; }

        h_tree->nDenJets = m_baseJets.size();
        h_tree->nDenEle = m_baseElectrons.size();
        h_tree->nDenMu = m_baseMuons.size();
        h_tree->nDenLep = m_baseLeptons.size();
        h_tree->nDenTruthLep = n_prompt_leptons(m_baseLeptons);
        h_tree->nDenFakeLep = n_fake_leptons(m_baseLeptons);

        
        // Get pointers to leading and subleading leptons
        const Susy::Lepton* lead_den_lep = m_baseLeptons.at(0);
        const Susy::Lepton* sublead_den_lep = m_baseLeptons.at(1);

        // Fill lepton variables
        for(int ii = 0 ; ii < m_baseLeptons.size(); ii++) {
            const Susy::Lepton* lep = m_baseLeptons.at(ii);
            lep_idx.emplace(lep, ii);
            h_tree->LepEta->push_back(lep->Eta());
            h_tree->LepPhi->push_back(lep->Phi());
            h_tree->LepPt->push_back(lep->Pt());
            h_tree->LepE->push_back(lep->E());
            h_tree->LepIsEle->push_back(lep->isEle());
            h_tree->LepPassOR->push_back(false);
            h_tree->LepPassEMuOR->push_back(false);
            h_tree->LepPassSignal->push_back(false);
            h_tree->LepTruthClass->push_back(get_lepton_truth_class(lep));

            float dRy = 10; // Dummy maximum value for events with no jets
            int jet_idx = -1;
            for (int jj = 0 ; jj < m_baseJets.size(); jj++) {
                const Susy::Jet* jet = m_baseJets.at(jj);
                float tmp_dRy = fabs(lep->DeltaRy(*jet));
                if (tmp_dRy < dRy) {
                  jet_idx = jj;
                  dRy = tmp_dRy;
                }
            }
            h_tree->dR_LepClosestJet->push_back(dRy);
            h_tree->dR_ClosestJetIdx->push_back(jet_idx);
        }
        // Fill jet variables
        for (int ii = 0 ; ii < m_baseJets.size(); ii++) {
            const Susy::Jet* jet = m_baseJets.at(ii);
            jet_idx.emplace(jet, ii);
            h_tree->JetEta->push_back(jet->Eta());
            h_tree->JetPhi->push_back(jet->Phi());
            h_tree->JetPt->push_back(jet->Pt());
            h_tree->JetE->push_back(jet->E());
            h_tree->JetPassOREle->push_back(false);
            h_tree->JetPassORPtRatio->push_back(false);
            h_tree->JetPassORDeltaRy->push_back(false);
            h_tree->JetPassORGhostMatched->push_back(false);
            h_tree->JetPassOR->push_back(false);
            h_tree->JetPassSignal->push_back(false);
            h_tree->JetBTagged->push_back(m_nttools.overlapTool().isBJetOR(jet));
            h_tree->JetTruth->push_back(jet->truthLabel);

            float dRy = 10; // Dummy maximum value for events with no jets
            int lep_idx = -1;
            for(int jj = 0 ; jj < m_baseLeptons.size(); jj++) {
                const Susy::Lepton* lep = m_baseLeptons.at(jj);
                float tmp_dRy = fabs(jet->DeltaRy(*lep));
                if (tmp_dRy < dRy) {
                  lep_idx = jj;
                  dRy = tmp_dRy;
                }
            }
            h_tree->dR_JetClosestLep->push_back(dRy);
            h_tree->dR_ClosestLepIdx->push_back(lep_idx);
        }

        // Apply overlap removal to baseline objects
        m_nttools.overlapTool().m_e_overlap(m_baseMuons, m_baseElectrons);
        m_nttools.overlapTool().e_m_overlap(m_baseElectrons, m_baseMuons);
        m_baseLeptons.clear();
        m_nttools.buildLeptons(m_baseLeptons, m_baseElectrons, m_baseMuons);
        for(const auto lep : m_baseLeptons) {
            h_tree->LepPassEMuOR->at(lep_idx.at(lep)) = true;
        }
        m_nttools.overlapTool().j_e_overlap(m_baseElectrons, m_baseJets, 0.2, conf->j_e_bjet_or);
        m_nttools.overlapTool().e_j_overlap(m_baseElectrons, m_baseJets, 0.4, conf->e_j_sliding_cone_or, true);
        for(const auto jet : m_baseJets) {
            h_tree->JetPassOREle->at(jet_idx.at(jet)) = true;
        }
        //m_nttools.overlapTool().j_m_overlap(m_baseJets, m_baseMuons, 0.2, conf->j_m_bjet_or, true);
        for(int iJ = m_baseJets.size()-1; iJ>=0; iJ--){
            const Jet* j = m_baseJets.at(iJ);

            if(conf->j_m_bjet_or){
                if (conf->truth_matching && j->truthLabel==5) continue;
                if (!conf->truth_matching && m_nttools.overlapTool().isBJetOR(j)) continue;
            }

            for(int iMu = 0; iMu < (int)m_baseMuons.size(); iMu++){
                const Muon* mu = m_baseMuons.at(iMu);

                float ptRatio = mu->Pt() / j->Pt() * 1.0;
                float trkRatio = mu->Pt() / j->sumTrkPt * 1.0;
                if( (j->nTracks >= 3) && ( ptRatio<0.5 || trkRatio<0.7) ) {
                    h_tree->JetPassORPtRatio->at(jet_idx.at(j)) = true;
                    continue;
                }

                bool remove_jet = false;
                // check dR match regardless
                float dR =0.2;
                if(j->DeltaRy(*mu) < dR) {
                    remove_jet = true;
                    h_tree->JetPassORDeltaRy->at(jet_idx.at(j)) = true;
                }
                bool doGhost = true;
                if(doGhost) {
                    if(m_nttools.overlapTool().muonIsGhostMatched(mu, j)) {
                        remove_jet = true;
                        h_tree->JetPassORGhostMatched->at(jet_idx.at(j)) = true;
                    }
                }
                if(remove_jet) {
                    m_baseJets.erase(m_baseJets.begin()+iJ);
                    break;
                }
            } // iMu
        } // iJ
        m_nttools.overlapTool().m_j_overlap(m_baseMuons, m_baseJets, 0.4, conf->m_j_sliding_cone_or, true);
        
        m_baseLeptons.clear();
        m_nttools.buildLeptons(m_baseLeptons, m_baseElectrons, m_baseMuons);
        int n_baseLep_afterOR = m_baseMuons.size() + m_baseElectrons.size();
        h_tree->nDenPassORJets = m_baseJets.size();
        h_tree->nDenPassOREle = m_baseElectrons.size();
        h_tree->nDenPassORMu = m_baseMuons.size();
        h_tree->nDenPassORLep = m_baseLeptons.size();
        h_tree->nDenTruthLep = n_prompt_leptons(m_baseLeptons);
        h_tree->nDenFakeLep = n_fake_leptons(m_baseLeptons);
        //for(int ii = 0 ; ii < m_baseLeptons.size(); ii++) {
        //    const Susy::Lepton* lep = m_baseLeptons.at(ii);
        //    float dRy = 10; // Dummy maximum value for events with no jets
        //    for (int jj = 0 ; jj < m_baseJets.size(); jj++) {
        //        const Susy::Jet* jet = m_baseJets.at(jj);
        //        float tmp_dRy = fabs(lep->DeltaRy(*jet));
        //        if (tmp_dRy < dRy) {
        //          dRy = tmp_dRy;
        //        }
        //    }
        //    h_tree->dR_LepClosestJetPassingOR->push_back(dRy);
        //}
        
        // Set lepton flags
        bool lead_den_lep_pass_or = false, sublead_den_lep_pass_or = false;
        if (std::find(m_baseLeptons.begin(), m_baseLeptons.end(), lead_den_lep) != m_baseLeptons.end()) {
            lead_den_lep_pass_or = true;
        }
        if (std::find(m_baseLeptons.begin(), m_baseLeptons.end(), sublead_den_lep) != m_baseLeptons.end()) {
            sublead_den_lep_pass_or = true;
        }
        for(const auto lep : m_baseLeptons) {
            h_tree->LepPassOR->at(lep_idx.at(lep)) = true;
        }
        for(const auto jet : m_baseJets) {
            h_tree->JetPassOR->at(jet_idx.at(jet)) = true;
        }
        
        // Filter for signal objects
        m_signalElectrons = getSignalElectrons(m_baseElectrons, *conf);
        m_signalMuons     = getSignalMuons(m_baseMuons, *conf);
        m_signalJets      = m_nttools.getSignalJets(m_baseJets);
        m_signalTaus      = m_nttools.getSignalTaus(m_baseTaus);
        m_signalPhotons   = m_nttools.getSignalPhotons(m_basePhotons);
        int n_signalLep = m_signalElectrons.size() + m_signalMuons.size();
        m_nttools.buildLeptons(m_signalLeptons, m_signalElectrons, m_signalMuons);
        bool lead_den_lep_pass_num = false, sublead_den_lep_pass_num = false;
        if (std::find(m_signalLeptons.begin(), m_signalLeptons.end(), lead_den_lep) != m_signalLeptons.end()) {
            lead_den_lep_pass_num = true;
        }
        if (std::find(m_signalLeptons.begin(), m_signalLeptons.end(), sublead_den_lep) != m_signalLeptons.end()) {
            sublead_den_lep_pass_num = true;
        }
        for(const auto lep : m_signalLeptons) {
            h_tree->LepPassSignal->at(lep_idx.at(lep)) = true;
        }
        for(const auto jet : m_signalJets) {
            h_tree->JetPassSignal->at(jet_idx.at(jet)) = true;
        }
       
        // Get event weight
        if(nt.evt()->isMC) {
            float lumi = 100000; // normalize the MC to 100 fb-1
            m_mc_weight = SusyNtAna::mcWeighter().getMCWeight(nt.evt(), lumi, NtSys::NOM);
        }
        else {
            m_mc_weight = 1.; // don't re-weight data
        }
        h_tree->eventweight = m_mc_weight;

        // check that the event passes the standard ATLAS event cleaning cuts
        if(!passEventCleaning(m_preMuons, m_baseMuons, m_baseJets)) return false;

        // Increment counters
        bool leading_lep_eff = false;
        bool subleading_lep_eff = false;
        // m_mc_weight = 1; // Turn off event weights

        if (leading_lep_eff) {
          conf->n_fake_den_leps += lead_den_lep ? m_mc_weight : 0;
          conf->n_fake_den_leps_pass_or += lead_den_lep_pass_or ? m_mc_weight : 0;
          conf->n_fake_num_leps += lead_den_lep_pass_num ? m_mc_weight : 0;
        } else if (subleading_lep_eff) {
          conf->n_fake_den_leps += sublead_den_lep ? m_mc_weight : 0;
          conf->n_fake_den_leps_pass_or += sublead_den_lep_pass_or ? m_mc_weight : 0;
          conf->n_fake_num_leps += sublead_den_lep_pass_num ? m_mc_weight : 0;
        } else {
          conf->n_fake_den_leps += n_baseLep_beforeOR * m_mc_weight; 
          conf->n_fake_den_leps_pass_or += n_baseLep_afterOR * m_mc_weight;
          conf->n_fake_num_leps += n_signalLep * m_mc_weight;
        }


        lep_idx.clear();
        if (m_make_hists) h_tree->Fill();
        h_tree->clear();
    } 
    
    return kTRUE;
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------------*/
ElectronVector SusyStop2lIsoEff::getSignalElectrons(const ElectronVector& baseElecs, const EffTree& conf)
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
MuonVector SusyStop2lIsoEff::getSignalMuons(const MuonVector& baseMuons, const EffTree& conf)
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
ElectronVector SusyStop2lIsoEff::getBaselineElectrons(const ElectronVector& preElecs, const EffTree& conf)
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
MuonVector SusyStop2lIsoEff::getBaselineMuons(const MuonVector& preMuons, const EffTree& conf)
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

    int SusyStop2lIsoEff::get_lepton_truth_class(const Susy::Lepton* lepton) {
    if (lepton==nullptr) return -INT_MAX;

    // Get Truth information
    int T = lepton->mcType;
    int O = lepton->mcOrigin;
    int MO = lepton->mcBkgTruthOrigin;
    int MT = 0; // Not stored in SusyNt::Lepton
    int M_ID = lepton->mcBkgMotherPdgId;

    using namespace MCTruthPartClassifier;

    bool mother_is_el = fabs(M_ID) == 11;
    //bool mother_is_piZero = fabs(M_ID) == 111;
    bool bkgEl_from_phoConv = T==BkgElectron && O==PhotonConv;
    bool bkgEl_from_EMproc = T==BkgElectron && O==ElMagProc;
    bool fromSMBoson = O==WBoson || O==ZBoson || O==Higgs || O==DiBoson;
    bool MfromSMBoson = MO==WBoson || MO==ZBoson || MO==Higgs || MO==DiBoson;
    //bool noChargeFlip = M_ID*lepton->q < 0;
    //bool chargeFlip = M_ID*lepton->q > 0;

    // Defs from https://indico.cern.ch/event/725960/contributions/2987219/attachments/1641430/2621432/TruthDef_April242018.pdf
    bool promptEl1 = T==IsoElectron; //&& noChargeFlip;
    bool promptEl2 = (bkgEl_from_phoConv && mother_is_el); //&& noChargeFlip;
    bool promptEl3 = bkgEl_from_EMproc && MT==IsoElectron && MfromSMBoson; //TESTING(MO==top || MfromSMBoson);
    bool promptEl = promptEl1 || promptEl2 || promptEl3;

    bool promptEl_from_FSR1 = bkgEl_from_phoConv && MO==FSRPhot;
    bool promptEl_from_FSR2 = T==NonIsoPhoton && O==FSRPhot;
    bool promptEl_from_FSR = promptEl_from_FSR1 || promptEl_from_FSR2;

    //bool promptChargeFlipEl1 = T==IsoElectron && chargeFlip;
    //bool promptChargeFlipEl2 = (bkgEl_from_phoConv && mother_is_el) && chargeFlip;
    //bool promptChargeFlipEl = promptChargeFlipEl1 || promptChargeFlipEl2;

    bool promptMuon = T==IsoMuon && (O==top || fromSMBoson || O==HiggsMSSM || O==MCTruthPartClassifier::SUSY);

    bool promptPho1 = T==IsoPhoton && O==PromptPhot;
    bool promptPho2 = bkgEl_from_phoConv && MT==IsoPhoton && MO==PromptPhot;
    bool promptPho3 = bkgEl_from_EMproc  && MT==IsoPhoton && MO==PromptPhot;
    bool promptPho4 = bkgEl_from_phoConv && MT==BkgPhoton && MO==UndrPhot;
    bool promptPho5 = T==BkgPhoton && O==UndrPhot;
    bool promptPho = promptPho1 || promptPho2 || promptPho3 || promptPho4 || promptPho5;

    bool hadDecay1 = T==BkgElectron && (O==DalitzDec || O==ElMagProc || O==LightMeson || O==StrangeMeson);
    bool hadDecay2 = bkgEl_from_phoConv && MT==BkgPhoton && (MO==PiZero || MO==LightMeson || MO==StrangeMeson);
    bool hadDecay3 = bkgEl_from_EMproc && ((MT==BkgElectron && MO==StrangeMeson) || (MT==BkgPhoton && MO==PiZero));
    bool hadDecay4 = T==BkgPhoton && (O==LightMeson || O==PiZero);
    bool hadDecay5 = T==BkgMuon && (O==LightMeson || O==StrangeMeson || O==PionDecay || O==KaonDecay);
    bool hadDecay6 = T==Hadron;
    bool hadDecay = hadDecay1 || hadDecay2 || hadDecay3 || hadDecay4 || hadDecay5 || hadDecay6;

    bool Mu_as_e1 = (T==NonIsoElectron || T==NonIsoPhoton) && O==Mu;
    bool Mu_as_e2 = bkgEl_from_EMproc && MT==NonIsoElectron && MO==Mu;
    bool Mu_as_e3 = bkgEl_from_phoConv && MT==NonIsoPhoton && MO==Mu;
    bool Mu_as_e = Mu_as_e1 || Mu_as_e2 || Mu_as_e3;

    bool HF_tau1 =  (T==NonIsoElectron || T==NonIsoPhoton) && O==TauLep;
    bool HF_tau2 =  bkgEl_from_phoConv && MT==NonIsoPhoton && MO==TauLep;
    bool HF_tau3 =  T==NonIsoMuon && O==TauLep;
    bool HF_tau =  HF_tau1 || HF_tau2 || HF_tau3;

    bool HF_B1 = T==NonIsoElectron && (O==BottomMeson || O==BBbarMeson || O==BottomBaryon);
    bool HF_B2 = T==BkgPhoton && O==BottomMeson;
    bool HF_B3 = bkgEl_from_phoConv && MT==BkgPhoton && MO==BottomMeson;
    bool HF_B4 = (T==IsoMuon || T==NonIsoMuon) && (O==BottomMeson || O==BBbarMeson || O==BottomBaryon);
    bool HF_B = HF_B1 || HF_B2 || HF_B3 || HF_B4;

    bool HF_C1 = T==NonIsoElectron && (O==CharmedMeson || O==CharmedBaryon || O==CCbarMeson);
    bool HF_C2 = T==BkgElectron && O==CCbarMeson;
    bool HF_C3 = T==BkgPhoton && (O==CharmedMeson || O==CCbarMeson);
    bool HF_C4 = bkgEl_from_phoConv && MT==BkgPhoton && (MO==CharmedMeson || MO==CCbarMeson);
    bool HF_C5 = T==NonIsoMuon && (O==CharmedMeson || O==CharmedBaryon || O==CCbarMeson);
    bool HF_C6 = (T==IsoMuon || T==BkgMuon) && (O==CCbarMeson || MO==CCbarMeson);
    bool HF_C =  HF_C1 || HF_C2 || HF_C3 || HF_C4 || HF_C5 || HF_C6;

    if      (promptEl)           return 1;
    else if (promptMuon)         return 2;
    else if (promptPho)          return 3;
    else if (promptEl_from_FSR)  return 4;
    else if (hadDecay)           return 5;
    else if (Mu_as_e)            return 6;
    else if (HF_tau)             return 7;
    else if (HF_B)               return 8;
    else if (HF_C)               return 9;
    else if (bkgEl_from_phoConv) return 10;
    else if (!(T || O || MT || MO || M_ID)) return -1;
    else if (T && O && !(MT || MO || M_ID)) return -2;
    else if (T && !O) return -3;
    // else if (promptChargeFlipEl) return 2;
    else if (T && O && M_ID) {
        cout << "Unexpected Truth Class: "
             << "T = " << T << ", "
             << "O = " << O << ", "
             << "MT = " << MT << ", "
             << "MO = " << MO << ", "
             << "M_ID = " << M_ID << endl;
    }
    return 0;
}
int SusyStop2lIsoEff::n_prompt_leptons(const LeptonVector& leptons) {
    uint n_prompt_leptons = 0;
    for (auto lep : leptons) {
        int truth_class = get_lepton_truth_class(lep);
        bool prompt_el = (truth_class == 1);
        bool prompt_mu = (truth_class == 2);
        if (prompt_el || prompt_mu) n_prompt_leptons++;
    }
    return n_prompt_leptons;
}
int SusyStop2lIsoEff::n_fake_leptons(const LeptonVector& leptons) {
    uint n_fake_leptons = 0;
    for (auto lep : leptons) {
        int truth_class = get_lepton_truth_class(lep);
        bool prompt_el = (truth_class == 1);
        bool prompt_mu = (truth_class == 2);
        if (!prompt_el && !prompt_mu) n_fake_leptons++;
    }
    return n_fake_leptons;
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
    m_ofile->cd();
    m_effTree = new EffTree("effTree","Title");
    m_effTree->initialize();
    for (auto conf : m_conf_vec) {
        conf->calculateResults();
        *m_effTree = *conf;
        //m_effTree->PrintResults();
        m_effTree->Fill();
        delete conf;
    }
    m_ofile->Write();
    m_hist_vec.clear();
    m_conf_vec.clear();
    delete m_effTree;
    m_ofile->Close();
    delete m_ofile;

    // close SusyNtAna and print timers
    SusyNtAna::Terminate();

    return;
}
//////////////////////////////////////////////////////////////////////////////
