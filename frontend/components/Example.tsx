import useApi from "../hooks/useApi";

type User = {
  username: string
}

export default function Example() {
  const { data, isLoading, isError, } = useApi<User>("/users/me/");

  if (isError) return <div>Failed to load</div>;
  if (isLoading) return <div>Loading...</div>;

  return (
    <div>
      <h1>Username</h1>
      <p>{data?.username}</p>
    </div>
  );
}