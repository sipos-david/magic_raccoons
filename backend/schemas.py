from pydantic import BaseModel
from datetime import datetime 

class CiffBase(BaseModel):
    width:int
    height:int
    collection_id:int
    tags:str

class CiffCreate(CiffBase):
    duration:int
    caption:str
    previewfile:str

class CommentBase(BaseModel):
    text:str
    date:datetime
    author_id:int

class Comment(CommentBase):
    id:int
    collection_id:int

class CaffBase(BaseModel):
    year:int
    month:int
    day:int
    hour:int
    minute:int
    creatorlen:int
    creator:str
    rawfile:str


class Caff(CaffBase):
    id:int
    

class Log(BaseModel):
    text:str
    level:str
    date:datetime
    author_id:int

class User(BaseModel):
    user_id:str
    username:str

