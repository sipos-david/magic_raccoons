from subprocess import check_output as qx
import subprocess

# TODO Mappát készíteni a kimenetnek /caff/data/out/{mappaId} és ezt átadni majd az args-nak a data/out/helyett
args = ["/caff/parser/caff_parser", "/caff/parser/res/1.caff",
        "/caff/data/out/"]

cmd = " ".join(args)

output = subprocess.run(cmd, shell=True)
# TODO parser visszatérési értékének ellenőrzése, ha nem 0 akkor nem folyatatni tovább

f = open("/caff/data/out/metadata.json", "r")
print(f.read())
f.close()
# TODO json beolvasasása fájlból és ebből lehet a többi lépés

# TODO tga-ból .gif vagy valami böngésző álltal elfogadott képformátum (webp, jpeg, bmp) => bármilyen könyvtár jó, de a nevet és verzió számod írd fel
# TODO a készített képet lementeni a /caff/data/preview mappába {id}.gif névvel
