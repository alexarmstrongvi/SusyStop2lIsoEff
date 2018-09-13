#pragma once
#include <TTree.h>
#include <vector>
#include <iostream>
#include "math.h"

using std::vector;
using std::cout;

enum EleIDWP {tightLLH, mediumLLH, looseLLHBLayer, looseLLH, N_EL_IDWPs};
enum MuoIDWP {tight, medium, loose, veryLoose, N_MU_IDWPs};
enum IsoWP {isoGradient, isoGradientLoose, isoFixedCutTightTrackOnly, isoLoose, isoLooseTrackOnly, N_ISO_WPs};

/// List of options to be looped over
// Every combination of the options below is used
vector<int> el_ID_WP_ops = {looseLLHBLayer};
vector<float> el_pt_min_ops = {10.0};
vector<int> mu_ID_WP_ops =  {medium};
vector<float> mu_pt_min_ops = {10.0};
vector<int> el_iso_WP_ops = {isoLooseTrackOnly};
vector<int> mu_iso_WP_ops = {isoLooseTrackOnly};
vector<int> truth_matching_ops = {false};
vector<bool> j_e_bjet_or_ops = {false};
vector<bool> e_j_sliding_cone_or_ops= {false};
vector<bool> j_m_bjet_or_ops = {false};
vector<bool> m_j_sliding_cone_or_ops = {true};

class EffTree : public TTree
{
public:
  // Constructor
  EffTree(const char* name, const char* title)
    : TTree(name, title) {}
  //////////////////////////////////////////////////////////////////////////////
  // Define variables for branches
    
  unsigned int config_id = 0;
  // Baseline lepton definitions
  int el_ID_WP = N_EL_IDWPs; 
  float el_pt_min = 0;
  int mu_ID_WP = N_MU_IDWPs; 
  float mu_pt_min = 0;

  // Overlap removal toggles
  bool j_e_bjet_or = true;
  bool e_j_sliding_cone_or = true;
  bool j_m_bjet_or = true;
  bool m_j_sliding_cone_or = true;

  // Isolation WPs
  int el_iso_WP = N_ISO_WPs;  
  int mu_iso_WP = N_ISO_WPs;  
  //float etconetopo20_max = FLT_MAX;
  //float etconetopo30_max = FLT_MAX;
  //float ptvarcone20_max = FLT_MAX;

  // Other
  bool truth_matching = false;

  // Results (split by fake/real and electron/muon)
  float n_fake_den_el = 0;
  float n_fake_den_el_pass_or = 0;
  float n_fake_num_el = 0;
  float fake_el_or_eff = -1;  ///< fake efficency of overlap removal
  float fake_el_iso_plus_or_eff = -1;  ///< fake efficency of isolation and overlap removal combined
  float fake_el_iso_after_or_eff = -1;  ///< fake efficency of isolation after overlap removal

  float n_fake_den_mu = 0;
  float n_fake_den_mu_pass_or = 0;
  float n_fake_num_mu = 0;
  float fake_mu_or_eff = -1;  ///< fake efficency of overlap removal
  float fake_mu_iso_plus_or_eff = -1;  ///< fake efficency of isolation and overlap removal combined
  float fake_mu_iso_after_or_eff = -1;  ///< fake efficency of isolation after overlap removal

  float n_real_den_el = 0;
  float n_real_den_el_pass_or = 0;
  float n_real_num_el = 0;
  float real_el_or_eff = -1;  ///< real efficency of overlap removal
  float real_el_iso_plus_or_eff = -1;  ///< real efficency of isolation and overlap removal combined
  float real_el_iso_after_or_eff = -1;  ///< real efficency of isolation after overlap removal

  float n_real_den_mu = 0;
  float n_real_den_mu_pass_or = 0;
  float n_real_num_mu = 0;
  float real_mu_or_eff = -1;  ///< real efficency of overlap removal
  float real_mu_iso_plus_or_eff = -1;  ///< real efficency of isolation and overlap removal combined
  float real_mu_iso_after_or_eff = -1;  ///< real efficency of isolation after overlap removal

