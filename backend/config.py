from pydantic import BaseSettings


class Settings(BaseSettings):
    keycloak_realm_url: str
    ui_url: str
    database_url: str
