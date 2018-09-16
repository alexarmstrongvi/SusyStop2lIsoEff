#!/bin/env python

import ROOT as r
r.gROOT.SetBatch(True)
r.gStyle.SetOptStat(False)

ifile_name = "../run/isoEff.root"
#ifile_name = "../run/isoEff_signal_389945_bWN_250_160_10000.root"
#ifile_name = "../run/isoEff_signal_389949_bWN_300_150_10000.root"
#ifile_name = "../run/isoEff_signal_389988_bffN_300_250_10000.root"
ttree_name = "histTree_conf1"
ttree_name2 = "histTree_conf2"
plot_dir = "./"
save_fmt = "pdf"

_file_tree_names = [
    (ifile_name, ttree_name),
    (ifile_name, ttree_name2),
]

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
_el_tuple = ('LepIsEle','1==LepTruthClass', 'RealEle')
_fel_tuple = ('LepIsEle','1!=LepTruthClass', 'FakeEle')
_mu_tuple = ('!LepIsEle','2==LepTruthClass', 'RealMu')
_fmu_tuple = ('!LepIsEle','2!=LepTruthClass', 'FakeMu')
_bjet_tuple = ('JetBTagged','5==JetTruth', 'RealBJet')
_fbjet_tuple = ('JetBTagged','5!=JetTruth', 'FakeBJet')
_ljet_tuple = ('!JetBTagged','5!=JetTruth', 'RealLJet')
_fljet_tuple = ('!JetBTagged','5==JetTruth', 'FakeLJet')

_lep_list = [el_tuple, fel_tuple, mu_tuple, fmu_tuple]
_jet_list = [bjet_tuple, fbjet_tuple, ljet_tuple, fljet_tuple]

def isLepVar(var):
    return var.startswith('Lep') or var.startswith('dR_Lep')

def main():

    # Variable efficiency plots
    for var, hist in vars_to_print.iteritems():
        hist.Sumw2()
        flavor_sels =  _lep_list if isLepVar(var) else _jet_list

        can.cd(1).SetLogy(True)

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

            conf_hists = {}
            for ifile_name, ttree_name in _file_tree_names:
                ifile = r.TFile(ifile_name,"r")
                ttree = ifile.Get(ttree_name)
                conf_hists[ifile_name+'_'+tree_name] = HistTuple(ttree, var, hist, den_sel, or_sel, sig_sel)

            for name, hists in conf_hists.items():
                save_name = plot_dir + hists.base_hist.GetName()+"_" + flavor_name + "." + save_fmt
                make_conf_plots(name, hists, save_name)

            compared_pairs = []
            for name1, hists1 in conf_hists.items():
                for name2, hists2 in conf_hists.items():
                    if name1==name2: continue
                    if (name1, name2) in compared_pairs: continue
                    else: compared_pairs.append((name1, name2))
                    if (name2, name1) in compared_pairs: continue
                    else: compared_pairs.append((name2, name1))
                    save_name = plot_dir + hists.base_hist.GetName()+ "_" + name1 +"_" name2 +"_"+ flavor_name + "." + save_fmt
                    make_conf_compare_plots(name1, hists1, name2, hists2, save_name)
        for ifile_name, ttree_name in _file_tree_names:

    for ifile_name, ttree_name in _file_tree_names:
        ifile = r.TFile(ifile_name,"r")
        ttree = ifile.Get(ttree_name)

        for jet_type in jet_types:
            save_name = plot_dir + "n" + jet_type + "." + save_fmt
            make_njet_plots(ttree, jet_type, save_name)

        calculate_stats(ttree)

