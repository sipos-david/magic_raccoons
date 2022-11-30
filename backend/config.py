from pydantic import BaseSettings


class Settings(BaseSettings):
    keycloak_realm_url: str
    ui_url: str

    class Config:
        case_sensitive = True
 

    class Config:
        env_file = ".env"
