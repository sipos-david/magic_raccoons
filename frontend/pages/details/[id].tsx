import { NextPage } from "next";
import { useRouter } from "next/router";
import Header from "../../components/Header";

const Details: NextPage = () => {
  const router = useRouter();
  const { id } = router.query;

  const caffId = Number(id);

  return (
    <>
      <Header />
      <main className="mt-16">
        <p>
          {caffId.toString()}
        </p>
      </main>
    </>
  );
};

export default Details;