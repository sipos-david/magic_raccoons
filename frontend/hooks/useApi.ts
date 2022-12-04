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
  }).then(async res => {
    // If the status code is not in the range 200-299,
    // we still try to parse and throw it.
    if (!res.ok) {
      const error: Error & { info?: unknown, status?: number } = new Error("An error occurred while fetching the data.");
      // Attach extra info to the error object.
      error.info = await res.json();
      error.status = res.status;
      throw error;
    }

    return res.json();
  });
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