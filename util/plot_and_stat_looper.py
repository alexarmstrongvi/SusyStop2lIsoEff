#!/bin/env python

import ROOT as r
r.gROOT.SetBatch(True)
r.gStyle.SetOptStat(False)

ifile_name = "../run/isoEff.root"
#ifile_name = "../run/isoEff_signal_389945_bWN_250_160_10000.root"
#ifile_name = "../run/isoEff_signal_389949_bWN_300_150_10000.root"
#ifile_name = "../run/isoEff_signal_389988_bffN_300_250_10000.root"
ttree_name = "histTree_conf1"
plot_dir = "./"
save_fmt = "pdf"

base_sel = "1"
weight = 'eventweight'

ifile = r.TFile(ifile_name,"r")
ttree = ifile.Get(ttree_name)


jet_types = []#'Jets','LJets','BJets']
vars_to_print = {
    'LepPt'        : r.TH1F("LepPt","Title; pT(lep); nLeptons",50,0,200),
    #'LepPt[0]'        : r.TH1F("LepPt0","Title; pT(l0); Events",50,0,200),
    #'LepPt[1]'        : r.TH1F("LepPt1","Title; pT(l1); Events",50,0,200),
    #'LepPt[2]'        : r.TH1F("LepPt2","Title; pT(l2); Events",50,0,200),
    ##'LepPt[3]'        : r.TH1F("LepPt3","Title; pT(l3); Events",50,0,200),
    ##'LepPt[4]'        : r.TH1F("LepPt4","Title; pT(l4); Events",50,0,200),
    'LepEta'        : r.TH1F("LepEta","Title; #eta(lep); nLeptons",50,-3,3),
    #'LepEta[0]'        : r.TH1F("LepEta0","Title; #eta(l0); Events",50,-3,3),
    #'LepEta[1]'        : r.TH1F("LepEta1","Title; #eta(l1); Events",50,-3,3),
    #'LepEta[2]'        : r.TH1F("LepEta2","Title; #eta(l2); Events",50,-3,3),
    ##'LepEta[3]'        : r.TH1F("LepEta3","Title; #eta(l3); Events",50,-3,3),
    ##'LepEta[4]'        : r.TH1F("LepEta4","Title; #eta(l4); Events",50,-3,3),
    'dR_LepClosestJet'        : r.TH1F("dR_LepClosestJet","Title; #DeltaR(lep,closest jet); nLeptons",60,0,6),
    #'dR_LepClosestJet[0]'        : r.TH1F("dR_Lep0ClosestJet","Title; #DeltaR(lep0,closest jet); nLeptons",60,0,6),
    #'dR_LepClosestJet[1]'        : r.TH1F("dR_Lep1ClosestJet","Title; #DeltaR(lep1,closest jet); nLeptons",60,0,6),
    #'dR_LepClosestJet[2]'        : r.TH1F("dR_Lep2ClosestJet","Title; #DeltaR(lep2,closest jet); nLeptons",60,0,6),
    ##'dR_LepClosestJet[3]'        : r.TH1F("dR_Lep3ClosestJet","Title; #DeltaR(lep3,closest jet); nLeptons",60,0,6),
    'LepTruthClass'        : r.TH1F("LepTruthClass","Title; Lepton Truth Class; nLeptons",16,-4.5,11.5),
    #'LepTruthClass[0]'        : r.TH1F("Lep0TruthClass","Title; Lepton0 Truth Class; Events",16,-4.5,11.5),
    #'LepTruthClass[1]'        : r.TH1F("Lep1TruthClass","Title; Lepton1 Truth Class; Events",16,-4.5,11.5),
    'JetPt'        : r.TH1F("JetPt","Title; pT(jets); nJets",50,0,250),
    #'JetPt[0]'        : r.TH1F("JetPt0","Title; pt(j0); Events",50,0,250),
    #'JetPt[1]'        : r.TH1F("JetPt1","Title; pt(j1); Events",50,0,250),
    #'JetPt[2]'        : r.TH1F("JetPt2","Title; pt(j2); Events",50,0,250),
    ##'JetPt[3]'        : r.TH1F("JetPt3","Title; pt(j3); Events",50,0,250),
    ##'JetPt[4]'        : r.TH1F("JetPt4","Title; pt(j4); Events",50,0,250),
    'JetEta'        : r.TH1F("JetEta","Title; #eta(jets); nJets",50,-5,5),
    #'JetEta[0]'        : r.TH1F("JetEta0","Title; #eta(j0); Events",50,-5,5),
    #'JetEta[1]'        : r.TH1F("JetEta1","Title; #eta(j1); Events",50,-5,5),
    #'JetEta[2]'        : r.TH1F("JetEta2","Title; #eta(j2); Events",50,-5,5),
    ##'JetEta[3]'        : r.TH1F("JetEta3","Title; #eta(j3); Events",50,-5,5),
    ##'JetEta[4]'        : r.TH1F("JetEta4","Title; #eta(j4); Events",50,-5,5),
    'dR_JetClosestLep'        : r.TH1F("dR_JetClosestLep","Title; #DeltaR(jet,closest lep); nJets",60,0,6),
    #'dR_JetClosestLep[0]'        : r.TH1F("dR_Jet0ClosestLep","Title; #DeltaR(jet0,closest lep); nJets",60,0,6),
    #'dR_JetClosestLep[1]'        : r.TH1F("dR_Jet1ClosestLep","Title; #DeltaR(jet1,closest lep); nJets",60,0,6),
    #'dR_JetClosestLep[2]'        : r.TH1F("dR_Jet2ClosestLep","Title; #DeltaR(jet2,closest lep); nJets",60,0,6),
    #'dR_JetClosestLep[3]'        : r.TH1F("dR_Jet3ClosestLep","Title; #DeltaR(jet3,closest lep); nJets",60,0,6),
    'JetTruth'        : r.TH1F("JetTruth","Title; Jet Truth Class; nLeptons",18,-1.5,17.5),
}
el_tuple = ('LepIsEle','1==LepTruthClass', 'RealEle')
fel_tuple = ('LepIsEle','1!=LepTruthClass', 'FakeEle')
mu_tuple = ('!LepIsEle','2==LepTruthClass', 'RealMu')
fmu_tuple = ('!LepIsEle','2!=LepTruthClass', 'FakeMu')
bjet_tuple = ('JetBTagged','5==JetTruth', 'RealBJet')
fbjet_tuple = ('JetBTagged','5!=JetTruth', 'FakeBJet')
ljet_tuple = ('!JetBTagged','5!=JetTruth', 'RealLJet')
fljet_tuple = ('!JetBTagged','5==JetTruth', 'FakeLJet')

