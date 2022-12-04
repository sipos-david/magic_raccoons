import { NextPage } from "next";
import Header from "../components/Header";

const FourOhFour: NextPage = () => {
  // TODO ui csinosítás

  return (
    <>
      <Header />
      <main className="mt-16">
        <p>
          404 - Keresett oldal nem található
        </p>
      </main>
    </>
  );
};

export default FourOhFour;