import { NextPage } from "next";
import Header from "../components/Header";
import CaffDto from "../dto/CaffDto";
import useApi from "../hooks/useApi";

const Caff: NextPage = () => {
  const { data, isLoading, isError, } = useApi<CaffDto[]>("/api");

  //TODO link a details page/re

  return (
    <>
      <Header />
      <main className="mt-16">
        <div>
          <div>
            {data && data.map((caff) => (
              <div key={caff.id}>
                <h1>{caff.id}</h1>
                <div>
                  <img src={`http://localhost:8000/preview/${caff.id}.gif`} alt="" width="30" height="24" ></img>
                </div>
              </div>
            ))}

          </div>
        </div>
      </main>
    </>
  );
};

export default Caff;
