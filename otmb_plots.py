import numpy as np
import matplotlib.pyplot as plt 
import pandas 
import argparse 

"""""
This code becomes auotmatic because the parser argument is used. 
To run on the command line use 'python3 otmb_plotting_automatic.py --sim_filename "<Name of simulation file>" --trig_filename "<Name of corresponding trigger results file>" --n_events "<Number of events to be plotted>" '
The code will automatically find the trigger results and simulation files in the Results_Files directory (L30 & L31 in the code)
The resulting plots are then found in the same directory as this code is located.
"""""

#1: Import CSV files 
#1a: Get Events
parser = argparse.ArgumentParser()
parser.add_argument(
	"--sim_filename",
	required = True,
	default = None,
	type = str,
	help = "Provide the name of the **Simulation** file that will be used to produce plots")

parser.add_argument(
	"--trig_filename",
	required = True,
	default = None,
	type = str,
	help = "Provide the name of the **Trigger** file that will be used to produce plots")

parser.add_argument(
	"--n_events",
	required = True,
	default = None,
	type = str,
	help = "Provide the number of events that the OTMB was tested on")
args = parser.parse_args()

sim_file_path = "/home/cscdev/Kyla/Results_Files/" + args.sim_filename
trig_file_path = "/home/cscdev/Kyla/Results_Files/" + args.trig_filename

Simulation = pandas.read_csv(sim_file_path)
Trigger = pandas.read_csv(trig_file_path)


#1b: Create Arrays
## Simulation 
sim_clct0_pid = np.array(Simulation.sim_clct0_pid)
sim_clct0_nhit = np.array(Simulation.sim_clct0_nhit)
sim_clct0_cc_code = np.array(Simulation.sim_clct0_cc_code)
sim_clct0_bend = np.array(Simulation.sim_clct0_bend)
sim_clct0_slope = np.array(Simulation.sim_clct0_slope)
sim_clct0_KeyHalfStrip = np.array(Simulation.sim_clct0_KeyHalfStrip)
sim_clct0_KeyQuartStrip = np.array(Simulation.sim_clct0_KeyQuartStrip)
sim_clct0_KeyEighthStrip = np.array(Simulation.sim_clct0_KeyEighthStrip)

sim_clct1_pid = np.array(Simulation.sim_clct1_pid)
sim_clct1_nhit = np.array(Simulation.sim_clct1_nhit)
sim_clct1_cc_code = np.array(Simulation.sim_clct1_cc_code)
sim_clct1_bend = np.array(Simulation.sim_clct1_bend)
sim_clct1_slope = np.array(Simulation.sim_clct1_slope)
sim_clct1_KeyHalfStrip = np.array(Simulation.sim_clct1_KeyHalfStrip)
sim_clct1_KeyQuartStrip = np.array(Simulation.sim_clct1_KeyQuartStrip)
sim_clct1_KeyEighthStrip = np.array(Simulation.sim_clct1_KeyEighthStrip)

##Trigger
trig_clct0_valid = np.array(Trigger.trig_clct0_valid)
trig_clct0_pid = np.array(Trigger.trig_clct0_pid)
trig_clct0_nhit = np.array(Trigger.trig_clct0_nhit)
trig_clct0_cc_code = np.array(Trigger.trig_clct0_cc_code)
trig_clct0_bend = np.array(Trigger.trig_clct0_bend)
trig_clct0_slope = np.array(Trigger.trig_clct0_slope)
trig_clct0_KeyHalfStrip = np.array(Trigger.trig_clct0_KeyHalfStrip)
trig_clct0_KeyQuartStrip = np.array(Trigger.trig_clct0_KeyQuartStrip)
trig_clct0_KeyEighthStrip = np.array(Trigger.trig_clct0_KeyEighthStrip)

