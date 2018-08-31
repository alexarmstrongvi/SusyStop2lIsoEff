#include <TTree.h>
#include <vector>
#include <iostream>
#include "math.h"

using std::vector;
using std::cout;

enum EleIDWP {tightLLH, mediumLLH, looseLLHBLayer, looseLLH, veryLooseLLH, N_EL_IDWPs};
enum MuoIDWP {tight, medium, loose, veryLoose, N_MU_IDWPs}
enum IsoWP {isoGradient, isoGradientLoose, isoLoose, isoLooseTrackOnly, isoFixedCutTightTrackOnly, N_ISO_WPs}

class OutputTree : public TTree
{
public:
  // Constructor
  OutputTree(const char* name, const char* title)
    : TTree(name, title) {}

  //////////////////////////////////////////////////////////////////////////////
  // Define variables for branches

  // Baseline lepton definitions
  EleIDWP el_ID_WP = N_EL_IDWPs; 
  float el_pt_min = 0;
  MuoIDWP mu_ID_WP = N_MU_IDWPs; 
  float mu_pt_min = 0;

  // Isolation WPs
  IsoWP el_iso_WP = N_ISO_WPs;  
  IsoWP mu_iso_WP = N_ISO_WPs;  
  //float etconetopo20_max = FLT_MAX;
  //float etconetopo30_max = FLT_MAX;
  //float ptvarcone20_max = FLT_MAX;

  // Overlap removal toggles
  bool j_e_bjet_or = true
  bool j_m_bjet_or = true

  // Result
  unsigned int n_den_leps = 0;
  unsigned int n_den_leps_pass_or = 0;
  unsigned int n_num_leps = 0;

  float iso_plus_or_eff = -1;  ///< efficency of isolation and overlap removal combined
  float iso_plus_or_rej = -1;
  float iso_after_or_eff = -1;  ///< efficency of isolation after overlap removal
  float iso_after_or_rej = -1;

  //////////////////////////////////////////////////////////////////////////////
  // Attach variables to branches
  void initialize() {
    this->Branch ("el_ID_WP", &el_ID_WP);
    this->Branch ("el_pt_min", &el_pt_min);
    this->Branch ("mu_ID_WP", &mu_ID_WP);
    this->Branch ("mu_pt_min", &mu_pt_min);
    this->Branch ("el_iso_WP", &el_iso_WP);
    this->Branch ("mu_iso_WP", &mu_iso_WP);
    this->Branch ("j_e_bjet_or", &j_e_bjet_or);
    this->Branch ("j_m_bjet_or", &j_m_bjet_or);
    this->Branch ("iso_plus_or_eff", &iso_plus_or_eff);
    this->Branch ("iso_plus_or_rej", &iso_plus_or_rej);
    this->Branch ("iso_after_or_eff", &iso_after_or_eff);
    this->Branch ("iso_after_or_rej", &iso_after_or_rej);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Useful functions
  void calculate_results() {
    if (n_den_leps == 0) {
       cout << "WARNING :: No leptons passed denominator requirements\n";
       return;
    }
    iso_plus_or_eff = float(n_num_leps) / float(n_den_leps);
    iso_plus_or_rej = 1.0 - iso_plus_or_eff;
    if (n_den_leps_pass_or == 0) {
       cout << "WARNING :: No leptons passed overlap removal\n";
       return;
    }
    iso_after_or_eff = float(n_num_leps) / float(n_den_leps_pass_or);
    iso_after_or_rej = 1 - iso_after_or_eff; 
  }


  //////////////////////////////////////////////////////////////////////////////
  // Clear vectors
  void clear() {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Destructor - delete vectors
  ~OutputTree() {
  }

};
