#ifndef SusyNtuple_SusyStop2lIsoEff_h
#define SusyNtuple_SusyStop2lIsoEff_h

//ROOT
#include "TTree.h"
#include "TChain.h"

//SusyNtuple
#include "SusyNtuple/SusyNtAna.h"
#include "SusyNtuple/SusyNtTools.h"

//std/stl
#include <fstream>

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


    private :
        int m_dbg;
        TChain* m_input_chain; // the TChain object we are processing
        float m_mc_weight;

}; //class


#endif