lep_list = [el_tuple, fel_tuple, mu_tuple, fmu_tuple]
jet_list = [bjet_tuple, fbjet_tuple, ljet_tuple, fljet_tuple]

c1 = r.TCanvas("c1","",800,600)
c1.Divide(1,2)
p2 = c1.cd(2)
p2.SetPad(0.0, 0.05, 1.0, 0.4)
p2.SetTopMargin(0.01)
p2.SetBottomMargin(0.2)
p1 = c1.cd(1)
p1.SetPad(0.0, 0.4, 1.0, 1.0)
p1.SetBottomMargin(0.01)
c1.SetLeftMargin(0.1)
c1.SetBottomMargin(0.01)

def isLepVar(var):
    return var.startswith('Lep') or var.startswith('dR_Lep')


for var, hist in vars_to_print.iteritems():
    hist.Sumw2()
    flavor_sels =  lep_list if isLepVar(var) else jet_list
    
    #if 'dR' in var: c1.SetLogy(True)
    #else: c1.SetLogy(False)
    c1.cd(1).SetLogy(True)
    
    for flavor_sel, flavor_truth, flavor_name in flavor_sels:
        pass_or = 'LepPassOR' if isLepVar(var) else 'JetPassOR'
        pass_signal = 'LepPassSignal' if isLepVar(var) else 'JetPassSignal'
        if var.endswith("]"):
            idx = var[-3:]
            flavor_sel += idx
            flavor_truth += idx
            pass_or += idx
            pass_signal += idx
        flavor_sel += " && " + flavor_truth 

        den_sel = base_sel + " && " + flavor_sel
        or_sel = base_sel + " && " + pass_or + " && " + flavor_sel
        sig_sel = base_sel + " && " + pass_signal + " && " + flavor_sel
        
        xax = hist.GetXaxis()
        xax.SetTitleOffset(999)
        yax = hist.GetYaxis()
        yax.SetTitleOffset(0.6)
        yax.SetTitleSize(0.06)
        or_hist = hist.Clone(hist.GetName()+"_or")
        sig_hist = hist.Clone(hist.GetName()+"_sig")
        
        draw_cmd = "%s >> %s" % (var, hist.GetName())
        ttree.Draw(draw_cmd, '(%s) * %s' % (den_sel, weight))
        draw_cmd = "%s >> %s" % (var, or_hist.GetName())
        ttree.Draw(draw_cmd, '(%s) * %s' % (or_sel, weight))
        draw_cmd = "%s >> %s" % (var, sig_hist.GetName())
        ttree.Draw(draw_cmd, '(%s) * %s' % (sig_sel, weight))

        or_eff = r.TEfficiency(or_hist, hist)
        sig_eff = r.TEfficiency(sig_hist, or_hist)

        #if isinstance(hist, r.TH2F):
        #    c1.SetRightMargin(0.20)
        #    hist.Draw("colz")
        c1.SetRightMargin(0.15)
        hist.SetLineColor(r.kBlack)
        hist.SetFillColor(r.kGray)
        hist.SetLineWidth(5)
        hist.SetMinimum(0.1)
        or_hist.SetLineColor(r.kRed)
        or_eff.SetLineColor(r.kRed)
        or_hist.SetFillColor(r.kRed-10)
        or_hist.SetLineWidth(3)
        sig_hist.SetLineColor(r.kGreen+3)
        sig_eff.SetLineColor(r.kGreen+3)
        sig_hist.SetFillColor(r.kGreen-10)
        sig_hist.SetLineWidth(1)
        
        c1.cd(1)
        hist.Draw("hist")
        or_hist.Draw("hist same")
        sig_hist.Draw("hist same")
        hist.SetTitle("Baseline->OR->Signal Efficiency (%s)" % flavor_name)
        c1.RedrawAxis()
        
        p2 = c1.cd(2)

        mg = r.TMultiGraph('mg',';%s;Efficiency' % hist.GetXaxis().GetTitle())
        or_eff_graph = or_eff.CreateGraph()
        mg.Add(or_eff_graph, 'P')
        sig_eff_graph = sig_eff.CreateGraph()
        mg.Add(sig_eff_graph, 'P')

        mg.Draw('AP')
        xmin = hist.GetXaxis().GetXmin()
        xmax = hist.GetXaxis().GetXmax()
        mg.GetXaxis().SetLimits(xmin, xmax)
        mg_hist = mg.GetHistogram()
        mg_hist.SetMinimum(-0.1)
        mg_hist.SetMaximum(1.1)
        mg_hist.SetLabelSize(0.08,'X')
        mg_hist.SetLabelSize(0.08,'Y')
        mg_hist.SetTitleSize(0.1,'X')
        mg_hist.SetTitleSize(0.1,'Y')
        mg_hist.SetTitleOffset(1,'X')
        mg_hist.SetTitleOffset(0.4,'Y')
        p2.Modified()
        p2.Update()

        save_name = plot_dir + hist.GetName()+"_" + flavor_name + "." + save_fmt
        c1.cd(1)
        c1.SaveAs(save_name)
        

