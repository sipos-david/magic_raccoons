import { SessionProvider } from "next-auth/react";

import "../styles/globals.css";
import type { AppProps } from "next/app";
import Head from "next/head";

export default function App({ Component, pageProps: { session, ...pageProps }, }: AppProps) {
  return (
    <>
      <Head>
        <title>CaffShop</title>
        <meta name="description" content="CaffShop" />
        <link rel="icon" href="/favicon.ico" />
      </Head>
      <SessionProvider session={session}>
        <Component {...pageProps} />
      </SessionProvider>
    </>
  );
}
