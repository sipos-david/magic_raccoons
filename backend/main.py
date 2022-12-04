from functools import lru_cache
import subprocess

from fastapi import Depends, FastAPI, HTTPException, File, UploadFile
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from fastapi.security import OAuth2PasswordBearer
from auth import Auth, Role, User

from config import Settings

from sqlalchemy.orm import Session
import crud
import models
import schemas
from database import SessionLocal, engine

from os import makedirs, listdir, remove
from uuid import uuid4
from json import load
from re import match
from PIL import Image
from starlette.responses import FileResponse
import shutil


models.Base.metadata.create_all(bind=engine)


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


@lru_cache()
def get_settings():
    return Settings()


oauth2_scheme = OAuth2PasswordBearer(tokenUrl="token")
auth = Auth(get_settings().keycloak_realm_url)


async def get_session_user(token: str = Depends(oauth2_scheme)):
    user = await auth.get_user(token)
    return user

app = FastAPI()

app.mount("/preview", StaticFiles(directory="../data/preview"), name="preview")

origins = [
    get_settings().ui_url,
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

ALLOWED_EXTENSIONS = set(['caff'])

# TODO user: User = Depends(get_session_user) minden hívás elé token ellenőrzés miatt, ahol kell role ellenőrzés
# TODO eseményeket logolni
# TODO caff fájl törlés
# TODO caff fájl keresés query paraméterekkel
# TODO nem használt endpoint-ok törlése


@app.get("/api/logs")
async def get_logs(db: Session = Depends(get_db), user: User = Depends(get_session_user)):
    if user.role != Role.ADMIN:
        raise HTTPException(status_code=403, detail="Forbidden")
    return crud.get_logs(db)


@app.get("/api/users")
async def get_users(db: Session = Depends(get_db)):
    return crud.get_logs(db)


@app.get("/api/users/me")
async def get_user_id_by_username(user: User = Depends(get_session_user),db: Session = Depends(get_db)):
    tmp_user = crud.get_user_by_userid(user_id=user.id,db=db)
    if(tmp_user == None):
        crud.create_user(schemas.User(user_id=str(user.id),username=str(user.name)),db=db)
    return user


@app.get("/api")
async def read_caffs(db: Session = Depends(get_db)):
    caffs = crud.get_caffs(db)
    return caffs


@app.post("/api")
async def create_caff(caff: schemas.CaffBase, db: Session = Depends(get_db)):
    return crud.create_caff(db=db, caff=caff)


@app.get("/api/")
async def read_caffs_with_comments(db: Session = Depends(get_db)):
    caffs = crud.get_caffs(db)
    ret: list = []
    for x in caffs:
        comments: list = crud.get_comments_by_collection_id(x.id, db=db)
        element = vars(x)
        element["comments"] = []
        element["comments"] += comments
        ret.append(element)
    return ret


@app.post("/api/{caff_id}/comments")
async def create_comment_to_caff(caff_id: int, comment: schemas.CommentBase, db: Session = Depends(get_db), user: User = Depends(get_session_user)):
    caff = crud.get_caff_by_id(caff_id, db=db, skip=0)
    comment.author_id = user.id
    if (caff == None):
        raise HTTPException(
            status_code=400, detail="There is not a Caff with id: "+str(caff_id))
    return crud.create_comment(db=db, comment=comment, collection_id=caff_id)


@app.get("/api/{caff_id}")
async def read_caff_by_id_with_comments(caff_id: int, db: Session = Depends(get_db)):
    caff = crud.get_caff_by_id(caff_id, db=db, skip=0)
    if (caff == None):
        raise HTTPException(
            status_code=400, detail="There is not a Caff with id: "+str(caff_id))
    comments = crud.get_comments_by_collection_id(collection_id=caff_id, db=db)
    comment_dict=[]
    for comment in comments:
        print(vars(comment))
        author = crud.get_user_by_userid(db=db,user_id=comment.author_id)
        if (caff == None):
            username = "Anonymus"
        else:
            username = author.username
        comment_element={"text":comment.text,"username":username,"date":comment.date}
        comment_dict.append(comment_element)
    caff_dict = vars(caff)
    caff_dict["comments"] = []
    caff_dict["comments"] += comment_dict
    return caff_dict


@app.get("/api/search?tag=")
async def get_caffs_by_tag(tag: str, db: Session = Depends(get_db)):
    caff_ids = crud.get_caff_ids_by_tag(tag, db)
    ret: list = []
    for x in caff_ids:
        caff_id = vars(x)
        print(x)
        print(caff_id)
        caff = crud.get_caff_by_id(caff_id["collection_id"], db)
        if caff not in ret:
            ret.append(caff)
    return ret


@app.get("/api/{caff_id}/comments/{comment_id}")
async def get_comment_by_id(caff_id: int, comment_id: int, db: Session = Depends(get_db)):
    caff = crud.get_caff_by_id(caff_id, db=db, skip=0)
    if (caff == None):
        raise HTTPException(
            status_code=400, detail="There is not a Caff with id: "+str(caff_id))
    comment = crud.get_comment_by_id(comment_id, db)
    if (comment == None):
        raise HTTPException(
            status_code=400, detail="There is not a comment with id: "+str(comment_id))
    return comment


@app.get("/download_caff/{caff_id}", response_class=FileResponse)
async def download_caff(caff_id: int, db: Session = Depends(get_db)):
    caff = crud.get_caff_by_id(caff_id, db=db, skip=0)
    if (caff == None):
        raise HTTPException(
            status_code=400, detail="There is not a Caff with id: "+str(caff_id))
    filename = caff.rawfile.split('/')[-1]
    return FileResponse(caff.rawfile, filename=filename)


@app.put("/api/{caff_id}/comments/{comment_id}")
async def update_comment_by_id(caff_id: int, comment_id: int, comment: schemas.CommentBase, db: Session = Depends(get_db)):
    caff = crud.get_caff_by_id(caff_id, db=db, skip=0)
    if (caff == None):
        raise HTTPException(
            status_code=400, detail="There is not a Caff with id: "+str(caff_id))
    comment_ret = crud.get_comment_by_id(comment_id, db)
    if (comment_ret == None):
        raise HTTPException(
            status_code=400, detail="There is not a comment with id: "+str(comment_id))
    comment_updated = crud.update_comment_by_id(
        comment_id=comment_id, comment=comment, db=db)
    return comment_updated


@app.delete("/api/{caff_id}")
async def delete_comment_by_id(caff_id: int, db: Session = Depends(get_db)):
    caff = crud.get_caff_by_id(caff_id, db=db, skip=0)
    if (caff == None):
        raise HTTPException(
            status_code=400, detail="There is not a Caff with id: "+str(caff_id))
    is_successful = crud.delete_caff_by_id (caff_id,db)
    if is_successful:
        return response.ok
    else:
        raise HTTPException(
            status_code=400, detail="Could not delete Caff with id: "+str(caff_id))
        

@app.delete("/api/{caff_id}/comments/{comment_id}")
async def delete_comment_by_id(caff_id: int, comment_id: int, db: Session = Depends(get_db)):
    caff = crud.get_caff_by_id(caff_id, db=db, skip=0)
    if (caff == None):
        raise HTTPException(
            status_code=400, detail="There is not a Caff with id: "+str(caff_id))
    comment = crud.get_comment_by_id(comment_id, db)
    if (comment == None):
        raise HTTPException(
            status_code=400, detail="There is not a comment with id: "+str(comment_id))
    return crud.delete_comment_by_id(comment_id, db)


@app.post("/upload_file")
async def create_upload_file(file: UploadFile = File(...), db: Session = Depends(get_db)):
    if not file:
        return {"message": "No upload file sent"}
    else:
        if allowed_file(file.filename) == True:
            folder_id = str(uuid4())
            folder = "/caff/data/out/"+folder_id
            filename_with_extension = "source.caff"
            makedirs(folder)
            with open(f'{folder}/{filename_with_extension}', 'wb')as buffer:
                shutil.copyfileobj(file.file, buffer)
            parse_caff(db=db, filename=filename_with_extension, dir=folder)
            return {"message": "Uploaded successfully"}
        else:
            return {"message": "Illegal file extension"}


def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


def parse_caff(db: Session, filename: str, dir: str):
    preview_path = '/caff/data/preview/'

    args = ["/caff/parser/caff_parser", dir+'/'+filename,
            dir]

    cmd = " ".join(args)

    output = subprocess.run(cmd, shell=True)

    if output.returncode != 0:
        remove("/caff/backend/data/"+filename)
        raise HTTPException(
            status_code=422, detail="The content of the uploaded file did not fit the CAFF file format. Upload did not complete.")

    f = open(dir+"/metadata.json", "r")
    metadata = load(f)
    f.close()

    year = metadata["credits"]["year"]
    day = metadata["credits"]["day"]
    hour = metadata["credits"]["hour"]
    month = metadata["credits"]["month"]
    creator = metadata["credits"]["creator"]
    creator_len = len(creator)

    animations = metadata["animation"]

    caff = crud.create_caff(db=db, caff=schemas.CaffBase(year=year, month=month, day=day,
                            hour=hour, minute=-1, creatorlen=creator_len, creator=creator, rawfile=dir+'/'+filename))

    for i in animations:
        duration = i["duration"]
        width = i["width"]
        height = i["height"]
        caption = i["caption"]
        tags = ';'.join(i["tags"])
        crud.create_ciff(db=db, ciff=schemas.CiffCreate(width=width, height=height,
                         collection_id=caff.id, duration=duration, caption=caption, tags=tags, previewfile='NA'))

    create_preview_gif(caff.id, preview_path, dir+'/')


def create_preview_gif(caff_id, preview_path, gen_path):
    files = listdir(gen_path)

    tgas = []

    preview_names = "preview\d\d*.tga"

    for filename in files:
        if match(preview_names, filename):
            tga = Image.open(gen_path + filename)
            tgas.append(tga)

    preview_filepath = preview_path + str(caff_id) + '.gif'
    tgas[0].save(preview_filepath, save_all=True,
                 append_images=tgas[1:], optimize=False, duration=40, loop=0)
    print(preview_filepath)
    print(type(preview_filepath))
