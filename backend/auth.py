from enum import Enum
import sys
import requests
from typing import Optional

from fastapi import HTTPException, status

from pydantic import BaseModel

from jose import JWTError, jwt
from jose.constants import ALGORITHMS


class RealmAccess(BaseModel):
    roles: list[str]


class ResourceAccess(BaseModel):
    account: RealmAccess


class JWT(BaseModel):
    username: Optional[str] = None
    exp: int
    iat: int
    auth_time: int
    jti: str
    iss: str
    aud: str
    sub: str
    typ: str
    azp: str
    session_state: str
    acr: str
    realm_access: dict
    scope: str
    sid: str
    email: str
    email_verified: bool
    name: str
    preferred_username: str
    given_name: str
    family_name: str
    realm_access: RealmAccess
    resource_access: ResourceAccess


class Role(Enum):
    ADMIN: str = "ADMIN"
    USER: str = "USER"


class User(BaseModel):
    id: str
    name: str
    role: Role


class Auth:
    def __init__(self, keycloak_realm_url: str):
        # Why doing this?
        # Because we want to fetch public key on start
        # Later we would verify incoming JWT tokens
        try:
            r = requests.get(keycloak_realm_url, timeout=3)
            r.raise_for_status()
            response_json = r.json()
        except requests.exceptions.HTTPError as errh:
            print("Http Error:", errh)
            sys.exit(1)
        except requests.exceptions.ConnectionError as errc:
            print("Error Connecting:", errc)
            sys.exit(1)
        except requests.exceptions.Timeout as errt:
            print("Timeout Error:", errt)
            sys.exit(1)
        except requests.exceptions.RequestException as err:
            print("OOps: Something Else", err)
            sys.exit(1)

        self.__SECRET_KEY = f'-----BEGIN PUBLIC KEY-----\r\n{response_json["public_key"]}\r\n-----END PUBLIC KEY-----'

    async def __parse_jwt(self, token: str):
        credentials_exception = HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Could not validate credentials",
            headers={"WWW-Authenticate": "Bearer"},
        )
        try:
            payload = jwt.decode(token, self.__SECRET_KEY, algorithms=[ALGORITHMS.RS256],
                                 options={"verify_signature": True, "verify_aud": False, "exp": True})
            username: str = payload.get("username")
            exp: int = payload.get("exp")
            iat: int = payload.get("iat")
            auth_time: int = payload.get("auth_time")
            jti: str = payload.get("jti")
            iss: str = payload.get("iss")
            aud: str = payload.get("aud")
            sub: str = payload.get("sub")
            typ: str = payload.get("typ")
            azp: str = payload.get("azp")
            session_state: str = payload.get("session_state")
            acr: str = payload.get("acr")
            realm_access: dict = payload.get("realm_access")
            scope: str = payload.get("scope")
            sid: str = payload.get("sid")
            email: str = payload.get("email")
            email_verified: bool = payload.get("email_verified")
            name: str = payload.get("name")
            preferred_username: str = payload.get("preferred_username")
            given_name: str = payload.get("given_name")
            family_name: str = payload.get("family_name")
            realm_access: RealmAccess = payload.get("realm_access")
            resource_access: ResourceAccess = payload.get("resource_access")
            token_data = JWT(username=username,
                             exp=exp,
                             iat=iat,
                             auth_time=auth_time,
                             jti=jti,
                             iss=iss,
                             aud=aud,
                             sub=sub,
                             typ=typ,
                             azp=azp,
                             session_state=session_state,
                             acr=acr,
                             realm_access=realm_access,
                             scope=scope,
                             sid=sid,
                             email=email,
                             email_verified=email_verified,
                             name=name,
                             preferred_username=preferred_username,
                             given_name=given_name,
                             family_name=family_name,
                             resource_access=resource_access)
        except JWTError as e:
            print(e)
            raise credentials_exception
        return token_data

    async def get_user(self, token) -> User | None:
        try:
            jwt = await self.__parse_jwt(token)
            role = Role.USER
            if "caff-admin" in jwt.realm_access.roles:
                role = Role.ADMIN
            return User(id=jwt.sub, name=jwt.name, role=role)
        except Exception as e:
            print(e)
            return None
