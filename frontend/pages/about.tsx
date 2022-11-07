import Link from "next/link";
import Example from "../components/Example";
import Header from "../components/Header";

export default function About() {
  return (<>
    <Header />
    <Link href="/">Home</Link>
    <Example />
  </>);
}