  EffTree& operator=(const EffTree& rhs) {
    this->config_id = rhs.config_id;
    this->el_ID_WP = rhs.el_ID_WP;
    this->el_pt_min = rhs.el_pt_min;
    this->mu_ID_WP = rhs.mu_ID_WP;
    this->mu_pt_min = rhs.mu_pt_min;
    this->j_e_bjet_or = rhs.j_e_bjet_or;
    this->e_j_sliding_cone_or = rhs.e_j_sliding_cone_or;
    this->j_m_bjet_or = rhs.j_m_bjet_or;
    this->m_j_sliding_cone_or = rhs.m_j_sliding_cone_or;
    this->el_iso_WP = rhs.el_iso_WP;
    this->mu_iso_WP = rhs.mu_iso_WP;
    this->truth_matching = rhs.truth_matching;

    this->n_fake_den_leps = rhs.n_fake_den_leps;
    this->n_real_den_leps = rhs.n_real_den_leps;
    this->n_fake_den_leps_pass_or = rhs.n_fake_den_leps_pass_or;
    this->n_real_den_leps_pass_or = rhs.n_real_den_leps_pass_or;
    this->n_fake_num_leps = rhs.n_fake_num_leps;
    this->n_real_num_leps = rhs.n_real_num_leps;
    this->fake_or_eff = rhs.fake_or_eff;
    this->real_or_eff = rhs.real_or_eff;
    this->fake_iso_plus_or_eff = rhs.fake_iso_plus_or_eff;
    this->real_iso_plus_or_eff = rhs.real_iso_plus_or_eff;
    this->fake_iso_after_or_eff = rhs.fake_iso_after_or_eff;
    this->real_iso_after_or_eff = rhs.real_iso_after_or_eff;
    return *this;
  }
  //////////////////////////////////////////////////////////////////////////////
  // Attach variables to branches
  void initialize() {
    this->Branch ("config_id", &config_id);
    this->Branch ("el_ID_WP", &el_ID_WP);
    this->Branch ("el_pt_min", &el_pt_min);
    this->Branch ("mu_ID_WP", &mu_ID_WP);
    this->Branch ("mu_pt_min", &mu_pt_min);
    this->Branch ("j_e_bjet_or", &j_e_bjet_or);
    this->Branch ("e_j_sliding_cone_or", &e_j_sliding_cone_or);
    this->Branch ("j_m_bjet_or", &j_m_bjet_or);
    this->Branch ("m_j_sliding_cone_or", &m_j_sliding_cone_or);
    this->Branch ("el_iso_WP", &el_iso_WP);
    this->Branch ("mu_iso_WP", &mu_iso_WP);
    this->Branch ("truth_matching", &truth_matching);
    this->Branch ("fake_or_eff", &fake_or_eff);
    this->Branch ("real_or_eff", &real_or_eff);
    this->Branch ("fake_iso_plus_or_eff", &fake_iso_plus_or_eff);
    this->Branch ("real_iso_plus_or_eff", &real_iso_plus_or_eff);
    this->Branch ("fake_iso_after_or_eff", &fake_iso_after_or_eff);
    this->Branch ("real_iso_after_or_eff", &real_iso_after_or_eff);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Print variables 
  void PrintConf() const {
    cout << "Configuration Options:\n";
    cout << ">> config_id = " << config_id << "\n";
    cout << "\t BaseDef: ";
    cout << "el_ID_WP = " << el_ID_WP;
    cout << ", el_pt_min = " << el_pt_min;
    cout << ", mu_ID_WP = " << mu_ID_WP;
    cout << ", mu_pt_min = " << mu_pt_min;
    cout << "\n";
    cout << "\t Overlap: ";
    cout << "j_e_bjet_or = " << j_e_bjet_or;
    cout << ", e_j_sliding_cone_or = " << e_j_sliding_cone_or;
    cout << ", j_m_bjet_or = " << j_m_bjet_or;
    cout << ", m_j_sliding_cone_or = " << m_j_sliding_cone_or;
    cout << "\n";
    cout << "\t SignalDef: ";
    cout << "el_iso_WP = " << el_iso_WP;
    cout << ", mu_iso_WP = " << mu_iso_WP;
    cout << "\n";
    cout << "\t Other: ";
    cout << "truth_matching = " << truth_matching;
    cout << "\n";
  }
  void PrintResults() const {
    cout << "Results:\n";
    cout << "\t n_fake_den_leps = " << n_fake_den_leps << '\n';
    cout << "\t n_real_den_leps = " << n_real_den_leps << '\n';
    cout << "\t n_fake_den_leps_pass_or = " << n_fake_den_leps_pass_or << '\n';
    cout << "\t n_real_den_leps_pass_or = " << n_real_den_leps_pass_or << '\n';
    cout << "\t n_fake_num_leps = " << n_fake_num_leps << '\n';
    cout << "\t n_real_num_leps = " << n_real_num_leps << '\n';
    cout << "\t fake_or_eff = " << fake_or_eff << '\n';
    cout << "\t real_or_eff = " << real_or_eff << '\n';
    cout << "\t fake_iso_plus_or_eff = " << fake_iso_plus_or_eff << '\n';
    cout << "\t real_iso_plus_or_eff = " << real_iso_plus_or_eff << '\n';
    cout << "\t fake_iso_after_or_eff = " << fake_iso_after_or_eff << '\n';
    cout << "\t real_iso_after_or_eff = " << real_iso_after_or_eff << '\n';
  }
  void Print() const {
    cout << "===============================================================\n";
    PrintConf();
    PrintResults();
    cout << "===============================================================\n";
  }

  //////////////////////////////////////////////////////////////////////////////
  // Clear vectors
  void clear() {
  }

  //////////////////////////////////////////////////////////////////////////////
  // Destructor - delete vectors
  ~EffTree() {
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Useful functions
  void calculateResults() {
    if (n_fake_den_leps == 0) {
       cout << "WARNING :: No leptons passed denominator requirements\n";
       return;
    }
    fake_or_eff = n_fake_den_leps_pass_or / n_fake_den_leps;
    fake_iso_plus_or_eff = n_fake_num_leps / n_fake_den_leps;
    if (n_fake_den_leps_pass_or == 0) {
       cout << "WARNING :: No leptons passed overlap removal\n";
       return;
    }
    fake_iso_after_or_eff = n_fake_num_leps / n_fake_den_leps_pass_or;
    
    if (n_real_den_leps == 0) {
       cout << "WARNING :: No leptons passed denominator requirements\n";
       return;
    }
    real_or_eff = n_real_den_leps_pass_or / n_real_den_leps;
    real_iso_plus_or_eff = n_real_num_leps / n_real_den_leps;
    if (n_real_den_leps_pass_or == 0) {
       cout << "WARNING :: No leptons passed overlap removal\n";
       return;
    }
    real_iso_after_or_eff = n_real_num_leps / n_real_den_leps_pass_or;
  }

};
