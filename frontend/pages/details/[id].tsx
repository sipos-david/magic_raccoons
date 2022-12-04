import { NextPage } from "next";
import { useRouter } from "next/router";
import { useRef } from "react";
import Header from "../../components/Header";
import CaffDto from "../../dto/CaffDto";
import useApi from "../../hooks/useApi";
import useMutate from "../../hooks/useMutate";
import { useSnackbar } from "../../context/snackbarContext";

const Details: NextPage = () => {
  const router = useRouter();
  const { id } = router.query;
  const caffId = Number(id);
  const { showSnackbar } = useSnackbar();
  const { data, isLoading, isError } = useApi<CaffDto | undefined>("/api/" + caffId);
  const { mutate } = useMutate();

  const commentInputRef = useRef<undefined | any>(null);

  const handleClick = (id: number) => {
    mutate(`/download_caff/${id}`, "GET", undefined)?.then((res) => {
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
  };

  // TODO komment szerkesztése
  // TODO ui csinosítás

  const handleAddCommentClick = () => {
    const commentText: string = commentInputRef.current.value;
    console.log("add comment with "+commentText);
    const data = { text: commentText, date: (new Date().toISOString().toString()), author_id: 0 };
    mutate(`/api/${caffId}/comments`, "POST", data);
  };

  const handleCommentDeleteClick=(id:number)=>{
    mutate(`/api/${caffId}/comments/${id}`,"DELETE",undefined)?.then((response)=>{
      if (response.ok) {
        showSnackbar({ text: "Komment törölve", severity: "success" });
      } else {
        showSnackbar({ text: "Komment törlése sikertelen", severity: "error" });
        response.json().then((data) => {
          console.error(data);
        });
      }
    });
  };

  return (
    <>
      <Header />
      <main className="mt-16">
        <p>
          {caffId.toString()}
        </p>
        <div>
          <div>
            <h2>{data && data.creator}</h2>
            <h2>{data && data.year} {data?.month} {data?.day} {data?.hour}:00</h2>
            <button onClick={() => handleClick(caffId)}>Download caff</button>
            <div>
              <img src={`http://localhost:8000/preview/${caffId}.gif`} alt="" width="30" height="24" />
            </div>
            {(data && data.comments.map((comment) => (
              <div key={comment.id}>
                <p>id: {comment.id}</p>
                <p>text:  {comment.text}</p>
                <p>date:  {comment.date}</p>
                <button onClick={()=>handleCommentDeleteClick(comment.id)}>Delete comment</button>
              </div>
            )))}
            <div>
              <div>New comment:</div>
              <div><input type="text" id="comment_input" ref={commentInputRef} /></div>
              <div><button onClick={() => handleAddCommentClick()}>Add comment</button></div>
            </div>
          </div>
        </div>
      </main>
    </>
  );
};

export default Details;
