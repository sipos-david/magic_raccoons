import { NextPage } from "next";
import { ChangeEvent, MouseEvent, useState } from "react";
import Header from "../components/Header";

const Upload: NextPage = () => {
  const [fileSelected, setFileSelected] = useState<File | undefined>();

  const onClickHandler = (event: MouseEvent<HTMLButtonElement>) => {
    event.preventDefault();
    if (fileSelected) {
      const url = "http://localhost:8000/upload_file";
      const form = new FormData();
      form.append("file", fileSelected);
      fetch(url, {
        headers:{
          "Content-Type": "multipart/form-data"
        },
        method: "POST",
        mode: "cors",
        body: form
      }).then((response) => {
        return response.json();
      }).then((data) => {
        alert(JSON.stringify(data));
      });
    }
  };

  const onChangeHandler = (event: ChangeEvent<HTMLInputElement>) => {
    const file = event.target.files?.item(0);
    if (file) {
      setFileSelected(file);
    }
  };

  return (
    <>
      <Header />
      <main className="mt-16">
        <div>
          <input type='file' name="file" onChange={onChangeHandler} />
          <button onClick={onClickHandler}>Upload</button>
        </div>
      </main>
    </>
  );
};

export default Upload;