trig_clct1_valid = np.array(Trigger.trig_clct1_valid)
trig_clct1_pid = np.array(Trigger.trig_clct1_pid)
trig_clct1_nhit = np.array(Trigger.trig_clct1_nhit)
trig_clct1_cc_code = np.array(Trigger.trig_clct1_cc_code)
trig_clct1_bend = np.array(Trigger.trig_clct1_bend)
trig_clct1_slope = np.array(Trigger.trig_clct1_slope)
trig_clct1_KeyHalfStrip = np.array(Trigger.trig_clct1_KeyHalfStrip)
trig_clct1_KeyQuartStrip = np.array(Trigger.trig_clct1_KeyQuartStrip)
trig_clct1_KeyEighthStrip = np.array(Trigger.trig_clct1_KeyEighthStrip)

#Remove invalid events
for i in range(len(trig_clct0_pid)):
	if (trig_clct0_valid[i] == 0):
		trig_clct0_pid[i] = -1
		trig_clct0_nhit[i] = -1
		trig_clct0_cc_code[i] = -1
		trig_clct0_bend[i] = -1
		trig_clct0_slope[i] = -1
		trig_clct0_KeyHalfStrip[i] = -1
		trig_clct0_KeyQuartStrip[i] = -1
		trig_clct0_KeyEighthStrip[i] = -1
		
		sim_clct0_pid[i] = -1
		sim_clct0_nhit[i] = -1
		sim_clct0_cc_code[i] = -1
		sim_clct0_bend[i] = -1
		sim_clct0_slope[i] = -1
		sim_clct0_KeyHalfStrip[i] = -1
		sim_clct0_KeyQuartStrip[i] = -1
		sim_clct0_KeyEighthStrip[i] = -1

for i in range(len(trig_clct1_pid)):
	if (trig_clct1_valid[i] == 0):
		trig_clct1_pid[i] = -1
		trig_clct1_nhit[i] = -1
		trig_clct1_cc_code[i] = -1
		trig_clct1_bend[i] = -1
		trig_clct1_slope[i] = -1
		trig_clct1_KeyHalfStrip[i] = -1
		trig_clct1_KeyQuartStrip[i] = -1
		trig_clct1_KeyEighthStrip[i] = -1
		
		sim_clct1_pid[i] = -1
		sim_clct1_nhit[i] = -1
		sim_clct1_cc_code[i] = -1
		sim_clct1_bend[i] = -1
		sim_clct1_slope[i] = -1
		sim_clct1_KeyHalfStrip[i] = -1
		sim_clct1_KeyQuartStrip[i] = -1
		sim_clct1_KeyEighthStrip[i] = -1

#2: List of Variables
simulation_names0 = [sim_clct0_pid, sim_clct0_nhit, sim_clct0_cc_code, sim_clct0_bend, sim_clct0_slope, sim_clct0_KeyHalfStrip, sim_clct0_KeyQuartStrip, sim_clct0_KeyEighthStrip]
simulation_names1 = [sim_clct1_pid, sim_clct1_nhit, sim_clct1_cc_code, sim_clct1_bend, sim_clct1_slope, sim_clct1_KeyHalfStrip, sim_clct1_KeyQuartStrip, sim_clct1_KeyEighthStrip]
trigger_names0 = [trig_clct0_pid, trig_clct0_nhit, trig_clct0_cc_code, trig_clct0_bend, trig_clct0_slope, trig_clct0_KeyHalfStrip, trig_clct0_KeyQuartStrip, trig_clct0_KeyEighthStrip]
trigger_names1 = [trig_clct1_pid, trig_clct1_nhit, trig_clct1_cc_code, trig_clct1_bend, trig_clct1_slope, trig_clct1_KeyHalfStrip, trig_clct1_KeyQuartStrip, trig_clct1_KeyEighthStrip]

variables = ["pID", "Quality", "CC_Code", "Bend", "Slope", "KeyHalfstrip", "KeyQuartstrip", "KeyEighthstrip"]
fancy_variable_names = ["Pattern ID", "Quality", "CC Code", "Bend", "Slope", "Key Halfstrip", "Key Quartstrip", "Key Eighthstrip"]

