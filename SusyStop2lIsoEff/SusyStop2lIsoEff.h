#ifndef SusyNtuple_SusyStop2lIsoEff_h
#define SusyNtuple_SusyStop2lIsoEff_h

//ROOT
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

//SusyNtuple
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"

//std/stl
#include <fstream>

//User classes
#include "SusyStop2lIsoEff/EffTree.h"
#include "SusyStop2lIsoEff/HistTree.h"

/////////////////////////////////////////////////////////////
//
// SusyStop2lIsoEff
// Class auto-generated with SusyNtuple/make_susy_skeleton on 2018-08-30 12:13
//
/////////////////////////////////////////////////////////////

// for TSelector analysis loopers processing susyNt you MUST inherit from SusyNtAna
// in order to pick up the susyNt class objects
class SusyStop2lIsoEff : public SusyNtAna
{

    public :
        SusyStop2lIsoEff();
        virtual ~SusyStop2lIsoEff() {};

        void set_debug(int dbg) { m_dbg = dbg; }
        int dbg() { return m_dbg; }

        void set_chain(TChain* chain) { m_input_chain = chain; }
        TChain* chain() { return m_input_chain; }

        ////////////////////////////////////////////
        // analysis methods
        ////////////////////////////////////////////

        // standard ATLAS event cleaning
        bool passEventCleaning(const MuonVector& preMuons, const MuonVector& baseMuons,
                const JetVector& baseJets);

        ////////////////////////////////////////////
        // TSelector methods override
        ////////////////////////////////////////////
        virtual void Begin(TTree* tree); // Begin is called before looping on entries
        virtual Bool_t Process(Long64_t entry); // Main event loop function called on each event
        virtual void Terminate(); // Terminate is called after looping has finished

        ElectronVector getBaselineElectrons(const ElectronVector& preElecs, const EffTree& conf);
        ElectronVector getSignalElectrons(const ElectronVector& baseElecs, const EffTree& conf);
        MuonVector getBaselineMuons(const MuonVector& preMuons, const EffTree& conf);
        MuonVector getSignalMuons(const MuonVector& baseMuons, const EffTree& conf);

        int n_btag_jets(const JetVector& jets);
        int get_lepton_truth_class(const Susy::Lepton* lepton);
        template<typename T>
        inline int n_real_leptons(const std::vector<T>& leptons) {
            uint n_real_leptons = 0;
            for (auto lep : leptons) {
                int truth_class = get_lepton_truth_class(lep);
                bool prompt_el = (truth_class == 1);
                bool prompt_mu = (truth_class == 2);
                if (prompt_el || prompt_mu) n_real_leptons++;
            }
            return n_real_leptons;
        }
        template<typename T>
        inline int n_fake_leptons(const std::vector<T>& leptons) {
            uint n_fake_leptons = 0;
            for (auto lep : leptons) {
                int truth_class = get_lepton_truth_class(lep);
                bool prompt_el = (truth_class == 1);
                bool prompt_mu = (truth_class == 2);
                if (!prompt_el && !prompt_mu) n_fake_leptons++;
            }
            return n_fake_leptons;
        }
    private :
        int m_dbg;
        TChain* m_input_chain; // the TChain object we are processing
        float m_mc_weight;
        bool m_make_hists;
        vector<EffTree*> m_conf_vec;
        vector<HistTree*> m_hist_vec;
        TFile* m_ofile;
        EffTree *m_effTree;


}; //class


#endif
