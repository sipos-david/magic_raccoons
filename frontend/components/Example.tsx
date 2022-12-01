import useApi from "../hooks/useApi";
import React from "react";

type User = {
  username: string
}

export default function Example() {
  const { data, isLoading, isError, } = useApi<User>("/users/me/");
  const [fileSelected,setFileSelected] = React.useState<File|undefined>();

  if (isError) return <div>Failed to load</div>;
  if (isLoading) return <div>Loading...</div>;

  // eslint-disable-next-line react-hooks/rules-of-hooks
  

  const onClickHandler = (event:any) => {
    event.preventDefault();
    const url="http://localhost:8000/upload_file";
    const form = new FormData();
    
    fetch(url, {
      method: "POST",
      mode:"cors",
      body: form
    }).then((response)=>{
      return response.json();
    }).then((data)=>{       
      alert(JSON.stringify(data.message));
    });
  };
   
  const onChangeHandler=(event:any)=>{
    setFileSelected(event.target.files[0]);
  };

  return (<>
    <div>
      <h1>Username</h1>
      <p>{data?.username}</p>
    </div>
    <div>
      <input type='file' name="file" onChange={onChangeHandler}/>
    </div>
    <div >
      <button  onClick={onClickHandler}>Upload</button>
    </div>
  </>
  );
}