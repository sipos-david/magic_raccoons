import NextAuth, { User } from "next-auth";
import { JWT } from "next-auth/jwt";
import KeycloakProvider from "next-auth/providers/keycloak";

/**
 * Takes a token, and returns a new token with updated
 * `accessToken` and `accessTokenExpires`. If an error occurs,
 * returns the old token and an error property
 */
async function refreshAccessToken(token: JWT) {
  try {
    const url = process.env.KEYCLOAK_ISSUER + "/protocol/openid-connect/token";

    const response = await fetch(url, {
      headers: {
        "Content-Type": "application/x-www-form-urlencoded",
      },
      method: "POST",
      body: new URLSearchParams({
        "client_id": process.env.KEYCLOAK_ID || "",
        "client_secret": process.env.KEYCLOAK_SECRET || "",
        "grant_type": "refresh_token",
        "refresh_token": token.refreshToken as string,
      })
    });

    const refreshedTokens = await response.json();

    if (!response.ok) {
      throw refreshedTokens;
    }

    return {
      ...token,
      accessToken: refreshedTokens.access_token,
      accesssTokenExpires: refreshedTokens.expires_at,
      refreshTokenExpires: Date.now() + refreshedTokens.refresh_expires_in,
      refreshToken: refreshedTokens.refresh_token ?? token.refreshToken, // Fall back to old refresh token
    };
  } catch (error) {
    console.log(error);

    return {
      ...token,
      error: "RefreshAccessTokenError",
    };
  }
}

export default NextAuth({
  session: {
    // Choose how you want to save the user session.
    // The default is `"jwt"`, an encrypted JWT (JWE) stored in the session cookie.
    // If you use an `adapter` however, we default it to `"database"` instead.
    // You can still force a JWT session by explicitly defining `"jwt"`.
    // When using `"database"`, the session cookie will only contain a `sessionToken` value,
    // which is used to look up the session in the database.
    strategy: "jwt",

    // Seconds - How long until an idle session expires and is no longer valid.
    maxAge: 5 * 60, // 5 minutes
  },
  callbacks: {
    async jwt({ token, user, account }) {
      // Initial sign in
      console.log(account);
      if (account && user) {
        return {
          accessToken: account.access_token,
          accesssTokenExpires: account.expires_at,
          refreshTokenExpires: Date.now() + (account.refresh_expires_in as number),
          refreshToken: account.refresh_token,
          user,
        };
      }

      // Return previous token if the refresh token AND access has not expired yet
      if (token.refreshTokenExpires && Date.now() < token.refreshTokenExpires && token.accesssTokenExpires && Date.now() < token.accesssTokenExpires) {
        return token;
      }

      // Access token has expired, try to update it
      return refreshAccessToken(token);
    },
    async session({ session, token }) {
      session.user = token.user as User;
      session.accessToken = token.accessToken as string | undefined;
      session.error = token.erroras as string | undefined;

      return session;
    },
  },
  // Configure one or more authentication providers
  providers: [
    KeycloakProvider({
      clientId: process.env.KEYCLOAK_ID || "",
      clientSecret: process.env.KEYCLOAK_SECRET || "",
      issuer: process.env.KEYCLOAK_ISSUER,
    }),
    // ...add more providers here
  ],
});