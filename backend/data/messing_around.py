from subprocess import check_output as qx
import subprocess

from os.path import exists as path_exists
from os import makedirs
from uuid import uuid4

from json import load

path = '/caff/data/out/'
id = str(uuid4())
gen_path = path + str(uuid4())

while path_exists(gen_path):
    id = str(uuid4())
    gen_path = path + str(uuid4())
    
makedirs(gen_path)
gen_path = gen_path + '/'

args = ["/caff/parser/caff_parser", "/caff/parser/res/1.caff",
        gen_path]

cmd = " ".join(args)

output = subprocess.run(cmd, shell=True)

if output.returncode != 0:
    exit

f = open("/caff/data/out/metadata.json", "r")
metadata = load(f)
f.close()



# TODO tga-ból .gif vagy valami böngésző álltal elfogadott képformátum (webp, jpeg, bmp) => bármilyen könyvtár jó, de a nevet és verzió számod írd fel
# TODO a készített képet lementeni a /caff/data/preview mappába {id}.gif névvel

prev_path = '/caff/data/preview/' + id

