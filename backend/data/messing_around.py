from subprocess import check_output as qx
import subprocess

cmd = ["./windows.exe", "./data/1.caff", "./result"]

output = subprocess.run(cmd,shell=True)
print (output)

'''
----------------------Windowson fordított instance---------------
---------------parancs: g++ *.cpp -o windows.exe-----------------

cmd =str("./windows.exe")
args=["./1.caff", "./"]
subprocess.run(executable=cmd,args=args)
'''


'''
----------------------WSL-en fordított instance---------------
----------------parancs: make all-----------------------------

subprocess.run("../caff-parser/caff_parser ./data/1.caff ./result",shell=True)
'''
