import { NextPage } from "next";
import { useRouter } from "next/router";
import { useRef, useState } from "react";
import Header from "../../components/Header";
import CaffDto from "../../dto/CaffDto";
import useApi from "../../hooks/useApi";
import useMutate from "../../hooks/useMutate";
import { useSnackbar } from "../../context/snackbarContext";
import Image from "next/image";
import LoadingSpinner from "../../components/LoadingSpinner";
import { useUser } from "../../context/userContext";
import CommentDto from "../../dto/CommentDto";

const Details: NextPage = () => {
  const router = useRouter();
  const { id } = router.query;
  const caffId = id ? Number(id) : undefined;
  const { showSnackbar } = useSnackbar();
  const { data, isLoading, isError } = useApi<CaffDto | undefined>("/api/" + caffId);
  const { mutate } = useMutate();
  const { user } = useUser();
  const isAuthorized = user?.role === "ADMIN";
  const [selectedCommentId, setSelectedCommentId] = useState<number | undefined>(undefined);
  const commentInputRef = useRef<HTMLTextAreaElement>(null);

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

  const handleAddCommentClick = () => {
    const commentText = commentInputRef?.current?.value;
    if (commentText && commentText.length !== 0) {
      const data = { text: commentText, date: (new Date().toISOString().toString()), author_id: 0 };
      commentInputRef.current.value = "";
      mutate(`/api/${caffId}/comments`, "POST", data)?.then((res) => {
        if (res.ok) {
          showSnackbar({ text: "Komment sikeresen hozzáadva", severity: "success" });
        } else {
          showSnackbar({ text: "Komment hozzáadása nem sikerült", severity: "error" });
        }
      });
    }
  };



  return (
    <>
      <Header />
      <main className="mt-16 flex flex-col items-center">
        {
          isLoading && <LoadingSpinner />
        }
        {!isLoading && isError && <div>Hiba történt</div>}
        {!isLoading && !isError && caffId !== undefined && data &&
          <>
            <Image className="rounded w-auto h-auto max-w-3xl max-h-max" priority src={`http://localhost:8000/preview/${caffId}.gif`} alt="Caff preview gif" width={0} height={0}></Image>
            <section className="flex flex-row m-2 items-center">
              <h2 className="text-lg m-1 pr-2 border-r-2 border-violet-500"><span className="font-bold mr-1">Keszítő:</span> {data && data.creator}</h2>
              <h2 className="text-lg m-1 pr-2 border-r-2 border-violet-500"><span className="font-bold  mr-1">Készítés időpontja:</span>{data && data.year}.{data.month < 10 ? "0" : ""}{data?.month}.{data.day < 10 ? "0" : ""}{data?.day} {data?.hour}:00</h2>
              <button onClick={() => handleClick(caffId)} className="m-2 bg-blue-50 hover:bg-blue-500 text-blue-700 font-semibold hover:text-white py-2 px-4 border border-blue-400 hover:border-transparent rounded">Letöltés</button>
            </section>
            <section className="flex flex-col w-2/3">
              <label htmlFor="message" className="mb-2 text-sm font-medium text-gray-900">Komment írása</label>
              <textarea ref={commentInputRef} id="message" rows={4} className=" resize-none p-2.5 w-full text-sm text-gray-900 bg-gray-50 rounded-lg border border-gray-300 focus:ring-blue-500 focus:border-blue-500" placeholder="Írd megjegyzést..."></textarea>
              <button className="ml-auto mt-2 bg-green-50 hover:bg-green-500 text-green-700 font-semibold hover:text-white py-2 px-4 border border-green-400 hover:border-transparent rounded" onClick={() => handleAddCommentClick()}>Megjegyzés</button>
            </section>
            <section className="flex flex-col w-2/3 mb-32">
              {data.comments.map((comment) => {
                if (selectedCommentId === comment.id) {
                  return (<CommentEdit key={comment.id} comment={comment} caffId={caffId} onCancel={() => setSelectedCommentId(undefined)} />);
                }
                return (
                  <div key={comment.id} className="m-2 flex flex-row items-center">
                    <span className="flex flex-col grow">
                      <p className="w-fit pb-1 border-b-2 border-violet-500"><span className="text-lg">{comment.username}</span> <span className="text-gray-500">{comment.date}</span></p>
                      <p>{comment.text}</p>
                    </span>
                    {isAuthorized &&
                      <button onClick={() => setSelectedCommentId(comment.id)} className="h-fit bg-violet-50 hover:bg-violet-500 text-violet-700 font-semibold hover:text-white py-2 px-4 border border-violet-400 hover:border-transparent rounded">Szerkesztés</button>
                    }
                  </div>
                );
              })}
            </section>
          </>
        }
      </main>
    </>
  );
};

export default Details;

type CommentEditProps = {
  comment: CommentDto,
  caffId: number,
  onCancel: () => void
}

function CommentEdit({ comment, caffId, onCancel }: CommentEditProps) {
  const { showSnackbar } = useSnackbar();
  const { mutate } = useMutate();
  const [commentText, setCommentText] = useState(comment.text);

  const handleCommentDeleteClick = () => {
    mutate(`/api/${caffId}/comments/${comment.id}`, "DELETE", undefined)?.then((response) => {
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

  const handleCommentEditClick = () => {
    if (commentText && commentText.length !== 0) {
      mutate(`/api/${caffId}/comments/${comment.id}`, "PUT", undefined)?.then((response) => {
        if (response.ok) {
          showSnackbar({ text: "Komment frissítve", severity: "success" });
        } else {
          showSnackbar({ text: "Komment frissítése sikertelen", severity: "error" });
          response.json().then((data) => {
            console.error(data);
          });
        }
      });
    }
  };

  return (
    <div key={comment.id} className="m-2 flex flex-row items-center">
      <span className="flex flex-col grow">
        <p className="w-fit pb-1 border-b-2 border-violet-500"><span className="text-lg">{comment.username}</span> <span className="text-gray-500">{comment.date}</span></p>
        <textarea value={commentText} onChange={(e) => setCommentText(e.target.value)} rows={4} className="resize-none mt-2 mr-2 p-2.5 text-sm text-gray-900 bg-gray-50 rounded-lg border border-gray-300 focus:ring-blue-500 focus:border-blue-500" placeholder="Megjegyzés szerkesztése..."></textarea>
      </span>
      <div className="flex flex-col items-stretch">
        <button onClick={handleCommentEditClick} className="h-fit mb-2 bg-green-50 hover:bg-green-500 text-green-700 font-semibold hover:text-white py-2 px-4 border border-green-400 hover:border-transparent rounded">Mentés</button>
        <button onClick={handleCommentDeleteClick} className="h-fit mb-2 bg-red-50 hover:bg-red-500 text-red-700 font-semibold hover:text-white py-2 px-4 border border-red-400 hover:border-transparent rounded">Törlés</button>
        <button onClick={() => onCancel()} className="h-fit bg-violet-50 hover:bg-violet-500 text-violet-700 font-semibold hover:text-white py-2 px-4 border border-violet-400 hover:border-transparent rounded">Mégse</button>
      </div>
    </div>
  );
} 