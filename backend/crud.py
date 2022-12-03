from sqlalchemy.orm import Session

import models, schemas

def get_caffs(db:Session):
    db_caff= db.query(models.Caff).all()
    return db_caff

def create_caff(db:Session,caff:schemas.CaffBase):
    db_caff=models.Caff(year=caff.year
                        ,month=caff.month
                        ,day=caff.day
                        ,hour=caff.hour
                        ,minute=caff.minute
                        ,creatorLen=caff.creatorlen
                        ,creator=caff.creator
                        ,rawfile=caff.rawfile)
    db.add(db_caff)
    db.commit()
    db.refresh(db_caff)
    return db_caff


def get_comments_by_collection_id(collection_id:int,db:Session):
    db_comments=db.query(models.Comment).filter(models.Comment.collection_id == collection_id).all()
    return db_comments

def get_caff_by_id(id:int,db:Session,skip:int=0):
    return db.query(models.Caff).filter(models.Caff.id == id).first()

def get_caff_by_id_with_comments(id:int,db:Session,skip:int=0):
    return db.query(models.Caff).filter(models.Caff.id==id).offset(skip).all()

def get_caff_ids_by_tag(tag:str,db:Session):
    return db.query(models.Ciff).filter(models.Ciff.tags.contains(tag)).all()
    
def create_ciff(db:Session,ciff=schemas.CiffCreate):
    db_ciff=models.Ciff(      width=ciff.width
                              ,height=ciff.height
                              ,collection_id=ciff.collection_id
                              ,tags=ciff.tags
                              ,duration=ciff.duration
                              ,caption=ciff.caption
                              ,previewfile=ciff.previewfile)
    db.add(db_ciff)
    db.commit()
    db.refresh(db_ciff)
    return db_ciff

def create_comment(db:Session,comment:schemas.CommentBase,collection_id:int):
    db_comment=models.Comment(text=comment.text
                             ,author_id=comment.author_id
                             ,date=comment.date,
                             collection_id=collection_id)
    db.add(db_comment)
    db.commit()
    db.refresh(db_comment)
    return db_comment


def get_users(db: Session, skip: int = 0, limit: int = 100):
    return db.query(models.User).offset(skip).limit(limit).all()

def get_comment_by_id(comment_id:int,db:Session):
    return db.query(models.Comment).filter(models.Comment.id==comment_id).first()

def update_comment_by_id(comment_id:int,comment:schemas.CommentBase,db:Session):
    db.query(models.Comment).filter(models.Comment.id == comment_id).update({'text': comment.text,'date':comment.date})
    
    db.commit()
    return 1

def delete_comment_by_id(comment_id:int,db:Session):
    comment=get_comment_by_id(comment_id=comment_id,db=db)
    result = db.delete(comment)
    db.commit()
    return result

def get_logs(db:Session):
    db_caff= db.query(models.Log).all()
    return db_caff

def create_log(log:schemas.Log,db:Session):
    model_log=models.Log(text=log.text
                        ,date=log.date
                        ,author_id=log.author_id)
    db.add(model_log)
    db.commit()
    db.refresh(model_log)
    return model_log

def get_users(db:Session):
    db_caff= db.query(models.User).all()
    return db_caff

def get_user_id_by_username(username:str, db:Session):
    return db.query(models.User).filter(models.User.username == username).first()

def create_user(user:schemas.User,db:Session):
    model_user=models.User(user_id=user.user_id,username=user.username)
    db.add(model_user)
    db.commit()
    db.refresh(model_user)
    return model_user
