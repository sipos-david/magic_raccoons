import { NextPage } from "next";
import Header from "../components/Header";
import CaffDto from "../dto/CaffDto";
import useApi from "../hooks/useApi";

const Caff: NextPage = () => {
  const { data, isLoading, isError, } = useApi<CaffDto[]>("/api/");

  const handleClick=(id:number)=>{
    fetch(`http://localhost:8000/download_caff/${id}`,{
      method:"GET",
      mode:"cors"
    }).then((res)=>{
      return res.blob();
    }).then((blob) => {
      const href = window.URL.createObjectURL(blob);
      const link = document.createElement("a");
      link.href = href;
      link.setAttribute("download", "source.caff"); //or any other extension
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
    }).catch((err) => {
      return Promise.reject({ Error: "Something Went Wrong", err });
    });
  }

  return (
    <>
      <Header />
      <main className="mt-16">
        <div>
          <div>
            {data && data.map((caff) => (
              <div key={caff.id}>
                <h1>{caff.id}</h1>
                <button onClick={()=>handleClick(caff.id)}>Download caff</button>
                <div>
                  <img src={`http://localhost:8000/preview/${caff.id}.gif`} alt="" width="30" height="24" ></img>
                </div>
                <div>
                  {caff.comments.map((comment) => (comment.text))}
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
