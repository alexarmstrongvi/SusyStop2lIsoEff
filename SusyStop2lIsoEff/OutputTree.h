#pragma once
#include <TTree.h>
#include <vector>
#include <iostream>
#include "math.h"

using std::vector;
using std::cout;

enum EleIDWP {tightLLH, mediumLLH, looseLLHBLayer, looseLLH, N_EL_IDWPs};
enum MuoIDWP {tight, medium, loose, veryLoose, N_MU_IDWPs};
enum IsoWP {isoGradient, isoGradientLoose, isoLoose, isoLooseTrackOnly, isoFixedCutTightTrackOnly, N_ISO_WPs};

/// List of options to be looped over
// Every combination of the options below is used
vector<int> el_ID_WP_ops = {looseLLHBLayer};
vector<float> el_pt_min_ops = {10.0};
vector<int> mu_ID_WP_ops =  {medium};
vector<float> mu_pt_min_ops = {10.0};
vector<int> el_iso_WP_ops = {isoGradient, isoGradientLoose, isoLoose, isoLooseTrackOnly, isoFixedCutTightTrackOnly,};
vector<int> mu_iso_WP_ops = {isoGradient};
vector<bool> j_e_bjet_or_ops = {true};
vector<bool> j_m_bjet_or_ops = {true};

class OutputTree : public TTree
{
public:
  // Constructor
  OutputTree(const char* name, const char* title)
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
  bool j_m_bjet_or = true;

  // Isolation WPs
  int el_iso_WP = N_ISO_WPs;  
  int mu_iso_WP = N_ISO_WPs;  
  //float etconetopo20_max = FLT_MAX;
  //float etconetopo30_max = FLT_MAX;
  //float ptvarcone20_max = FLT_MAX;

  // Result
  float n_den_leps = 0;
  float n_den_leps_pass_or = 0;
  float n_num_leps = 0;

  float iso_plus_or_eff = -1;  ///< efficency of isolation and overlap removal combined
  float iso_plus_or_rej = -1;
  float iso_after_or_eff = -1;  ///< efficency of isolation after overlap removal
  float iso_after_or_rej = -1;

  OutputTree& operator=(const OutputTree& rhs) {
    this->config_id = rhs.config_id;
    this->el_ID_WP = rhs.el_ID_WP;
    this->el_pt_min = rhs.el_pt_min;
    this->mu_ID_WP = rhs.mu_ID_WP;
    this->mu_pt_min = rhs.mu_pt_min;
    this->j_e_bjet_or = rhs.j_e_bjet_or;
    this->j_m_bjet_or = rhs.j_m_bjet_or;
    this->el_iso_WP = rhs.el_iso_WP;
    this->mu_iso_WP = rhs.mu_iso_WP;
    this->n_den_leps = rhs.n_den_leps;
    this->n_den_leps_pass_or = rhs.n_den_leps_pass_or;
    this->n_num_leps = rhs.n_num_leps;
    this->iso_plus_or_eff = rhs.iso_plus_or_eff;
    this->iso_plus_or_rej = rhs.iso_plus_or_rej;
    this->iso_after_or_eff = rhs.iso_after_or_eff;
    this->iso_after_or_rej = rhs.iso_after_or_rej;
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
    this->Branch ("j_m_bjet_or", &j_m_bjet_or);
    this->Branch ("el_iso_WP", &el_iso_WP);
    this->Branch ("mu_iso_WP", &mu_iso_WP);
    this->Branch ("iso_plus_or_eff", &iso_plus_or_eff);
    this->Branch ("iso_plus_or_rej", &iso_plus_or_rej);
    this->Branch ("iso_after_or_eff", &iso_after_or_eff);
    this->Branch ("iso_after_or_rej", &iso_after_or_rej);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Print variables 
  void PrintConf() const {
    cout << "Configuration Options:\n";
    cout << "\t config_id = " << config_id << "\n";
    cout << "\t el_ID_WP = " << el_ID_WP << "\n";
    cout << "\t el_pt_min = " << el_pt_min << "\n";
    cout << "\t mu_ID_WP = " << mu_ID_WP << "\n";
    cout << "\t mu_pt_min = " << mu_pt_min << "\n";
    cout << "\t j_e_bjet_or = " << j_e_bjet_or << "\n";
    cout << "\t j_m_bjet_or = " << j_m_bjet_or << "\n";
    cout << "\t el_iso_WP = " << el_iso_WP << "\n";
    cout << "\t mu_iso_WP = " << mu_iso_WP << "\n";
  }
  void PrintResults() const {
    cout << "Results:\n";
    cout << "\t n_den_leps = " << n_den_leps << '\n';
    cout << "\t n_den_leps_pass_or = " << n_den_leps_pass_or << '\n';
    cout << "\t n_num_leps = " << n_num_leps << '\n';
    cout << "\t iso_plus_or_eff = " << iso_plus_or_eff << '\n';
    cout << "\t iso_plus_or_rej = " << iso_plus_or_rej << '\n';
    cout << "\t iso_after_or_eff = " << iso_after_or_eff << '\n';
    cout << "\t iso_after_or_rej = " << iso_after_or_rej << '\n';
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
  ~OutputTree() {
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Useful functions
  void calculateResults() {
    if (n_den_leps == 0) {
       cout << "WARNING :: No leptons passed denominator requirements\n";
       return;
    }
    iso_plus_or_eff = n_num_leps / n_den_leps;
    iso_plus_or_rej = 1.0 - iso_plus_or_eff;
    if (n_den_leps_pass_or == 0) {
       cout << "WARNING :: No leptons passed overlap removal\n";
       return;
    }
    iso_after_or_eff = n_num_leps / n_den_leps_pass_or;
    iso_after_or_rej = 1 - iso_after_or_eff; 
  }

};
