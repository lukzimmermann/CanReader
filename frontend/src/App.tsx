import "./App.css";
import { useState } from "react";
import Header from "./modules/header";
import { BrowserRouter as Router, Route, Routes } from "react-router-dom";
import MenuBar from "./modules/menubar";
import Channels from "./modules/channels/channels.page";
import Dashboard from "./modules/dashboard/dashboard";
import Dbc from "./modules/dbc/dbc.page";

function App() {
  // setInterval(fetchData, 5000);

  const [isSidebarOpen, setIsSidebarOpen] = useState(false);

  const handleMenuClick = () => {
    setIsSidebarOpen(true);
  };

  const handleCloseClick = () => {
    setIsSidebarOpen(false);
  };

  return (
    <Router>
      <div className="relative flex flex-col items-center m-0 p-0">
        <Header onMenuClick={() => handleMenuClick()} />
        <MenuBar isOpen={isSidebarOpen} onCloseClick={handleCloseClick} />
        {isSidebarOpen && <div onClick={handleCloseClick} className="fixed inset-0 bg-black bg-opacity-50 z-40"></div>}

        <div className="flex w-screen px-5 justify-center"></div>
        <Routes>
          <Route path="/" element={<Dashboard />} />
          <Route path="/channels" element={<Channels />} />
          <Route path="/dbc" element={<Dbc />} />
          <Route path="/contact" element={null} />
        </Routes>
      </div>
    </Router>
  );
}

export default App;
