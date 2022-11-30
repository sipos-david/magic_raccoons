from sqlalchemy import Boolean, Column, ForeignKey, Integer, String, Text, Date
from sqlalchemy.orm import relationship

from database import Base

class Caff(Base):
    __tablename__="caffs"

    id = Column(Integer, primary_key=True, index=True)
    year = Column(Integer)
    month = Column(Integer)
    day = Column(Integer)
    hour = Column(Integer)
    minute = Column(Integer)
    creatorLen = Column(Integer)
    creator = Column(String(256))
    rawfile = Column(Text,nullable=False)

    animations = relationship("Ciff", back_populates="collection")
    comments = relationship("Comment",back_populates="collection")

class Ciff(Base):
    __tablename__="ciffs"
    
    id = Column(Integer, primary_key=True, index=True)
    duration = Column(Integer)
    width=Column(Integer,nullable=False)
    height=Column(Integer,nullable=False)
    caption=Column(Text)
    collection_id=Column(Integer,ForeignKey("caffs.id"),nullable=False)
    previewfile=Column(Text,nullable=False) 

    collection=relationship("Caff",back_populates="animations")

    
class Comment(Base):
    __tablename__="comments"

    id=Column(Integer,primary_key=True,index=True)
    text=Column(Text)
    author_id=Column(String(256))
    date=Column(Date)
    collection_id=Column(Integer,ForeignKey("caffs.id"),nullable=False)

    collection=relationship("Caff",back_populates="comments")

class User(Base):
    __tablename__="users"

    user_id=Column(String(100),primary_key=True)
    username=Column(String(100),nullable=False)

class Log(Base):
    __tablename__="logs"

    id=Column(Integer, primary_key=True)
    text=Column(Text)
    date=Column(Date)
    author_id=Column(Integer)