import "./App.css";
import { useState } from "react";
import json from "./raw-can-data.json";
import dbc from "./dbc.json";
import CanMessageList from "./components/canMessageList/canMessageList";
import { CanFrame } from "./interfaces/can";
import { List, X } from "@phosphor-icons/react";

const jsonData: CanFrame[] = json.sort((a, b) => a.id - b.id);

function App() {
  const [data] = useState<CanFrame[]>(jsonData);

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

  const [isSidebarOpen, setIsSidebarOpen] = useState(false);

  const handleMenuClick = () => {
    setIsSidebarOpen(true);
  };

  const handleCloseClick = () => {
    setIsSidebarOpen(false);
  };

  return (
    <div className="relative flex flex-col items-center m-0 p-0">
      {/* Header with hamburger button */}
      <div className="w-screen bg-gray-600 mb-5 p-2 sticky top-0 z-50 flex items-center justify-between">
        <button onClick={handleMenuClick} className="text-white hover:bg-gray-500 p-2 rounded-xl">
          <List size={24} />
        </button>
        <h1 className="text-white text-xl font-bold text-center flex-grow">CanReader</h1>
        <div className="w-8"></div> {/* Placeholder for spacing */}
      </div>

      {/* Sidebar */}
      <div
        className={`fixed top-0 left-0 h-full w-64 bg-gray-800 text-white transform ${
          isSidebarOpen ? "translate-x-0" : "-translate-x-full"
        } transition-transform duration-300 ease-in-out z-50`}
      >
        <div className="flex justify-between items-center p-4 border-b border-gray-700">
          <h2 className="text-lg font-bold">Menu</h2>
          <button onClick={handleCloseClick} className="text-white">
            <X size={24} />
          </button>
        </div>
        <ul className="p-4 space-y-4 text-left">
          <li>
            <a href="#" className="hover:underline">
              Dashboard
            </a>
          </li>
          <li>
            <a href="#" className="hover:underline">
              Channels
            </a>
          </li>
          <li>
            <a href="#" className="hover:underline">
              DBC-File
            </a>
          </li>
          <li>
            <a href="#" className="hover:underline">
              Settings
            </a>
          </li>
        </ul>
      </div>

      {/* Overlay */}
      {isSidebarOpen && <div onClick={handleCloseClick} className="fixed inset-0 bg-black bg-opacity-50 z-40"></div>}

      {/* Main Content */}
      <div className="w-screen px-5">
        <CanMessageList data={data} dbc={dbc} />
      </div>
    </div>
  );
}

export default App;