#3: Dictionary 
plotting_values = { 
	"pID" : {"binning" : 10, "label" : "Pattern ID", "variable_name" : "pID", "nice_variable_name" : "Pattern ID", "x_range":[0,10]}, 
	"Quality" : {"binning" : 4, "label" : "Quality", "variable_name" : "Quality", "nice_variable_name" : "Quality", "x_range":[3,7]},
	"CC_code" : {"binning" : 64, "label" : "CC Code", "variable_name" : "CC_Code", "nice_variable_name" : "CC Code", "x_range":[0,4096]},
	"Bend" : {"binning" : 2, "label" : "Bend", "variable_name" : "Bend", "nice_variable_name" : "Bend", "x_range":[0,2]},
	"Slope" : {"binning" : 16, "label" : "Slope", "variable_name" : "Slope", "nice_variable_name" : "Slope", "x_range":[0,16]},
	"KeyHalfStrip" : {"binning" : 56, "label" : "Key Halfstrip", "variable_name" : "KeyHalfstrip", "nice_variable_name" : "Key Halfstrip", "x_range" : [0,224]},
	"KeyQuartStrip" : {"binning" : 56, "label" : "Key Quarter Strip", "variable_name" : "KeyQuartStrip", "nice_variable_name" : "Key Quartstrip", "x_range" : [0,448]},
	"KeyEighthStrip" : {"binning" : 56, "label" : "Key Eighth Strip", "variable_name" : "KeyEighthStrip", "nice_variable_name" : "Key Eighth Strip", "x_range": [0,896]},
}

#4: Functions for Plots
def largest(arr, n): #array, size of array
    # Initialize maximum element
    max = arr[0]
    # Traverse array elements from second
    # and compare every element with
    # current max
    for i in range(1, n):
        if arr[i] > max:
            max = arr[i]
    return max
 
def selection_sort(x): #Sorts an array into smallest to largest (Primarly helpful for binning array)
	for i in range(len(x)):
		swap = i + np.argmin(x[i:])
		(x[i], x[swap]) = (x[swap], x[i])
	return x

