import csv
# import numpy as np
import ROOT
import uproot
import glob
import re
# from itertools import islice

file = uproot.recreate("moderator_hist.root")

filepath = "/home/yash/ArtieSim/data/DICER_MARKIV_*.0.csv"

for filename in glob.glob(filepath):
    with open(filename) as csv_file:

        print(filename)

        time = []
        pdf = []

        csv_reader = csv.reader(csv_file, delimiter=',')

        bin_edges = []
        linecount = 0
        for row in csv_reader:
            pdf.append(float(row[1]))
            time.append(float(row[0]))
            if linecount < 3:
                bin_edges.append(float(row[0]))
                linecount += 1

        # pdf_np = np.array(pdf)
        # time_np = np.array(time)

        bin_width = (float(bin_edges[1]) - float(bin_edges[0]))
        bin_width = round(bin_width, 3)

        bins = (time[-1]-time[0])/bin_width + 1
        min = time[0] - (bin_width/2)
        max = time[-1] + (bin_width/2)

        bins = round(bins, 3)
        min = round(min, 3)
        max = round(max, 3)

        print(bins, "; ",min, "; ", max)

        h1 = ROOT.TH1D("h1","time pdf hist",int(bins),min,max)

        for i in range(int(bins)):
            h1.SetBinContent(i, pdf[i])

        regex = re.compile(r'\d+')
        num = regex.findall(filename)   #[int(x) for x in regex.findall(filename)]

        file["hist_"+num[0]+"ev"] = h1

        del h1