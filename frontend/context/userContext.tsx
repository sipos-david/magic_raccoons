import { createContext, useContext, ReactNode, Fragment } from "react";
import useApi from "../hooks/useApi";

function isUser(obj: unknown): obj is User {
  return typeof obj === "object" && obj !== null && "id" in obj && "name" in obj && "role" in obj;
}

type User = {
  id: string,
  name: string,
  role: "ADMIN" | "USER",
}

interface UseUserProps {
  user?: User
}

interface UserProviderProps {
  children?: ReactNode
}

const UserContext = createContext<UseUserProps | undefined>(undefined);
const defaultContext: UseUserProps = { user: undefined, };

export const useUser = () => useContext(UserContext) ?? defaultContext;

const _UserProvider = ({ children }: UserProviderProps) => {
  const { data, isLoading, isError, } = useApi<unknown>("/api/users/me");
  let user: User | undefined = undefined;
  if (!isError && !isLoading && data && isUser(data)) {
    user = data;
  }

  return (
    <UserContext.Provider value={{ user: user }} >
      {children}
    </UserContext.Provider>
  );
};

export const UserProvider = (props: UserProviderProps) => {
  const context = useContext(UserContext);

  // Ignore nested context providers, just passthrough children
  if (context) return <Fragment>{props.children}</Fragment>;
  return <_UserProvider {...props} />;
};