class HistTuple:
    self.base_sel = "1"
    self.weight = 'eventweight'
    # Format (line color, fill color, line_width)
    self.base_format = (r.kBlack, r.kGray, 5)
    self.or_format = (r.kRed, r.r.kRed-10, 3)
    self.sig_format = (r.kGreen+3, r.kGreen-10, 1)

    def __init__(self, ttree, hist, var, den_sel, or_sel, sig_sel):
        self.var = var
        self.ttree_name = ttree.GetName()
        self.base_hist = self.format_base_hist(hist)
        self.or_hist = self.base_hist.Clone(hist.GetName()+"_or")
        self.sig_hist = self.base_hist.Clone(hist.GetName()+"_sig")

        fill_hists(self.base_hist, ttree, den_sel, self.base_format)
        fill_hists(self.or_hist, ttree, or_sel, self.or_format)
        fill_hists(self.sig_hist, ttree, sig_sel, self.sig_format)

        self.or_eff_graph = make_eff_graph(num=self.or_hist, den=self.base_hist, color=self.or_format[0])
        self.sig_eff_graph = make_eff_graph(num=self.sig_hist, den=self.or_hist, color=self.sig_format[0])

    def format_base_hist(self, hist):
        xax = hist.GetXaxis()
        xax.SetTitleOffset(999)
        yax = hist.GetYaxis()
        yax.SetTitleOffset(0.6)
        yax.SetTitleSize(0.06)
        return hist

    def fill_hist(self, hist, sel, format_tuple):
            draw_cmd = "%s >> %s" % (var, hist.GetName())
            ttree.Draw(draw_cmd, '(%s) * %s' % (sel, self.weight))
            hist.SetLineColor(format_tuple[0])
            hist.SetFillColor(format_tuple[1])
            hist.SetLineWidth(format_tuple[2])

    def make_eff_graph(num, den, color):
            eff_graph = r.TEfficiency(num, den)
            eff_graph.SetLineColor(color)
            return eff_graph

def make_conf_plots(name, hists, save_name):
    c_name = "c_" + name
    can = r.TCanvas(c_name,"",800,600)
    can.Divide(1,2)

    p2 = can.cd(2)
    p2.SetPad(0.0, 0.05, 1.0, 0.4)
    p2.SetTopMargin(0.01)
    p2.SetBottomMargin(0.2)

    p1 = can.cd(1)
    p1.SetPad(0.0, 0.4, 1.0, 1.0)
    p1.SetBottomMargin(0.01)

    can.SetLeftMargin(0.1)
    can.SetRightMargin(0.1)
    can.SetBottomMargin(0.01)

    # Paint top pad
    can.cd(1)
    hists.base_hist.SetMinimum(0.1)
    hists.base_hist.Draw("hist")
    hists.or_hist.Draw("hist same")
    hists.sig_hist.Draw("hist same")
    title = "Baseline#rightarrowOR#rightarrowSignal Efficiency (%s)" % flavor_name
    hists.base_hist.SetTitle(title)
    can.RedrawAxis()

    # Paint bottom pad
    can.cd(2)
    x_label = hists.base_hist.GetXaxis().GetTitle()
    mg = r.TMultiGraph('mg',';%s;Efficiency' % x_label)
    or_eff_graph = hists.or_eff.CreateGraph()
    mg.Add(or_eff_graph, 'P')
    sig_eff_graph = hists.sig_eff.CreateGraph()
    mg.Add(sig_eff_graph, 'P')

    mg.Draw('AP')
    xmin = hists.base_hist.GetXaxis().GetXmin()
    xmax = hists.base_hist.GetXaxis().GetXmax()
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

    can.SaveAs(save_name)

