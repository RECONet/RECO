from Tkinter import *
import subprocess


# MME customized modules' own configuration settings
MME_APP = '\
{\n\
};\n\n\n'				

NAS = '\
{			 \n\
    ORDERED_SUPPORTED_INTEGRITY_ALGORITHM_LIST = [ "EIA2" , "EIA1" , "EIA0" ]; \n\
    ORDERED_SUPPORTED_CIPHERING_ALGORITHM_LIST = [ "EEA0" , "EEA1" , "EEA2" ]; \n\
    T3402                                 =  12;                               \n\
    T3412                                 =  54;                               \n\
    T3422                                 =  6;                                \n\
    T3450                                 =  6;                                \n\
    T3460                                 =  6;                                \n\
    T3470                                 =  6;                                \n\
    T3485                                 =  8;                                \n\
    T3486                                 =  8;                                \n\
    T3489                                 =  4;                                \n\
    T3495                                 =  8;                                \n\
    FORCE_REJECT_TAU                      = "yes";                             \n\
    FORCE_REJECT_SR                       = "yes";                             \n\
    DISABLE_ESM_INFORMATION_PROCEDURE     = "yes";                             \n\
};\n\n\n'

S11_MME = '\
{\n\
};\n\n\n'

S1AP = '\
{			\n\
    S1AP_OUTCOME_TIMER = 10; \n\
};\n\n\n'

LOCATION = '\
{\n\
};\n\n\n'

S6A = '\
{\n\
    S6A_CONF                   = "/usr/local/etc/oai/freeDiameter/mme_fd.conf";  \n\
    HSS_HOSTNAME               = "hss";                                     	 \n\
};'



# list of customized modules' configuration settings
mod_conf = [MME_APP, NAS, S11_MME, S1AP, LOCATION, S6A]


# MME configuration file from
rfd = open("../ETC/mme.conf", 'rt')
wfd = open("/usr/local/etc/oai/mme.conf", 'wt')


# generate MOD_LIST in mme.conf
def print_mod_list():
	wfd.write("    MOD_LIST : \n")
	wfd.write("    {\n")
	wfd.write("        MODNAME = [ ")
	for i in range(0,18,3):
		if var[i] != -1 and var[i].get() == 1:
			wfd.write('"'+modules[i]+'"')
		if var[i+1] != -1 and var[i+1].get() == 1:
			wfd.write('"'+modules[i+1]+'"')
		if var[i+2] != -1 and var[i+2].get() == 1:
			wfd.write('"'+modules[i+2]+'"')
		if i != 15:
			wfd.write(', ')
	wfd.write(" ];\n")
	wfd.write("    };\n\n")
	wfd.write("};\n\n\n")


# generate MME customized modules' own configuration settings in mme.conf
def print_mod_conf():
	for i in range(0,18,3):
		if var[i] != -1 and var[i].get() == 1:
			wfd.write(modules[i]+" :\n")
		if var[i+1] != -1 and var[i+1].get() == 1:
			wfd.write(modules[i+1]+" :\n")
		if var[i+2] != -1 and var[i+2].get() == 1:
			wfd.write(modules[i+2]+" :\n")
		wfd.write(mod_conf[i/3])


# generate mme.conf at /usr/local/etc/oai and start MME
def cfg_file():
	while True:
		line = rfd.readline()
		if line == '    MOD_LIST :\n':
			break;
		wfd.write(line)
	print_mod_list()
	print_mod_conf()
	wfd.close()
	subprocess.call("./run_mme", shell=True)
	master.quit()


# pseudo_identifier GUI
master = Tk()
master.title("RECO MME")

Label(master, text="MME base modules").grid(row=0, column=0,sticky=W)
Label(master, text="            High mobility modules").grid(row=0, column=10, sticky=W)
Label(master, text="  High security modules ").grid(row=0, column=20, sticky=W)

var = [0,0,-1,0,0,0,0,0,-1,0,-1,-1,0,-1,-1,0,-1,0]

modules = ["MME_APP", "HMB_MME_APP", "NULL", "NAS", "HMB_NAS", "HS_NAS", "S11_MME", "HMB_S11_MME", "NULL" \
          ,"S1AP", "NULL", "NULL", "LOCATION", "NULL", "NULL", "S6A", "NULL", "HS_S6A"]

for i in range(0,18,3):
	if modules[i] != "NULL":
		var[i] = IntVar()
		Checkbutton(master, text=modules[i]+'.so', variable=var[i]).grid(row=i/3+1, column=0, padx=0, sticky=W)
	if modules[i+1] != "NULL":
		var[i+1] = IntVar()
		Checkbutton(master, text=modules[i+1]+'.so', variable=var[i+1]).grid(row=i/3+1, column=10, padx=50, sticky=W)
	if modules[i+2] != "NULL":
		var[i+2] = IntVar()
		Checkbutton(master, text=modules[i+2]+'.so', variable=var[i+2]).grid(row=i/3+1, column=20, padx=10, sticky=W)


Button(master, text='Run', command=cfg_file).grid(row=7, sticky=W, pady=4)

master.quit()
mainloop()

