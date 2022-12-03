import { NextPage } from "next";
import Header from "../components/Header";
import useApi from "../hooks/useApi";

const Home: NextPage = () => {
  const { data, isLoading, isError, } = useApi<{ username: string }>("/users/me/");

  if (isError) return <div>Failed to load</div>;
  if (isLoading) return <div>Loading...</div>;

  return (
    <>
      <Header />
      <main className="mt-16">
        <div>
          <p>{data?.username}</p>
        </div>
      </main>
    </>
  );
};

export default Home;
