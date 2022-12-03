import { useSession, signOut, signIn } from "next-auth/react";
import Link from "next/link";
import { useEffect } from "react";

export default function Header() {
  const { data: session } = useSession();

  useEffect(() => {
    if (session?.error === "RefreshAccessTokenError") {
      signIn(); // Force sign in to hopefully resolve error
    }
  }, [session]);

  const navs = [
    { title: "Feltöltés", href: "/upload" },
    { title: "Események", href: "/logs" },
  ];

  if (session) {
    return (
      <header className="fixed w-full top-0 border-b bg-black text-white flex pr-4 pl-4 pt-1 pb-1">
        <nav className="flex gap-4 items-center justify-center font-semibold ">
          <Link href="/" className="text-2xl"><span className="text-violet-600">Caff</span>Shop</Link>
          {navs.map(opt =>
            <Link
              className="p-2 text-lg hover:underline hover:text-gray-300 decoration-violet-600 decoration-2 hover:cursor-pointer"
              key={opt.title}
              href={opt.href}>
              {opt.title}
            </Link>)}
        </nav>
        <section className="flex grow flex-col items-end">
          <span className="text-lg">
            {session.user?.name}
          </span>
          <button onClick={() => signOut()} className="text-gray-400 hover:text-red-500">Kilépés</button>
        </section>
      </header>
    );
  }
  return (
    <header className="fixed w-full top-0 border-b bg-black text-white flex pr-4 pl-4 pt-1 pb-1">
      <nav className="flex gap-4 items-center justify-center font-semibold ">
        <Link href="/" className="text-2xl"><span className="text-violet-600">Caff</span>Shop</Link>
      </nav>
      <section className="flex grow flex-col items-end">
        <button className="text-gray-400 hover:text-red-500" onClick={() => signIn()}>Belépés</button>
      </section>
    </header>
  );
}