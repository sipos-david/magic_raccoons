import { NextPage } from "next";
import { ChangeEvent, MouseEvent, useState } from "react";
import Header from "../components/Header";
import { useSnackbar } from "../context/snackbarContext";

const Upload: NextPage = () => {
  const [fileSelected, setFileSelected] = useState<File | undefined>();
  const { showSnackbar } = useSnackbar();

  const onClickHandler = (event: MouseEvent<HTMLButtonElement>) => {
    event.preventDefault();
    if (fileSelected) {
      const url = "http://localhost:8000/upload_file";
      const form = new FormData();
      form.append("file", fileSelected);
      fetch(url, {
        method: "POST",
        mode: "cors",
        body: form
      }).then((response) => {
        if (response.ok) {
          showSnackbar({ text: "Sikeres fájl feltöltés", severity: "success" });
        } else {
          showSnackbar({ text: "Sikertelen fájl feltöltés", severity: "error" });
          response.json().then((data) => {
            console.error(data);
          });
        }
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
      <main className="mt-16 flex flex-col items-center">
        <div className="w-2/3 flex items-center">
          <div className="grow">
            <label className="mb-2 text-sm font-medium text-gray-900" htmlFor="file_input">Caff feltöltése</label>
            <input className="appearance-none w-full p-2 text-sm text-gray-900 border border-gray-300 rounded-lg cursor-pointer bg-gray-50 focus:outline-none file:border-solid 
              file:border file:border-violet-300 file:mr-4 file:py-2 file:px-4 file:rounded file:text-sm file:font-semibold file:bg-violet-50 file:text-violet-700
              hover:file:border-transparent hover:file:bg-violet-700 hover:file:text-white " aria-describedby="file_input_help" id="file_input" type="file" name="file" onChange={onChangeHandler} />
            <p className="mt-1 text-sm text-gray-500" id="file_input_help">.CAFF</p>
          </div>
          <button onClick={onClickHandler} className="ml-2 bg-green-50 hover:bg-green-500 text-green-700 font-semibold hover:text-white py-2 px-4 border border-green-400 hover:border-transparent rounded">
            Feltöltés
          </button>
        </div>
      </main>
    </>
  );
};

export default Upload;