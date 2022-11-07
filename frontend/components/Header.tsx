import { useSession, signOut, signIn } from "next-auth/react";

export default function Header() {
  const { data: session } = useSession();
  if (session) {
    return (
      <header>
        Signed in as {session.user?.email} <br />
        <button onClick={() => signOut()}>Sign out</button>
      </header>
    );
  }
  return (
    <header>
      Not signed in <br />
      <button onClick={() => signIn()}>Sign in</button>
    </header>
  );
}