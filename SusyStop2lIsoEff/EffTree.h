#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "math.h"
#include <TTree.h>
#include <TEfficiency.h>
#include <TH1F.h>

using std::vector;
using std::cout;
using std::string;

enum EleIDWP {tightLLH, mediumLLH, looseLLHBLayer, looseLLH, N_EL_IDWPs};
enum MuoIDWP {tight, medium, loose, veryLoose, N_MU_IDWPs};
enum IsoWP {isoGradient, isoGradientLoose, isoFixedCutTightTrackOnly, isoLoose, isoLooseTrackOnly, N_ISO_WPs};

/// List of options to be looped over
// Every combination of the options below is used
vector<int> el_ID_WP_ops = {looseLLHBLayer};
vector<float> el_pt_min_ops = {10.0};
vector<int> mu_ID_WP_ops =  {medium};
vector<float> mu_pt_min_ops = {10.0};
vector<int> el_iso_WP_ops = {isoGradientLoose};
vector<int> mu_iso_WP_ops = {isoGradientLoose};
vector<int> truth_matching_ops = {false};
vector<bool> j_e_bjet_or_ops = {true};
vector<bool> e_j_sliding_cone_or_ops= {false};
vector<bool> j_m_bjet_or_ops = {true};
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
  float fake_el_or_eff_unc_low = 0;  ///< fake efficency uncertainty of overlap removal
  float fake_el_or_eff_unc_up = 0;  ///< fake efficency uncertainty of overlap removal
  float fake_el_iso_plus_or_eff_unc_low = 0;  ///< fake efficency uncertainty of isolation and overlap removal combined
  float fake_el_iso_plus_or_eff_unc_up = 0;  ///< fake efficency uncertainty of isolation and overlap removal combined
  float fake_el_iso_after_or_eff_unc_low = 0;  ///< fake efficency uncertainty of isolation after overlap removal
  float fake_el_iso_after_or_eff_unc_up = 0;  ///< fake efficency uncertainty of isolation after overlap removal

  float n_fake_den_mu = 0;
  float n_fake_den_mu_pass_or = 0;
  float n_fake_num_mu = 0;
  float fake_mu_or_eff = -1;  ///< fake efficency of overlap removal
  float fake_mu_iso_plus_or_eff = -1;  ///< fake efficency of isolation and overlap removal combined
  float fake_mu_iso_after_or_eff = -1;  ///< fake efficency of isolation after overlap removal
  float fake_mu_or_eff_unc_low = 0;  ///< fake efficency uncertainty of overlap removal
  float fake_mu_or_eff_unc_up = 0;  ///< fake efficency uncertainty of overlap removal
  float fake_mu_iso_plus_or_eff_unc_low = 0;  ///< fake efficency uncertainty of isolation and overlap removal combined
  float fake_mu_iso_plus_or_eff_unc_up = 0;  ///< fake efficency uncertainty of isolation and overlap removal combined
  float fake_mu_iso_after_or_eff_unc_low = 0;  ///< fake efficency uncertainty of isolation after overlap removal
  float fake_mu_iso_after_or_eff_unc_up = 0;  ///< fake efficency uncertainty of isolation after overlap removal

  float n_real_den_el = 0;
  float n_real_den_el_pass_or = 0;
  float n_real_num_el = 0;
  float real_el_or_eff = -1;  ///< real efficency of overlap removal
  float real_el_iso_plus_or_eff = -1;  ///< real efficency of isolation and overlap removal combined
  float real_el_iso_after_or_eff = -1;  ///< real efficency of isolation after overlap removal
  float real_el_or_eff_unc_low = 0;  ///< real efficency uncertainty of overlap removal
  float real_el_or_eff_unc_up = 0;  ///< real efficency uncertainty of overlap removal
  float real_el_iso_plus_or_eff_unc_low = 0;  ///< real efficency uncertainty of isolation and overlap removal combined
  float real_el_iso_plus_or_eff_unc_up = 0;  ///< real efficency uncertainty of isolation and overlap removal combined
  float real_el_iso_after_or_eff_unc_low = 0;  ///< real efficency uncertainty of isolation after overlap removal
  float real_el_iso_after_or_eff_unc_up = 0;  ///< real efficency uncertainty of isolation after overlap removal

  float n_real_den_mu = 0;
  float n_real_den_mu_pass_or = 0;
  float n_real_num_mu = 0;
  float real_mu_or_eff = -1;  ///< real efficency of overlap removal
  float real_mu_iso_plus_or_eff = -1;  ///< real efficency of isolation and overlap removal combined
  float real_mu_iso_after_or_eff = -1;  ///< real efficency of isolation after overlap removal
  float real_mu_or_eff_unc_low = 0;  ///< real efficency uncertainty of overlap removal
  float real_mu_or_eff_unc_up = 0;  ///< real efficency uncertainty of overlap removal
  float real_mu_iso_plus_or_eff_unc_low = 0;  ///< real efficency uncertainty of isolation and overlap removal combined
  float real_mu_iso_plus_or_eff_unc_up = 0;  ///< real efficency uncertainty of isolation and overlap removal combined
  float real_mu_iso_after_or_eff_unc_low = 0;  ///< real efficency uncertainty of isolation after overlap removal
  float real_mu_iso_after_or_eff_unc_up = 0;  ///< real efficency uncertainty of isolation after overlap removal

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

    this->n_fake_den_el = rhs.n_fake_den_el;
    this->n_fake_den_el_pass_or = rhs.n_fake_den_el_pass_or;
    this->n_fake_num_el = rhs.n_fake_num_el;
    this->fake_el_or_eff = rhs.fake_el_or_eff;
    this->fake_el_iso_plus_or_eff = rhs.fake_el_iso_plus_or_eff;
    this->fake_el_iso_after_or_eff = rhs.fake_el_iso_after_or_eff;
    this->fake_el_or_eff_unc_low = rhs.fake_el_or_eff_unc_low;
    this->fake_el_or_eff_unc_up = rhs.fake_el_or_eff_unc_up;
    this->fake_el_iso_plus_or_eff_unc_low = rhs.fake_el_iso_plus_or_eff_unc_low;
    this->fake_el_iso_plus_or_eff_unc_up = rhs.fake_el_iso_plus_or_eff_unc_up;
    this->fake_el_iso_after_or_eff_unc_low = rhs.fake_el_iso_after_or_eff_unc_low;
    this->fake_el_iso_after_or_eff_unc_up = rhs.fake_el_iso_after_or_eff_unc_up;
    this->n_fake_den_mu = rhs.n_fake_den_mu;
    this->n_fake_den_mu_pass_or = rhs.n_fake_den_mu_pass_or;
    this->n_fake_num_mu = rhs.n_fake_num_mu;
    this->fake_mu_or_eff = rhs.fake_mu_or_eff;
    this->fake_mu_iso_plus_or_eff = rhs.fake_mu_iso_plus_or_eff;
    this->fake_mu_iso_after_or_eff = rhs.fake_mu_iso_after_or_eff;
    this->fake_mu_or_eff_unc_low = rhs.fake_mu_or_eff_unc_low;
    this->fake_mu_or_eff_unc_up = rhs.fake_mu_or_eff_unc_up;
    this->fake_mu_iso_plus_or_eff_unc_low = rhs.fake_mu_iso_plus_or_eff_unc_low;
    this->fake_mu_iso_plus_or_eff_unc_up = rhs.fake_mu_iso_plus_or_eff_unc_up;
    this->fake_mu_iso_after_or_eff_unc_low = rhs.fake_mu_iso_after_or_eff_unc_low;
    this->fake_mu_iso_after_or_eff_unc_up = rhs.fake_mu_iso_after_or_eff_unc_up;
    this->n_real_den_el = rhs.n_real_den_el;
    this->n_real_den_el_pass_or = rhs.n_real_den_el_pass_or;
    this->n_real_num_el = rhs.n_real_num_el;
    this->real_el_or_eff = rhs.real_el_or_eff;
    this->real_el_iso_plus_or_eff = rhs.real_el_iso_plus_or_eff;
    this->real_el_iso_after_or_eff = rhs.real_el_iso_after_or_eff;
    this->real_el_or_eff_unc_low = rhs.real_el_or_eff_unc_low;
    this->real_el_or_eff_unc_up = rhs.real_el_or_eff_unc_up;
    this->real_el_iso_plus_or_eff_unc_low = rhs.real_el_iso_plus_or_eff_unc_low;
    this->real_el_iso_plus_or_eff_unc_up = rhs.real_el_iso_plus_or_eff_unc_up;
    this->real_el_iso_after_or_eff_unc_low = rhs.real_el_iso_after_or_eff_unc_low;
    this->real_el_iso_after_or_eff_unc_up = rhs.real_el_iso_after_or_eff_unc_up;
    this->n_real_den_mu = rhs.n_real_den_mu;
    this->n_real_den_mu_pass_or = rhs.n_real_den_mu_pass_or;
    this->n_real_num_mu = rhs.n_real_num_mu;
    this->real_mu_or_eff = rhs.real_mu_or_eff;
    this->real_mu_iso_plus_or_eff = rhs.real_mu_iso_plus_or_eff;
    this->real_mu_iso_after_or_eff = rhs.real_mu_iso_after_or_eff;
    this->real_mu_or_eff_unc_low = rhs.real_mu_or_eff_unc_low;
    this->real_mu_or_eff_unc_up = rhs.real_mu_or_eff_unc_up;
    this->real_mu_iso_plus_or_eff_unc_low = rhs.real_mu_iso_plus_or_eff_unc_low;
    this->real_mu_iso_plus_or_eff_unc_up = rhs.real_mu_iso_plus_or_eff_unc_up;
    this->real_mu_iso_after_or_eff_unc_low = rhs.real_mu_iso_after_or_eff_unc_low;
    this->real_mu_iso_after_or_eff_unc_up = rhs.real_mu_iso_after_or_eff_unc_up;
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

    this->Branch ("n_fake_den_el", &n_fake_den_el);
    this->Branch ("n_fake_den_el_pass_or", &n_fake_den_el_pass_or);
    this->Branch ("n_fake_num_el", &n_fake_num_el);
    this->Branch ("fake_el_or_eff", &fake_el_or_eff);
    this->Branch ("fake_el_iso_plus_or_eff", &fake_el_iso_plus_or_eff);
    this->Branch ("fake_el_iso_after_or_eff", &fake_el_iso_after_or_eff);
    this->Branch ("fake_el_or_eff_unc_low", &fake_el_or_eff_unc_low);
    this->Branch ("fake_el_or_eff_unc_up", &fake_el_or_eff_unc_up);
    this->Branch ("fake_el_iso_plus_or_eff_unc_low", &fake_el_iso_plus_or_eff_unc_low);
    this->Branch ("fake_el_iso_plus_or_eff_unc_up", &fake_el_iso_plus_or_eff_unc_up);
    this->Branch ("fake_el_iso_after_or_eff_unc_low", &fake_el_iso_after_or_eff_unc_low);
    this->Branch ("fake_el_iso_after_or_eff_unc_up", &fake_el_iso_after_or_eff_unc_up);
    this->Branch ("n_fake_den_mu", &n_fake_den_mu);
    this->Branch ("n_fake_den_mu_pass_or", &n_fake_den_mu_pass_or);
    this->Branch ("n_fake_num_mu", &n_fake_num_mu);
    this->Branch ("fake_mu_or_eff", &fake_mu_or_eff);
    this->Branch ("fake_mu_iso_plus_or_eff", &fake_mu_iso_plus_or_eff);
    this->Branch ("fake_mu_iso_after_or_eff", &fake_mu_iso_after_or_eff);
    this->Branch ("fake_mu_or_eff_unc_low", &fake_mu_or_eff_unc_low);
    this->Branch ("fake_mu_or_eff_unc_up", &fake_mu_or_eff_unc_up);
    this->Branch ("fake_mu_iso_plus_or_eff_unc_low", &fake_mu_iso_plus_or_eff_unc_low);
    this->Branch ("fake_mu_iso_plus_or_eff_unc_up", &fake_mu_iso_plus_or_eff_unc_up);
    this->Branch ("fake_mu_iso_after_or_eff_unc_low", &fake_mu_iso_after_or_eff_unc_low);
    this->Branch ("fake_mu_iso_after_or_eff_unc_up", &fake_mu_iso_after_or_eff_unc_up);
    this->Branch ("n_real_den_el", &n_real_den_el);
    this->Branch ("n_real_den_el_pass_or", &n_real_den_el_pass_or);
    this->Branch ("n_real_num_el", &n_real_num_el);
    this->Branch ("real_el_or_eff", &real_el_or_eff);
    this->Branch ("real_el_iso_plus_or_eff", &real_el_iso_plus_or_eff);
    this->Branch ("real_el_iso_after_or_eff", &real_el_iso_after_or_eff);
    this->Branch ("real_el_or_eff_unc_low", &real_el_or_eff_unc_low);
    this->Branch ("real_el_or_eff_unc_up", &real_el_or_eff_unc_up);
    this->Branch ("real_el_iso_plus_or_eff_unc_low", &real_el_iso_plus_or_eff_unc_low);
    this->Branch ("real_el_iso_plus_or_eff_unc_up", &real_el_iso_plus_or_eff_unc_up);
    this->Branch ("real_el_iso_after_or_eff_unc_low", &real_el_iso_after_or_eff_unc_low);
    this->Branch ("real_el_iso_after_or_eff_unc_up", &real_el_iso_after_or_eff_unc_up);
    this->Branch ("n_real_den_mu", &n_real_den_mu);
    this->Branch ("n_real_den_mu_pass_or", &n_real_den_mu_pass_or);
    this->Branch ("n_real_num_mu", &n_real_num_mu);
    this->Branch ("real_mu_or_eff", &real_mu_or_eff);
    this->Branch ("real_mu_iso_plus_or_eff", &real_mu_iso_plus_or_eff);
    this->Branch ("real_mu_iso_after_or_eff", &real_mu_iso_after_or_eff);
    this->Branch ("real_mu_or_eff_unc_low", &real_mu_or_eff_unc_low);
    this->Branch ("real_mu_or_eff_unc_up", &real_mu_or_eff_unc_up);
    this->Branch ("real_mu_iso_plus_or_eff_unc_low", &real_mu_iso_plus_or_eff_unc_low);
    this->Branch ("real_mu_iso_plus_or_eff_unc_up", &real_mu_iso_plus_or_eff_unc_up);
    this->Branch ("real_mu_iso_after_or_eff_unc_low", &real_mu_iso_after_or_eff_unc_low);
    this->Branch ("real_mu_iso_after_or_eff_unc_up", &real_mu_iso_after_or_eff_unc_up);
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
    cout << "\t Fake Electron:\n";
    cout << "\t\t n_fake_den_el = " << n_fake_den_el << '\n';
    cout << "\t\t n_fake_den_el_pass_or = " << n_fake_den_el_pass_or << '\n';
    cout << "\t\t n_fake_num_el = " << n_fake_num_el << '\n';
    cout << "\t\t fake_el_or_eff = " << fake_el_or_eff << '\n';
    cout << "\t\t fake_el_iso_plus_or_eff = " << fake_el_iso_plus_or_eff << '\n';
    cout << "\t\t fake_el_iso_after_or_eff = " << fake_el_iso_after_or_eff << '\n';
    cout << "\t\t fake_el_or_eff = " << fake_el_or_eff << " +/- " << fake_el_or_eff_unc_up << "," << fake_el_or_eff_unc_low << '\n';
    cout << "\t\t fake_el_iso_plus_or_eff = " << fake_el_iso_plus_or_eff << " +/- " << fake_el_iso_plus_or_eff_unc_up << "," << fake_el_iso_plus_or_eff_unc_low << '\n';
    cout << "\t\t fake_el_iso_after_or_eff = " << fake_el_iso_after_or_eff << " +/- " << fake_el_iso_after_or_eff_unc_up << "," << fake_el_iso_after_or_eff_unc_low << '\n';

    cout << "\t Fake Muon:\n";
    cout << "\t\t n_fake_den_mu = " << n_fake_den_mu << '\n';
    cout << "\t\t n_fake_den_mu_pass_or = " << n_fake_den_mu_pass_or << '\n';
    cout << "\t\t n_fake_num_mu = " << n_fake_num_mu << '\n';
    cout << "\t\t fake_mu_or_eff = " << fake_mu_or_eff << '\n';
    cout << "\t\t fake_mu_iso_plus_or_eff = " << fake_mu_iso_plus_or_eff << '\n';
    cout << "\t\t fake_mu_iso_after_or_eff = " << fake_mu_iso_after_or_eff << '\n';
    cout << "\t\t fake_mu_or_eff = " << fake_mu_or_eff << " +/- " << fake_mu_or_eff_unc_up << "," << fake_mu_or_eff_unc_low << '\n';
    cout << "\t\t fake_mu_iso_plus_or_eff = " << fake_mu_iso_plus_or_eff << " +/- " << fake_mu_iso_plus_or_eff_unc_up << "," << fake_mu_iso_plus_or_eff_unc_low << '\n';
    cout << "\t\t fake_mu_iso_after_or_eff = " << fake_mu_iso_after_or_eff << " +/- " << fake_mu_iso_after_or_eff_unc_up << "," << fake_mu_iso_after_or_eff_unc_low << '\n';

    cout << "\t Real Electron:\n";
    cout << "\t\t n_real_den_el = " << n_real_den_el << '\n';
    cout << "\t\t n_real_den_el_pass_or = " << n_real_den_el_pass_or << '\n';
    cout << "\t\t n_real_num_el = " << n_real_num_el << '\n';
    cout << "\t\t real_el_or_eff = " << real_el_or_eff << '\n';
    cout << "\t\t real_el_iso_plus_or_eff = " << real_el_iso_plus_or_eff << '\n';
    cout << "\t\t real_el_iso_after_or_eff = " << real_el_iso_after_or_eff << '\n';
    cout << "\t\t real_el_or_eff = " << real_el_or_eff << " +/- " << real_el_or_eff_unc_up << "," << real_el_or_eff_unc_low << '\n';
    cout << "\t\t real_el_iso_plus_or_eff = " << real_el_iso_plus_or_eff << " +/- " << real_el_iso_plus_or_eff_unc_up << "," << real_el_iso_plus_or_eff_unc_low << '\n';
    cout << "\t\t real_el_iso_after_or_eff = " << real_el_iso_after_or_eff << " +/- " << real_el_iso_after_or_eff_unc_up << "," << real_el_iso_after_or_eff_unc_low << '\n';

    cout << "\t Real Muon:\n";
    cout << "\t\t n_real_den_mu = " << n_real_den_mu << '\n';
    cout << "\t\t n_real_den_mu_pass_or = " << n_real_den_mu_pass_or << '\n';
    cout << "\t\t n_real_num_mu = " << n_real_num_mu << '\n';
    cout << "\t\t real_mu_or_eff = " << real_mu_or_eff << '\n';
    cout << "\t\t real_mu_iso_plus_or_eff = " << real_mu_iso_plus_or_eff << '\n';
    cout << "\t\t real_mu_iso_after_or_eff = " << real_mu_iso_after_or_eff << '\n';
    cout << "\t\t real_mu_or_eff = " << real_mu_or_eff << " +/- " << real_mu_or_eff_unc_up << "," << real_mu_or_eff_unc_low << '\n';
    cout << "\t\t real_mu_iso_plus_or_eff = " << real_mu_iso_plus_or_eff << " +/- " << real_mu_iso_plus_or_eff_unc_up << "," << real_mu_iso_plus_or_eff_unc_low << '\n';
    cout << "\t\t real_mu_iso_after_or_eff = " << real_mu_iso_after_or_eff << " +/- " << real_mu_iso_after_or_eff_unc_up << "," << real_mu_iso_after_or_eff_unc_low << '\n';
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
    calculateEffAndUnc(n_fake_den_el_pass_or, n_fake_den_el, fake_el_or_eff, fake_el_or_eff_unc_up, fake_el_or_eff_unc_low);
    calculateEffAndUnc(n_fake_num_el, n_fake_den_el, fake_el_iso_plus_or_eff, fake_el_iso_plus_or_eff_unc_up, fake_el_iso_plus_or_eff_unc_low);
    calculateEffAndUnc(n_fake_num_el, n_fake_den_el_pass_or, fake_el_iso_after_or_eff, fake_el_iso_after_or_eff_unc_up, fake_el_iso_after_or_eff_unc_low);
    
    calculateEffAndUnc(n_fake_den_mu_pass_or, n_fake_den_mu, fake_mu_or_eff, fake_mu_or_eff_unc_up, fake_mu_or_eff_unc_low);
    calculateEffAndUnc(n_fake_num_mu, n_fake_den_mu, fake_mu_iso_plus_or_eff, fake_mu_iso_plus_or_eff_unc_up, fake_mu_iso_plus_or_eff_unc_low);
    calculateEffAndUnc(n_fake_num_mu, n_fake_den_mu_pass_or, fake_mu_iso_after_or_eff, fake_mu_iso_after_or_eff_unc_up, fake_mu_iso_after_or_eff_unc_low);
    
    calculateEffAndUnc(n_real_den_el_pass_or, n_real_den_el, real_el_or_eff, real_el_or_eff_unc_up, real_el_or_eff_unc_low);
    calculateEffAndUnc(n_real_num_el, n_real_den_el, real_el_iso_plus_or_eff, real_el_iso_plus_or_eff_unc_up, real_el_iso_plus_or_eff_unc_low);
    calculateEffAndUnc(n_real_num_el, n_real_den_el_pass_or, real_el_iso_after_or_eff, real_el_iso_after_or_eff_unc_up, real_el_iso_after_or_eff_unc_low);
    
    calculateEffAndUnc(n_real_den_mu_pass_or, n_real_den_mu, real_mu_or_eff, real_mu_or_eff_unc_up, real_mu_or_eff_unc_low);
    calculateEffAndUnc(n_real_num_mu, n_real_den_mu, real_mu_iso_plus_or_eff, real_mu_iso_plus_or_eff_unc_up, real_mu_iso_plus_or_eff_unc_low);
    calculateEffAndUnc(n_real_num_mu, n_real_den_mu_pass_or, real_mu_iso_after_or_eff, real_mu_iso_after_or_eff_unc_up, real_mu_iso_after_or_eff_unc_low);

    //calculateEff("fake electrons",
    //              n_fake_den_el, n_fake_den_el_pass_or, n_fake_num_el,
    //              fake_el_or_eff, fake_el_iso_plus_or_eff, fake_el_iso_after_or_eff,
    //              fake_el_or_eff_unc_up, fake_el_iso_plus_or_eff_unc_up, fake_el_iso_after_or_eff_unc_up,
    //              fake_el_or_eff_unc_low, fake_el_iso_plus_or_eff_unc_low, fake_el_iso_after_or_eff_unc_low);
    //calculateEff("fake muons",
    //              n_fake_den_mu, n_fake_den_mu_pass_or, n_fake_num_mu,
    //              fake_mu_or_eff, fake_mu_iso_plus_or_eff, fake_mu_iso_after_or_eff,
    //              fake_mu_or_eff_unc_up, fake_mu_iso_plus_or_eff_unc_up, fake_mu_iso_after_or_eff_unc_up,
    //              fake_mu_or_eff_unc_low, fake_mu_iso_plus_or_eff_unc_low, fake_mu_iso_after_or_eff_unc_low);
    //calculateEff("real electrons",
    //              n_real_den_el, n_real_den_el_pass_or, n_real_num_el,
    //              real_el_or_eff, real_el_iso_plus_or_eff, real_el_iso_after_or_eff,
    //              real_el_or_eff_unc_up, real_el_iso_plus_or_eff_unc_up, real_el_iso_after_or_eff_unc_up,
    //              real_el_or_eff_unc_low, real_el_iso_plus_or_eff_unc_low, real_el_iso_after_or_eff_unc_low);
    //calculateEff("real muons",
    //              n_real_den_mu, n_real_den_mu_pass_or, n_real_num_mu,
    //              real_mu_or_eff, real_mu_iso_plus_or_eff, real_mu_iso_after_or_eff,
    //              real_mu_or_eff_unc_up, real_mu_iso_plus_or_eff_unc_up, real_mu_iso_after_or_eff_unc_up,
    //              real_mu_or_eff_unc_low, real_mu_iso_plus_or_eff_unc_low, real_mu_iso_after_or_eff_unc_low);
  }
  void calculateEff(string lepType,
                    const float& n_den,
                    const float& n_den_pass_or,
                    const float& n_num,
                    float& or_eff,
                    float& iso_plus_or_eff,
                    float& iso_after_or_eff,
                    float& or_eff_unc_up,
                    float& iso_plus_or_eff_unc_up,
                    float& iso_after_or_eff_unc_up,
                    float& or_eff_unc_low,
                    float& iso_plus_or_eff_unc_low,
                    float& iso_after_or_eff_unc_low) {
      
    if (n_den > 0) {
      or_eff = n_den_pass_or / n_den;
      iso_plus_or_eff = n_num / n_den;
    } else {
       cout << "WARNING :: No " << lepType << " passed denominator requirements\n";
       or_eff = 1;
       iso_plus_or_eff = 1;
    }
    if (n_den_pass_or > 0) {
      iso_after_or_eff = n_num / n_den_pass_or;
    } else {
       cout << "WARNING :: No " << lepType << " passed overlap removal\n";
       iso_after_or_eff = 1;
    }
  }
  void calculateEffAndUnc(uint n_num, uint n_den, float& eff, float& eff_unc_up, float& eff_unc_low) {
    TH1F h_num("h_num","",1,0,1);
    h_num.SetBinContent(1, n_num);
    TH1F h_den("h_den","",1,0,1);
    h_den.SetBinContent(1, n_den);
    
    TEfficiency h_eff(h_num, h_den);
    h_eff.SetStatisticOption(TEfficiency::kFAC);
    eff = h_eff.GetEfficiency(1);
    eff_unc_up = h_eff.GetEfficiencyErrorUp(1);
    eff_unc_low = h_eff.GetEfficiencyErrorLow(1);
  }

};
