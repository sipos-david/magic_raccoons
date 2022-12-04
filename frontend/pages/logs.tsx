import { NextPage } from "next";
import Header from "../components/Header";
import LoadingSpinner from "../components/LoadingSpinner";
import LogDto from "../dto/LogDto";
import useApi from "../hooks/useApi";

const Logs: NextPage = () => {
  const { data, isLoading, isError, } = useApi<LogDto[]>("/api/logs");

  return (
    <>
      <Header />
      <main className="mt-16 flex justify-center">
        <section className="flex flex-col w-2/3 justify-center flex-wrap">
          {isLoading &&
            <div>
              <LoadingSpinner />
            </div>
          }
          {isError && <div className="text-xl mt-32">Hiba történt :,(</div>}
          {!isLoading && !isError && data && data.map((log) => {
            const id = (new Date()).getTime() * Math.random();
            return (
              <div key={id} className="m-2 flex flex-row items-center">
                <span className="flex flex-col grow">
                  <p className="w-fit pb-1 border-b-2 border-violet-500"><span className="text-lg inline-block w-16 font-bold pr-1 border-r-2 mr-3 border-gray-500">{log.level}</span><span className="text-lg mr-3">{log.username}</span><span className="text-gray-500">{log.date}</span></p>
                  <p>{log.text}</p>
                </span>
              </div>
            );
          }
          )}
          {!isLoading && !isError && data && data.length === 0 &&
            <p className="text-xl">Nincs találat</p>}
        </section>
      </main>
    </>
  );
};

export default Logs;
