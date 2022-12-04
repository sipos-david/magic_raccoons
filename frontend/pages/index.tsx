import { NextPage } from "next";
import Header from "../components/Header";
import useApi from "../hooks/useApi";

const Home: NextPage = () => {
  const { data, isLoading, isError, } = useApi<unknown>("/api/users/me");

  if (isError) return <div>Failed to load</div>;
  if (isLoading) return <div>Loading...</div>;

  // TODO caff fájl keresés tagek alapján
  // TODO .caff.tsx-ből ide migrálni
  // TODO ui csinosítás

  return (
    <>
      <Header />
      <main className="mt-16">
        <div>
          <p>{JSON.stringify(data)}</p>
        </div>
      </main>
    </>
  );
};

export default Home;
