import { NextPage } from "next";
import Image from "next/image";
import Link from "next/link";
import Header from "../components/Header";
import CaffDto from "../dto/CaffDto";
import useApi from "../hooks/useApi";
import { useState } from "react";
import LoadingSpinner from "../components/LoadingSpinner";

const Home: NextPage = () => {
  const [searchedTags, setSearchedTags] = useState<string>("");
  let sanitizedQuery: string | undefined = undefined;
  if (searchedTags && searchedTags.length > 0) {
    const tags = searchedTags.split(" ").join(";");
    const params = new URLSearchParams();
    params.set("tag", tags);
    sanitizedQuery = params.toString();
  }
  const url = sanitizedQuery ? `/api?${sanitizedQuery}` : "/api";
  const { data, isLoading, isError, } = useApi<CaffDto[]>(url);

  return (
    <>
      <Header />
      <main className="mt-16">
        <div className="mt-20 mb-3 flex justify-center">
          <form className="w-2/3">
            <label htmlFor="default-search" className="mb-2 text-sm font-medium text-gray-900 sr-only ">Search</label>
            <div className="relative">
              <div className="absolute inset-y-0 left-0 flex items-center pl-3 pointer-events-none">
                <svg aria-hidden="true" className="w-5 h-5 text-gray-500" fill="none" stroke="currentColor" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0z"></path></svg>
              </div>
              <input type="search" value={searchedTags} onChange={(e) => setSearchedTags(e.target.value)} id="default-search" className="w-full p-4 pl-10 text-sm text-gray-900 border border-gray-300 rounded-lg bg-gray-50 focus:ring-blue-500 focus:border-blue-500  " placeholder="Caff keresése tag-ek alapján..." required />
            </div>
            <p className="mt-1 text-sm text-gray-500" >Tagek elválasztva szóközzel. pl.: landscape sunset ...</p>
          </form>
        </div>
        <section className="flex flex-row justify-center flex-wrap">
          {isLoading &&
            <div>
              <LoadingSpinner />
            </div>
          }
          {isError && <div className="text-xl mt-32">Hiba történt :,(</div>}
          {!isLoading && !isError && data && data.map((caff) => (
            <div key={caff.id} className="border border-solid border-gray-400 bg-gray-50 p-4 m-2 rounded">
              <Link href={`/details/${caff.id}`}>
                <Image className="rounded w-auto h-auto max-w-xs" priority src={`http://localhost:8000/preview/${caff.id}.gif`} alt="Caff preview gif" width={256} height={256}></Image>
              </Link>
              <div className="flex flex-row justify-between mt-1">
                <span className="font-semibold text-lg">{caff.creator}</span>
                <span className="text-gray-600">{caff.year}.{caff.month}.{caff.day}</span>
              </div>
            </div>
          ))}
          {!isLoading && !isError && data && data.length === 0 &&
            <p className="text-xl">Nincs találat</p>}
        </section>
      </main>
    </>
  );
};

export default Home;
