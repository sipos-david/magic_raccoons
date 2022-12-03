import useSWR from "swr";
import useAccessToken from "./useAccessToken";

const fetcher = (url: string, accessToken: string | undefined) => {
  if (!accessToken) {
    return new Promise(() => undefined);
  }

  return fetch(url, {
    headers: {
      "Authorization": `Bearer ${accessToken}`,
      "Client-Id": `${process.env.NEXT_PUBLIC_CLIENT_ID}`
    }
  }).then(res => res.json());
};


export default function useApi<T>(path: string) {
  const accessToken = useAccessToken();
  const { data, error } = useSWR<T | undefined>(
    [process.env.NEXT_PUBLIC_API_URL + path, accessToken]
    , fetcher
  );

  return {
    data: data,
    isLoading: !error && !data,
    isError: error
  };
}