def make_ratio_plots(sim_clct0, sim_clct1, trig_clct0, trig_clct1, binning, event_number, x_range, fancy_variable_names, variables):
	#Remove 0 events
	index_sim0 = np.argwhere(sim_clct0==-1)
	index_sim1 = np.argwhere(sim_clct1==-1)
	index_trig0 = np.argwhere(trig_clct0==-1)
	index_trig1 = np.argwhere(trig_clct1==-1)
	sim_clct0 = np.delete(sim_clct0, index_sim0)
	sim_clct1 = np.delete(sim_clct1, index_sim1)
	trig_clct0 = np.delete(trig_clct0, index_trig0)
	trig_clct1 = np.delete(trig_clct1, index_trig1)

	if (fancy_variable_names != "Bend"):
		index_of0_simclct0 = np.argwhere(sim_clct0==0)
		index_of0_simclct1 = np.argwhere(sim_clct1==0)
		index_of0_trigclct0 = np.argwhere(trig_clct0==0)
		index_of0_trigclct1 = np.argwhere(trig_clct1==0)
		sim_clct0 = np.delete(sim_clct0, index_of0_simclct0)
		sim_clct1 = np.delete(sim_clct1, index_of0_simclct1)
		trig_clct0 = np.delete(trig_clct0, index_of0_trigclct0)
		trig_clct1 = np.delete(trig_clct1, index_of0_trigclct1)

	fig, (ax1,ax2) = plt.subplots(2, sharex=True, figsize = (8,6), gridspec_kw={'height_ratios': [2, 1]})

	#Plot 1 Variables 
	binwidth = int((x_range[1] - x_range[0]) / binning)
	#print("range ",x_range, " nbin ", binning," width ", binwidth)#, " binning ", range(x_range[0], x_range[1], binwidth))
	
	n_sim0, nbins_sim0, patches = ax1.hist(sim_clct0, bins = range(x_range[0], x_range[1]+binwidth, binwidth) , histtype = "stepfilled", alpha = 0.8, color = "sandybrown", label = "Simulation CLCT0")
	n_sim1, nbins_sim1, patches = ax1.hist(sim_clct1, bins = range(x_range[0], x_range[1]+binwidth, binwidth), alpha = 0.8, histtype = "stepfilled", color = "dodgerblue", label = "Simulation CLCT1")
	n_trig0, nbins_trig0, patches = ax1.hist(trig_clct0, bins = range(x_range[0], x_range[1]+binwidth, binwidth), alpha = 0.8, histtype = "step", color = "brown", label = "Trigger CLCT0")
	n_trig1, nbins_trig1, patches = ax1.hist(trig_clct1, bins = range(x_range[0], x_range[1]+binwidth, binwidth), alpha = 0.8, histtype = "step", color = "black", label = "Trigger CLCT1")

	if (max(n_sim0) > max(n_sim1)):
		chosen_variable = n_sim0
	if (max(n_sim1) > max(n_sim0)):
		chosen_variable = n_sim1
	
	largest_bin = max(chosen_variable)		
	y_range = 1.5*largest_bin

	#Plot 1 Aesthetics
	ax1.set_ylabel("Events")
	ax1.ticklabel_format(style = 'scientific', axis = 'y', scilimits=(0,0))
	ax1.set_ylim([0,y_range])
	ax1.legend(loc = "upper left")

	#Plot 2 Aesthetics
	ax2.set_ylabel("Difference / Simulation")
	ax2.set_xlabel(fancy_variable_names)
	#ax2.set_ylim([0,2])
	ax2.set_xlim(x_range)
	#Plot 2 Variables
	#Replace 0 values with 1
	for i in range(len(n_sim0)):
		if n_sim0[i] == 0: 
			n_sim0[i] = 1
		if n_sim1[i] == 0: 
			n_sim1[i] = 1
		if n_trig0[i] == 0:
			n_trig0[i] = 1
		if n_trig1[i] == 0:
			n_trig1[i] = 1
		
	clct0_ratio = [0 for x in range(len(n_trig0))]
	clct1_ratio = [0 for x in range(len(n_sim1))]
	for j in range(len(n_trig0)):
		clct0_ratio[j] = (n_trig0[j] - n_sim0[j]) / n_sim0[j]
		clct1_ratio[j] = (n_trig1[j] - n_sim1[j]) / n_trig1[j]

	nbins_sim0 = np.delete(nbins_sim0, 0)
	nbins_sim1 = np.delete(nbins_sim1, 0)

	#Scaling the y axis on ax2

	first_half0 = [0 for x in range(int(binning/2))]
	first_half1 = [0 for x in range(int(binning/2))]
	for i in range(int(binning/2)):
		first_half0[i] = clct0_ratio[i]
		first_half1[i] = clct1_ratio[i]
		if first_half0[i] == 0:
			first_half0[i] = -0.05
		if first_half1[i] == 0:
			first_half1[i] = -0.05

	last_half0 = [0 for x in range(int(binning/2))]
	last_half1 = [0 for x in range(int(binning/2))]
	for i in range(int(binning/2)):
		last_half0[i] = clct0_ratio[i+int(binning/2)]
		last_half1[i] = clct1_ratio[i+int(binning/2)]


	if (max(clct1_ratio) > max(clct0_ratio) and max(clct1_ratio) > 0):
		ax2.set_ylim([2.5*min(first_half1), 1.5*max(clct1_ratio)+0.01])
		if (min(last_half1) < min(first_half1)):
			ax2.set_ylim([1.5*min(last_half1), 1.5*max(clct1_ratio)])
	elif (max(clct0_ratio) > max(clct1_ratio) and max(clct0_ratio) > 0):
		ax2.set_ylim([2.5*min(first_half1), 1.5*max(clct0_ratio)+0.01])
		if (min(last_half1) < min(first_half1)):
			ax2.set_ylim([1.5*min(last_half1), 1.5*max(clct1_ratio)])
	elif (max(first_half0) == -0.05 and max(first_half1) == -0.05):
		ax2.set_ylim([2.5*min(first_half1), 0.05])
	elif (min(clct0_ratio) == 0 and min(clct1_ratio) == 0):
		ax2.set_ylim([-0.05, 1.5*max(clct1_ratio)])
	else:
		ax2.set_ylim([-0.05, 0.05])	


	ax2.axhline(y=0, color = "gray")
	ax2.plot(nbins_sim0, clct0_ratio, alpha = 0.8, color = "brown", label = "CLCT0", marker = 'o')
	ax2.plot(nbins_sim1, clct1_ratio, alpha = 0.8, color = "black", label = "CLCT1", marker = 'o')

	ax2.legend(loc="lower left")
	ax2.ticklabel_format(style = 'scientific', axis = 'y', scilimits=(-2,-2))
	plt.savefig("linear_fit_%s_%s_events.pdf" %(variables, event_number))