def make_conf_compare_plots(name1, hists1, name2, hists2, save_name):
    c_name = "c_" + name1 + '_' + name2
    can = r.TCanvas(c_name,"",800,600)
    can.Divide(1,2)

    p2 = can.cd(2)
    p2.SetPad(0.0, 0.05, 1.0, 0.4)
    p2.SetTopMargin(0.01)
    p2.SetBottomMargin(0.2)

    p1 = can.cd(1)
    p1.SetPad(0.0, 0.4, 1.0, 1.0)
    p1.SetBottomMargin(0.01)

    can.SetLeftMargin(0.1)
    can.SetRightMargin(0.1)
    can.SetBottomMargin(0.01)

    # Paint bottom pad
    can.cd(1)
    x_label = hists1.base_hist.GetXaxis().GetTitle()
    mg = r.TMultiGraph('mg',';%s;Efficiency' % x_label)
    or_eff_graph1 = hists1.or_eff.CreateGraph()
    mg.Add(or_eff_graph1, 'P')
    or_eff_graph2 = hists2.or_eff.CreateGraph()
    mg.Add(or_eff_graph2, 'P')

    mg.Draw('AP')
    xmin = hists.base_hist.GetXaxis().GetXmin()
    xmax = hists.base_hist.GetXaxis().GetXmax()
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
    p1.Modified()
    p1.Update()

    can.cd(2)
    eff_plot = tgraphAsymmErrors_divide(or_eff_graph1, or_eff_graph2)
    eff_plot.Draw('AP')
    eff_plot.GetXaxis().SetLimits(xmin, xmax)
    h_ef = eff_plot.GetHistogram()
    h_ef.SetMinimum(-0.1)
    h_ef.SetMaximum(1.1)
    h_ef.SetLabelSize(0.08,'X')
    h_ef.SetLabelSize(0.08,'Y')
    h_ef.SetTitleSize(0.1,'X')
    h_ef.SetTitleSize(0.1,'Y')
    h_ef.SetTitleOffset(1,'X')
    h_ef.SetTitleOffset(0.4,'Y')
    p2.Modified()
    p2.Update()

    can.SaveAs(save_name)

def make_njet_plots(ttree, jet_type, save_name):
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

    can.SetRightMargin(0.15)
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

    can.RedrawAxis()
    can.SaveAs(save_name)

def tgraphAsymmErrors_divide(g_num, g_den) :
    n_num = g_num.GetN()
    n_den = g_den.GetN()
    if n_num != n_den :
        print "tgraphAsymmErrors_divide ERROR    input TGraphs do not have same number of entries!"
    g3 = ROOT.TGraphAsymmErrors()

    iv = 0
    for inum in xrange(n_num) :
        for iden in xrange(n_den) :
            x_num = ROOT.Double(0.0)
            y_num = ROOT.Double(0.0)
            x_den = ROOT.Double(0.0)
            y_den = ROOT.Double(0.0)


            ex = ROOT.Double(0.0)
            ey_num_up = ROOT.Double(0.0)
            ey_num_dn = ROOT.Double(0.0)
            ey_den_up = ROOT.Double(0.0)
            ey_den_dn = ROOT.Double(0.0)

            g_num.GetPoint(inum, x_num, y_num)
            g_den.GetPoint(iden, x_den, y_den)

            if x_num != x_den : continue

            if y_num >0 :
                ey_num_up = g_num.GetErrorYhigh(inum)/y_num
                ey_num_dn = g_num.GetErrorYlow(inum)/y_num
            if y_den >0 :
                ey_den_up = g_den.GetErrorYhigh(iden)/y_den
                ey_den_dn = g_den.GetErrorYlow(iden)/y_den

            if y_num <= 0. or y_den <= 0.:
                g3.SetPoint(iv, x_num, -10)
            else:
                g3.SetPoint(iv, x_num, y_num/y_den)
            ex = g_num.GetErrorX(iv)

            e_up = ROOT.Double(0.0)
            e_dn = ROOT.Double(0.0)
            if y_num > 0 and y_den > 0 :
                e_up = sqrt(ey_num_up*ey_num_up + ey_den_up*ey_den_up)*(y_num/y_den)
                e_dn = sqrt(ey_num_dn*ey_num_dn + ey_den_dn*ey_den_dn)*(y_num/y_den)
            g3.SetPointError(iv, ex, ex, e_dn, e_up)

            iv += 1
    return g3

def calculate_stats(ttree):

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

def stat_print(msg, x, total):
    perc = "%.1f%%" % (x*100 / float(total)) if total else '-'
    print "%s: %d (%s)" % (msg, x, perc)

if __name__ == '__main__':
    main()