for jet_type in jet_types:
    var = 'nDen' + jet_type 
    or_var = 'nDenPassOR' + jet_type
    sig_var = 'nNum' + jet_type
    hist = r.TH1F(var,'Title; n'+jet_type+'; Events',16,-0.5,15.5)
    xax = hist.GetXaxis()
    xax.SetTitleOffset(1.5)
    yax = hist.GetYaxis()
    yax.SetTitleOffset(1.5)
    
    or_hist = hist.Clone(hist.GetName()+"_or")
    sig_hist = hist.Clone(hist.GetName()+"_sig")

    draw_cmd = "%s >> %s" % (var, hist.GetName())
    ttree.Draw(draw_cmd, '(%s) * %s' % (base_sel, weight))
    draw_cmd = "%s >> %s" % (or_var, or_hist.GetName())
    ttree.Draw(draw_cmd, '(%s) * %s' % (base_sel, weight))
    draw_cmd = "%s >> %s" % (sig_var, sig_hist.GetName())
    ttree.Draw(draw_cmd, '(%s) * %s' % (base_sel, weight))

    c1.SetRightMargin(0.15)
    scale_factor = 1.0/ttree.GetEntries();
    sig_hist.SetLineColor(r.kGreen+3)
    sig_hist.Scale(scale_factor);
    sig_hist.SetLineWidth(1)
    sig_hist.Draw("hist")
    hist.SetLineColor(r.kBlack)
    hist.SetLineWidth(8)
    hist.Scale(scale_factor);
    hist.Draw("hist same")
    or_hist.SetLineColor(r.kRed)
    or_hist.SetLineWidth(4)
    or_hist.Scale(scale_factor);
    or_hist.Draw("hist same")
    sig_hist.Draw("hist same")
    sig_hist.SetTitle("Baseline->OR->Signal Efficiency (%s)" % jet_type)


    save_name = plot_dir + "n" + jet_type + "." + save_fmt
    c1.RedrawAxis()
    c1.SaveAs(save_name)

