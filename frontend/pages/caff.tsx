import { NextPage } from "next";
import Header from "../components/Header";
import CaffDto from "../dto/CaffDTO";
import useApi from "../hooks/useApi";

const Caff: NextPage = () => {

  const { data, isLoading, isError, } = useApi<CaffDto[]>("/api/");


  return (
    <>
      <Header />
      <main className="mt-16">
        <div>
          <div>
            {data && data.map((caff)=>(
              <div key={caff.id}>
                 <h1>{caff.creator}</h1>
                   <div>
                    {caff.comments.map((comment)=>(comment.text))}
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
