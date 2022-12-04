import { useMemo } from "react";
import useAccessToken from "./useAccessToken";

export default function useMutate() {
  const accessToken = useAccessToken();

  const memoizedMutate = useMemo(() => {
    const mutate = (url: string, method: "GET" | "PUT" | "POST" | "DELETE", data: unknown | undefined, json = true) => {
      if (accessToken) {
        let body: unknown | undefined | string = data;
        let headers: {
          Authorization: string;
          "Client-Id": string;
          "Content-Type"?: string;
        } = {
          "Authorization": `Bearer ${accessToken}`,
          "Client-Id": `${process.env.NEXT_PUBLIC_CLIENT_ID}`,
        };
        if (json && data) {
          headers = { ...headers, "Content-Type": "application/json" };
          body = JSON.stringify(data);
        }
        return fetch(process.env.NEXT_PUBLIC_API_URL + url, {
          headers: headers,
          method: method,
          mode: "cors",
          body: body as any
        });
      }
    };

    return mutate;
  }, [accessToken]);

  return {
    mutate: memoizedMutate
  };
}