h_LepIsEle = r.TH1F("h_LepIsEle","",2,-0.5,1.5)
h_LepTruthClass = r.TH1F("h_LepTruthClass","",14,-3.5,10.5)
h_JetTruth = r.TH1F("h_JetTruth","",16,-0.5,15.5)

# Initialize counters
n_leps = 0
n_el_rm = 0
n_mu_rm = 0
n_el_rm_by_btag = 0
n_mu_rm_by_btag = 0
n_true_el_rm = 0
n_true_mu_rm = 0
n_bjet_killas = 0
n_trueB_killas = 0
n_trueC_killas = 0
n_trueT_killas = 0
n_trueL_killas = 0

for ilep in range(0):
    ttree.Draw("LepIsEle[%d] >> h_LepIsEle" % ilep, base_sel)
    n_leps += h_LepIsEle.Integral(0,-1)

    sel_str = "dR_LepClosestJet[%d] < 0.2 && LepPassEMuOR[%d] && !LepPassOR[%d]" % (ilep, ilep, ilep)
    ttree.Draw("LepIsEle[%d] >> h_LepIsEle" % ilep, base_sel + " && " + sel_str)
    n_el_rm += h_LepIsEle.GetBinContent(2)
    n_mu_rm += h_LepIsEle.GetBinContent(1) 

    sel_str += " && dR_LepClosestJet[%d] < 0.2 && JetPassOR[dR_ClosestJetIdx[%d]] && JetBTagged[dR_ClosestJetIdx[%d]]" % (ilep, ilep, ilep)
    ttree.Draw("LepIsEle[%d] >> h_LepIsEle" % ilep, base_sel + " && " + sel_str)
    print base_sel + " && " + sel_str
    n_el_rm_by_btag += h_LepIsEle.GetBinContent(2)
    n_mu_rm_by_btag += h_LepIsEle.GetBinContent(1) 

    ttree.Draw("LepTruthClass[%d] >> h_LepTruthClass" % ilep, base_sel + " && " + sel_str + " && LepIsEle[%d]" % ilep)
    print base_sel + " && " + sel_str + " && LepIsEle[%d]" % ilep
    n_true_el_rm += h_LepTruthClass.GetBinContent(1 + 4)
    
    ttree.Draw("LepTruthClass[%d] >> h_LepTruthClass" % ilep, base_sel + " && " + sel_str + " && !LepIsEle[%d]" % ilep)
    print base_sel + " && " + sel_str + " && !LepIsEle[%d]" % ilep
    n_true_mu_rm += h_LepTruthClass.GetBinContent(2 + 4) 

    ttree.Draw("JetTruth[dR_ClosestJetIdx[%d]] >> h_JetTruth" % ilep, base_sel + " && " + sel_str)
    #n_bjet_killas += h_JetTruth.Integral(0,-1)
    n_trueB_killas += h_JetTruth.GetBinContent(5 + 1)
    n_trueC_killas += h_JetTruth.GetBinContent(4 + 1)
    n_trueT_killas += h_JetTruth.GetBinContent(15 + 1)
    n_trueL_killas += h_JetTruth.GetBinContent(0 + 1)

def stat_print(msg, x, total):
    perc = "%.1f%%" % (x*100 / float(total)) if total else '-'
    print "%s: %d (%s)" % (msg, x, perc)

print "Statistics summary"
print "File name:", ifile_name
stat_print("Leptons removed by jets in OR", n_el_rm + n_mu_rm  , n_leps)
stat_print("\tLeptons removed by B-tagged jets in OR", n_el_rm_by_btag + n_mu_rm_by_btag, n_el_rm + n_mu_rm)
stat_print("\t\tElectrons", n_el_rm_by_btag, n_el_rm_by_btag + n_mu_rm_by_btag)
stat_print("\t\t\tTruth Matched Electrons", n_true_el_rm, n_el_rm_by_btag)
stat_print("\t\tMuons", n_mu_rm_by_btag, n_el_rm_by_btag + n_mu_rm_by_btag)
stat_print("\t\t\tTruth Matched Muons", n_true_mu_rm, n_mu_rm_by_btag)
stat_print("\t\tB-tagged jets truth matched to B hadrons", n_trueB_killas, n_el_rm_by_btag + n_mu_rm_by_btag)
stat_print("\t\tB-tagged jets truth matched to C hadrons", n_trueC_killas, n_el_rm_by_btag + n_mu_rm_by_btag)
stat_print("\t\tB-tagged jets truth matched to Tau jets", n_trueT_killas, n_el_rm_by_btag + n_mu_rm_by_btag)
stat_print("\t\tB-tagged jets truth matched to light hadrons", n_trueL_killas, n_el_rm_by_btag + n_mu_rm_by_btag)


