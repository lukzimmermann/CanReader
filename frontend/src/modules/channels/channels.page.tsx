import CanMessageList from "./canMessageList/canMessageList";

import json from "../../raw-can-data.json";
import dbc from "../../dbc.json";
import { CanFrame } from "../../interfaces/can";
import { useEffect, useState } from "react";

const jsonData: CanFrame[] = json.sort((a, b) => a.id - b.id);

function Channels() {
  const [data, setData] = useState<CanFrame[]>(jsonData);

  useEffect(() => {
    const fetchAsyncData = async () => {
      await fetchData();
    };

    fetchAsyncData();
  }, []);

  async function fetchData() {
    try {
      const response = await fetch("http://10.0.0.203/get-can-data");

      const unsortedData = (await response.json()) as CanFrame[];

      setData(unsortedData.sort((a, b) => Number(a.id) - Number(b.id)));
    } catch (error) {
      console.error(error);
    }
  }

  return <CanMessageList data={data} dbc={dbc} />;
}

export default Channels;
