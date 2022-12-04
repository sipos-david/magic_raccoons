import { NextPage } from "next";
import Header from "../components/Header";

const FourOhFour: NextPage = () => {
  return (
    <>
      <Header />
      <main className="h-screen w-screen flex justify-center items-center">
        <p className="text-center flex items-center">
          <span className="text-6xl text-violet-600 pr-2 mr-2 border-r-4 border-gray-700">404</span><span className="text-2xl">Keresett oldal nem található</span>
        </p>
      </main>
    </>
  );
};

export default FourOhFour;