"""""
def make_ratio_logplots(sim_clct0, sim_clct1, trig_clct0, trig_clct1, binning, event_number, x_range, fancy_variable_names, variables):
	#Remove 0 events
	index_sim0 = np.argwhere(sim_clct0==-1)
	index_sim1 = np.argwhere(sim_clct1==-1)
	index_trig0 = np.argwhere(trig_clct0==-1)
	index_trig1 = np.argwhere(trig_clct1==-1)
	sim_clct0 = np.delete(sim_clct0, index_sim0)
	sim_clct1 = np.delete(sim_clct1, index_sim1)
	trig_clct0 = np.delete(trig_clct0, index_trig0)
	trig_clct1 = np.delete(trig_clct1, index_trig1)

	if (fancy_variable_names != "Bend"):
		index_of0_simclct0 = np.argwhere(sim_clct0==0)
		index_of0_simclct1 = np.argwhere(sim_clct1==0)
		index_of0_trigclct0 = np.argwhere(trig_clct0==0)
		index_of0_trigclct1 = np.argwhere(trig_clct1==0)
		sim_clct0 = np.delete(sim_clct0, index_of0_simclct0)
		sim_clct1 = np.delete(sim_clct1, index_of0_simclct1)
		trig_clct0 = np.delete(trig_clct0, index_of0_trigclct0)
		trig_clct1 = np.delete(trig_clct1, index_of0_trigclct1)

	fig, (ax1,ax2) = plt.subplots(2, sharex=True, figsize = (8,6), gridspec_kw={'height_ratios': [2, 1]})
	#Plot 1 Variables 
	binwidth = int((x_range[1] - x_range[0]) / binning)
	n_sim0, nbins_sim0, patches = ax1.hist(sim_clct0, bins = range(x_range[0], x_range[1], binwidth), histtype = "stepfilled", alpha = 0.8, color = "sandybrown", label = "Simulation CLCT0")
	n_sim1, nbins_sim1, patches = ax1.hist(sim_clct1, bins = range(x_range[0], x_range[1], binwidth), alpha = 0.8, histtype = "stepfilled", color = "dodgerblue", label = "Simulation CLCT1")
	n_trig0, nbins_trig0, patches = ax1.hist(trig_clct0, bins = range(x_range[0], x_range[1], binwidth), alpha = 0.8, histtype = "step", color = "brown", label = "Trigger CLCT0")
	n_trig1, nbins_trig1, patches = ax1.hist(trig_clct1, bins = range(x_range[0], x_range[1], binwidth), alpha = 0.8, histtype = "step", color = "black", label = "Trigger CLCT1")

	#Plot 1 Aesthetics
	if (max(n_sim0) > max(n_sim1)):
		chosen_variable = n_sim0
	if (max(n_sim1) > max(n_sim0)):
		chosen_variable = n_sim1
	
	largest_bin = max(chosen_variable)		
	y_range = 10*largest_bin

	#plt.suptitle("%s" %(fancy_variable_names))
	ax1.set_ylabel("Events")
	ax1.ticklabel_format(style = 'scientific', axis = 'y', scilimits=(0,0))
	ax1.set_yscale('log')
	ax1.set_ylim([0,y_range])
	ax1.legend(loc = "upper left")

	#Plot 2 Aesthetics
	ax2.set_ylabel("Difference / Simulation")
	ax2.set_xlabel(fancy_variable_names)
	#ax2.set_ylim([0,2])
	ax2.set_xlim(x_range)
	#Plot 2 Variables
	#Replace 0 values with 1
	for i in range(len(n_sim0)):
		if n_sim0[i] == 0: 
			n_sim0[i] = 1
		if n_sim1[i] == 0: 
			n_sim1[i] = 1
		if n_trig0[i] == 0:
			n_trig0[i] = 1
		if n_trig1[i] == 0:
			n_trig1[i] = 1
		
		
	clct0_ratio = (n_trig0 - n_sim0) / n_sim0
	clct1_ratio = (n_trig1 - n_sim1) / n_sim1
	#print("clct0_ratio", clct0_ratio)
	#print("clct1_ratio", clct1_ratio)
	chosen_variable2 = 0
	if (max(clct0_ratio) > max(clct1_ratio)):
		chosen_variable2 = clct0_ratio
	if (max(clct1_ratio) > max(clct0_ratio)):
		chosen_variable2 = clct1_ratio
	
	largest_bin2 = max(chosen_variable2)		
	y_range2 = 10*largest_bin2

	nbins_sim0 = np.delete(nbins_sim0, 0)
	nbins_sim1 = np.delete(nbins_sim1, 0)

	ax2.axhline(y=0, color = "gray")
	ax2.plot(nbins_sim0, clct0_ratio, alpha = 0.8, color = "brown", label = "CLCT0", marker = 'o')
	ax2.plot(nbins_sim1, clct1_ratio, alpha = 0.8, color = "black", label = "CLCT1", marker = 'o')

	ax2.legend(loc="upper left")
	ax2.y_lim(0,y_range2)
	ax2.ticklabel_format(style = 'scientific', axis = 'y', scilimits=(0,0))
	plt.savefig("plots/Test3/log_fit_%s_%s_events.pdf" %(variables, event_number))
"""

