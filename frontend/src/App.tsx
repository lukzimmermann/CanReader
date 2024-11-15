import { useEffect, useState } from "react";
import "./App.css";
import json from "./raw-can-data.json";
import { CanData } from "./interfaces/Can";
import Table from "./components/table";
import List from "./components/list";

const jsonData: CanData[] = json.sort((a, b) => Number(a.id) - Number(b.id));

function App() {
  const [data, setData] = useState<CanData[]>(jsonData);

  // useEffect(() => {
  //   const fetchAsyncData = async () => {
  //     await fetchData();
  //   };

  //   fetchAsyncData();
  // }, []);

  // async function fetchData() {
  //   try {
  //     const response = await fetch("http://192.168.1.118:3000/raw-can");

  //     const unsortedData = (await response.json()) as CanData[];

  //     setData(unsortedData.sort((a, b) => Number(a.id) - Number(b.id)));
  //   } catch (error) {
  //     console.error(error);
  //   }
  // }

  // setInterval(fetchData, 200);

  return (
    <div className="flex flex-col gap-10 items-center xl:w-3/4">
      <h1 className="text-3xl font-bold"></h1>
      <div className="p-5">
        <List data={data} />
      </div>
    </div>
  );
}

export default App;