#5: Make Plots 
i = 0
for key,values in plotting_values.items():
	#for i in range(len(variables)):
	print("Making plot for: ", key)
	make_ratio_plots(simulation_names0[i], simulation_names1[i], trigger_names0[i], trigger_names1[i], plotting_values[key]["binning"], args.n_events, plotting_values[key]["x_range"], plotting_values[key]["nice_variable_name"], plotting_values[key]["variable_name"])
	#make_ratio_logplots(simulation_names0_6k[i], simulation_names1_6k[i], trigger_names0_6k[i], trigger_names1_6k[i], plotting_values[key]["binning"], "6k", plotting_values[key]["x_range"], plotting_values[key]["nice_variable_name"], plotting_values[key]["variable_name"])
	#make_ratio_logplots(simulation_names0_800[i], simulation_names1_800[i], trigger_names0_800[i], trigger_names1_800[i], plotting_values[key]["binning"], "800", plotting_values[key]["x_range"], plotting_values[key]["nice_variable_name"], plotting_values[key]["variable_name"])
	i+=1

"""""
make_ratio_plots(sim_clct0_bend_6k, sim_clct1_bend_6k, trig_clct0_bend_6k, trig_clct1_bend_6k, 2, "6k", [0,2], "Bend","Bend")
#make_ratio_logplots(sim_clct0_bend_6k, sim_clct1_bend_6k, trig_clct0_bend_6k, trig_clct1_bend_6k, 2, "6k", [0,2], "Bend", "Bend")
make_ratio_plots(sim_clct0_bend_800, sim_clct1_bend_800, trig_clct0_bend_800, trig_clct1_bend_800, 2, "800", [0,2], "Bend","Bend")
#make_ratio_logplots(sim_clct0_bend_800, sim_clct1_bend_800, trig_clct0_bend_800, trig_clct1_bend_800, 2, "800", [0,2], "Bend", "Bend")


make_ratio_plots(sim_clct0_nhit_6k, sim_clct1_nhit_6k, trig_clct0_nhit_6k, trig_clct1_nhit_6k, 4, "6k", [3,7], "Quality","Quality")
#make_ratio_logplots(sim_clct0_nhit_6k, sim_clct1_nhit_6k, trig_clct0_nhit_6k, trig_clct1_bnhit_6k, 4, "6k", [3,7], "Quality", "Quality")
make_ratio_plots(sim_clct0_nhit_800, sim_clct1_nhit_800, trig_clct0_nhit_800, trig_clct1_nhit_800, 4, "800", [3,7], "Quality","Quality")
#make_ratio_logplots(sim_clct0_nhit_800, sim_clct1_nhit_800, trig_clct0_nhit_800, trig_clct1_nhit_800, 4, "800", [3,7], "Quality", "Quality")

"""""





